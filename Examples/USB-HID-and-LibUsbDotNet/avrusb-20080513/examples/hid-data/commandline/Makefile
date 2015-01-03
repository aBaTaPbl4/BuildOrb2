# Имя: Makefile
# Project: пример hid-data
# Автор: Christian Starkjohann
# Перевод: microsin.ru
# Дата создания: 2008-04-11
# Табуляция: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
# Ревизия: $Id: Makefile 553 2008-04-17 19:00:20Z cs $

# Сконфигурируйте следующие параметры в соответствии с используемой Вами системой:

# Используйте следующие 3 строки на Unix и Mac OS X:
#USBFLAGS=   `libusb-config --cflags`
#USBLIBS=    `libusb-config --libs`
#EXE_SUFFIX=

# Используйте следующие 3 строки на Windows и закомментируйте 3 предшествующие:
#USBFLAGS=
USBFLAGS = -I"c:/Program Files/LibUSB-Win32/include"
#USBLIBS=    -lhid -lusb -lsetupapi
USBLIBS = -L"c:/Program Files/LibUSB-Win32/lib/gcc" -lhid -lusb -lsetupapi
#EXE_SUFFIX= .exe

CC=				gcc
CFLAGS=			-O -Wall $(USBFLAGS)
LIBS=			$(USBLIBS)

OBJ=		hidtool.o hiddata.o
PROGRAM=	hidtool$(EXE_SUFFIX)

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) -o $(PROGRAM) $(OBJ) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f $(OBJ) $(PROGRAM)

.c.o:
	$(CC) $(ARCH_COMPILE) $(CFLAGS) -c $*.c -o $*.o
