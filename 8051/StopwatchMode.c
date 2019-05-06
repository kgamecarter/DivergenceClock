#include "Display.h"
#include "StopwatchMode.h"
#include "TimeMode.h"
                   
void stopwatch_onEnter();
void stopwatch_onExit();
void scanStopwatch();
void stopwatch_Button1Press();
void stopwatch_Button1Hold();
void stopwatch_Button2Press();
void stopwatch_Button3Press();

code ModeLink stopwatchMode =
{
    scanStopwatch,
    stopwatch_onEnter,
    stopwatch_onExit,
    stopwatch_Button1Press,
    stopwatch_Button1Hold,
    stopwatch_Button2Press,
    stopwatch_Button3Press,
    stopwatchUpdate
};

// ssw = 0 stop, 1 run, 2 stop&lock
byte ssw = 0, sss = 0, smm = 0, shh = 0, sCount = 0;

void stopwatchUpdate()
{
    if (!ssw)
        return;
    if (sCount < 100)
        sCount++;
    else
    {
        byte tmp = sss + 1;
        sCount = 0;
        if (tmp < 60)
            sss++;
        else
        {
            sss = 0;
            tmp = smm + 1;
            if (tmp < 60)
                smm++;
            else
            {
                smm = 0;
                tmp = shh + 1;
                if (shh <= 90)
                    shh++;
                else
                    shh = 0;
            }
        }
    }
}

void stopwatch_onEnter()
{
    nextStr[0] = sCount % 10;
    nextStr[1] = sCount / 10;
    nextStr[2] = sss % 10 | RDOT;
    nextStr[3] = sss / 10;
    nextStr[4] = smm % 10 | RDOT;
    nextStr[5] = smm / 10;
    nextStr[6] = shh % 10 | RDOT;
    nextStr[7] = shh / 10;
}

void stopwatch_onExit()
{
}

void scanStopwatch()
{
    if (ssw != 2)
        stopwatch_onEnter();
    scan(nextStr);
}

void stopwatch_Button1Press()
{                  
    changeMode(&timeMode);
}

void stopwatch_Button1Hold()
{
}

void stopwatch_Button2Press()
{
    if (ssw == 0x02)
        return;
    ssw ^= 1;
}

void stopwatch_Button3Press()
{
    if (ssw == 1)
    {
        ssw = 2;
        return;
    }
    if (ssw == 2)
    {
        ssw = 1;
        return;
    }
    sss = 0;
    smm = 0;
    shh = 0;
    sCount = 0;
}