/* ���: opendevice.c
 * ������: ���������� ����� AVR-USB
 * �����: Christian Starkjohann
 * �������: microsin.ru
 * ���� ��������: 2008-04-10
 * ���������: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt) 
 * �������: $Id: opendevice.c 553 2008-04-17 19:00:20Z cs $
 */

/*
�������� ��������:
������� �� ����� ������ ����� �������������� ��� ���������� � �������� ����������, ����������� �� 
libusb ��� libusb-win32.
*/

#include <stdio.h>
#include "opendevice.h"

/* ------------------------------------------------------------------------- */

#define MATCH_SUCCESS			1
#define MATCH_FAILED			0
#define MATCH_ABORT				-1

/* ������� ��������� (private interface): �������� �� ����������� text and p, ������� MATCH_SUCCESS, MATCH_FAILED ��� MATCH_ABORT. */
static int  _shellStyleMatch(char *text, char *p)
{
int last, matched, reverse;

    for(; *p; text++, p++){
        if(*text == 0 && *p != '*')
            return MATCH_ABORT;
        switch(*p){
        case '\\':
            /* ��������� (literal) ������������ �� ��������� ��������. */
            p++;
            /* FALLTHROUGH */
        default:
            if(*text != *p)
                return MATCH_FAILED;
            continue;
        case '?':
            /* ����������� � ����� ��������. */
            continue;
        case '*':
            while(*++p == '*')
                /* ��������� ���� �� ������ ��������� ����������� ��� ����. */
                continue;
            if(*p == 0)
                /* ��������� ��������� ��������� � ����� ������� ��������. */
                return MATCH_SUCCESS;
            while(*text)
                if((matched = _shellStyleMatch(text++, p)) != MATCH_FAILED)
                    return matched;
            return MATCH_ABORT;
        case '[':
            reverse = p[1] == '^';
            if(reverse) /* ��������������� ����� �������. */
                p++;
            matched = MATCH_FAILED;
            if(p[1] == ']' || p[1] == '-')
                if(*++p == *text)
                    matched = MATCH_SUCCESS;
            for(last = *p; *++p && *p != ']'; last = *p)
                if (*p == '-' && p[1] != ']' ? *text <= *++p && *text >= last : *text == *p)
                    matched = MATCH_SUCCESS;
            if(matched == reverse)
                return MATCH_FAILED;
            continue;
        }
    }
    return *text == 0;
}

/* ��������� ��������� (public interface) ��� ���������� � ����� ����� (shell): ������� 0 ���� ��� ����������, 1 ���� ���������� */
static int shellStyleMatch(char *text, char *pattern)
{
    if(pattern == NULL) /* ����� NULL �������� ��������� ��������� "*" */
        return 1;
    return _shellStyleMatch(text, pattern) == MATCH_SUCCESS;
}

/* ------------------------------------------------------------------------- */

int usbGetStringAscii(usb_dev_handle *dev, int index, char *buf, int buflen)
{
char    buffer[256];
int     rval, i;

    if((rval = usb_get_string_simple(dev, index, buf, buflen)) >= 0) /* ���� ��� ��������, ����������� ������ libusb */
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
    /* �������������� � �������� � ISO Latin1: */
    for(i=1;i<rval;i++){
        if(i > buflen)              /* ������������ ������ ���������� */
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0)  /* ����� �� ������� ISO Latin1 */
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}

/* ------------------------------------------------------------------------- */

int usbOpenDevice(usb_dev_handle **device, int vendorID, char *vendorNamePattern, int productID, char *productNamePattern, char *serialNamePattern, FILE *printMatchingDevicesFp, FILE *warningsFp)
{
struct usb_bus      *bus;
struct usb_device   *dev;
usb_dev_handle      *handle = NULL;
int                 errorCode = USBOPEN_ERR_NOTFOUND;

    usb_find_busses();
    usb_find_devices();
    for(bus = usb_get_busses(); bus; bus = bus->next){
        for(dev = bus->devices; dev; dev = dev->next){  /* ���������������� ����� ���� ��������� �� ���� ����� */
            if((vendorID == 0 || dev->descriptor.idVendor == vendorID)
                        && (productID == 0 || dev->descriptor.idProduct == productID)){
                char    vendor[256], product[256], serial[256];
                int     len;
                handle = usb_open(dev); /* ��� ����� ������� ���������� � ����������� ������� ������� */
                if(!handle)
                {
                    errorCode = USBOPEN_ERR_ACCESS;
                    if(warningsFp != NULL)
                        fprintf(warningsFp, "Warning: cannot open VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
                    continue;
                }
                /* ������ ��������� ����� �� ����������: */
                len = vendor[0] = 0;
                if(dev->descriptor.iManufacturer > 0){
                    len = usbGetStringAscii(handle, dev->descriptor.iManufacturer, vendor, sizeof(vendor));
                }
                if(len < 0){
                    errorCode = USBOPEN_ERR_ACCESS;
                    if(warningsFp != NULL)
                        fprintf(warningsFp, "Warning: cannot query manufacturer for VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
                }else{
                    errorCode = USBOPEN_ERR_NOTFOUND;
                    /* printf("seen device from vendor ->%s<-\n", vendor); */
                    if(shellStyleMatch(vendor, vendorNamePattern)){
                        len = product[0] = 0;
                        if(dev->descriptor.iProduct > 0){
                            len = usbGetStringAscii(handle, dev->descriptor.iProduct, product, sizeof(product));
                        }
                        if(len < 0){
                            errorCode = USBOPEN_ERR_ACCESS;
                            if(warningsFp != NULL)
                                fprintf(warningsFp, "Warning: cannot query product for VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
                        }else{
                            errorCode = USBOPEN_ERR_NOTFOUND;
                            /* printf("seen product ->%s<-\n", product); */
                            if(shellStyleMatch(product, productNamePattern)){
                                len = serial[0] = 0;
                                if(dev->descriptor.iSerialNumber > 0){
                                    len = usbGetStringAscii(handle, dev->descriptor.iSerialNumber, serial, sizeof(serial));
                                }
                                if(len < 0){
                                    errorCode = USBOPEN_ERR_ACCESS;
                                    if(warningsFp != NULL)
                                        fprintf(warningsFp, "Warning: cannot query serial for VID=0x%04x PID=0x%04x: %s\n", dev->descriptor.idVendor, dev->descriptor.idProduct, usb_strerror());
                                }
                                if(shellStyleMatch(serial, serialNamePattern)){
                                    if(printMatchingDevicesFp != NULL){
                                        if(serial[0] == 0){
                                            fprintf(printMatchingDevicesFp, "VID=0x%04x PID=0x%04x vendor=\"%s\" product=\"%s\"\n", dev->descriptor.idVendor, dev->descriptor.idProduct, vendor, product);
                                        }else{
                                            fprintf(printMatchingDevicesFp, "VID=0x%04x PID=0x%04x vendor=\"%s\" product=\"%s\" serial=\"%s\"\n", dev->descriptor.idVendor, dev->descriptor.idProduct, vendor, product, serial);
                                        }
                                    }else{
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
        }
        if(handle)  /* �� ����� ���������� */
            break;
    }
    if(handle != NULL){
        errorCode = 0;
        *device = handle;
    }
    if(printMatchingDevicesFp != NULL)  /* �� ���������� ������ ������ ��� �������� */
        errorCode = 0;
    return errorCode;
}

/* ------------------------------------------------------------------------- */
