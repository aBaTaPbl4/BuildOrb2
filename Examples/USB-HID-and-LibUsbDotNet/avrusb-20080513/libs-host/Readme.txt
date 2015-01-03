Этот файл Readme для папки libs-host. Папка содержит куски кода, которые используются для разработки USB-программ для хоста.


ЧТО НАХОДИТСЯ В ЭТОЙ ПАПКЕ?
===========================

opendevice.c и opendevice.h
  Этот модуль содержит функцию для нахождения и открытия устройства по его цифровому ID (VID, PID), имени (vendor name и product name) и серийному номеру. Основан на библиотеке libusb/libusb-win32 и возвращает хендл устройства libusb. См. opendevice.h для документации по API.

hiddata.c и hiddata.h
  Этот модуль содержит функции для передачи данных через особенность репортов HID (HID feature reports). Модуль базируется на библиотеке libusb на Unix и на встроенных (native) Windows-функциях на Windows. На Windows не нужны DLL драйвера. См. hiddata.h для документации по API.

hidsdi.h
  Этот заголовочный файл DDK отсутствует в свободной MinGW версии Windows DDK. Используйте этот файл, если Вы получаете ошибку "include file not found".


----------------------------------------------------------------------------
(c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH.
http://www.obdev.at/
Перевод http://microsin.ru/
