#include "usb.h"

typedef int	            (__cdecl *pusb_control_msg)       (usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);
typedef int             (__cdecl *pusb_get_string_simple) (usb_dev_handle *dev, int index, char *buf, size_t buflen);
typedef int             (__cdecl *pusb_close)	            (usb_dev_handle *dev);
typedef char*           (__cdecl *pusb_strerror)          (void);
typedef usb_dev_handle* (__cdecl *pusb_open)              (struct usb_device *dev);
typedef struct usb_bus* (__cdecl *pusb_get_busses)        (void);
typedef int             (__cdecl *pusb_find_devices)      (void);
typedef int             (__cdecl *pusb_find_busses)       (void);
typedef void            (__cdecl *pusb_init)              (void);

extern HINSTANCE hHID;
extern pusb_control_msg       usb_control_msg;
extern pusb_get_string_simple usb_get_string_simple;
extern pusb_close			  usb_close;
extern pusb_strerror          usb_strerror;
extern pusb_open              usb_open;
extern pusb_get_busses        usb_get_busses;
extern pusb_find_devices      usb_find_devices;
extern pusb_find_busses       usb_find_busses;
extern pusb_init              usb_init;

void LoadLibusbFuncFromDLL (void);
void UnloadLibusb (void);
