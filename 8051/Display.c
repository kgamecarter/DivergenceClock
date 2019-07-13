#include <at89c51xd2.h>
#include <INTRINS.H>
#include "Display.h"

void delayus(uint);

byte cursorPosition = 0xFF;
byte twinkleCount = 0;
byte preStr[8], nextStr[8] = { 0 }, preState = 0xFF;

byte defaultLightness, crossfade;

void watchDog()
{
    WDTRST = 0x1E;
    WDTRST = 0xE1;
}

bool cmpPreState(byte i)
{
    if (preState != i)
    {
        preState = i;
        for (i = 0; i < 8; i++)
            preStr[i] = nextStr[i];
        return true;
    }
    return false;
}

// 掃描顯示
void scan(byte* str)
{
    byte i;
    for (i = 0; i < 8; i++)
    {
        watchDog();
        if (str[i] == OVERCHAR)
            break;
        if (str[i] == NONUM)
        {
            delayms(2);
            continue;
        }
        P1 = 1 << i;

        P0 = str[i];
        delayus(defaultLightness);

        P0 = NONUM;
        P1 = 0;
        delayus(200 - defaultLightness);
    }
    P0 = NONUM;
    delayms((8 - i) << 1); // delay滿16ms
}

void scanCrossfade(byte fadeCount)
{
    byte i;
    fadeCount = 100 - fadeCount;
    if (fadeCount < crossfade) // 需要Fade
        fadeCount = defaultLightness * fadeCount / crossfade; //計算延遲
    else
        fadeCount = 0xFF;
    for (i = 0; i < 8; i++)
    { 
        watchDog();
        if (preStr[i] == OVERCHAR)
            break;
        if (preStr[i] == NONUM)
        {
            delayms(2);
            continue;
        }
        P1 = 1 << i;
        P0 = preStr[i];
        if (fadeCount == 0xFF || preStr[i] == nextStr[i])
        {
            delayus(defaultLightness);
    
            P0 = NONUM;
            P1 = 0;
            delayus(200 - defaultLightness);
        }
        else // cross fade
        { 
            delayus(fadeCount);
            P0 = nextStr[i];
            delayus(defaultLightness - fadeCount);
    
            P0 = NONUM;
            P1 = 0;
            delayus(200 - defaultLightness);
        }
    }
    P0 = NONUM;
    delayms((8 - i) << 1); // delay滿16ms
}

void scanLightness(byte* str, byte* lightness)
{
    byte i;
    for (i = 0; i < 8; i++)
    {
        watchDog();
        if (str[i] == OVERCHAR)
            break;
        if (str[i] == NONUM)
        {
            delayms(2);
            continue;
        }

        P1 = 1 << i;

        P0 = str[i];
        delayus(lightness[i]);

        P0 = NONUM;
        P1 = 0;
        delayus(200 - lightness[i]);
    }
    P0 = NONUM;
    delayms((8 - i) << 1); // delay滿16ms
}

void displayOff()
{
    P0 = NONUM;
    P1 = 0;
}

void delayms(byte time)
{
    while (time--)
    { 
        delayus(100);
        watchDog();
    }
}

void delayus(uint time)
{
    while (time)
        time--;
}