# ���: Makefile
# ������: ������ hid-custom-rq
# �����: Christian Starkjohann
# �������: microsin.ru 
# ���� ��������: 2008-04-06
# ���������: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
# �������: $Id: Makefile 605 2008-05-13 15:53:12Z cs $

# ��������������� ��������� ����������� � ������������ � ����� ��������.
# ���� Makefile ������������� �� Mac OS X, Linux � Windows.

# ����������� ��������� 3 ������ �� Unix (����������������� ��� �������� ��������� Mac OS X):
#USBFLAGS = `libusb-config --cflags`
#USBLIBS = `libusb-config --libs`
#EXE_SUFFIX =

# ����������� ��������� 3 ������ �� Windows � ��������������� 3 ����������. �� ������ 
#  �������� ���� include ��� ������������ ����� ��������� libusb-win32

#USBFLAGS = -I/usr/local/include
USBFLAGS = -I"c:/.My projects/BuildOrb/Libs/LibUSB-Win32/include"
#USBFLAGS = -I"C:/LibUSB/include/"

#USBLIBS = -L/usr/local/lib -lusb
USBLIBS = -L"c:/.My projects/BuildOrb/Libs/LibUSB-Win32/lib/gcc" -lusb
#USBLIBS = -L"C:/LibUSB/lib/gcc" -lusb

#EXE_SUFFIX = .exe 

NAME = set-led

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
