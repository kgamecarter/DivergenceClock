#ifndef main_H
#define main_H

#include "DS1307.h"
#include "Types.h"


#define UART_IN_DATA_MAXSIZE 16

typedef void (*Action)();

typedef struct _modeLink
{
    Action scanAction;
    Action onEnter;
    Action onExit;
    Action button1Press;
    Action button1Hold;
    Action button2Press;
    Action button3Press;
    Action timerInterrupt;
} ModeLink;


extern ModeLink* mode;
extern DateTime date;
extern byte defaultLightness;
extern byte tCount;


extern idata byte uartInData[UART_IN_DATA_MAXSIZE];
extern idata byte* uartOutData;
extern idata byte uartOutType, uartOutSize;

extern byte crossfade;

void timeUpdate();
void memcpyA(byte* dest, byte* sour, byte count);
void changeMode(ModeLink* newMode);

#endif