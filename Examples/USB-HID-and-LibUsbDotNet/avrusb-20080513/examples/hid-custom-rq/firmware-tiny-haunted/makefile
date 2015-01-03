###############################################################################
# Makefile for the project hid-custom-rq
###############################################################################

## General Flags
PROJECT = hid-custom-rq
MCU = attiny85
TARGET = hid-custom-rq.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99             -DF_CPU=16500000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 
## Include Directories
CFLAGS += -Iusbdrv -I.
CFLAGS += -DDEBUG_LEVEL=0

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=hid-custom-rq.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o oddebug.o usbdrv.o usbdrvasm.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) hid-custom-rq.hex hid-custom-rq.eep hid-custom-rq.lss size

## Compile
usbdrvasm.o: usbdrv/usbdrvasm.S
	$(CC) $(ASMFLAGS) -c  $<

main.o: main.c
	$(CC) $(CFLAGS) -c  $<

oddebug.o: usbdrv/oddebug.c
	$(CC) $(CFLAGS) -c  $<

usbdrv.o: usbdrv/usbdrv.c
	$(CC) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) hid-custom-rq.elf dep/* hid-custom-rq.hex hid-custom-rq.eep hid-custom-rq.lss hid-custom-rq.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

