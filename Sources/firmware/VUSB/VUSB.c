/*
Этот пример должен работать на большинстве AVR с минимальными изменениями. Не используются
никакие аппаратные ресурсы микроконтроллера, за исключением INT0. Вы можете поменять 
usbconfig.h для использования других ножек I/O USB. Пожалуйста помните, что USB D+ должен
быть подсоединен на ножку INT0, или также как минимум быть соединенным с INT0.
Мы предполагаем, что LED подсоединен к порту B, бит 0. Если Вы подсоединили его на
другой порт или бит, поменяйте макроопределение ниже:
*/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define RED_BIT               3 
#define GREEN_BIT             4
#define BLUE_BIT              0

#define RED_BIT_IN_PACKET	  1
#define GREEN_BIT_IN_PACKET   2
#define BLUE_BIT_IN_PACKET    4

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* для sei() */
#include <util/delay.h>     /* для _delay_ms() */

#include <avr/pgmspace.h>   /* нужен для usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* Это также пример использования макроса отладки */
#include "requests.h"       /* номера custom request, используемые нами */

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM const char usbHidReportDescriptor[22] = {    /* дескриптор репорта USB */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

void SetColor(uchar colorByte) 
{
	        if((colorByte & RED_BIT_IN_PACKET) == 0) //у нас светодиод с реверсом напряжения (у него все наооборот)
			{/* очистить LED */
                LED_PORT_OUTPUT |= _BV(RED_BIT);
            }
			else
			{/* установить LED */
                LED_PORT_OUTPUT &= ~_BV(RED_BIT);
            }
            if((colorByte & GREEN_BIT_IN_PACKET) == 0)
			{
	            LED_PORT_OUTPUT |= _BV(GREEN_BIT);
	        } 
			else
			{
	            LED_PORT_OUTPUT &= ~_BV(GREEN_BIT);				
            }			
            if((colorByte & BLUE_BIT_IN_PACKET) == 0)
			{
	            LED_PORT_OUTPUT |= _BV(BLUE_BIT);
	        }
			else
			{
	            LED_PORT_OUTPUT &= ~_BV(BLUE_BIT);
            }
}

/* Дескриптор выше - только макет, это заглушает драйверы. Репорт, который его 
 *  описывает, состоит из одного байта неопределенных данных. Мы не передаем
 *  наши данные через HID-репорты, вместо этого мы используем custom-запросы.
 */

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	//LED_PORT_OUTPUT |= _BV(BLUE_BIT);
	//return 1;	
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR)
	{
        //DBG1(0x50, &rq->bRequest, 1);   /* отладочный вывод: печатаем наш запрос */
        if(rq->bRequest == CUSTOM_RQ_SET_STATUS)
		{			
			uchar colorByte = rq->wValue.bytes[0];
			
            SetColor(colorByte);
			
        }
		else if(rq->bRequest == CUSTOM_RQ_GET_STATUS)
		{
            static uchar dataBuffer[1];     /* буфер должен оставаться валидным привыходе из usbFunctionSetup */
			dataBuffer[0] = 0x00;
			if ((LED_PORT_OUTPUT & _BV(RED_BIT)) == 0)
			{
				dataBuffer[0] = RED_BIT_IN_PACKET;
			}
			if ((LED_PORT_OUTPUT & _BV(GREEN_BIT)) == 0)
			{
				dataBuffer[0] += GREEN_BIT_IN_PACKET;
			}			
			if ((LED_PORT_OUTPUT & _BV(BLUE_BIT)) == 0)
			{
				dataBuffer[0] += BLUE_BIT_IN_PACKET;
			}			            
            usbMsgPtr = dataBuffer;         /* говорим драйверу, какие данные вернуть */
            return 1;                       /* говорим драйверу послать 1 байт */
        }
		else if(rq->bRequest == CUSTOM_RQ_GET_PORT_STATUS)
		{
			static uchar dataBuffer[1];
			dataBuffer[0] = 0x00 | LED_PORT_OUTPUT;
			usbMsgPtr = dataBuffer;
			return 1;		
		}
    }
	else
	{
        /* вызовы запросов USBRQ_HID_GET_REPORT и USBRQ_HID_SET_REPORT не реализованы,
         *  поскольку мы их не вызываем. Операционная система также не будет обращаться к ним, 
         *  потому что наш дескриптор не определяет никакого значения.
         */
    }
    return 0;   /* default для нереализованных запросов: не возвращаем назад данные хосту */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

//DDRA = 0xff;
//PORTA = 0xff;
    wdt_enable(WDTO_1S);
    /* Даже если Вы не используете сторожевой таймер (watchdog), выключите его здесь. На более новых 
     *  микроконтроллерах состояние watchdog (вкл\выкл, период) СОХРАНЯЕТСЯ ЧЕРЕЗ СБРОС!
    */
    /* RESET статус: все биты портов являются входамибез нагрузочных резисторов (pull-up).
     *  Это нужно для D+ and D-. Таким образом, нам не нужна какая-либо дополнительная 
     *  инициализация портов.
     */
    odDebugInit();
    usbInit();
    usbDeviceDisconnect();  /* принудительно запускаем ре-энумерацию, делайте это, когда прерывания запрещены! */
    i = 0;
    while(--i)
    {             /* иммитируем USB дисконнект на время > 250 мс */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
	uchar leds = _BV(RED_BIT) | _BV(GREEN_BIT) | _BV(BLUE_BIT);
    LED_PORT_DDR |= leds;   /* ноги на которых висит светодиод конфигурируются как выходы(т.е. на них будем подавать напряжение, НО не принимать) */
	LED_PORT_OUTPUT |= leds;
    sei();
	int counter = 0;
    for(;;){                /* цикл событий main */
        wdt_reset();
        usbPoll();	
		if ((counter % 1511) == 0)
		{
			SetColor(RED_BIT_IN_PACKET);			
			counter = 0;
		}
		else if ((counter % 1009) == 0)
		{
			SetColor(BLUE_BIT_IN_PACKET);
		}
		else if ((counter % 503) == 0)
		{
			SetColor(GREEN_BIT_IN_PACKET);
		}
		_delay_ms(2);
		counter++;
		
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
