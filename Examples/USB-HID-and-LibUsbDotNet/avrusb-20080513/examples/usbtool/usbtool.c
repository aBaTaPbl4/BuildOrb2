/* Имя: usbtool.c
 * Проект: примеры AVR-USB examples, программа для хоста
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru
 * Дата создания: 2008-04-06
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt) 
 * Ревизия: $Id: usbtool.c 553 2008-04-17 19:00:20Z cs $
 */

/*
Основное описание:
Эта утилита командной строки может выполнять разные запросы к произвольным 
устройствам USB. Она разработана как универсальный инструмент для хоста для 
экспериментирования и отладки. Программа должна быть слинкована с libusb, 
библиотекой для доступа к шине USB для Linux, FreeBSD, Mac OS X и других 
операционных систем семейства Unix.
Libusb может быть получена по ссылке http://libusb.sourceforge.net/.
На Windows используйте libusb-win32 по ссылке http://libusb-win32.sourceforge.net/.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>

#include <usb.h>        /* это libusb, см. http://libusb.sourceforge.net/ */
#include "opendevice.h" /* общий код перемещен в отдельный модуль */

#define DEFAULT_USB_VID         0   /* любой */
#define DEFAULT_USB_PID         0   /* любой */

static void usage(char *name)
{
    fprintf(stderr, "usage: %s [options] <command>\n", name);
    fprintf(stderr,
        "Options are:\n"
        "  -h or -? (print this help and exit)\n"
        "  -v <vendor-id> (defaults to 0x%x, can be '*' for any VID)\n"
        "  -p <product-id> (defaults to 0x%x, can be '*' for any PID)\n"
        "  -V <vendor-name-pattern> (shell style matching, defaults to '*')\n"
        "  -P <product-name-pattern> (shell style matching, defaults to '*')\n"
        "  -S <serial-pattern> (shell style matching, defaults to '*')\n"
        "  -d <databytes> (data byte for request, comma separated list)\n"
        "  -D <file> (binary data for request taken from file)\n"
        "  -O <file> (write received data bytes to file)\n"
        "  -b (binary output format, default is hex)\n"
        "  -n <count> (maximum number of bytes to receive)\n"
        "  -e <endpoint> (specify endpoint for some commands)\n"
        "  -t <timeout> (specify USB timeout in milliseconds)\n"
        "  -c <configuration> (device configuration to choose)\n"
        "  -i <interface> (configuration interface to claim)\n"
        "  -w (suppress USB warnings, default is verbose)\n"
        "\n"
        "Commands are:\n"
        "  list (list all matching devices by name)\n"
        "  control in|out <type> <recipient> <request> <value> <index> (send control request)\n"
        "  interrupt in|out (send or receive interrupt data)\n"
        "  bulk in|out (send or receive bulk data)\n"
        "For valid enum values for <type> and <recipient> pass \"x\" for the value.\n"
        "Objective Development's free VID/PID pairs are:\n"
        "  5824/1500 for vendor class devices\n"
        "  5824/1503 for HID class devices excluding mice and keyboards\n"
        "  5824/1505 for CDC-ACM class devices\n"
        "  5824/1508 for MIDI class devices\n"
        , DEFAULT_USB_VID, DEFAULT_USB_PID
    );


}

static int  vendorID = DEFAULT_USB_VID;
static int  productID = DEFAULT_USB_PID;
static char *vendorNamePattern = "*";
static char *productNamePattern = "*";
static char *serialPattern = "*";
static char *sendBytes = NULL;
static int  sendByteCount;
static char *outputFile = NULL;
static int  endpoint = 0;
static int  outputFormatIsBinary = 0;
static int  showWarnings = 1;
static int  usbTimeout = 5000;
static int  usbCount = 128;
static int  usbConfiguration = 1;
static int  usbInterface = 0;

static int  usbDirection, usbType, usbRecipient, usbRequest, usbValue, usbIndex; /* аргументы для управляющей передачи (control transfer) */

/* ------------------------------------------------------------------------- */

/* преобразование ASCII в целое [integer] (парсинг чисел), которое позволяет на входе использовать 
 *  числа hex (с префиксом 0x), воьмеричные octal (префикс 0) и десятичные (префикс 1-9).
 */
static int  myAtoi(char *text)
{
long    l;
char    *endPtr;

    if(strcmp(text, "*") == 0)
        return 0;
    l = strtol(text, &endPtr, 0);
    if(endPtr == text){
        fprintf(stderr, "warning: can't parse numeric parameter ->%s<-, defaults to 0.\n", text);
        l = 0;
    }else if(*endPtr != 0){
        fprintf(stderr, "warning: numeric parameter ->%s<- only partially parsed.\n", text);
    }
    return l;
}

static int  parseEnum(char *text, ...)
{
va_list vlist;
char    *entries[64];
int     i, numEntries;

    va_start(vlist, text);
    for(i = 0; i < 64; i++){
        entries[i] = va_arg(vlist, char *);
        if(entries[i] == NULL)
            break;
    }
    numEntries = i;
    va_end(vlist);
    for(i = 0; i < numEntries; i++){
        if(strcasecmp(text, entries[i]) == 0)
            return i;
    }
    if(isdigit(*text)){
        return myAtoi(text);
    }
    fprintf(stderr, "Enum value \"%s\" not allowed. Allowed values are:\n", text);
    for(i = 0; i < numEntries; i++){
        fprintf(stderr, "  %s\n", entries[i]);
    }
    exit(1);
}

/* ------------------------------------------------------------------------- */

#define ACTION_LIST         0
#define ACTION_CONTROL      1
#define ACTION_INTERRUPT    2
#define ACTION_BULK         3

int main(int argc, char **argv)
{
usb_dev_handle  *handle = NULL;
int             opt, len, action, argcnt;
char            *myName = argv[0], *s, *rxBuffer = NULL;
FILE            *fp;

    while((opt = getopt(argc, argv, "?hv:p:V:P:S:d:D:O:e:n:tbw")) != -1){
        switch(opt){
        case 'h':
        case '?':   /* -h или -? (выводит эту подсказку, и затем происходит выход из программы) */
            usage(myName);
            exit(1);
        case 'v':   /* -v <vendor-id> (default в 0x%x, может быть '*' для любого VID) */
            vendorID = myAtoi(optarg);
            break;
        case 'p':   /* -p <product-id> (default в 0x%x, может быть '*' для любого PID) */
            productID = myAtoi(optarg);
            break;
        case 'V':   /* -V <vendor-name-pattern> (соответствие в стиле shell, default в '*') */
            vendorNamePattern = optarg;
            break;
        case 'P':   /* -P <product-name-pattern> (соответствие в стиле shell, default в '*') */
            productNamePattern = optarg;
            break;
        case 'S':   /* -S <serial-pattern> (соответствие в стиле shell, default в '*') */
            serialPattern = optarg;
            break;
        case 'd':   /* -d <databytes> (байты данных для запросов, передаваемых в командной строке) */
            while((s = strtok(optarg, ", ")) != NULL){
                optarg = NULL;
                if(sendBytes != NULL){
                    sendBytes = realloc(sendBytes, sendByteCount + 1);
                }else{
                    sendBytes = malloc(sendByteCount + 1);
                }
                sendBytes[sendByteCount++] = myAtoi(s);
            }
            break;
        case 'D':   /* -D <file> (байты данных для запросов, передаваемых через содержимое файла) */
            if((fp = fopen(optarg, "rb")) == NULL){
                fprintf(stderr, "error opening %s: %s\n", optarg, strerror(errno));
                exit(1);
            }
            fseek(fp, 0, SEEK_END);
            len = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            if(sendBytes != NULL){
                sendBytes = realloc(sendBytes, sendByteCount + len);
            }else{
                sendBytes = malloc(sendByteCount + len);
            }
            fread(sendBytes + sendByteCount, 1, len, fp);   /* нуждался бы в проверке ошибок */
            sendByteCount += len;
            fclose(fp);
            break;
        case 'O':   /* -O <file> (запись принятых байт данных в файл) */
            outputFile = optarg;
            break;
        case 'e':   /* -e <endpoint> (указание конечной точки для некоторых команд) */
            endpoint = myAtoi(optarg);
            break;
        case 't':   /* -t <timeout> (указание таймаута USB в милисекундах) */
            usbTimeout = myAtoi(optarg);
            break;
        case 'b':   /* -b (двоичный формат вывода, default hex) */
            outputFormatIsBinary = 1;
            break;
        case 'n':   /* -n <count> (max количество байт для приема) */
            usbCount = myAtoi(optarg);
            break;
        case 'c':   /* -c <configuration> (выбор конфигурации устройства) */
            usbConfiguration = myAtoi(optarg);
            break;
        case 'i':   /* -i <interface> (требуемый интерфейс конфигурации) */
            usbInterface = myAtoi(optarg);
            break;
        case 'w':   /* -w (подавление предупреждений USB, default - подробные предупреждения) */
            showWarnings = 0;
            break;
        default:
            fprintf(stderr, "Option -%c unknown\n", opt);
            exit(1);
        }
    }
    argc -= optind;
    argv += optind;
    if(argc < 1){
        usage(myName);
        exit(1);
    }
    argcnt = 2;
    if(strcasecmp(argv[0], "list") == 0){
        action = ACTION_LIST;
        argcnt = 1;
    }else if(strcasecmp(argv[0], "control") == 0){
        action = ACTION_CONTROL;
        argcnt = 7;
    }else if(strcasecmp(argv[0], "interrupt") == 0){
        action = ACTION_INTERRUPT;
    }else if(strcasecmp(argv[0], "bulk") == 0){
        action = ACTION_BULK;
    }else{
        fprintf(stderr, "command %s not known\n", argv[0]);
        usage(myName);
        exit(1);
    }
    if(argc < argcnt){
        fprintf(stderr, "Not enough arguments.\n");
        usage(myName);
        exit(1);
    }
    if(argc > argcnt){
        fprintf(stderr, "Warning: only %d arguments expected, rest ignored.\n", argcnt);
    }
    usb_init();
    if(usbOpenDevice(&handle, vendorID, vendorNamePattern, productID, productNamePattern, serialPattern, action == ACTION_LIST ? stdout : NULL, showWarnings ? stderr : NULL) != 0){
        fprintf(stderr, "Could not find USB device with VID=0x%x PID=0x%x Vname=%s Pname=%s Serial=%s\n", vendorID, productID, vendorNamePattern, productNamePattern, serialPattern);
        exit(1);
    }
    if(action == ACTION_LIST)
        exit(0);                /* мы сделали то, что нас уже попросили сделать */
    usbDirection = parseEnum(argv[1], "out", "in", NULL);
    if(usbDirection){   /* IN transfer */
        rxBuffer = malloc(usbCount);
    }
    if(action == ACTION_CONTROL){
        int requestType;
        usbType = parseEnum(argv[2], "standard", "class", "vendor", "reserved", NULL);
        usbRecipient = parseEnum(argv[3], "device", "interface", "endpoint", "other", NULL);
        usbRequest = myAtoi(argv[4]);
        usbValue = myAtoi(argv[5]);
        usbIndex = myAtoi(argv[6]);
        requestType = ((usbDirection & 1) << 7) | ((usbType & 3) << 5) | (usbRecipient & 0x1f);
        if(usbDirection){   /* IN transfer */
            len = usb_control_msg(handle, requestType, usbRequest, usbValue, usbIndex, rxBuffer, usbCount, usbTimeout);
        }else{              /* передача OUT */
            len = usb_control_msg(handle, requestType, usbRequest, usbValue, usbIndex, sendBytes, sendByteCount, usbTimeout);
        }
    }else{  /* лолжно быть ACTION_INTERRUPT или ACTION_BULK */
        int retries = 1;
        if(usb_set_configuration(handle, usbConfiguration) && showWarnings){
            fprintf(stderr, "Warning: could not set configuration: %s\n", usb_strerror());
        }
        /* теперь попробуем затребовать интерфейс и отсоединить (detach) драйвер ядра HID на
         * linux и других операционных системах, которые поддерживают вызов.
         */
        while((len = usb_claim_interface(handle, usbInterface)) != 0 && retries-- > 0){
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
            if(usb_detach_kernel_driver_np(handle, 0) < 0 && showWarnings){
                fprintf(stderr, "Warning: could not detach kernel driver: %s\n", usb_strerror());
            }
#endif
        }
        if(len != 0 && showWarnings)
            fprintf(stderr, "Warning: could not claim interface: %s\n", usb_strerror());
        if(action == ACTION_INTERRUPT){
            if(usbDirection){   /* передача IN */
                len = usb_interrupt_read(handle, endpoint, rxBuffer, usbCount, usbTimeout);
            }else{
                len = usb_interrupt_write(handle, endpoint, sendBytes, sendByteCount, usbTimeout);
            }
        }else{
            if(usbDirection){   /* передача IN */
                len = usb_bulk_read(handle, endpoint, rxBuffer, usbCount, usbTimeout);
            }else{
                len = usb_bulk_write(handle, endpoint, sendBytes, sendByteCount, usbTimeout);
            }
        }
    }
    if(len < 0){
        fprintf(stderr, "USB error: %s\n", usb_strerror());
        exit(1);
    }
    if(usbDirection == 0)   /* OUT */
        printf("%d bytes sent.\n", len);
    if(rxBuffer != NULL){
        FILE *fp = stdout;
        if(outputFile != NULL){
            fp = fopen(outputFile, outputFormatIsBinary ? "wb" : "w");
            if(fp == NULL){
                fprintf(stderr, "Error writing \"%s\": %s\n", outputFile, strerror(errno));
                exit(1);
            }
        }
        if(outputFormatIsBinary){
            fwrite(rxBuffer, 1, len, fp);
        }else{
            int i;
            for(i = 0; i < len; i++){
                if(i != 0){
                    if(i % 16 == 0){
                        fprintf(fp, "\n");
                    }else{
                        fprintf(fp, " ");
                    }
                }
                fprintf(fp, "0x%02x", rxBuffer[i] & 0xff);
            }
            if(i != 0)
                fprintf(fp, "\n");
        }
    }
    usb_close(handle);
    if(rxBuffer != NULL)
        free(rxBuffer);
    return 0;
}
