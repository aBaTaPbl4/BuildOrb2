//#define F_CPU 20000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include <avr/pgmspace.h>   /* нужно для usbdrv.h */
#include "usbdrv.h"


struct dataexchange_t       // Описание структуры для передачи данных
{
   uchar red;        // Я решил для примера написать структуру на 3 байта.
   uchar green;        // На каждый байт подцепим ногу из PORTB. Конечно это
   uchar blue;        // не рационально (всего то 3 бита нужно).
};                  // Но в целях демонстрации в самый раз.
                    // Для наглядности прикрутить по светодиоду и созерцать :)


struct dataexchange_t pdata = {1, 0, 0};
	
#define RED_BIT 3
#define GREEN_BIT 4
#define BLUE_BIT 0


PROGMEM const char usbHidReportDescriptor[22] = { // USB report descriptor         // Дескриптор описывает структуру пакета данных для обмена
    0x06, 0x00, 0xff,                       // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                             // USAGE (Vendor Usage 1)
    0xa1, 0x01,                             // COLLECTION (Application)
    0x15, 0x00,                             //    LOGICAL_MINIMUM (0)        // min. значение для данных
    0x26, 0xff, 0x00,                       //    LOGICAL_MAXIMUM (255)      // max. значение для данных, 255 тут не случайно, а чтобы уложиться в 1 байт
    0x75, 0x08,                             //    REPORT_SIZE (8)            // информация передается порциями, это размер одного "репорта" 8 бит
    0x95, sizeof(struct dataexchange_t),    //    REPORT_COUNT               // количество порций (у нашем примере = 3, описанная выше структура передастся за три репорта)
    0x09, 0x00,                             //    USAGE (Undefined)
    0xb2, 0x02, 0x01,                       //    FEATURE (Data,Var,Abs,Buf)
    0xc0                                    // END_COLLECTION
};
/* Здесь мы описали только один report, из-за чего не нужно использовать report-ID (он должен быть первым байтом).
 * С его помощью передадим 3 байта данных (размер одного REPORT_SIZE = 8 бит = 1 байт, их количество REPORT_COUNT = 3).
 */


/* Эти переменные хранят статус текущей передачи */
static uchar    currentAddress;
static uchar    bytesRemaining;


/* usbFunctionRead() вызывается когда хост запрашивает порцию данных от устройства
 * Для дополнительной информации см. документацию в usbdrv.h
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    if(len > bytesRemaining)
        len = bytesRemaining;

    uchar *buffer = (uchar*)&pdata;

    if(!currentAddress)        // Ни один кусок данных еще не прочитан.
    {                          // Заполним структуру для передачи
        if ( PINB & _BV(RED_BIT) )
            pdata.red = 1;
        else
            pdata.red = 0;


        if ( PINB & _BV(GREEN_BIT) )
            pdata.green = 1;
        else
            pdata.green = 0;


        if ( PINB & _BV(BLUE_BIT) )
            pdata.blue = 1;
        else
            pdata.blue = 0;
    }

    uchar j;
    for(j=0; j<len; j++)
        data[j] = buffer[j+currentAddress];

    currentAddress += len;
    bytesRemaining -= len;
    return len;
}


/* usbFunctionWrite() вызывается когда хост отправляет порцию данных к устройству
 * Для дополнительной информации см. документацию в usbdrv.h
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if(bytesRemaining == 0)
        return 1;               /* конец передачи */

    if(len > bytesRemaining)
        len = bytesRemaining;

    uchar *buffer = (uchar*)&pdata;
    
    uchar j;
    for(j=0; j<len; j++)
        buffer[j+currentAddress] = data[j];

    currentAddress += len;
    bytesRemaining -= len;

    if(bytesRemaining == 0)     // Все данные получены
    {                           // Выставим значения на PORTB
        if ( pdata.red )
            PORTB |= _BV(RED_BIT);
        else
            PORTB &= ~_BV(RED_BIT);


        if ( pdata.green )
            PORTB |= _BV(GREEN_BIT);
        else
            PORTB &= ~_BV(GREEN_BIT);


        if ( pdata.blue )
            PORTB |= _BV(BLUE_BIT);
        else
            PORTB &= ~_BV(BLUE_BIT);
    }

    return bytesRemaining == 0; /* 0 означает, что есть еще данные */
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = static_cast<usbRequest_t*>((void*)data);

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID устройство */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            // у нас только одна разновидность репорта, можем игнорировать report-ID
            bytesRemaining = sizeof(struct dataexchange_t);
            currentAddress = 0;
            return USB_NO_MSG;  // используем usbFunctionRead() для отправки данных хосту
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            // у нас только одна разновидность репорта, можем игнорировать report-ID
            bytesRemaining = sizeof(struct dataexchange_t);
            currentAddress = 0;
            return USB_NO_MSG;  // используем usbFunctionWrite() для получения данных от хоста
        }
    }else{
        /* остальные запросы мы просто игнорируем */
    }
    return 0;
}
/* ------------------------------------------------------------------------- */

int main(void)
{
    DDRB = 0b00000110;      // PB1,PB2,PB3 - выход

    usbInit();
    usbDeviceDisconnect();  // принудительно отключаемся от хоста, так делать можно только при выключенных прерываниях!
    
    uchar i = 0;
    while(--i){             // пауза > 250 ms
        _delay_ms(1);
    }
    
    usbDeviceConnect();     // подключаемся

    sei();                  // разрешаем прерывания

    for(;;){                // главный цикл программы
        usbPoll();          // эту функцию надо регулярно вызывать с главного цикла, максимальная задержка между вызовами - 50 ms
    }
    return 0;
}
/* ------------------------------------------------------------------------- */ 