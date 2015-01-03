/* ���: main.c
 * ������: custom-class, ������� ������ ���������� USB
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2008-04-09
 * ���������: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: main.c 602 2008-05-13 15:15:13Z cs $
 */

/*
���� ������ ������ �������� �� ������ AVR � ���������� �����������. �� ������������
������� ����������� ���������� �������, ����� INT0. �� ������ �������� usbconfig.h ���
������ ����� I/O USB. ����������, ������ � ����, ��� USB D+ ������ ���� ����������� �
����� INT0. �� ������������, ��� LED ����������� �� ���� B ������ 0. ���� �� ������������
��� �� ������ �����, ��������� ������ ����:
*/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_BIT             0

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* ��� sei() */
#include <util/delay.h>     /* ��� _delay_ms() */

#include <avr/pgmspace.h>   /* ��������� ��� usbdrv.h */
#include "usbdrv.h"
#include "requests.h"       /* �� ���������� ���������������� ������ �������� */

/* ------------------------------------------------------------------------- */
/* ----------------------------- ��������� USB ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;
static uchar    dataBuffer[4];  /* ����� ������ ���������� �������� ��� �������� �� usbFunctionSetup */

    if(rq->bRequest == CUSTOM_RQ_ECHO)
    { /* ��� -- ������������ ��� ������ ���������� */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* ������� ��������, ����� ������ ����� ���������� */
        return 4;
    }
    else if(rq->bRequest == CUSTOM_RQ_SET_STATUS)
    {
        if(rq->wValue.bytes[0] & 1){    /* ������������� LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        }else{                          /* ������� LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    }
    else if(rq->bRequest == CUSTOM_RQ_GET_STATUS)
    {
        dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
        usbMsgPtr = dataBuffer;          /* ������� ��������, ����� ������ ����� ���������� */
        return 1;                       /* ������� �������� ������� 1 ���� */
    }
    return 0;   /* default ��� �� ������������� ��������: �� ���� ������� ������ �� ������������ */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* ���� �� ������������ watchdog, ��������� ��� �������. ��� ����� ���������
        ������ watchdog (on/off, period) ����������� ����� ������!
     */
    /* ������ RESET: ��� ����� ������ � ������ ����� ��� ����������� ���������� (pull-up).
        ��� ��, ��� ����� ��� D+ and D-, ����� �������, �� �� ��������� � �������������� ����������
        �������������.
     */
    usbInit();              // ��. usbdrv.h � usbdrv.c
    usbDeviceDisconnect();  /* ��. usbdrv.h - ��������� ������������, ������ ���, ���� ��������� ����������! */
    i = 0;
    while(--i)
    {            /* ���������� USB disconnect �� ����� > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();     // ��. usbdrv.h
    LED_PORT_DDR |= _BV(LED_BIT);   /* ����������� ����� LED � ����� ������ */
    sei();
    for(;;){                /* ������� ���� ������� */
        wdt_reset();
        usbPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
