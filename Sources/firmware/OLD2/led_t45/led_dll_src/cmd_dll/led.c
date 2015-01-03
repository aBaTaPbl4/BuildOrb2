#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../Libs/LibUSB-Win32/include/usb.h"//<usb.h>        /* это libusb */
//#include "C:\Program Files (x86)\LibUSB-Win32\include\usb.h"//<usb.h>        /* это libusb */
//#include <usb.h>
#include "opendevice.h" /* общий код перемещен в отдельный модуль */

#include "../requests.h"   /* номера custom request */
#include "../usbconfig.h"  /* имена и VID/PID устройства */

#define		bit_is_set(sfr, bit) ((sfr) & (1 << (bit)))

void led_set( int isOn )
{
usb_dev_handle      *handle = NULL;
const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
char                buffer[4];
int                 cnt, vid, pid;//, isOn;

    usb_init();
    /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* Следующая функция реализована в opendevice.c: */
    if(usbOpenDevice(	&handle, 
						vid, 
						vendor, 
						pid, 
						product, 
						NULL, 
						NULL, 
						NULL
						) != 0)
	{
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        exit(1);
    }
	// led set ================================================================
	//isOn=0;
	cnt = usb_control_msg(	handle, 
							USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, 
							CUSTOM_RQ_SET_STATUS, 
							isOn, 
							0, 
							buffer, 
							0, 
							5000
							);
	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
	}
	// ========================================================================
	usb_close(handle);
	//return 0;
}


int	led_get( void )
{
usb_dev_handle      *handle = NULL;
const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
char                buffer[4];
int                 cnt, vid, pid, isOn=-1;

    usb_init();
    /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* Следующая функция реализована в opendevice.c: */
    if(usbOpenDevice(	&handle, 
						vid, 
						vendor, 
						pid, 
						product, 
						NULL, 
						NULL, 
						NULL
						) != 0)
	{
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        //exit(1);
		return isOn;
    }
	// led get ================================================================
        cnt = usb_control_msg(	handle, 
								USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 
								CUSTOM_RQ_GET_STATUS, 
								0, 
								0, 
								buffer, 
								sizeof(buffer), 
								5000
								);
        if(cnt < 1)
		{
            if(cnt < 0)
			{
                fprintf(stderr, "USB error: %s\n", usb_strerror());
            }
			else
			{
                fprintf(stderr, "only %d bytes received.\n", cnt);
            }
        }
		else
		{
            //printf("LED is %s\n", buffer[0] ? "on" : "off");
			isOn = bit_is_set(buffer[0],0);
        }
	// ========================================================================
    usb_close(handle);
    return isOn;
}
/*
int main(int argc, char **argv)
{
	if(strcasecmp(argv[1], "status") == 0)
		printf("LED is %s\n", led_get() ? "on" : "off");
	else
		if(strcasecmp(argv[1], "on") == 0)led_set(1);
	else
		if(strcasecmp(argv[1], "off") == 0)led_set(0);
	return 0;
}
*/
