11 мая 2009
Здесь находится простая демонстрационная программа, показывающая, как вызывать и использовать функции библиотеки libusb-Win32 (находящиеся в libusb0.dll) из программы на C++, написанной в MS Visual Studio. 
    
Для работы программа нуждается в установке библиотеки libusb для Windows, которую закачать можно отсюда - http://sourceforge.net/projects/libusb-win32/ (установка никаких вопросов и затруднений не вызывает). Программа управляет светодиодом на макетке AVR-USB-MEGA16 (http://microsin.ru/content/view/605/44/), в которую прошит firmware из примера hid-custom-rq пакета AVR-USB компании Objective Development (это firmware работает как устройство USB HID). AVR-USB теперь переименовали в V-USB, и пакет примеров можно свободно скачать с сайта компании http://www.obdev.at/products/vusb/download.html. Русифицированный пакет можно также скачать здесь - http://microsin.ru/Download.cnt/avr/avr-usb-russian.rar, прошивка firmware находится в файле examples\hid-custom-rq\firmware\main.hex архива пакета.
    
Программа сделана на основе диалогового окна, и её шаблон подготовлен мастером Visual Studio в режиме "Dialog-based MFC application". Алгоритм работы программы прост - при запуске читается состояние светодиода на макетке. Если он горит, ставится галочка в чекбоксе "Светодиод горит", а если нет, то галочка снимается. Если пользователь меняет состояние галочки, то светодиод соответсвенно зажигается или гаснет.
    
Принцип работы программы и код управления взят из консольной программы хоста, файлы set-led.c и opendevice.c (находящиеся в том же пакете примеров, папка examples\hid-custom-rq\commandline\). Добавлен только код загрузки библиотеки libusb0.dll) и инициализация указателей на используемые в ней библиотечные функции, что реализовано в модуле linusb_func.cpp. Идея загрузки библиотеки и указателей на функции взята из документа "HID device coding example.pdf", где имеется пример работы с HID-устройством USB, но через другую библиотеку - hid.dll (эта библиотека входит в состав Windows).
    
Проект можно скачать здесь - http://microsin.ru/Download.cnt/avr/set-led-gui.zip. Список файлов проекта и их краткое описание:
doc\"HID device coding example.pdf"     пример загрузки dll и использование функций из неё
Debug\set-led-gui.exe                   скомпилированное в режиме Debug приложение
Debug\set-led-gui.exe                   скомпилированное в режиме Release приложение
res\*.*                                 файлы ресурсов проекта
libusb_func.h                           заголовочный файл для загружаемых из libusb0.dll функций, а также объявление функции LoadLibusbFuncFromDLL, загружающей libusb0.dll и инициализирующей указатели на функции.
linusb_func.cpp                         Код LoadLibusbFuncFromDLL и объявление необходимых переменных.
opendevice.cpp                          файл, взятый почти без изменений из консольного ПО хоста hid-custom-rq\commandline. Здесь находятся вспомогательные функции для открытия устройства.
opendevice.h                            заголовочный файл для opendevice.cpp.
ReadMe.txt                              текст, который Вы сейчас читаете
requests.h                              заголовочный файл, взятый без изменения из examples\hid-custom-rq\firmware\ пакета AVR-USB. Тут описаны используемые запросы USB
Resource.h                              заголовочный файл Visual Studio для использования ресурсов
set-led-gui.aps                         один из файлов проекта Visual Studio
set-led-gui.cpp                         файл, сгенерированный Visual Studio
set-led-gui.h                           заголовочный файл, сгенерированный Visual Studio
set-led-gui.ncb                         один из файлов проекта Visual Studio
set-led-gui.rc                          файл описания ресурсов, сгенерированный Visual Studio
set-led-gui.sln                         файл проекта Visual Studio - "файл решений"
set-led-gui.suo                         один из файлов проекта Visual Studio
set-led-gui.vcproj                      основной файл проекта Visual Studio
set-led-guiDlg.cpp                      файл описания функцианала диалогового окна. Здесь сосредоточен основной код приложения
set-led-guiDlg.h                        заголовочный файл, сгенерированный Visual Studio
stdafx.cpp                              файл, сгенерированный Visual Studio
stdafx.h                                заголовочный файл, сгенерированный Visual Studio
usb.h                                   файл из пакета libusb, в котором я позаимствовал определения библиотечных функций. Функции, загружаемые из libusb0.dll, в этом файле закомментированы
usbconfig.h                             заголовочный файл, взятый без изменения из examples\hid-custom-rq\firmware\. В нем определены основные параметры устройства USB HID, с которым работает программа (Vendor ID, Product ID и др.).