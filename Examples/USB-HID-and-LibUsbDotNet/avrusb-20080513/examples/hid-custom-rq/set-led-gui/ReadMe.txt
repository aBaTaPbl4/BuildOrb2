11 ��� 2009
����� ��������� ������� ���������������� ���������, ������������, ��� �������� � ������������ ������� ���������� libusb-Win32 (����������� � libusb0.dll) �� ��������� �� C++, ���������� � MS Visual Studio. 
    
��� ������ ��������� ��������� � ��������� ���������� libusb ��� Windows, ������� �������� ����� ������ - http://sourceforge.net/projects/libusb-win32/ (��������� ������� �������� � ����������� �� ��������). ��������� ��������� ����������� �� ������� AVR-USB-MEGA16 (http://microsin.ru/content/view/605/44/), � ������� ������ firmware �� ������� hid-custom-rq ������ AVR-USB �������� Objective Development (��� firmware �������� ��� ���������� USB HID). AVR-USB ������ ������������� � V-USB, � ����� �������� ����� �������� ������� � ����� �������� http://www.obdev.at/products/vusb/download.html. ���������������� ����� ����� ����� ������� ����� - http://microsin.ru/Download.cnt/avr/avr-usb-russian.rar, �������� firmware ��������� � ����� examples\hid-custom-rq\firmware\main.hex ������ ������.
    
��������� ������� �� ������ ����������� ����, � � ������ ����������� �������� Visual Studio � ������ "Dialog-based MFC application". �������� ������ ��������� ����� - ��� ������� �������� ��������� ���������� �� �������. ���� �� �����, �������� ������� � �������� "��������� �����", � ���� ���, �� ������� ���������. ���� ������������ ������ ��������� �������, �� ��������� ������������� ���������� ��� ������.
    
������� ������ ��������� � ��� ���������� ���� �� ���������� ��������� �����, ����� set-led.c � opendevice.c (����������� � ��� �� ������ ��������, ����� examples\hid-custom-rq\commandline\). �������� ������ ��� �������� ���������� libusb0.dll) � ������������� ���������� �� ������������ � ��� ������������ �������, ��� ����������� � ������ linusb_func.cpp. ���� �������� ���������� � ���������� �� ������� ����� �� ��������� "HID device coding example.pdf", ��� ������� ������ ������ � HID-����������� USB, �� ����� ������ ���������� - hid.dll (��� ���������� ������ � ������ Windows).
    
������ ����� ������� ����� - http://microsin.ru/Download.cnt/avr/set-led-gui.zip. ������ ������ ������� � �� ������� ��������:
doc\"HID device coding example.pdf"     ������ �������� dll � ������������� ������� �� ��
Debug\set-led-gui.exe                   ���������������� � ������ Debug ����������
Debug\set-led-gui.exe                   ���������������� � ������ Release ����������
res\*.*                                 ����� �������� �������
libusb_func.h                           ������������ ���� ��� ����������� �� libusb0.dll �������, � ����� ���������� ������� LoadLibusbFuncFromDLL, ����������� libusb0.dll � ���������������� ��������� �� �������.
linusb_func.cpp                         ��� LoadLibusbFuncFromDLL � ���������� ����������� ����������.
opendevice.cpp                          ����, ������ ����� ��� ��������� �� ����������� �� ����� hid-custom-rq\commandline. ����� ��������� ��������������� ������� ��� �������� ����������.
opendevice.h                            ������������ ���� ��� opendevice.cpp.
ReadMe.txt                              �����, ������� �� ������ �������
requests.h                              ������������ ����, ������ ��� ��������� �� examples\hid-custom-rq\firmware\ ������ AVR-USB. ��� ������� ������������ ������� USB
Resource.h                              ������������ ���� Visual Studio ��� ������������� ��������
set-led-gui.aps                         ���� �� ������ ������� Visual Studio
set-led-gui.cpp                         ����, ��������������� Visual Studio
set-led-gui.h                           ������������ ����, ��������������� Visual Studio
set-led-gui.ncb                         ���� �� ������ ������� Visual Studio
set-led-gui.rc                          ���� �������� ��������, ��������������� Visual Studio
set-led-gui.sln                         ���� ������� Visual Studio - "���� �������"
set-led-gui.suo                         ���� �� ������ ������� Visual Studio
set-led-gui.vcproj                      �������� ���� ������� Visual Studio
set-led-guiDlg.cpp                      ���� �������� ����������� ����������� ����. ����� ������������ �������� ��� ����������
set-led-guiDlg.h                        ������������ ����, ��������������� Visual Studio
stdafx.cpp                              ����, ��������������� Visual Studio
stdafx.h                                ������������ ����, ��������������� Visual Studio
usb.h                                   ���� �� ������ libusb, � ������� � ������������� ����������� ������������ �������. �������, ����������� �� libusb0.dll, � ���� ����� ����������������
usbconfig.h                             ������������ ����, ������ ��� ��������� �� examples\hid-custom-rq\firmware\. � ��� ���������� �������� ��������� ���������� USB HID, � ������� �������� ��������� (Vendor ID, Product ID � ��.).