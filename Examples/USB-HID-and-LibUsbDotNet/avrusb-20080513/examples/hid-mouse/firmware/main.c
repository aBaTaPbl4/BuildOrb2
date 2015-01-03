/* Имя: main.c
 * Проект: hid-мышь, очень простой пример HID
 * Автор: Christian Starkjohann
 * Дата создания: 2008-04-07
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
 * Ревизия: $Id: main.c 592 2008-05-04 20:07:30Z cs $
 */

/*
Этот пример должен работать на многих AVR с небольшими изменениями. Не используются
никакие специальные аппаратные ресурсы, кроме INT0. Вы можете изменить usbconfig.h для
других ножек I/O USB. Пожалуйста, имейте в виду, что USB D+ должен быть подсоединен к
ножке INT0, либо как минимум также должен быть соединен с INT0.

Мы используем пару VID/PID 0x046D/0xC00E, которая поставляется вместе с мышью Logitech. 
Не публикуйте любое железо с использованием этих ID! Здесь они используются только для 
демонстрации!
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* для sei() */
#include <util/delay.h>     /* для _delay_ms() */

#include <avr/pgmspace.h>   /* требуется для usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* Этот также пример для использования макроса отладки */

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[52] = { /* дескриптор репорта USB, размер должен соответствовать usbconfig.h */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};
/* Это то же самый репорт дескриптор, какой мы видим в мыши Logitech. Данные,
 *  указанные в этом дескрипторе, состоят из 4 байт:
 *      .  .  .  .  . B2 B1 B0 .... один байт с сосояниями кнопки мыши
 *     X7 X6 X5 X4 X3 X2 X1 X0 .... 8 бит знаковое число - относительная координата x
 *     Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 .... 8 бит знаковое число - относительная координата y
 *     W7 W6 W5 W4 W3 W2 W1 W0 .... 8 бит знаковое число - относительная координата колесика
 */
typedef struct{
    uchar   buttonMask;
    char    dx;
    char    dy;
    char    dWheel;
}report_t;

static report_t reportBuffer;
static int      sinus = 7 << 6, cosinus = 0;
static uchar    idleRate;   /* скорость повтора для клавиатур, не используется для мыши */


/* Следующая функция продвигает sin/cos на фиксированный угол
 *  и сохраняет отличие от предыдущих координат в дескрипторе репорта.
 * Алгоритм - моделирование дифференциального уравнения второго порядка.
 */
static void advanceCircleByFixedAngle(void)
{
char    d;

#define DIVIDE_BY_64(val)  (val + (val > 0 ? 32 : -32)) >> 6    /* округление деления */
    reportBuffer.dx = d = DIVIDE_BY_64(cosinus);
    sinus += d;
    reportBuffer.dy = d = DIVIDE_BY_64(sinus);
    cosinus -= d;
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    /* Следующие запросы не используются. Однако поскольку они требуются 
     *  по спецификации, мы должны их реализовать в этом примере.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* тип запроса class */
        DBG1(0x50, &rq->bRequest, 1);   /* отладочный вывод: печать нашего запроса */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* мы только имеем один тип репорта, так что не смотрим на wValue */
            usbMsgPtr = (void *)&reportBuffer;
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* не реализованы специфические запросы вендора */
    }
    return 0;   /* default для не реализованных запросов: не возвращаем хосту назад данные */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* Даже если Вы не используете watchdog, выключите его здесь. На новых устройствах
     *  состояние watchdog (вкл/выкл, период) СОХРАНЯЮТСЯ ЧЕРЕЗ RESET!
     */
    DBG1(0x00, 0, 0);       /* debug output: main starts */
    /* статус RESET: все биты порта работают как входы без нагрузочных резисторов (pull-up).
     * Это то, что нужно для D+ и D-. Таким образом, нам не нужна дополнительная инициализация портов.
     */
    odDebugInit();
    usbInit();
    usbDeviceDisconnect();  /* запускаем принудительно реэнумерацию, делайте это, когда прерывания запрещены! */
    i = 0;
    while(--i){             /* эмулируем USB дисконнект на время > 250 мс */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();
    DBG1(0x01, 0, 0);       /* отладочный вывод: вход в цикл main */
    for(;;){                /* цикл событий main */
        DBG1(0x02, 0, 0);   /* отладочный вывод: повторы цикла main */
        wdt_reset();
        usbPoll();
        if(usbInterruptIsReady()){
            /* вызывается после каждого опроса конечной точки interrupt */
            advanceCircleByFixedAngle();
            DBG1(0x03, 0, 0);   /* отладочный вывод: подготовлен interrupt репорт */
            usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
        }
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
