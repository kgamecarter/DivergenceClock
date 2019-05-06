#include "Display.h"
#include "TimeMode.h"
#include "StopwatchMode.h"
#include "WorldLineMode.h"
#include "SetTimeMode.h"
#include "DateMode.h"
#include "BCDCompute.h"

void time_onEnter();
void time_onExit();
void scanTime();
void time_Button1Press();
void time_Button1Hold();
void time_Button2Press();
void time_Button3Press();

code ModeLink timeMode =
{
    scanTime,
    time_onEnter,
    time_onExit,
    time_Button1Press,
    time_Button1Hold,
    time_Button2Press,
    time_Button3Press,
    timeUpdate
};

void time_onEnter()
{
    DS1307_getDateTime(&date);
    preState = 0xFF;
    tCount = 100 - crossfade;
}

void time_onExit()
{
}

void scanTime()
{
    if (cmpPreState(date.second))
    {
        nextStr[0] = date.second & 0x0F;
        nextStr[1] = date.second >> 4;
        nextStr[2] = date.minute & 0x0F;
        nextStr[3] = date.minute >> 4;
        nextStr[4] = date.hour & 0x0F;
        nextStr[5] = date.hour >> 4;
        nextStr[6] = NONUM | RDOT;
        nextStr[7] = date.day == 7 ? 0 : date.day;
    }
    scanCrossfade(tCount * 100 / 112);
}

void time_Button1Press()
{
    changeMode(&dateMode);
}

void time_Button1Hold()
{  
    changeMode(&setTimeMode);
}

void time_Button2Press()
{             
    changeMode(&stopwatchMode);
}

void time_Button3Press()
{                            
    initialWorldLine();
    changeMode(&worldLineMode);
}