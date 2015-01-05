//#define F_CPU 20000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>   /* ����� ��� usbdrv.h */
#include "usbdrv.h"


struct dataexchange_t       // �������� ��������� ��� �������� ������
{
   uchar red;        // � ����� ��� ������� �������� ��������� �� 3 �����.
   uchar green;        // �� ������ ���� �������� ���� �� PORTB. ������� ���
   uchar blue;        // �� ����������� (����� �� 3 ���� �����).
};                  // �� � ����� ������������ � ����� ���.
                    // ��� ����������� ���������� �� ���������� � ��������� :)


struct dataexchange_t pdata = {1, 0, 0};
	
#define RED_BIT 3
#define GREEN_BIT 4
#define BLUE_BIT 0


PROGMEM const char usbHidReportDescriptor[22] = { // USB report descriptor         // ���������� ��������� ��������� ������ ������ ��� ������
    0x06, 0x00, 0xff,                       // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                             // USAGE (Vendor Usage 1)
    0xa1, 0x01,                             // COLLECTION (Application)
    0x15, 0x00,                             //    LOGICAL_MINIMUM (0)        // min. �������� ��� ������
    0x26, 0xff, 0x00,                       //    LOGICAL_MAXIMUM (255)      // max. �������� ��� ������, 255 ��� �� ��������, � ����� ��������� � 1 ����
    0x75, 0x08,                             //    REPORT_SIZE (8)            // ���������� ���������� ��������, ��� ������ ������ "�������" 8 ���
    0x95, sizeof(struct dataexchange_t),    //    REPORT_COUNT               // ���������� ������ (� ����� ������� = 3, ��������� ���� ��������� ���������� �� ��� �������)
    0x09, 0x00,                             //    USAGE (Undefined)
    0xb2, 0x02, 0x01,                       //    FEATURE (Data,Var,Abs,Buf)
    0xc0                                    // END_COLLECTION
};
/* ����� �� ������� ������ ���� report, ��-�� ���� �� ����� ������������ report-ID (�� ������ ���� ������ ������).
 * � ��� ������� ��������� 3 ����� ������ (������ ������ REPORT_SIZE = 8 ��� = 1 ����, �� ���������� REPORT_COUNT = 3).
 */


/* ��� ���������� ������ ������ ������� �������� */
static uchar    currentAddress;
static uchar    bytesRemaining;


/* usbFunctionRead() ���������� ����� ���� ����������� ������ ������ �� ����������
 * ��� �������������� ���������� ��. ������������ � usbdrv.h
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    if(len > bytesRemaining)
        len = bytesRemaining;

    uchar *buffer = (uchar*)&pdata;

    if(!currentAddress)        // �� ���� ����� ������ ��� �� ��������.
    {                          // �������� ��������� ��� ��������
        if ( PINB & _BV(RED_BIT) )
            pdata.red = 1;
        else
            pdata.red = 0;


        if ( PINB & _BV(GREEN_BIT) )
            pdata.green = 1;
        else
            pdata.green = 0;


        if ( PINB & _BV(BLUE_BIT) )
            pdata.blue = 1;
        else
            pdata.blue = 0;
    }

    uchar j;
    for(j=0; j<len; j++)
        data[j] = buffer[j+currentAddress];

    currentAddress += len;
    bytesRemaining -= len;
    return len;
}


/* usbFunctionWrite() ���������� ����� ���� ���������� ������ ������ � ����������
 * ��� �������������� ���������� ��. ������������ � usbdrv.h
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* ����� �������� */

    if(len > bytesRemaining)
        len = bytesRemaining;

    uchar *buffer = (uchar*)&pdata;
    
    uchar j;
    for(j=0; j<len; j++)
        buffer[j+currentAddress] = data[j];

    currentAddress += len;
    bytesRemaining -= len;

    if(bytesRemaining == 0)     // ��� ������ ��������
    {                           // �������� �������� �� PORTB
        if ( pdata.red )
            PORTB |= _BV(RED_BIT);
        else
            PORTB &= ~_BV(RED_BIT);


        if ( pdata.green )
            PORTB |= _BV(GREEN_BIT);
        else
            PORTB &= ~_BV(GREEN_BIT);


        if ( pdata.blue )
            PORTB |= _BV(BLUE_BIT);
        else
            PORTB &= ~_BV(BLUE_BIT);
    }

    return bytesRemaining == 0; /* 0 ��������, ��� ���� ��� ������ */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = static_cast<usbRequest_t*>((void*)data);

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID ���������� */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            // � ��� ������ ���� ������������� �������, ����� ������������ report-ID
            bytesRemaining = sizeof(struct dataexchange_t);
            currentAddress = 0;
            return USB_NO_MSG;  // ���������� usbFunctionRead() ��� �������� ������ �����
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            // � ��� ������ ���� ������������� �������, ����� ������������ report-ID
            bytesRemaining = sizeof(struct dataexchange_t);
            currentAddress = 0;
            return USB_NO_MSG;  // ���������� usbFunctionWrite() ��� ��������� ������ �� �����
        }
    }else{
        /* ��������� ������� �� ������ ���������� */
    }
    return 0;
}
/* ------------------------------------------------------------------------- */

int main(void)
{
    DDRB = 0b00000110;      // PB1,PB2,PB3 - �����

    usbInit();
    usbDeviceDisconnect();  // ������������� ����������� �� �����, ��� ������ ����� ������ ��� ����������� �����������!
    
    uchar i = 0;
    while(--i){             // ����� > 250 ms
        _delay_ms(1);
    }
    
    usbDeviceConnect();     // ������������

    sei();                  // ��������� ����������

    for(;;){                // ������� ���� ���������
        usbPoll();          // ��� ������� ���� ��������� �������� � �������� �����, ������������ �������� ����� �������� - 50 ms
    }
    return 0;
}
/* ------------------------------------------------------------------------- */ 