���� ���� Readme ��� ������� hid-data. ����� �� �������, ��� ����� ������������ � ����������� USB ������� ������, ��������� ������ ���������� ������ HID. ��������� �������� ��� ������ HID �������� � Windows, ��� �� ����� ������������� ������������� ��������.


��� ��� ���������������?
========================
���� ������� �������������, ��� ����� HID ����� "�����������" �������������� ��� �������� ����� ������ �������������� ������� (����������� �� �������, �������������� ������������ ������� �������� ��������) ����� ����������� HID feature reports. ��� ������� ����� ������� �������� ��� ������ ��������� � Windows, ��������� �� ����� ����� DLL �������� (������ hid-custom-rq ���-���� ������� libusb-win32 DLL, ���� �� ����� ��������� � ����� � ����������). ��������� (host side application) �� ��������� � ����������� �����������, ��� ����� ����������� ���� � CD. ���� ������ ����� �������������, ��� ���������� ������ � ������� ������� usbFunctionWrite() � usbFunctionRead().


��������������� ����������
==========================
������� ������: ��� ����� ����� �� AVR, ���������� �� ����� �������� (��. ����� "circuits" �� ������� ������ ����� ������), �������� metaboard (http://www.obdev.at/goto.php?t=metaboard).

����� ���������� AVR: ��������� gcc tool chain ��� AVR, ��� �������� ��� ��� �������� ��. ������ "��������������� ����������" � ����� Readme ������������ �� �������� ������ ������.

����� ���������� �����: ���������� C � ���������� libusb �� Unix. �� Windows ��� ����� Driver Development Kit (DDK) ������ libusb. ������� MinGW ���������� ��������� ������ DDK.


������ FIRMWARE
===============
��������� � ����� "firmware" � ������������� Makefile � ������������ � ����� ������������ (������� CPU, ��� ��������������� (target device), �������� ��������� (fuse values)) � �������������� ISP. ������������ usbconfig.h � ������������ � ����� ����������� ����� ��� D+ and D-. �������� �� ��������� ��������� ��� ����� metaboard.

������� "make hex" ��� ������ main.hex, ����� "make flash" ��� �������� firmware � ���������� (� ������� ������������� ISP). �� ������� ��������� "make fuse" ���� ��� ��� ���������������� ��������� (fuses). ���� �� ����������� �������� ����� � ����������� (boot loader), �������� ����������� �� ���������� �����������.

���������� ������� �� ��������, ��� ������ ����� "make hex" �������� ������� �� �������� ������ � ����� firmware (���������� ����� usbdrv ������ � ����������). ���� �� ����������� ������ ������� ����������, �������� �� ������ Makefile, �� ������ ����������� ������� �������.


������ ��������� �����
======================
��������������, ��� � ��� ���� ���������������� libusb (�� Unix) ��� DDK (�� Windows). �� ����������� ������� MinGW ��� Windows, ��������� ��� �������� ��������� ������ DDK. 
��� ������ ��������� � ����� "commandline" � ��������� "make" �� Unix ��� "make -f Makefile.windows" �� Windows (���������� �����������: ����� ��������� make � �� Windows, ���� �� ��������� �������������� Makefile. �� ������������ ������ ��� � �������, � ������������ Makefile ��������� �� ������ ������ ��� ������ Makefile.orig). 


������������� ��������� �����
=============================
���������� USB ��������� �������� ������ �� 128 ���� � EEPROM AVR. � ������� ��������� ����� �� ������ ������� ���� �� 128 ���� �� USB-���������� ��� ��������� 128 ���� � USB-����������.

������ �������� ����� ������ �� ����������:

    hidtool write 0x01,0x02,0x03,0x04,...

������ ������ ����� � ����������:

    hidtool read


----------------------------------------------------------------------------
(c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH.
http://www.obdev.at/
������� http://microsin.ru/
