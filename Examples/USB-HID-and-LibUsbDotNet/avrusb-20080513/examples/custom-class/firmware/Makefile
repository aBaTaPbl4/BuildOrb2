# ���: Makefile
# ������: ������ custom-class
# �����: Christian Starkjohann
# �������: microsin.ru 
# ���� ��������: 2008-04-07
# ���������: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# ��������: GNU GPL v2 (��. License.txt) ��� ������������� (CommercialLicense.txt)
# �������: $Id: Makefile 553 2008-04-17 19:00:20Z cs $

DEVICE  = atmega16
F_CPU   = 16000000	# � ������
# � ATMega16 ���������� ��������� ����� ��������� � ATMega8 - ������� ������ � 2 ����� FUSE_H (6 � 7)
FUSE_L  = ff
FUSE_H  = 09

#AVRDUDE = avrdude -c jtag2 -p $(DEVICE) -P usb:xx -v 
JTAGICEII = "c:/Program Files/Atmel/AVR Tools/JTAGICEmkII/jtagiceii.exe" -d $(DEVICE) -mi

CFLAGS  = -Iusbdrv -I. -DDEBUG_LEVEL=0
OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o main.o

#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
#COMPILE = avr-gcc -Wall -O0 -gdwarf-2 -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
COMPILE = avr-gcc -Wall -Os -gstabs -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
RAR = "c:/Program Files/WinRAR/WinRAR.exe"

##############################################################################
# �������� ��������� (������) ��� ��������� ����� �����������������
##############################################################################
# ���� ��� ��������������� �� ������������ �����, ��. ������
#  http://palmavr.sourceforge.net/cgi-bin/fc.cgi
#  � �������� ����� ��� ������� ������ � ���������� �������� �������� �������
#
################################## ATMega8 ##################################
# ATMega8 FUSE_L (������� ���� ������):
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, BOD ��������)
#        | +------------------ BODEN (BrownOut Detector ��������)
#        +-------------------- BODLEVEL (2.7V)
# ATMega8 FUSE_H (������� ���� ������):
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (������ ������ �������� 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (�� ��������� EEPROM ��� �������� ����)
#        | | | +-------------- CKOPT (������ ������ ������ ����������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ WDTON (WDT ������� �� ������)
#        +-------------------- RSTDISBL (����� ������ ���������)
#
################################## ATMega16 ##################################
# ATMega16 FUSE_L (������� ���� ������):
# 0xff = 1 1 1 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, ��������� ���������� ������ �������)
#        | +------------------ BODEN (BrownOut Detector ��������)
#        +-------------------- BODLEVEL (2.7V)
# ATMega16 FUSE_H (������� ���� ������):
# 0x09 = 0 0 0 0   1 0 0 1 <-- BOOTRST (������ ������ �������� 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (�� ��������� EEPROM ��� �������� ����)
#        | | | +-------------- CKOPT (������ ������ ������ ����������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ JTAGEN (����������������, JTAG ��������)
#        +-------------------- OCDEN (����������������, OCD ��������)
#
############################## ATMega48/88/168 ##############################
# ATMega*8 FUSE_L (������� ���� ������):
# 0xdf = 1 1 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (������� �������� � �������� > 8 ���)
#        | |  +--------------- SUT 1..0 (��������� � ����������, BOD ��������)
#        | +------------------ CKOUT (���� 0: ����� ������ ��������)
#        +-------------------- CKDIV8 (���� 0: ������� �������� ������ 8)
# ATMega*8 FUSE_H (������� ���� ������):
# 0xde = 1 1 0 1   1 1 1 0
#        ^ ^ ^ ^   ^ \-+-/
#        | | | |   |   +------ BODLEVEL 0..2 (110 = 1.8 V)
#        | | | |   + --------- EESAVE (��������� EEPROM ��� �������� ����)
#        | | | +-------------- WDTON (���� 0: watchdog ������ �������)
#        | | +---------------- SPIEN (��������� ���������������� ����������������)
#        | +------------------ DWEN (��������� debug wire)
#        +-------------------- RSTDISBL (����� ������ ���������)
#
############################## ATTiny25/45/85 ###############################
# ATMega*5 FUSE_L (������� ���� ������):
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (����� �������� ������� -> �������� @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD ��������, ������� ���������� ������ �������)
#        | +------------------ CKOUT (����� ������ �� ����� CKOUT -> ��������)
#        +-------------------- CKDIV8 (�������� ������ �� 8 -> ��������)
# ATMega*5 FUSE_H (������� ���� ������):
# 0xdd = 1 1 0 1   1 1 0 1
#        ^ ^ ^ ^   ^ \-+-/ 
#        | | | |   |   +------ BODLEVEL 2..0 (������� �������� brownout -> 2.7V)
#        | | | |   +---------- EESAVE (��������� EEPROM ��� �������� ���� -> ���)
#        | | | +-------------- WDTON (watchdog ������ ������� -> ���������)
#        | | +---------------- SPIEN (��������� ���������������� ���������������� -> ��)
#        | +------------------ DWEN (��������� debug wire)
#        +-------------------- RSTDISBL (������ �������� ������ -> ���)
#
################################ ATTiny2313 #################################
# ATTiny2313 FUSE_L (������� ���� ������):
# 0xef = 1 1 1 0   1 1 1 1
#        ^ ^ \+/   \--+--/
#        | |  |       +------- CKSEL 3..0 (����� �������� ������� -> �������� @ 12 MHz)
#        | |  +--------------- SUT 1..0 (BOD ��������, ������� ���������� ������ �������)
#        | +------------------ CKOUT (����� ������ �� ����� CKOUT -> ��������)
#        +-------------------- CKDIV8 (�������� ������ �� 8 -> ��������)
# ATTiny2313 FUSE_H (������� ���� ������):
# 0xdb = 1 1 0 1   1 0 1 1
#        ^ ^ ^ ^   \-+-/ ^
#        | | | |     |   +---- RSTDISBL (������ �������� ������ -> ���)
#        | | | |     +-------- BODLEVEL 2..0 (������� �������� brownout -> 2.7V)
#        | | | +-------------- WDTON (watchdog ������ ������� -> ���������)
#        | | +---------------- SPIEN (��������� ���������������� ���������������� -> ��)
#        | +------------------ EESAVE (��������� EEPROM ��� �������� ���� -> ���)
#        +-------------------- DWEN (��������� debug wire)


# ������������� ���� (symbolic targets):
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware"
	@echo "make clean ..... to delete objects and hex file"
	@echo "make erase ..... erase AVR chip"
	@echo "make backup ..... backup project"

hex: main.hex

program: flash fuse

# ������� ��� ���������������� ��� ������:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
#	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
	$(JTAGICEII) -f 0x$(FUSE_H)$(FUSE_L)

# ������� ��� �������� firmware:
flash: main.hex
#	$(AVRDUDE) -U flash:w:main.hex:i
	$(JTAGICEII) -pf -if main.hex

# ������� ��� �������� ������ ������������ (������� ����� ���� ��������� �������� Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o usbdrv/*.o main.s usbdrv/oddebug.s usbdrv/usbdrv.s
	rm -f usbdrv/*.*
	rmdir usbdrv

# ������� (generic) ������� ��� �������������� ������ �� ����� C:
.c.o:
	$(COMPILE) -c $< -o $@
	
# ������� (generic) ������� ��� ���������� ������ �� ����� ����������:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" �� ������ ���� ���������, ��������� ��� ������
#  � ����������� .S (����� ��������� S) ����� �� ���������. ������, ������� 
#  � ������� �������� �� ������ ����������� � ������ �� Windows. ����� ����������
#  ������������� � WinAVR ������� ��� ����� �������. 

# Generic ������� ��� �������������� ������ ����� C � ���������, ������������ ������ ��� �������.
.c.s:
	$(COMPILE) -S $< -o $@
 
# �������� ���� (file targets): 
# ��������� �� �� ����� ���������� ������� ����� ��� � ������ �������, �� �������� ������� 
#  � ���� ������:
usbdrv:
	cp -r ../../../usbdrv .

main.elf: usbdrv $(OBJECTS)	# ����������� (dependency) usbdrv ����� ������ ��� �����������
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-objcopy --debugging -O coff-ext-avr $< main.cof
	avr-size main.hex

# ���� �������:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c

# ���� ������� ��������� ����������������
erase:
	$(JTAGICEII) -e

# ���� ������
backup:
	$(RAR) a -r -dh -ep1 avrusb.rar ../../../../avrusb-20080513
	mv avrusb.rar c:\!MyDoc\FLOPPI\avr\usb\obdev.at
	autoname /pattern:YYMMDDhhmmss c:/!MyDoc/FLOPPI/avr/usb/obdev.at/avrusb.rar
	