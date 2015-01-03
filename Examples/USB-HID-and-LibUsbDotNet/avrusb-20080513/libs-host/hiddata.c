/* ИМя: hiddata.c
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru 
 * Дата создания: 2008-04-11
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
 * Ревизия: $Id: hiddata.c 553 2008-04-17 19:00:20Z cs $
 */

#include <stdio.h>
#include "hiddata.h"

/* ######################################################################## */
#if defined(WIN32) /* ##################################################### */
/* ######################################################################## */

#include <windows.h>
#include <setupapi.h>
#include "hidsdi.h"
#include <ddk/hidpi.h>

#ifdef DEBUG
#define DEBUG_PRINT(arg)    printf arg
#else
#define DEBUG_PRINT(arg)
#endif

/* ------------------------------------------------------------------------ */

static void convertUniToAscii(char *buffer)
{
unsigned short  *uni = (void *)buffer;
char            *ascii = buffer;

    while(*uni != 0){
        if(*uni >= 256){
            *ascii++ = '?';
        }else{
            *ascii++ = *uni++;
        }
    }
    *ascii++ = 0;
}

int usbhidOpenDevice(usbDevice_t **device, int vendor, char *vendorName, int product, char *productName, int usesReportIDs)
{
GUID                                hidGuid;        /* GUID для драйвера HID */
HDEVINFO                            deviceInfoList;
SP_DEVICE_INTERFACE_DATA            deviceInfo;
SP_DEVICE_INTERFACE_DETAIL_DATA     *deviceDetails = NULL;
DWORD                               size;
int                                 i, openFlag = 0;  /* может быть FILE_FLAG_OVERLAPPED */
int                                 errorCode = USBOPEN_ERR_NOTFOUND;
HANDLE                              handle = INVALID_HANDLE_VALUE;
HIDD_ATTRIBUTES                     deviceAttributes;
				
    HidD_GetHidGuid(&hidGuid);
    deviceInfoList = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
    deviceInfo.cbSize = sizeof(deviceInfo);
    for(i=0;;i++){
        if(handle != INVALID_HANDLE_VALUE){
            CloseHandle(handle);
            handle = INVALID_HANDLE_VALUE;
        }
        if(!SetupDiEnumDeviceInterfaces(deviceInfoList, 0, &hidGuid, i, &deviceInfo))
            break;  /* больше входов нет */
        /* сначала пустой (dummy) вызов только для того, чтобы определить действительную требуемую длину */
        SetupDiGetDeviceInterfaceDetail(deviceInfoList, &deviceInfo, NULL, 0, &size, NULL);
        if(deviceDetails != NULL)
            free(deviceDetails);
        deviceDetails = malloc(size);
        deviceDetails->cbSize = sizeof(*deviceDetails);
        /* этот вызов по-настоящему: */
        SetupDiGetDeviceInterfaceDetail(deviceInfoList, &deviceInfo, deviceDetails, size, &size, NULL);
        DEBUG_PRINT(("checking HID path \"%s\"\n", deviceDetails->DevicePath));
        /* попытка открытия на чтение/запись (R/W) -- мы не заботимся об устройствах, которые могут не давать доступ */
        handle = CreateFile(deviceDetails->DevicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, openFlag, NULL);
        if(handle == INVALID_HANDLE_VALUE){
            DEBUG_PRINT(("opening failed: %d\n", (int)GetLastError()));
            /* errorCode = USBOPEN_ERR_ACCESS; открытие будет всегда ошибочным для мыши  -- игнорируем */
            continue;
        }
        deviceAttributes.Size = sizeof(deviceAttributes);
        HidD_GetAttributes(handle, &deviceAttributes);
        DEBUG_PRINT(("device attributes: vid=%d pid=%d\n", deviceAttributes.VendorID, deviceAttributes.ProductID));
        if(deviceAttributes.VendorID != vendor || deviceAttributes.ProductID != product)
            continue;   /* игнорируем это устройство */
        errorCode = USBOPEN_ERR_NOTFOUND;
        if(vendorName != NULL && productName != NULL){
            char    buffer[512];
            if(!HidD_GetManufacturerString(handle, buffer, sizeof(buffer))){
                DEBUG_PRINT(("error obtaining vendor name\n"));
                errorCode = USBOPEN_ERR_IO;
                continue;
            }
            convertUniToAscii(buffer);
            DEBUG_PRINT(("vendorName = \"%s\"\n", buffer));
            if(strcmp(vendorName, buffer) != 0)
                continue;
            if(!HidD_GetProductString(handle, buffer, sizeof(buffer))){
                DEBUG_PRINT(("error obtaining product name\n"));
                errorCode = USBOPEN_ERR_IO;
                continue;
            }
            convertUniToAscii(buffer);
            DEBUG_PRINT(("productName = \"%s\"\n", buffer));
            if(strcmp(productName, buffer) != 0)
                continue;
        }
        break;  /* мы нашли устройство, которое ищем! */
    }
    SetupDiDestroyDeviceInfoList(deviceInfoList);
    if(deviceDetails != NULL)
        free(deviceDetails);
    if(handle != INVALID_HANDLE_VALUE){
        *device = (usbDevice_t *)handle;
        errorCode = 0;
    }
    return errorCode;
}

/* ------------------------------------------------------------------------ */

void    usbhidCloseDevice(usbDevice_t *device)
{
    CloseHandle((HANDLE)device);
}

/* ------------------------------------------------------------------------ */

int usbhidSetReport(usbDevice_t *device, char *buffer, int len)
{
BOOLEAN rval;

    rval = HidD_SetFeature((HANDLE)device, buffer, len);
    return rval == 0 ? USBOPEN_ERR_IO : 0;
}

/* ------------------------------------------------------------------------ */

int usbhidGetReport(usbDevice_t *device, int reportNumber, char *buffer, int *len)
{
BOOLEAN rval = 0;

    buffer[0] = reportNumber;
    rval = HidD_GetFeature((HANDLE)device, buffer, *len);
    return rval == 0 ? USBOPEN_ERR_IO : 0;
}

/* ------------------------------------------------------------------------ */

/* ######################################################################## */
#else /* defined WIN32 #################################################### */
/* ######################################################################## */

#include <string.h>
#include <usb.h>

#define usbDevice   usb_dev_handle  /* использование структуры устройства libusb */

/* ------------------------------------------------------------------------- */

#define USBRQ_HID_GET_REPORT    0x01
#define USBRQ_HID_SET_REPORT    0x09

#define USB_HID_REPORT_TYPE_FEATURE 3


static int  usesReportIDs;

/* ------------------------------------------------------------------------- */

static int usbhidGetStringAscii(usb_dev_handle *dev, int index, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_get_string_simple(dev, index, buf, buflen)) >= 0) /* используйте libusb версию, если это работает */
        return rval;
    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, 0x0409, buffer, sizeof(buffer), 5000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING){
        *buf = 0;
        return 0;
    }
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    /* преобразование с потерями (lossy) к ISO Latin1: */
    for(i=1;i<rval;i++){
        if(i > buflen)              /* переполнение целевого буфера */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* вне диапазона ISO Latin1 */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}

int usbhidOpenDevice(usbDevice_t **device, int vendor, char *vendorName, int product, char *productName, int _usesReportIDs)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *handle = NULL;
int                 errorCode = USBOPEN_ERR_NOTFOUND;
static int          didUsbInit = 0;

    if(!didUsbInit){
        usb_init();
        didUsbInit = 1;
    }
    usb_find_busses();
    usb_find_devices();
    for(bus=usb_get_busses(); bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product){
                char    string[256];
                int     len;
                handle = usb_open(dev); /* нам нужно открыть устройство для того, чтобы запросить строки */
                if(!handle){
                    errorCode = USBOPEN_ERR_ACCESS;
                    fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){  /* имя не имеет значения */
                    break;
                }
                /* теперь проверим, совпадает ли имя: */
                len = usbhidGetStringAscii(handle, dev->descriptor.iManufacturer, string, sizeof(string));
                if(len < 0){
                    errorCode = USBOPEN_ERR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                }else{
                    errorCode = USBOPEN_ERR_NOTFOUND;
                    /* fprintf(stderr, "seen device from vendor ->%s<-\n", string); */
                    if(strcmp(string, vendorName) == 0){
                        len = usbhidGetStringAscii(handle, dev->descriptor.iProduct, string, sizeof(string));
                        if(len < 0){
                            errorCode = USBOPEN_ERR_IO;
                            fprintf(stderr, "Warning: cannot query product for device: %s\n", usb_strerror());
                        }else{
                            errorCode = USBOPEN_ERR_NOTFOUND;
                            /* fprintf(stderr, "seen product ->%s<-\n", string); */
                            if(strcmp(string, productName) == 0)
                                break;
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
        *device = (void *)handle;
        usesReportIDs = _usesReportIDs;
    }
    return errorCode;
}

/* ------------------------------------------------------------------------- */

void    usbhidCloseDevice(usbDevice_t *device)
{
    if(device != NULL)
        usb_close((void *)device);
}

/* ------------------------------------------------------------------------- */

int usbhidSetReport(usbDevice_t *device, char *buffer, int len)
{
int bytesSent;

    if(!usesReportIDs){
        buffer++;   /* пропускаем пустой (dummy) репорт ID */
        len--;
    }
    bytesSent = usb_control_msg((void *)device, USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_OUT, USBRQ_HID_SET_REPORT, USB_HID_REPORT_TYPE_FEATURE << 8 | (buffer[0] & 0xff), 0, buffer, len, 5000);
    if(bytesSent != len){
        if(bytesSent < 0)
            fprintf(stderr, "Error sending message: %s\n", usb_strerror());
        return USBOPEN_ERR_IO;
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

int usbhidGetReport(usbDevice_t *device, int reportNumber, char *buffer, int *len)
{
int bytesReceived, maxLen = *len;

    if(!usesReportIDs){
        buffer++;   /* оставим место для пустого (dummy) репорт ID */
        maxLen--;
    }
    bytesReceived = usb_control_msg((void *)device, USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_IN, USBRQ_HID_GET_REPORT, USB_HID_REPORT_TYPE_FEATURE << 8 | reportNumber, 0, buffer, maxLen, 5000);
    if(bytesReceived < 0){
        fprintf(stderr, "Error sending message: %s\n", usb_strerror());
        return USBOPEN_ERR_IO;
    }
    *len = bytesReceived;
    if(!usesReportIDs){
        buffer[-1] = reportNumber;  /* добавляем dummy репорт ID */
        *len++;
    }
    return 0;
}

/* ######################################################################## */
#endif /* defined WIN32 ################################################### */
/* ######################################################################## */
