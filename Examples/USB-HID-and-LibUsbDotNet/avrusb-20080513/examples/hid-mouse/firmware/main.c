/* ���: main.c
 * ������: hid-����, ����� ������� ������ HID
 * �����: Christian Starkjohann
 * ���� ��������: 2008-04-07
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

�� ���������� ���� VID/PID 0x046D/0xC00E, ������� ������������ ������ � ����� Logitech. 
�� ���������� ����� ������ � �������������� ���� ID! ����� ��� ������������ ������ ��� 
������������!
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* ��� sei() */
#include <util/delay.h>     /* ��� _delay_ms() */

#include <avr/pgmspace.h>   /* ��������� ��� usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h"        /* ���� ����� ������ ��� ������������� ������� ������� */

/* ------------------------------------------------------------------------- */
/* ----------------------------- ��������� USB ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[52] = { /* ���������� ������� USB, ������ ������ ��������������� usbconfig.h */
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
/* ��� �� �� ����� ������ ����������, ����� �� ����� � ���� Logitech. ������,
 *  ��������� � ���� �����������, ������� �� 4 ����:
 *      .  .  .  .  . B2 B1 B0 .... ���� ���� � ���������� ������ ����
 *     X7 X6 X5 X4 X3 X2 X1 X0 .... 8 ��� �������� ����� - ������������� ���������� x
 *     Y7 Y6 Y5 Y4 Y3 Y2 Y1 Y0 .... 8 ��� �������� ����� - ������������� ���������� y
 *     W7 W6 W5 W4 W3 W2 W1 W0 .... 8 ��� �������� ����� - ������������� ���������� ��������
 */
typedef struct{
    uchar   buttonMask;
    char    dx;
    char    dy;
    char    dWheel;
}report_t;

static report_t reportBuffer;
static int      sinus = 7 << 6, cosinus = 0;
static uchar    idleRate;   /* �������� ������� ��� ���������, �� ������������ ��� ���� */


/* ��������� ������� ���������� sin/cos �� ������������� ����
 *  � ��������� ������� �� ���������� ��������� � ����������� �������.
 * �������� - ������������� ����������������� ��������� ������� �������.
 */
static void advanceCircleByFixedAngle(void)
{
char    d;

#define DIVIDE_BY_64(val)  (val + (val > 0 ? 32 : -32)) >> 6    /* ���������� ������� */
    reportBuffer.dx = d = DIVIDE_BY_64(cosinus);
    sinus += d;
    reportBuffer.dy = d = DIVIDE_BY_64(sinus);
    cosinus -= d;
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    /* ��������� ������� �� ������������. ������ ��������� ��� ��������� 
     *  �� ������������, �� ������ �� ����������� � ���� �������.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* ��� ������� class */
        DBG1(0x50, &rq->bRequest, 1);   /* ���������� �����: ������ ������ ������� */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* �� ������ ����� ���� ��� �������, ��� ��� �� ������� �� wValue */
            usbMsgPtr = (void *)&reportBuffer;
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* �� ����������� ������������� ������� ������� */
    }
    return 0;   /* default ��� �� ������������� ��������: �� ���������� ����� ����� ������ */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* ���� ���� �� �� ����������� watchdog, ��������� ��� �����. �� ����� �����������
     *  ��������� watchdog (���/����, ������) ����������� ����� RESET!
     */
    DBG1(0x00, 0, 0);       /* debug output: main starts */
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
        if(usbInterruptIsReady()){
            /* ���������� ����� ������� ������ �������� ����� interrupt */
            advanceCircleByFixedAngle();
            DBG1(0x03, 0, 0);   /* ���������� �����: ����������� interrupt ������ */
            usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
        }
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
