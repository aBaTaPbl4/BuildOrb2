# Имя: Makefile
# Проект: пример hid-custom-rq
# Автор: Christian Starkjohann
# Перевод: microsin.ru 
# Дата создания: 2008-04-06
# Табуляция: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
# Ревизия: $Id: Makefile 605 2008-05-13 15:53:12Z cs $

# Сконфигурируйте следующие определения в соответствии с Вашей системой.
# Этот Makefile протестирован на Mac OS X, Linux и Windows.

# Используйте следующие 3 строки на Unix (расскомментируйте для рабочего окружения Mac OS X):
#USBFLAGS = `libusb-config --cflags`
#USBLIBS = `libusb-config --libs`
#EXE_SUFFIX =

# Используйте следующие 3 строки на Windows и закомментируйте 3 предыдущие. Вы можете 
#  изменить пути include для соответствия месту установки libusb-win32

#USBFLAGS = -I/usr/local/include
USBFLAGS = -I"c:/Program Files/LibUSB-Win32/include"
#USBFLAGS = -I"C:/Program Files (x86)/LibUSB-Win32/include"
#USBFLAGS = -I"C:/LibUSB/include/"

#USBLIBS = -L/usr/local/lib -lusb
USBLIBS = -L"c:/Program Files/LibUSB-Win32/lib/gcc" -lusb
#USBLIBS = -L"C:/Program Files (x86)/LibUSB-Win32/lib/gcc" -lusb
#USBLIBS = -L"C:/LibUSB/lib/gcc" -lusb

EXE_SUFFIX = .dll
#.exe 

NAME = led

OBJECTS = opendevice.o $(NAME).o

CC		= gcc
CFLAGS	= $(CPPFLAGS) $(USBFLAGS) -O -g -Wall -shared
LIBS	= $(USBLIBS)

PROGRAM = $(NAME)$(EXE_SUFFIX)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LIBS) -o $(PROGRAM) 

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)
