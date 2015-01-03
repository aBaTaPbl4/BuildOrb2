# Имя: Makefile
# Проект: usbtool
# Автор: Christian Starkjohann
# Перевод: microsin.ru
# Дата создания: 2008-04-06
# Табуляция: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
# Ревизия: $Id: Makefile 605 2008-05-13 15:53:12Z cs $


# Конфигурируйте следующие определения в соответствии с Вашей системой.
# Этот Makefile протестирован на Mac OS X, Linux и Windows.

# Используйте следующие 3 строки на Unix (раскомментируйте в рабочем окружении Mac OS X):
USBFLAGS = `libusb-config --cflags`
USBLIBS = `libusb-config --libs`
EXE_SUFFIX =

# Используйте следующие 3 строки на Windows и закомментируйте 3 предшествующие. 
# Вы можете поменять пути include, чтобы они соответсвовали месту установки libusb-win32
#USBFLAGS = -I/usr/local/include
USBFLAGS = -I"c:/Program Files/LibUSB-Win32/include"
#USBLIBS = -L/usr/local/lib -lusb
USBLIBS = -L"c:/Program Files/LibUSB-Win32/lib/gcc" -lusb
EXE_SUFFIX = .exe

NAME = usbtool

OBJECTS = opendevice.o $(NAME).o

CC		= gcc
CFLAGS	= $(CPPFLAGS) $(USBFLAGS) -O -g -Wall
LIBS	= $(USBLIBS)

PROGRAM = $(NAME)$(EXE_SUFFIX)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)
