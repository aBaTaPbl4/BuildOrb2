/* ���: main.c
 * ������: ������������ ������������ ��������
 * �����: Christian Starkjohann
 * �������: microsin.ru 
 * ���� ��������: 2008-04-29
 * ���������: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
 * �������: $Id: main.c 589 2008-05-03 14:00:19Z cs $
 */

/*
���� ������ �������� ������-��-�������� �������� �����, ������� ��������� (���� ���������� � ���) 
��������� USB. ������ ������������ ��� ����������� ������� ���� ��� ��������� ����� � ��������,
������������� �� ��� �� ����� �������.
*/
#include <avr/io.h>
#include <avr/interrupt.h>  /* ��� sei() */
#include <avr/pgmspace.h>   /* ��������� ��� usbdrv.h */
#include <util/delay.h>     /* ��� _delay_ms() */
#include "usbdrv.h"
#if USE_INCLUDE
#include "usbdrv.c"
#endif

/* ------------------------------------------------------------------------- */
/* ----------------------------- ��������� USB ----------------------------- */
/* ------------------------------------------------------------------------- */

#if USB_CFG_IMPLEMENT_FN_WRITE
uchar usbFunctionWrite(uchar *data, uchar len)
{
    return 1;
}
#endif

#if USB_CFG_IMPLEMENT_FN_READ
uchar usbFunctionRead(uchar *data, uchar len)
{
    return len;
}
#endif

#if USB_CFG_IMPLEMENT_FN_WRITEOUT
void usbFunctionWriteOut(uchar *data, uchar len)
{
}
#endif

#if USE_DYNAMIC_DESCRIPTOR

static PROGMEM char myDescriptorDevice[] = {    /* ���������� ���������� USB */
    18,         /* sizeof(usbDescriptorDevice): ����� ����������� � ������ */
    USBDESCR_DEVICE,        /* ��� ����������� */
    0x10, 0x01,             /* �������������� ������ USB */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,                      /* �������� */
    8,                      /* max ������ ������ */
    /* ��������� 2 �������������� ���� ������ ������ �� ������ ���� ���������, �� �����
     *  ����������, ����� �������� �������������� �� ���������� �� ���������.
     */
    (char)USB_CFG_VENDOR_ID,/* 2 bytes */
    (char)USB_CFG_DEVICE_ID,/* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    USB_CFG_DESCR_PROPS_STRING_VENDOR != 0 ? 1 : 0,         /* ������ ������ ������������� */
    USB_CFG_DESCR_PROPS_STRING_PRODUCT != 0 ? 2 : 0,        /* ������ ������ �������� */
    USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER != 0 ? 3 : 0,  /* ������ ������ ��������� ������ */
    1,          /* ���������� ������������ */
};

static PROGMEM char myDescriptorConfiguration[] = { /* ���������� ������������ USB */
    9,          /* sizeof(usbDescriptorConfiguration): ����� ����������� � ������ */
    USBDESCR_CONFIG,    /* ��� ����������� */
    18 + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT + (USB_CFG_DESCR_PROPS_HID & 0xff), 0,
                /* ����� ����� ������������ ������ (������� inline ���������� �����������) */
    1,          /* ���������� ����������� � ���� ������������ */
    1,          /* ������ ���� ������������ */
    0,          /* ������ ������ ����� ������������ */
#if USB_CFG_IS_SELF_POWERED
    USBATTR_SELFPOWER,      /* �������� */
#else
    (char)USBATTR_BUSPOWER, /* �������� */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max ������������ ��� �� USB � �������� 2 �� */
/* ���������� ���������� ������� inline: */
    9,          /* sizeof(usbDescrInterface): ����� ����������� � ������ */
    USBDESCR_INTERFACE, /* ��� ����������� */
    0,          /* ������ ����� ���������� */
    0,          /* �������������� ��������� ��� ����� ���������� */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* �������� ����� �� ����������� 0: ���������� ��������� endpoint-������������ */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* ������ ������ ��� ���������� */
#if (USB_CFG_DESCR_PROPS_HID & 0xff)    /* ���������� HID */
    9,          /* sizeof(usbDescrHID): ����� ����������� � ������ */
    USBDESCR_HID,   /* ��� �����������: HID */
    0x01, 0x01, /* BCD ������������� ������ HID */
    0x00,       /* ��� ������� ������ */
    0x01,       /* ����� ������������ HID ����������� ������� (��� ������� HID class)  */
    0x22,       /* ��� �����������: ������ */
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  /* ����� ����� ����������� ������� */
#endif
#if USB_CFG_HAVE_INTRIN_ENDPOINT    /* ���������� �������� ����� 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* ��� ����������� = endpoint */
    (char)0x81, /* IN endpoint ����� 1 */
    0x03,       /* �������: �������� ����� Interrupt */
    8, 0,       /* ������������ ������ ������ */
    USB_CFG_INTR_POLL_INTERVAL, /* � ������������ */
#endif
};

USB_PUBLIC usbMsgLen_t usbFunctionDescriptor(usbRequest_t *rq)
{
uchar *p = 0, len = 0;

    if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
        p = (uchar *)myDescriptorDevice;
        len = sizeof(myDescriptorDevice);
    }else{  /* ������ ���� ���������� ������������ */
        p = (uchar *)(myDescriptorConfiguration);
        len = sizeof(myDescriptorConfiguration);
    }
    usbMsgPtr = p;
    return len;
}
#endif

USB_PUBLIC usbMsgLen_t  usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if(rq->bRequest == 0)   /* ������ � �������������� usbFunctionRead()/usbFunctionWrite() */
        return 0xff;
	return 0;   /* ��������� ��� ��������������� ��������: ������ ����� ����� �� ������������ */
}

/* ------------------------------------------------------------------------- */

int	main(void)
{
uchar   i;

	usbInit();
    usbDeviceDisconnect();  /* ��������� ������������� ��������������, ������� ��� ����� ���������� ���������! */
    i = 0;
    while(--i){             /* ���������� USB ���������� �� ����� > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
	sei();
	for(;;){                /* �������� ���� ��������� ������� */
		usbPoll();
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
