���� ���� Readme ��� ����� libs-host. ����� �������� ����� ����, ������� ������������ ��� ���������� USB-�������� ��� �����.


��� ��������� � ���� �����?
===========================

opendevice.c � opendevice.h
  ���� ������ �������� ������� ��� ���������� � �������� ���������� �� ��� ��������� ID (VID, PID), ����� (vendor name � product name) � ��������� ������. ������� �� ���������� libusb/libusb-win32 � ���������� ����� ���������� libusb. ��. opendevice.h ��� ������������ �� API.

hiddata.c � hiddata.h
  ���� ������ �������� ������� ��� �������� ������ ����� ����������� �������� HID (HID feature reports). ������ ���������� �� ���������� libusb �� Unix � �� ���������� (native) Windows-�������� �� Windows. �� Windows �� ����� DLL ��������. ��. hiddata.h ��� ������������ �� API.

hidsdi.h
  ���� ������������ ���� DDK ����������� � ��������� MinGW ������ Windows DDK. ����������� ���� ����, ���� �� ��������� ������ "include file not found".


----------------------------------------------------------------------------
(c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH.
http://www.obdev.at/
������� http://microsin.ru/
