#include "stdafx.h"
#include "libusb_func.h"

HINSTANCE hHID = NULL;

pusb_control_msg		usb_control_msg			= NULL;
pusb_get_string_simple	usb_get_string_simple	= NULL;
pusb_close				usb_close				= NULL;
pusb_strerror           usb_strerror            = NULL;
pusb_open               usb_open                = NULL;
pusb_get_busses         usb_get_busses          = NULL;
pusb_find_devices       usb_find_devices        = NULL;
pusb_find_busses        usb_find_busses         = NULL;
pusb_init               usb_init                = NULL;

void LoadLibusbFuncFromDLL (void)
{
	hHID = LoadLibrary("libusb0.dll");
	//Update the pointers:
	usb_control_msg         = (pusb_control_msg)        GetProcAddress(hHID, "usb_control_msg");
	usb_get_string_simple   = (pusb_get_string_simple)  GetProcAddress(hHID, "usb_get_string_simple");
	usb_close               = (pusb_close)              GetProcAddress(hHID, "usb_close");
	usb_strerror            = (pusb_strerror)           GetProcAddress(hHID, "usb_strerror");
	usb_open                = (pusb_open)               GetProcAddress(hHID, "usb_open");
	usb_get_busses          = (pusb_get_busses)         GetProcAddress(hHID, "usb_get_busses");
	usb_find_devices        = (pusb_find_devices)       GetProcAddress(hHID, "usb_find_devices");
	usb_find_busses         = (pusb_find_busses)        GetProcAddress(hHID, "usb_find_busses");
	usb_init                = (pusb_init)               GetProcAddress(hHID, "usb_init");
}

void UnloadLibusb (void)
{
    FreeLibrary(hHID);
}
