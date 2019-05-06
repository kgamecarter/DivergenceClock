#include "Display.h"
#include "SetTimeMode.h"
#include "TimeMode.h"
#include "BCDCompute.h"
            
void setTimeMode_onEnter();
void setTimeMode_onExit();
void scanSetTime();
void setTime_Button1Press();
void setTime_Button1Hold();
void setTime_Button2Press();
void setTime_Button3Press();

code ModeLink setTimeMode =
{
    scanSetTime,
    setTimeMode_onEnter,
    setTimeMode_onExit,
    setTime_Button1Press,
    setTime_Button1Hold,
    setTime_Button2Press,
    setTime_Button3Press
};

void setTimeMode_onEnter()
{
    byte v = 0x80;
    DS1307_setBytes(DS1307_SECOND, 1, &v); // 停止時間
    cursorPosition = 0;
}

void setTimeMode_onExit()
{
    DS1307_setDateTime(&date); // 寫回時間
    cursorPosition = 0xFF;
}

void scanSetTime()
{
    nextStr[0] = date.second & 0x0F;
    nextStr[1] = date.second >> 4;
    nextStr[2] = date.minute & 0x0F;
    nextStr[3] = date.minute >> 4;
    nextStr[4] = date.hour & 0x0F;
    nextStr[5] = date.hour >> 4;
    nextStr[7] = date.day == 7 ? 0 : date.day;
    if (20 <= twinkleCount && twinkleCount < 40) // 在目標上閃爍
    {
        nextStr[cursorPosition << 1] = NONUM;
        nextStr[(cursorPosition << 1) + 1] = NONUM;
    }
    else if (twinkleCount >= 40)
        twinkleCount = 0;
    twinkleCount++;
    nextStr[6] = NONUM | RDOT;
    scan(nextStr);
}

void setTime_Button1Press()
{
    if (++cursorPosition >= 4)
        cursorPosition = 0;
}

void setTime_Button1Hold()
{
    changeMode(&timeMode);
}


void setTime_Button2Press()    // +1
{
    switch (cursorPosition)
    {
    case 0:
        date.second = BCDaddOne(date.second, 0, 0x59);
        break;
    case 1:
        date.minute = BCDaddOne(date.minute, 0, 0x59);
        break;
    case 2:
        date.hour = BCDaddOne(date.hour, 0, 0x23);
        break;
    case 3:
        date.day = BCDaddOne(date.day, 0, 0x6);
        break;
    }
}

void setTime_Button3Press()    // -1
{
    switch (cursorPosition)
    {
    case 0:
        date.second = BCDsubOne(date.second, 0, 0x59);
        break;
    case 1:
        date.minute = BCDsubOne(date.minute, 0, 0x59);
        break;
    case 2:
        date.hour = BCDsubOne(date.hour, 0, 0x23);
        break;
    case 3:
        date.day = BCDsubOne(date.day, 0, 0x6);
        break;
    }
}