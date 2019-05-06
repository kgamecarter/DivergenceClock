#include "Display.h"
#include "WorldLineMode.h"
#include "TimeMode.h"
#include "stdlib.h"
                    
void worldLine_onEnter();
void worldLine_onExit();
void scanWorldLine();
void worldLine_Button1Press();
void worldLine_Button1Hold();
void worldLine_Button2Press();
void worldLine_Button3Press();

code ModeLink worldLineMode =
{
    scanWorldLine,
    worldLine_onEnter,
    worldLine_onExit,
    worldLine_Button1Press,
    worldLine_Button1Hold,
    worldLine_Button2Press,
    worldLine_Button3Press
};

code byte lWordLine[] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 };
code ulong tvWorldLine[] =
{
    0x10130426,
    0x00571024,
    0x00571015,
    0x00523299,
    0x00456903,
    0x00409420,
    0x00337187,
    0x00409431,
    0x00456914,
    0x00523307,
    0x00571046,
    0x10130205,
    0xA0275349,
    0x10048596
};
#define TVMAX 14
byte swapTable[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
byte worldLine[8] = { 0, 0, 0, 0, 0, 0, NONUM | RDOT, 0 };
byte tableIndex = 0;
byte rndFlag = 0x40;
byte rndCount = 0;

void worldLine_onEnter()
{
    cursorPosition = 0xFF;
}

void worldLine_onExit()
{
    memcpyA(nextStr, worldLine, 8);
    cursorPosition = 0xFF;
    initialWorldLine();
}

void initialWorldLine()
{
    byte i, tmp, rnd;
    if (tableIndex <= 6) // 馬上更新會發生不明錯誤
        return;
    worldLine[6] = NONUM | RDOT;
    if (twinkleCount == TVMAX-1) //TV last
    {
        for (i = 0; i < 8; i++)
            swapTable[i] = 7 - i;
    }
    else // 一般
    {
        for (i = 0; i < 8; i++)
            swapTable[i] = i;
        for (i = 0; i < 8; i++)
        {
            rnd = rand() % 8;
            tmp = swapTable[i];
            swapTable[i] = swapTable[rnd];
            swapTable[rnd] = tmp;
        }
    }
    rndCount = 0;
    tableIndex = 0;
    rndFlag = 0x40;
}

void scanWorldLine()
{
    byte i;
    if (rndFlag != 0xFF)
    {
        if (rndCount < 17)
            rndCount++;
        else
        {
            rndCount = 0;
            i = swapTable[tableIndex++];
            if (!((rndFlag >> i) & 1))
            {
                if (cursorPosition == 1)
                    worldLine[i] = tvWorldLine[twinkleCount] >> (i << 2) & 0xF; //TV
                else if (i == 7)
                    worldLine[7] = lWordLine[rand() % 10];
                rndFlag |= 1 << i;
            }
        }
        for (i = 0; i < 8; i++)
        {
            if ((rndFlag >> i) & 1)
                continue;
            worldLine[i] = rand() % 10;
        }
    }
    else if (cursorPosition != 0xFF) // auto
    {
        if (rndCount < 200)
            rndCount++;
        else
        {
            rndCount = 0;
            twinkleCount++;
            if (twinkleCount >= TVMAX)
                twinkleCount = 0;
            initialWorldLine();
            if (cursorPosition == 2)
            {
                worldLine_onExit();
                mode = &timeMode;
                mode->onEnter();
            }
        }
    }
    scan(worldLine);
}

void worldLine_Button1Press()
{
    cursorPosition = 0xFF;
    initialWorldLine();
}

void worldLine_Button1Hold()
{                      
    changeMode(&timeMode);
}

void worldLine_Button2Press()
{
    initialWorldLine();
    cursorPosition = 0; // Auto run
}

void worldLine_Button3Press()
{
    initialWorldLine();
    cursorPosition = 1; // Auto run
    twinkleCount = 0;
}