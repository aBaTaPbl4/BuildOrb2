/* Имя: main.c
 * Проект: пример hid-data, показывающий как использовать HID для передачи данных
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru 
 * Дата создания: 2008-04-11
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
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* для sei() */
#include <util/delay.h>     /* для _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* требуется для usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* Этот также пример для использования макроса отладки */

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[22] = {    /* дескриптор репорта USB */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};
/* Поскольку мы задали только один feature-репорт, мы не используем идентификаторы 
 *  report-ID (которые должны быть в первом байте репорта). Весь репорт состоит из 128
 *  opaque байт данных.
 */

/* Следующие переменные сохраняют состояние текущей передачи данных */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() вызывается, когда хост запрашивает кусок данных от устройства.
 *  Для большей информации см. документацию в usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_read_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return len;
}

/* usbFunctionWrite() вызывается, когда хост посылает кусок данных в устройство.
 *  Для большей информации см. документацию в usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* окончание передачи */
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; /* возврат 1, если это был последний кусок */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* запрос HID class */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* поскольку мы имеем только один тип репорта, мы можем игнорировать репорт-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* использование usbFunctionRead() для получения данных хостом от устройства */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* поскольку мы имеем только один тип репорта, мы можем игнорировать репорт-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() для получения данных устройством от хоста */
        }
    }else{
        /* игнорируем запросы типа вендора, мы их все равно не используем */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* Даже если Вы не используете watchdog, выключите его здесь. На новых устройствах
     *  состояние watchdog (вкл/выкл, период) СОХРАНЯЮТСЯ ЧЕРЕЗ RESET!
     */
    DBG1(0x00, 0, 0);       /* отладочный вывод: стартует тело main */
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
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
