/* Имя: main.c
 * Проект: тестирование возможностей драйвера
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru 
 * Дата создания: 2008-04-29
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
 * Ревизия: $Id: main.c 589 2008-05-03 14:00:19Z cs $
 */

/*
Этот модуль является ничего-не-делающим тестовым кодом, который линкуется (либо включается в код) 
драйвером USB. Модуль используется для определения размера кода для различных опций и проверки,
компилируется ли код со всеми опциями.
*/
#include <avr/io.h>
#include <avr/interrupt.h>  /* для sei() */
#include <avr/pgmspace.h>   /* требуется для usbdrv.h */
#include <util/delay.h>     /* для _delay_ms() */
#include "usbdrv.h"
#if USE_INCLUDE
#include "usbdrv.c"
#endif

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
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

static PROGMEM char myDescriptorDevice[] = {    /* дескриптор устройства USB */
    18,         /* sizeof(usbDescriptorDevice): длина дескриптора в байтах */
    USBDESCR_DEVICE,        /* тип дескриптора */
    0x10, 0x01,             /* поддерживаемая версия USB */
    USB_CFG_DEVICE_CLASS,
    USB_CFG_DEVICE_SUBCLASS,
    0,                      /* протокол */
    8,                      /* max размер пакета */
    /* следующие 2 преобразования типа влияют только на первый байт константы, но этого
     *  достаточно, чтобы избежать предупреждения со значениями по умолчанию.
     */
    (char)USB_CFG_VENDOR_ID,/* 2 bytes */
    (char)USB_CFG_DEVICE_ID,/* 2 bytes */
    USB_CFG_DEVICE_VERSION, /* 2 bytes */
    USB_CFG_DESCR_PROPS_STRING_VENDOR != 0 ? 1 : 0,         /* индекс строки производителя */
    USB_CFG_DESCR_PROPS_STRING_PRODUCT != 0 ? 2 : 0,        /* индекс строки продукта */
    USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER != 0 ? 3 : 0,  /* индекс строки серийного номера */
    1,          /* количество конфигураций */
};

static PROGMEM char myDescriptorConfiguration[] = { /* дескриптор конфигурации USB */
    9,          /* sizeof(usbDescriptorConfiguration): длина дескриптора в байтах */
    USBDESCR_CONFIG,    /* тип дескриптора */
    18 + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT + (USB_CFG_DESCR_PROPS_HID & 0xff), 0,
                /* общая длина возвращаемых данных (включая inline встроенные дескрипторы) */
    1,          /* количество интерфейсов в этой конфигурации */
    1,          /* индекс этой конфигурации */
    0,          /* индекс строки имени конфигурации */
#if USB_CFG_IS_SELF_POWERED
    USBATTR_SELFPOWER,      /* атрибуты */
#else
    (char)USBATTR_BUSPOWER, /* атрибуты */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max потребляемый ток от USB в единицах 2 мА */
/* дескриптор интерфейса следует inline: */
    9,          /* sizeof(usbDescrInterface): длина дескриптора в байтах */
    USBDESCR_INTERFACE, /* тип дескриптора */
    0,          /* индекс этого интерфейса */
    0,          /* альтернативные установки для этого интерфейса */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* конечные точки за исключением 0: количество следующих endpoint-дескрипторов */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* индекс строки для интерфейса */
#if (USB_CFG_DESCR_PROPS_HID & 0xff)    /* дескриптор HID */
    9,          /* sizeof(usbDescrHID): длина дескриптора в байтах */
    USBDESCR_HID,   /* тип дескриптора: HID */
    0x01, 0x01, /* BCD представление версии HID */
    0x00,       /* код целевой страны */
    0x01,       /* номер последующего HID дескриптора репорта (или другого HID class)  */
    0x22,       /* тип дескриптора: репорт */
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  /* общая длина дескриптора репорта */
#endif
#if USB_CFG_HAVE_INTRIN_ENDPOINT    /* дескриптор конечной точки 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* тип дескриптора = endpoint */
    (char)0x81, /* IN endpoint номер 1 */
    0x03,       /* атрибут: конечная точка Interrupt */
    8, 0,       /* максимальний размер пакета */
    USB_CFG_INTR_POLL_INTERVAL, /* в милисекундах */
#endif
};

USB_PUBLIC usbMsgLen_t usbFunctionDescriptor(usbRequest_t *rq)
{
uchar *p = 0, len = 0;

    if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
        p = (uchar *)myDescriptorDevice;
        len = sizeof(myDescriptorDevice);
    }else{  /* должен быть дескриптор конфигурации */
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

    if(rq->bRequest == 0)   /* запрос с использованием usbFunctionRead()/usbFunctionWrite() */
        return 0xff;
	return 0;   /* умолчание для нереализованных запросов: данные назад хосту не возвращаются */
}

/* ------------------------------------------------------------------------- */

int	main(void)
{
uchar   i;

	usbInit();
    usbDeviceDisconnect();  /* запускаем принудительно переэнумерацию, делайте это когда прерывания запрещены! */
    i = 0;
    while(--i){             /* иммитируем USB дисконнект на время > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
	sei();
	for(;;){                /* основной цикл обработки событий */
		usbPoll();
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
