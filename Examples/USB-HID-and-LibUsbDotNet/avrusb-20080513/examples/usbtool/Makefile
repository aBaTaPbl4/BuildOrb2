# ���: Makefile
# ������: usbtool
# �����: Christian Starkjohann
# �������: microsin.ru
# ���� ��������: 2008-04-06
# ���������: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
# �������: $Id: Makefile 605 2008-05-13 15:53:12Z cs $


# �������������� ��������� ����������� � ������������ � ����� ��������.
# ���� Makefile ������������� �� Mac OS X, Linux � Windows.

# ����������� ��������� 3 ������ �� Unix (���������������� � ������� ��������� Mac OS X):
USBFLAGS = `libusb-config --cflags`
USBLIBS = `libusb-config --libs`
EXE_SUFFIX =

# ����������� ��������� 3 ������ �� Windows � ��������������� 3 ��������������. 
# �� ������ �������� ���� include, ����� ��� �������������� ����� ��������� libusb-win32
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
