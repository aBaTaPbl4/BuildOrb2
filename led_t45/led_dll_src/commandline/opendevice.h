/* ���: opendevice.h
 * ������: ���������� ����� AVR-USB
 * �����: Christian Starkjohann
 * �������: microsin.ru
 * ���� ��������: 2008-04-10
 * ���������: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt) 
 * �������: $Id: opendevice.h 553 2008-04-17 19:00:20Z cs $
 */

/*
�������� ��������:
���� ������ ���������� �������������� ���������� ��� ��������� ����� �� ������
libusb ��� libusb-win32. �� �������� ������� ��� ���������� � �������� ���������� 
�� ��������� �������������� ID � ���������� ��������. �� ����� �������� ������� 
��� ��������� ��������� �������� �� ����������.

��� ������������� ������ ������ ���������� opendevice.c � opendevice.h � ��� 
������ � �������� ��� � Makefile. �� ������ �������������� � �������������� ��� �����
� ������������ � ��������� GNU General Public License (GPL) ������ 2.
*/

#ifndef __OPENDEVICE_H_INCLUDED__
#define __OPENDEVICE_H_INCLUDED__

//#include "C:\Program Files\LibUSB-Win32\include\usb.h" //<usb.h>    /* ��� libusb, ��. http://libusb.sourceforge.net/ */
//#include "C:\Program Files (x86)\LibUSB-Win32\include\usb.h" //<usb.h>    /* ��� libusb, ��. http://libusb.sourceforge.net/ */
#include <usb.h>    /* ��� libusb, ��. http://libusb.sourceforge.net/ */
#include <stdio.h>

int usbGetStringAscii(usb_dev_handle *dev, int index, char *buf, int buflen);
/* ��� ������� �������� ��������� ��������� �� ����������. 'index' �������� 
 * �������� ���������� ���������. ������ ������������ � ��������� ISO Latin 1 �
 * ������ 'buf', ��������� ��������� ������ ������ ������ 0 (ASCIIZ ������). 
 * ������ ������ ������ ������������ � ���������� 'buflen' ��� �������������� 
 * ������������ ������. ����� ���������� libusb ������ ������������ � 'dev'.
 * �������: ����� ������ (�������� ����������� 0) �� ������������� �����
 * � ������ ������. ���� ��������� ������, ����������� usb_strerror() 
 * ��� ��������� ��������� �� ������.
 */

int usbOpenDevice(usb_dev_handle **device, int vendorID, char *vendorNamePattern, int productID, char *productNamePattern, char *serialNamePattern, FILE *printMatchingDevicesFp, FILE *warningsFp);
/* ��� ������� ���������� ��� ���������� �� ���� ����� USB � ���� ���������� ����������.
 * ����� ���������� �� ���������� Vendor- � Product-ID (���������� � ���������� 
 * 'vendorID' � 'productID'. ID ������ 0 ��������� � ����� �������� ID (wildcard).
 * ����� ���������� ������� �� ��������������� IDs, ����������� ���������� �� �����.
 * ��������� ����� ���� �� ���������� ����� ������� ('vendorNamePattern'), ����� ��������
 * ('productNamePattern') � ��������� ������ ('serialNamePattern'). ���������� ���������
 * ������ �����, ����� ��������� ��� �������� (non-null) �����. ���� ��� �� ���� ���������
 * ������������ ������, ������ ��������� NULL � �������� �����. ����� �������� � �����
 * Unix shell:
 * '*' �������� 0 ��� ������ ���������� ��������, '?' �������� ����� ��������� ������, 
 * ����� ������ �� ������, ��������� ����������� �������� (����� �������� ��� ��������
 * ��������� ��������) � ���� ������ �������� ���������� � ������� caret ('^'), �� ��
 * ������������� ������ ������� �� �� ����� ������.
 * ������ ��������� �������: ���� 'warningsFp' �� NULL, ��������������� (warning) ��������� 
 * ����� ���������� � ���� �������� ���������� � ������� ������� fprintf(). ����
 * 'printMatchingDevicesFp' �� NULL, ���������� �� ����� �������, �� ��������� ����������
 * ����� ����������� � ��������������� �������� ���������� 'warningsFp' � ������� ������� 
 * fprintf(). ���� ���������� �������, �������������� USB=����� ����������� � '*device'. 
 * ����� ������ ���� ������� ��������� �� ��� ���������� "usb_dev_handle *".
 * �������: 0 � ������ ������ ��� ��� ������ (��. ��������� define �����).
 */

/* ���� ������ usbOpenDevice(): */
#define USBOPEN_SUCCESS         0   /* ��� ������ */
#define USBOPEN_ERR_ACCESS      1   /* �� ������� ���� ��� �������� ���������� */
#define USBOPEN_ERR_IO          2   /* ������ �����/������ */
#define USBOPEN_ERR_NOTFOUND    3   /* ���������� �� ������� */


/* ��������� USB �������������� ID Obdev, ��������� ��. � USBID-License.txt */
#define USB_VID_OBDEV_SHARED        5824    /* obdev ����� vendor ID */
#define USB_PID_OBDEV_SHARED_CUSTOM 1500    /* ����� PID ��� ��������� custom class */
#define USB_PID_OBDEV_SHARED_HID    1503    /* ����� PID ��� ��������� HID, �������� ���� � ���������� */
#define USB_PID_OBDEV_SHARED_CDCACM 1505    /* ����� PID ��� ��������� CDC Modem */
#define USB_PID_OBDEV_SHARED_MIDI   1508    /* ����� PID ��� ��������� MIDI class */

#endif /* __OPENDEVICE_H_INCLUDED__ */
