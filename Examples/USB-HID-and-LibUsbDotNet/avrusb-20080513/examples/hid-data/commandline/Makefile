# ���: Makefile
# Project: ������ hid-data
# �����: Christian Starkjohann
# �������: microsin.ru
# ���� ��������: 2008-04-11
# ���������: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
# �������: $Id: Makefile 553 2008-04-17 19:00:20Z cs $

# ��������������� ��������� ��������� � ������������ � ������������ ���� ��������:

# ����������� ��������� 3 ������ �� Unix � Mac OS X:
#USBFLAGS=   `libusb-config --cflags`
#USBLIBS=    `libusb-config --libs`
#EXE_SUFFIX=

# ����������� ��������� 3 ������ �� Windows � ��������������� 3 ��������������:
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
