/* ���: main.c
 * ������: ������ hid-data, ������������ ��� ������������ HID ��� �������� ������
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2008-04-11
 * ���������: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: main.c 592 2008-05-04 20:07:30Z cs $
 */

/*
���� ������ ������ �������� �� ������ AVR � ���������� �����������. �� ������������
������� ����������� ���������� �������, ����� INT0. �� ������ �������� usbconfig.h ���
������ ����� I/O USB. ����������, ������ � ����, ��� USB D+ ������ ���� ����������� �
����� INT0, ���� ��� ������� ����� ������ ���� �������� � INT0.
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* ��� sei() */
#include <util/delay.h>     /* ��� _delay_ms() */
#include <avr/eeprom.h>

#include <avr/pgmspace.h>   /* ��������� ��� usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* ���� ����� ������ ��� ������������� ������� ������� */

/* ------------------------------------------------------------------------- */
/* ----------------------------- ��������� USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[22] = {    /* ���������� ������� USB */
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
/* ��������� �� ������ ������ ���� feature-������, �� �� ���������� �������������� 
 *  report-ID (������� ������ ���� � ������ ����� �������). ���� ������ ������� �� 128
 *  opaque ���� ������.
 */

/* ��������� ���������� ��������� ��������� ������� �������� ������ */
static uchar    currentAddress;
static uchar    bytesRemaining;

/* ------------------------------------------------------------------------- */

/* usbFunctionRead() ����������, ����� ���� ����������� ����� ������ �� ����������.
 *  ��� ������� ���������� ��. ������������ � usbdrv/usbdrv.h.
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

/* usbFunctionWrite() ����������, ����� ���� �������� ����� ������ � ����������.
 *  ��� ������� ���������� ��. ������������ � usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* ��������� �������� */
    if(len > bytesRemaining)
        len = bytesRemaining;
    eeprom_write_block(data, (uchar *)0 + currentAddress, len);
    currentAddress += len;
    bytesRemaining -= len;
    return bytesRemaining == 0; /* ������� 1, ���� ��� ��� ��������� ����� */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* ������ HID class */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* ��������� �� ����� ������ ���� ��� �������, �� ����� ������������ ������-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* ������������� usbFunctionRead() ��� ��������� ������ ������ �� ���������� */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* ��������� �� ����� ������ ���� ��� �������, �� ����� ������������ ������-ID */
            bytesRemaining = 128;
            currentAddress = 0;
            return USB_NO_MSG;  /* use usbFunctionWrite() ��� ��������� ������ ����������� �� ����� */
        }
    }else{
        /* ���������� ������� ���� �������, �� �� ��� ����� �� ���������� */
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* ���� ���� �� �� ����������� watchdog, ��������� ��� �����. �� ����� �����������
     *  ��������� watchdog (���/����, ������) ����������� ����� RESET!
     */
    DBG1(0x00, 0, 0);       /* ���������� �����: �������� ���� main */
    /* ������ RESET: ��� ���� ����� �������� ��� ����� ��� ����������� ���������� (pull-up).
     * ��� ��, ��� ����� ��� D+ � D-. ����� �������, ��� �� ����� �������������� ������������� ������.
     */
    odDebugInit();
    usbInit();
    usbDeviceDisconnect();  /* ��������� ������������� ������������, ������� ���, ����� ���������� ���������! */
    i = 0;
    while(--i){             /* ��������� USB ���������� �� ����� > 250 �� */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();
    DBG1(0x01, 0, 0);       /* ���������� �����: ���� � ���� main */
    for(;;){                /* ���� ������� main */
        DBG1(0x02, 0, 0);   /* ���������� �����: ������� ����� main */
        wdt_reset();
        usbPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
