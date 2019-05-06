#include "Display.h"
#include "SetDateMode.h"
#include "DateMode.h"
#include "BCDCompute.h"
           
void setDateMode_onEnter(); 
void setDateMode_onExit();
void scanSetDate();
void setDateMode_Button1Press();
void setDateMode_Button1Hold();
void setDateMode_Button2Press();
void setDateMode_Button3Press();

code ModeLink setDateMode =
{
    scanSetDate,
    setDateMode_onEnter,
    setDateMode_onExit,
    setDateMode_Button1Press,
    setDateMode_Button1Hold,
    setDateMode_Button2Press,
    setDateMode_Button3Press
};

void setDateMode_onEnter()
{
    cursorPosition = 0;
}

void setDateMode_onExit()
{
    DS1307_setBytes(DS1307_DATE, 3, &date.date); // 個別寫入避免動到時間
    cursorPosition = 0xFF;
}

void scanSetDate()
{
    nextStr[0] = date.date & 0x0F;
    nextStr[1] = date.date >> 4;
    nextStr[2] = date.month & 0x0F;
    nextStr[3] = date.month >> 4;
    nextStr[4] = date.year & 0x0F;
    nextStr[5] = date.year >> 4;
    nextStr[6] = 0;
    nextStr[7] = 2;
    if (20 <= twinkleCount && twinkleCount < 40) // 在目標上閃爍
    {
        nextStr[cursorPosition << 1] = NONUM;
        nextStr[(cursorPosition << 1) + 1] = NONUM;
    }
    else if (twinkleCount >= 40)
        twinkleCount = 0;
    twinkleCount++;
    nextStr[2] |= RDOT;
    nextStr[4] |= RDOT;

    scan(nextStr);
}

void setDateMode_Button1Press()
{
    if (++cursorPosition >= 3)
        cursorPosition = 0;
}

void setDateMode_Button1Hold() // 寫回時間
{                    
    changeMode(&dateMode);
}

void setDateMode_Button2Press()
{
    switch (cursorPosition)
    {
    case 0:
        date.date = BCDaddOne(date.date, 1, 0x31);
        break;
    case 1:
        date.month = BCDaddOne(date.month, 1, 0x12);
        break;
    case 2:
        date.year = BCDaddOne(date.year, 0, 0x99);
        break;
    }
}

void setDateMode_Button3Press()
{
    switch (cursorPosition)
    {
    case 0:
        date.date = BCDsubOne(date.date, 1, 0x31);
        break;
    case 1:
        date.month = BCDsubOne(date.month, 1, 0x12);
        break;
    case 2:
        date.year = BCDsubOne(date.year, 0, 0x99);
        break;
    }
}