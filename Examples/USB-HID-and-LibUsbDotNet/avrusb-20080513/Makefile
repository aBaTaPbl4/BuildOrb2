# Хей, Emacs, это -*- makefile -*-
#----------------------------------------------------------------------------
# Шаблон Makefile WinAVR, написанный Eric B. Weddington, Jцrg Wunsch и другие.
#
# Опубликовано в публичный домен (Public Domain)
#
# Дополнительные материалы для этого makefile написаны авторами:
# Peter Fleury
# Tim Henigan
# Colin O'Flynn
# Reiner Patommel
# Markus Pfaff
# Sander Pool
# Frederik Rouleau
# Carlos Lamas
#
#----------------------------------------------------------------------------
# Использование этого Makefile через командную строку:
#
# make all = сборка программы.
#
# make clean = очистка от временных файлов проекта (образуются при сборке).
#
# make coff = конвертирование формата ELF в формат AVR COFF.
#
# make extcoff = конвертирование формата ELF в формат AVR Extended COFF.
#
# make program = загрузка hex-файла ("прошивка", содержащая программу) в устройство 
#                с использованием программатора avrdude. Пожалуйста, предварительно
#                измените необходимые настройки avrdude (указаны далее)!
#
# make debug = запуск либо simulavr или avarice как указано для отладки, 
#              вместе с avr-gdb или avr-insight в качестве front end для отладки.
#
# make filename.s = просто скомпилировать filename.c только в ассемблерный код.
#
# make filename.i = создать предварительно обработанный (preprocessed) исходный файл
#					для использования в сообщениях об ошибках (bug reports) 
#					проекту GCC.
#
# Для пересборки проекта сделайте "make clean", а затем "make all".
#----------------------------------------------------------------------------


# имя (тип микроконтроллера) MCU
MCU = atmega16


# Тактовая частота процессора в Герцах.
#     Это создает символ F_CPU, который во всем исходном коде эквивалентен значению 
#      тактовой частоты микроконтроллера. Вы можете использовать этот символ в исходном
#      коде для вычисления интервалов времени. НЕ ДОБАВЛЯЙТЕ в конец 'UL', это автоматически
#      создаст 32-битное значение в Вашем исходном коде.
#     Типичные значения F_CPU:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
F_CPU = 8000000


# Выходной формат (может быть srec, ihex, binary).
FORMAT = ihex


# Целевое имя файла (без расширения).
TARGET = main


# Папка для объектных файлов
#     Для того, чтобы объектные файлы оставались в текущей директории, используйте точку (.), 
#      НЕ ОСТАВЛЯЙТЕ этот параметр пустым или blank macro!
OBJDIR = .


# Здесь указан список исходных файлов на языке C (C зависимости генерируются автоматически).
SRC = $(TARGET).c 


# То же самое, но для файлов C++ (C зависимости генерируются автоматически).
CPPSRC = 


# Список исходных файлов на ассемблере.
#     Делайте всегда эти файлы оканчивающимися на .S (буква "S" заглавная). Файлы, оканчивающиеся
#      на маленькую .s, не рассматриваются как исходные файлы, однако рассматриваются как 
#      сгенерированные файлы (ассемблерный вывод от компилятора), и будут удалены командой
#      "make clean"!
#     Поскольку файловая система DOS/Win* не делает различий между .s and .S,
#      разбор имен файлов сохраняется, и gcc сам заботится о том, как имя записывается по буквам 
#      в его командной строке.
ASRC =


# Уровень оптимизации, может быть [0, 1, 2, 3, s]. 
#     0 = выключить оптимизацию. s = оптимизировать по размеру.
#     (замечание: 3 не всегда лучший уровень оптимизации. См. avr-libc FAQ.)
OPT = 0


# Отладочный формат.
#     Естественные форматы для AVR-GCC (опция -g) являются dwarf-2 [default] или stabs.
#     AVR Studio 4.10 требует dwarf-2.
#     AVR [Extended] COFF формат требует stabs, плюс запуск утилиты avr-objcopy.
DEBUG = stabs


# Список путей до дополнительных папок, где будут искаться включаемые файлы (директива #include).
#     Каждое путь до папки должен быть отделен пробелом. Используйте прямой слеш (/)
#      в качестве разделителя директорий в пути. Для папки, путь до которой имеет пробелы,
#      используйте путь, заключенный в кавычки.
EXTRAINCDIRS = 


# Флаги компилятора для установки стандартного уровня C.
#     c89   = "ANSI" C
#     gnu89 = c89 плюс расширения GCC
#     c99   = ISO C99 стандарт (пока не полностью реализованный)
#     gnu99 = c99 плюс расширения GCC
CSTANDARD = -std=gnu99


# Поместите здесь опции -D или -U для исходников C
CDEFS = -DF_CPU=$(F_CPU)UL


# Поместите здесь опции -D или -U для исходников ASM
ADEFS = -DF_CPU=$(F_CPU)


# Поместите здесь опции -D или -U для исходников C++
CPPDEFS = -DF_CPU=$(F_CPU)UL
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS



#---------------- Опции компилятора C ----------------
#  -g*:          генерировать отладочную информацию
#  -O*:          уровень оптимизации
#  -f...:        настройка, см. руководство GCC и документацию avr-libc
#  -Wall...:     уровень предупреждающих сообщений
#  -Wa,...:      говорит GCC передать это ассемблеру.
#    -adhlns...: создание листинга ассемблера
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
#CFLAGS += -mshort-calls
#CFLAGS += -fno-unit-at-a-time
#CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)


#---------------- Опции компилятора C++ ----------------
#  -g*:          генерировать отладочную информацию
#  -O*:          уровень оптимизации
#  -f...:        настройка, см. руководство GCC и документацию avr-libc
#  -Wall...:     уровень предупреждающих сообщений
#  -Wa,...:      говорит GCC передать это ассемблеру.
#    -adhlns...: создание листинга ассемблера
CPPFLAGS = -g$(DEBUG)
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += -Wall
CFLAGS += -Wundef
#CPPFLAGS += -mshort-calls
#CPPFLAGS += -fno-unit-at-a-time
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wa,-adhlns=$(<:%.cpp=$(OBJDIR)/%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
#CPPFLAGS += $(CSTANDARD)


#---------------- Опции ассемблера ----------------
#  -Wa,...:   говорит GCC передать это ассемблеру.
#  -adhlns:   создать листинг
#  -gstabs:   указывает ассемблеру создавать информацию о номерах строк; примите во внимание,
#             что для использования в файлах COFF необходима дополнительная информация 
#             об именах файлов и функций, представленная в ассемблерных исходных файлах,
#             см. avr-libc документацию [FIXME: пока не описано здесь]
#  -listing-cont-lines: устанавливает максимальное число строк продолжения hex-дампа
#             которые будут отображаться для единственной строки исходного файла.
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs,--listing-cont-lines=100


#---------------- Библиотечные опции ----------------
# минималистская версия printf
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# версия printf, поддерживающая плавающую точку (требует MATH_LIB = -lm ниже)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# Если этот параметр оставить пустым, будет использоваться стандартная версия printf.
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)


# минималистская версия scanf
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Плавающая точка + %[ версия scanf (требует MATH_LIB = -lm ниже)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# Если этот параметр оставить пустым, будет использоваться стандартная версия scanf.
SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)


MATH_LIB = -lm


# Список любых дополнительных папок, где при компиляции будут искаться библиотеки.
#     Каждое путь до папки должен быть отделен пробелом. Используйте прямой слеш (/)
#      в качестве разделителя директорий в пути. Для папки, путь до которой имеет пробелы,
#      используйте путь, заключенный в кавычки.
EXTRALIBDIRS = 



#---------------- Опции внешней (External) памяти ----------------

# 64 KB внешней RAM, начинающаяся после внутренней RAM (ATmega128!),
#  используется для переменных (.data/.bss) и кучи (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB внешней RAM, начинающаяся после внутренней RAM (ATmega128!),
#  используется только для кучи (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =



#---------------- Опции линкера ----------------
#  -Wl,...:     говорит GCC передать это линкеру.
#    -Map:      создать map-файл
#    --cref:    добавить перекрестную ссылку на map-файл
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)
#LDFLAGS += -T linker_script.x



#---------------- Опции программатора (avrdude) ----------------

# Программирующее железо
# Введите: avrdude -c ?
# для получения полного листинга.
#
AVRDUDE_PROGRAMMER = jtagmkII

# com1 = последовательный порт. Используйте lpt1 для подсоединения через параллельный порт.
AVRDUDE_PORT = usb

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep


# Раскомментируйте следующее, если Вы хотите стереть счетчик циклов avrdude.
# Примите во внимание, что этот счетчик сначала должен быть проинициализирован
#  с использованием -Yn, смотрите руководство программатора avrdude.
#AVRDUDE_ERASE_COUNTER = -y

# Раскомментируйте следующее, если Вы хотите /не хотите/ сделать верификацию
#  после программирования устройства.
#AVRDUDE_NO_VERIFY = -V

# Увеличить уровень "многословности" (уровень информационности сообщений). Пожалуйста
#  используйте это для сообщения о багах avrdude. См. <http://savannah.nongnu.org/projects/avrdude> 
#  по поводу bug-репортов.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)



#---------------- Опции отладки ----------------

# Только для simulavr - тактовая частота целевого MCU.
DEBUG_MFREQ = $(F_CPU)

# Установите DEBUG_UI либо в gdb, либо в insight.
# DEBUG_UI = gdb
DEBUG_UI = insight

# Устанавливает back-end отладки либо в avarice, либо в simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

# имя файла GDB Init.
GDBINIT_FILE = __avr_gdbinit

# установка для JTAG, когда используется avarice
JTAG_DEV = /dev/com1

# Порт отладки, используемы для коммуникации между GDB / avarice / simulavr.
DEBUG_PORT = 4242

# Отлаживаемый хост, используемый для коммуникации между GDB / avarice / simulavr, 
#     нормально устанавливается в localhost, за исключением сумасшедшейотладки 
#     некоторого сорта, когда avarice запускается на другом компьютере.
DEBUG_HOST = localhost



#============================================================================


# Определение программ и команд.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
WINSHELL = cmd


# Определение сообщений
# (на английском языке)
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:




# Определение всех объектных файлов.
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Определение всех файлов листинга.
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst) 


# Флаги компилятора для генерирования файлов зависимости (dependency files).
#  Если сказать по-человечески, то для генерирования выходных файлов.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d


# Комбинирование всех необходимых и опциональных флагов.
# Добавление целевого процессора к флагам.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)





# Цель по умолчанию (Default target).
all: begin gccversion sizebefore build sizeafter end

# изменение цели сборки (build target) для сборки HEX-файла или библиотеки.
build: elf hex eep lss sym extcoff
#сборка: lib


elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym
LIBNAME=lib$(TARGET).a
lib: $(LIBNAME)



# Eye candy.
# AVR Studio 3.x не проверяет код выхода из make, но зависит от следующих 
#  волшебных строк, которые генерирует процесс компиляции.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo


# Отображение размера файла.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	2>/dev/null; echo; fi



# Отображение версии компилятора.
gccversion : 
	@$(CC) --version



# Прошивка устройства.
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)


# Генерация файла avr-gdb config/init которая делает следующее:
#     задает сигнал сброса, загружает целевой файл, соединяется с целью и
#     устанавливает точку останова в тело main().
gdb-config: 
	@$(REMOVE) $(GDBINIT_FILE)
	@echo define reset >> $(GDBINIT_FILE)
	@echo SIGNAL SIGHUP >> $(GDBINIT_FILE)
	@echo end >> $(GDBINIT_FILE)
	@echo file $(TARGET).elf >> $(GDBINIT_FILE)
	@echo target remote $(DEBUG_HOST):$(DEBUG_PORT)  >> $(GDBINIT_FILE)
ifeq ($(DEBUG_BACKEND),simulavr)
	@echo load  >> $(GDBINIT_FILE)
endif
	@echo break main >> $(GDBINIT_FILE)

debug: gdb-config $(TARGET).elf
ifeq ($(DEBUG_BACKEND), avarice)
	@echo Starting AVaRICE - Press enter when "waiting to connect" message displays.
	@$(WINSHELL) /c start avarice --jtag $(JTAG_DEV) --erase --program --file \
	$(TARGET).elf $(DEBUG_HOST):$(DEBUG_PORT)
	@$(WINSHELL) /c pause

else
	@$(WINSHELL) /c start simulavr --gdbserver --device $(MCU) --clock-freq \
	$(DEBUG_MFREQ) --port $(DEBUG_PORT)
endif
	@$(WINSHELL) /c start avr-$(DEBUG_UI) --command=$(GDBINIT_FILE)




# Конвертация ELF в COFF для использования при отладке / симуляции в AVR Studio или VMLAB.
COFFCONVERT = $(OBJCOPY) --debugging
COFFCONVERT += --change-section-address .data-0x800000
COFFCONVERT += --change-section-address .bss-0x800000
COFFCONVERT += --change-section-address .noinit-0x800000
COFFCONVERT += --change-section-address .eeprom-0x810000



coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof


extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof



# Создание конечных выходных файлов (.hex, .eep) из выходного файла ELF.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O $(FORMAT) $< $@ || exit 0

# Создание расширенного файла листинга из выходного файла ELF.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S -z $< > $@

# Создание таблицы символов из выходного файла ELF.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@



# Создание библиотеки из объектных файлов.
.SECONDARY : $(TARGET).a
.PRECIOUS : $(OBJ)
%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)


# Линковка: создание выходного файла ELF из объектных файлов.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)


# Компиляция: создание объектных файлов из исходных файлов на языке C.
$(OBJDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Компиляция: создание объектных файлов из исходных файлов на языке C++.
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@ 


# Компиляция: создание файлов ассемблера из исходных файлов на языке C.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Компиляция: создание файлов ассемблера из исходных файлов на языке C++.
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@


# Ассемблирование: создание объектных файлов из исходных файлов на языке ассемблера.
$(OBJDIR)/%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Создание предварительно обработанного (preprocessed) источника для использования
#  в отправке баг-репорта.
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@ 


# Цель: очистка проекта.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/%.o)
	$(REMOVE) $(SRC:%.c=$(OBJDIR)/%.lst)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVEDIR) .dep


# Создание папки для объектных файлов
$(shell mkdir $(OBJDIR) 2>/dev/null)


# Включение файлов зависимостей (dependency).
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Распечатка ложных (phony) целей.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff \
clean clean_list program debug gdb-config


