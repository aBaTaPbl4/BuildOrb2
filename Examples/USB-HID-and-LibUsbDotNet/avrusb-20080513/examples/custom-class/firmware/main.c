/* Имя: main.c
 * Проект: custom-class, базовый пример устройства USB
 * Автор: Christian Starkjohann
 * Перевод: microsin.ru 
 * Дата создания: 2008-04-09
 * Табуляция: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * Лицензия: GNU GPL v2 (см. License.txt) или проприетарная (CommercialLicense.txt)
 * Ревизия: $Id: main.c 602 2008-05-13 15:15:13Z cs $
 */

/*
Этот пример должен работать на многих AVR с небольшими изменениями. Не используются
никакие специальные аппаратные ресурсы, кроме INT0. Вы можете изменить usbconfig.h для
других ножек I/O USB. Пожалуйста, имейте в виду, что USB D+ должен быть подсоединен к
ножке INT0. Мы предполагаем, что LED подсоединен на порт B разряд 0. Если Вы подсоединили
его на другую ножку, поменяйте макрос ниже:
*/
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_BIT             0

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* для sei() */
#include <util/delay.h>     /* для _delay_ms() */

#include <avr/pgmspace.h>   /* требуется для usbdrv.h */
#include "usbdrv.h"
#include "requests.h"       /* Мы используем пользовательские номера запросов */

/* ------------------------------------------------------------------------- */
/* ----------------------------- интерфейс USB ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;
static uchar    dataBuffer[4];  /* буфер должен оставаться валидным при возврате из usbFunctionSetup */

    if(rq->bRequest == CUSTOM_RQ_ECHO)
    { /* эхо -- используется для тестов надежности */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* говорим драйверу, какие данные будут возвращены */
        return 4;
    }
    else if(rq->bRequest == CUSTOM_RQ_SET_STATUS)
    {
        if(rq->wValue.bytes[0] & 1){    /* устанавливаем LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        }else{                          /* очищаем LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    }
    else if(rq->bRequest == CUSTOM_RQ_GET_STATUS)
    {
        dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
        usbMsgPtr = dataBuffer;          /* говорим драйверу, какие данные будут возвращены */
        return 1;                       /* говорим драйверу послать 1 байт */
    }
    return 0;   /* default для не реализованных запросов: на хост обратно данные не возвращаются */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
uchar   i;

    wdt_enable(WDTO_1S);
    /* Если не используется watchdog, отключите эту строчку. Для новых устройств
        статус watchdog (on/off, period) СОХРАНЯЕТСЯ ПОСЛЕ СБРОСА!
     */
    /* Статус RESET: все ножки портов в режиме ввода без нагрузочных резисторов (pull-up).
        Это то, что нужно для D+ and D-, таким образом, мы не нуждаемся в дополнительной аппаратной
        инициализации.
     */
    usbInit();              // см. usbdrv.h и usbdrv.c
    usbDeviceDisconnect();  /* см. usbdrv.h - запускает реэнумерацию, делаем это, пока отключены прерывания! */
    i = 0;
    while(--i)
    {            /* подделывам USB disconnect на время > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();     // см. usbdrv.h
    LED_PORT_DDR |= _BV(LED_BIT);   /* переключаем ножку LED в режим вывода */
    sei();
    for(;;){                /* главный цикл события */
        wdt_reset();
        usbPoll();
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
