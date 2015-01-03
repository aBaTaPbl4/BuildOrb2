/* Имя: opendevice.h
 * Проект: библиотека хоста AVR-USB
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru
 * Дата создания: 2008-04-10
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt) 
 * Ревизия: $Id: opendevice.h 553 2008-04-17 19:00:20Z cs $
 */

/*
Основное описание:
Этот модуль предлагает дополнительный функционал для драйверов хоста на основе
libusb или libusb-win32. Он включает функцию для нахождения и открытия устройства 
по цифровому идентификатору ID и текстовому описанию. Он также включает функцию 
для получения текстовых описаний от устройства.

Для использования модуля просто скопируйте opendevice.c и opendevice.h в Ваш 
проект и добавьте его в Makefile. Вы можете модифицировать и распространять эти файлы
в соответствии с лицензией GNU General Public License (GPL) версия 2.
*/

#ifndef __OPENDEVICE_H_INCLUDED__
#define __OPENDEVICE_H_INCLUDED__

#include "C:\Program Files\LibUSB-Win32\include\usb.h" //<usb.h>    /* это libusb, см. http://libusb.sourceforge.net/ */
//#include "C:\Program Files (x86)\LibUSB-Win32\include\usb.h" //<usb.h>    /* это libusb, см. http://libusb.sourceforge.net/ */
//#include <usb.h>    /* это libusb, см. http://libusb.sourceforge.net/ */
#include <stdio.h>

int usbGetStringAscii(usb_dev_handle *dev, int index, char *buf, int buflen);
/* Эта функция получает строковый оптсатель от устройства. 'index' является 
 * индексом строкового описателя. Строка возвращается в кодировке ISO Latin 1 в
 * буфере 'buf', признаком окончания строки служит символ 0 (ASCIIZ строка). 
 * Размер буфера должен передаваться в переменной 'buflen' для предотвращения 
 * переполнения буфера. Хендл устройства libusb должен передаваться в 'dev'.
 * Возврат: Длина строки (исключая завершающий 0) ил отрицательное число
 * в случае ошибки. Если произошла ошибка, используйте usb_strerror() 
 * для получения сообщения об ошибке.
 */

int usbOpenDevice(usb_dev_handle **device, int vendorID, char *vendorNamePattern, int productID, char *productNamePattern, char *serialNamePattern, FILE *printMatchingDevicesFp, FILE *warningsFp);
/* Эта функция опрашивает все устройства на всех шинах USB и ищет конкретное устройство.
 * Поиск начинается по совпадению Vendor- и Product-ID (переданных в переменных 
 * 'vendorID' и 'productID'. ID равный 0 совпадает с любым цифровым ID (wildcard).
 * Когда устройство совпало по идентификаторам IDs, проверяется совпадение по имени.
 * Сопадение может быть по текстовому имени вендора ('vendorNamePattern'), имени продукта
 * ('productNamePattern') и серийному номеру ('serialNamePattern'). Устройство совпадает
 * только тогда, когда совпедают все непустые (non-null) маски. Если Вам не надо проверять
 * определенную строку, просто передайте NULL в качестве маски. Маски работают в стиле
 * Unix shell:
 * '*' означает 0 или другое количество символов, '?' означает любой одиночный символ, 
 * любой символ из списка, заданного квадратными скобками (дефис разрешен для указания
 * диапазона символов) и если список символов начинается с символа caret ('^'), то он
 * соответствует любому символу НЕ из этого списка.
 * Другие параметры функции: если 'warningsFp' не NULL, предупреждающие (warning) сообщения 
 * будут печататься в этот файловый дескриптор с помощью функции fprintf(). Если
 * 'printMatchingDevicesFp' не NULL, устройства не будут открыты, но совпавшие устройства
 * будут распечатаны в предоставленный файловый дескриптор 'warningsFp' с помощью функции 
 * fprintf(). Если устройство открыто, результирующий USB=хендл сохраняется в '*device'. 
 * Здесь должен быть передан Указатель на тип переменной "usb_dev_handle *".
 * Возврат: 0 в случае успеха или код ошибки (см. операторы define далее).
 */

/* Коды ошибок usbOpenDevice(): */
#define USBOPEN_SUCCESS         0   /* нет ошибки */
#define USBOPEN_ERR_ACCESS      1   /* не хватает прав для открытия устройства */
#define USBOPEN_ERR_IO          2   /* ошибка ввода/вывода */
#define USBOPEN_ERR_NOTFOUND    3   /* устройство не найдено */


/* Свободные USB идентификаторы ID Obdev, подробнее см. в USBID-License.txt */
#define USB_VID_OBDEV_SHARED        5824    /* obdev общий vendor ID */
#define USB_PID_OBDEV_SHARED_CUSTOM 1500    /* общий PID для устройств custom class */
#define USB_PID_OBDEV_SHARED_HID    1503    /* общий PID для устройств HID, исключая мыши и клавиатуры */
#define USB_PID_OBDEV_SHARED_CDCACM 1505    /* общий PID для устройств CDC Modem */
#define USB_PID_OBDEV_SHARED_MIDI   1508    /* общий PID для устройств MIDI class */

#endif /* __OPENDEVICE_H_INCLUDED__ */
