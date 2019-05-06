#include "Display.h"
#include "MarqueeMode.h"
#include "DateMode.h"
#include "ISD4004.h"

#define LENGTH 25
   
void MarqueeMode_onEnter();
void MarqueeMode_onExit();
void scanMarquee();
void marqueeMode_Button1Press();
void marqueeMode_Button1Hold();
void marqueeMode_Button2Press();
void marqueeMode_Button3Press();
code ModeLink marqueeMode =
{
    scanMarquee,
    MarqueeMode_onEnter,
    MarqueeMode_onExit,
    marqueeMode_Button1Press,
    marqueeMode_Button1Hold,
    marqueeMode_Button2Press,
    marqueeMode_Button3Press,
    timeUpdate
};

void MarqueeMode_onEnter()
{
    cursorPosition = 0;
    twinkleCount = 0;
    DS1307_getDateTime(&date);
    ISD_PowerUp();
}

void MarqueeMode_onExit()
{
    cursorPosition = 0xFF;

    ISD_PowerDown();
}

void scanMarquee()
{
    byte i;
    for (i = 0; i < 8; i++)
    {
        byte pos = i + cursorPosition;
        if (pos >= LENGTH)
            pos -= LENGTH;
        switch (pos)
        {
        case 0:
            nextStr[i] = date.date & 0x0F;
            break;
        case 1:
            nextStr[i] = date.date >> 4;
            break;
        case 2:
            nextStr[i] = date.month & 0x0F | RDOT;
            break;
        case 3:
            nextStr[i] = date.month >> 4;
            break;
        case 4:
            nextStr[i] = date.year & 0x0F | RDOT;
            break;
        case 5:
            nextStr[i] = date.year >> 4;
            break;
        case 6:
            nextStr[i] = 0;
            break;
        case 7:
            nextStr[i] = 2;
            break;
        case 8:
            nextStr[i] = NONUM | RDOT | LDOT;
            break;
        case 9:
            nextStr[i] = date.second & 0x0F;
            break;
        case 10:
            nextStr[i] = date.second >> 4;
            break;
        case 11:
            nextStr[i] = date.minute & 0x0F;
            break;
        case 12:
            nextStr[i] = date.minute >> 4;
            break;
        case 13:
            nextStr[i] = date.hour & 0x0F;
            break;
        case 14:
            nextStr[i] = date.hour >> 4;
            break;
        case 15:
            nextStr[i] = NONUM | RDOT;
            break;
        case 16:
            nextStr[i] = date.day == 7 ? 0 : date.day;
            break;
        default:
            nextStr[i] = NONUM | RDOT | LDOT;
            break;
        }
    }
    if (twinkleCount >= LENGTH - 1)
    {
        twinkleCount = 0;
        if (cursorPosition == 0)
            cursorPosition = LENGTH - 1;
        else
            cursorPosition--;
    }
    twinkleCount++;
    scan(nextStr);
}

void marqueeMode_Button1Press()
{                   
    changeMode(&dateMode);
}

void marqueeMode_Button1Hold()
{
}

void marqueeMode_Button2Press()
{ 
    ISD_SetPlay(0);
    ISD_Play();
}

void marqueeMode_Button3Press()
{
    ISD_Stop();
}