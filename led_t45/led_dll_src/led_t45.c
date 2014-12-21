/** Подключение библиотек ****************************************************/
#include <avr/io.h>				/* порты и регистры							*/
#include <inttypes.h>			/* типы данных								*/
#include <util/delay.h>			/* задержки	_delay_us(double __us)			*/
								/*			_delay_ms(double __ms)			*/
#include <avr/interrupt.h>		/* прерывания	ISR()						*/
#include <compat/deprecated.h>	/* функции sbi(), cbi()						*/
#include <avr/pgmspace.h>		/* pgm_read_byte();	prog_					*/
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "usbdrv.h"
#include "requests.h"			/* номера custom request, используемые нами */
/*****************************************************************************/

/*****************************************************************************/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_BIT             1
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
        if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
            if(rq->wValue.bytes[0] & 1){					/* установить LED */
                //LED_PORT_OUTPUT |= _BV(LED_BIT);
				sbi(LED_PORT_OUTPUT,LED_BIT);
            }else{											/* очистить LED */
                //LED_PORT_OUTPUT &= ~_BV(LED_BIT);
				cbi(LED_PORT_OUTPUT,LED_BIT);
            }
        }else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
            static uchar dataBuffer[1];						/* буфер должен оставаться валидным привыходе из usbFunctionSetup */
            //dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
            dataBuffer[0] = (bit_is_set(LED_PORT_OUTPUT,LED_BIT) != 0);
            usbMsgPtr = dataBuffer;							/* говорим драйверу, какие данные вернуть */
            return 1;										/* говорим драйверу послать 1 байт */
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
/*****************************************************************************/



/*****************************************************************************/
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/

void    usbEventResetReady(void)
{
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}

/*****************************************************************************/


/*****************************************************************************/
/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */

int main( void )
{
	//===========================================
	uchar   calibrationValue;
    calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
    if(calibrationValue != 0xff){
        OSCCAL = calibrationValue;
    }
	//===========================================
	usbInit();
    wdt_enable(WDTO_1S);
    /* Даже если Вы не используете сторожевой таймер (watchdog), выключите его здесь. На более новых
     *  микроконтроллерах состояние watchdog (вкл\выкл, период) СОХРАНЯЕТСЯ ЧЕРЕЗ СБРОС!
     */
	//===========================================
    usbDeviceDisconnect();
    _delay_ms(300);			/* 300 ms disconnect */
    usbDeviceConnect();
	//===========================================
    //LED_PORT_DDR |= _BV(LED_BIT);		/* делаем ножку, куда подключен LED, выходом */
	sbi(LED_PORT_DDR,LED_BIT);			/* делаем ножку, куда подключен LED, выходом */
	//===========================================
	sei();		/* Разрешаем прерывания*/
    for(;;){    /* main event loop */
        wdt_reset();
        usbPoll();
	}
	//===========================================
}
/*****************************************************************************/
