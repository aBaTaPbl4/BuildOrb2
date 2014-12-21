/* Имя: set-led.c
 * Проект: пример hid-custom-rq
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru
 * Дата создания: 2008-04-10
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt) 
 * Ревизия: $Id: set-led.c 604 2008-05-13 15:52:47Z cs $
 */

/*
Основное описание:
Это драйвер на стороне хоста для примера устройства custom-class. Он ищет на шине USB
устройство LEDControl и отправляет запросы, понимаемые и распознаваемые этим устройством.
Эта программа должна быть слинкована вместе с libusb на Unix и libusb-win32 на Windows.
См. http://libusb.sourceforge.net/ или http://libusb-win32.sourceforge.net/ соответственно.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "C:\Program Files\LibUSB-Win32\include\usb.h"//<usb.h>        /* это libusb */
//#include "C:\Program Files (x86)\LibUSB-Win32\include\usb.h"//<usb.h>        /* это libusb */
#include <usb.h>
#include "opendevice.h" /* общий код перемещен в отдельный модуль */

#include "../requests.h"   /* номера custom request */
#include "../usbconfig.h"  /* имена и VID/PID устройства */

static void usage(char *name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "  %s on ....... turn on LED\n", name);
    fprintf(stderr, "  %s off ...... turn off LED\n", name);
    fprintf(stderr, "  %s status ... ask current status of LED\n", name);
#if ENABLE_TEST
    fprintf(stderr, "  %s test ..... run driver reliability test\n", name);
#endif /* ENABLE_TEST */
}

int main(int argc, char **argv)
{
usb_dev_handle      *handle = NULL;
const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
char                buffer[4];
int                 cnt, vid, pid, isOn;

    usb_init();
    if(argc < 2){   /* нам нужен как минимум один аргумент */
        usage(argv[0]);
        exit(1);
    }
    /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* Следующая функция реализована в opendevice.c: */
    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0){
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        exit(1);
    }
    /* Поскольку мы используем только control endpoint 0, нам не нужно выбирать конфигурацию
     *  и интерфейс.
     */
    if(strcasecmp(argv[1], "status") == 0){
        cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_GET_STATUS, 0, 0, buffer, sizeof(buffer), 5000);
        if(cnt < 1){
            if(cnt < 0){
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            }else{
                fprintf(stderr, "only %d bytes received.\n", cnt);
            }
        }else{
            printf("LED is %s\n", buffer[0] ? "on" : "off");
        }
    }else if((isOn = (strcasecmp(argv[1], "on") == 0)) || strcasecmp(argv[1], "off") == 0){
        cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_SET_STATUS, isOn, 0, buffer, 0, 5000);
        if(cnt < 0){
            fprintf(stderr, "USB error: %s\n", usb_strerror());
        }
#if ENABLE_TEST
    }else if(strcasecmp(argv[1], "test") == 0){
        int i;
        srandomdev();
        for(i = 0; i < 50000; i++){
            int value = random() & 0xffff, index = random() & 0xffff;
            int rxValue, rxIndex;
            if((i+1) % 100 == 0){
                fprintf(stderr, "\r%05d", i+1);
                fflush(stderr);
            }
            cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, CUSTOM_RQ_ECHO, value, index, buffer, sizeof(buffer), 5000);
            if(cnt < 0){
                fprintf(stderr, "\nUSB error in iteration %d: %s\n", i, usb_strerror());
                break;
            }else if(cnt != 4){
                fprintf(stderr, "\nerror in iteration %d: %d bytes received instead of 4\n", i, cnt);
                break;
            }
            rxValue = ((int)buffer[0] & 0xff) | (((int)buffer[1] & 0xff) << 8);
            rxIndex = ((int)buffer[2] & 0xff) | (((int)buffer[3] & 0xff) << 8);
            if(rxValue != value || rxIndex != index){
                fprintf(stderr, "\ndata error in iteration %d:\n", i);
                fprintf(stderr, "rxValue = 0x%04x value = 0x%04x\n", rxValue, value);
                fprintf(stderr, "rxIndex = 0x%04x index = 0x%04x\n", rxIndex, index);
            }
        }
        fprintf(stderr, "\nTest completed.\n");
#endif /* ENABLE_TEST */
    }else{
        usage(argv[0]);
        exit(1);
    }
    usb_close(handle);
    return 0;
}
