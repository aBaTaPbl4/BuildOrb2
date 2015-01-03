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
#define LED_BIT             4

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
/* Дескриптор выше - только макет, это заглушает драйверы. Репорт, который его 
 *  описывает, состоит из одного байта неопределенных данных. Мы не передаем
 *  наши данные через HID-репорты, вместо этого мы используем custom-запросы.
 */

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR){
        DBG1(0x50, &rq->bRequest, 1);   /* отладочный вывод: печатаем наш запрос */
        if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
            if(rq->wValue.bytes[0] & 1){    /* установить LED */
                LED_PORT_OUTPUT |= _BV(LED_BIT);
            }else{                          /* очистить LED */
                LED_PORT_OUTPUT &= ~_BV(LED_BIT);
            }
        }else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
            static uchar dataBuffer[1];     /* буфер должен оставаться валидным привыходе из usbFunctionSetup */
            dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
            usbMsgPtr = dataBuffer;         /* говорим драйверу, какие данные вернуть */
            return 1;                       /* говорим драйверу послать 1 байт */
        }
    }else{
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
    DBG1(0x00, 0, 0);       /* отладочный вывод: стартует main */
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
    LED_PORT_DDR |= _BV(LED_BIT);   /* делаем ножку, куда подключен LED, выходом */
    sei();
    DBG1(0x01, 0, 0);       /* отладочный вывод: стартует цикл main */
    for(;;){                /* цикл событий main */
#if 0   /* это несколько агрессивно для отладочного вывода */
        DBG2(0x02, 0, 0);   /* отладочный вывод: повторы цикла main */
#endif
        wdt_reset();
        usbPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
