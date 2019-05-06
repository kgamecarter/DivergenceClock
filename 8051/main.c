#include <at89c51xd2.h>
#include <INTRINS.H>
#include <stdlib.h>
#include "main.h"
#include "TimeMode.h"
#include "Display.h"
#include "DateMode.h"
#include "StopwatchMode.h"
#include "WorldLineMode.h"
#include "SetTimeMode.h"
#include "SetDateMode.h"
#include "TemperatureMode.h"
#include "MarqueeMode.h"
#include "UartMode.h"
#include "DS18B20.h"
#include "BCDCompute.h"
#include "ISD4004.h"

#define UART_SETDATETIME  0x01
#define UART_UART         0x02
#define UART_SETLIGHTNESS 0x03
#define UART_SETCROSSFADE 0x04
#define UART_SETALARM     0x05
#define UART_SETSLEEPTIME 0x06
#define UART_BUTTON1      0x11
#define UART_BUTTON2      0x12
#define UART_BUTTON3      0x13
#define UART_TIME         0x20
#define UART_DATE         0x21
#define UART_STOPWATCH    0x22
#define UART_WORLDLINE    0x23
#define UART_MARQUEE      0x24
#define UART_TEMPERATURE  0x25
#define UART_GETDATETIME  0x41
#define UART_GETLIGHTNESS 0x43
#define UART_GETCROSSFADE 0x44
#define UART_GETALARM     0x45
#define UART_GETSLEEPTIME 0x46
#define UART_READY        0xFF


#define EFFECTCOUNT 1

ModeLink* mode;    // 用函數指標模擬物件導向的多形
DateTime date;
code ModeLink* effectModes[EFFECTCOUNT] = { &worldLineMode };

typedef struct _time
{
    byte minute;								 
    byte hour;
} Time;

idata struct
{
    bool enable;
    Time time;
} alarm;

idata struct
{
    bool enable;
    Time start;
    Time end;
} sleepTime;

bit isSleep;
bit effectEnable;


void main()
{
    // 防止燈管爆亮, 先熄燈
    displayOff();

    // 設定溫度IC精度
    SetConfiguration(125, -55, 4);

    // 載入存在RTC的環境變數    
    DS1307_getBytes(DS1307_RAM(0), 1, &defaultLightness);
    if (!defaultLightness)
        defaultLightness = 60;
    DS1307_getBytes(DS1307_RAM(1), 1, &crossfade);
    DS1307_getBytes(DS1307_RAM(2), 3, (byte*)&alarm);
    DS1307_getBytes(DS1307_RAM(5), 5, (byte*)&sleepTime);

	// ISD4004
	ISD_PowerDown();
    // 初始化計時器0
    TMOD = 0x21;  //13bit mode
    // 初始化計時器1 for 串列埠, 9600
	TL1 = 0xFD;
	TH1 = 0xFD;
    SCON = 0x70;
    TR1 = 1;
    ES = 1;
    // 初始化計時器2 10ms中斷
    RCAP2H = 0xDC;
    RCAP2L = 0x5C;
    T2CON = 0x04;
	ET2 = 1;

    //開始主要迴圈

    mode = &timeMode;
    mode->onEnter();

    EA = 1;
    while (true)
    {
	    watchDog();
        if (isSleep)
		{
		    displayOff(); //確保燈管一定不亮
    	    continue;
		}
        mode->scanAction();
    }

}

void changeMode(ModeLink* newMode)
{
    mode->onExit();
    mode = newMode;
    mode->onEnter();
}

sbyte cmpTime(Time* a, Time* b)
{
    if (a->hour > b->hour)
	    return 1;
	if (a->hour < b->hour)
	    return -1;
	if (a->minute > b->minute)
	    return 1;
	if (a->minute < b->minute)
	    return -1;
	return 0;
}


byte tCount = 0;
idata byte b1Count = 0;
idata byte b2Count = 0;
idata byte b3Count = 0;
// 每秒會呼叫100次
void timer2_int() interrupt 5
{
    TF2 = 0; // 計時器2溢位旗標歸零
    // 偵測按鍵1
    if (P3_2 == ENABLE)
    {
        if (b1Count < 200)
            b1Count++;
        else if (b1Count == 200 && !isSleep) // 長按2秒
        {
            b1Count++;
            mode->button1Hold();
        }
    }
    else
    {
        if (b1Count && b1Count < 70)  // 短按
    	{
    	    if (isSleep)
    	        isSleep = false;
    	    else
                mode->button1Press();
    	}
        b1Count = 0;
    }
    // 偵測按鍵2
    if (P3_3 == ENABLE && !isSleep)
    {
        if (b2Count < 225)
            b2Count++;
    }
    else
    {
        if (b2Count && b2Count < 70)  // 短按
            mode->button2Press();
        b2Count = 0;
    }
    // 偵測按鍵3
    if (P3_4 == ENABLE && !isSleep)
    {
        if (b3Count < 200)
            b3Count++;
    }
    else
    {
        if (b3Count && b3Count < 70)  // 短按
            mode->button3Press();
        b3Count = 0;
    }

    // 更新時間
	if (mode->timerInterrupt != NULL)
	    mode->timerInterrupt();
}

void timeUpdate()
{
	if (tCount < 100)
	    tCount++;
	else
	{
	    tCount = 0;
	    date.second = BCDaddOne(date.second, 0x00, 0x59);
	}
	if (date.second == 0x59 && (tCount & 0x15) == 0) // 59 to 00
	{
	    byte newSec;
	    DS1307_getBytes(DS1307_SECOND, 1, &newSec);
	    if (newSec < date.second)
	    {
	        date.second = newSec;
	        DS1307_getBytes(DS1307_MINUTE, 6, &date.minute);
	    	tCount = 0;
	    	// 睡眠功能
	    	if (sleepTime.enable)
	    	{
	    	    if (cmpTime(&sleepTime.start ,&sleepTime.end) == -1)
	    	    {
	    	        if (cmpTime(&sleepTime.start, (Time*)&date.minute) == -1 &&
	    	    	    cmpTime((Time*)&date.minute, &sleepTime.end)   == -1)
	    	    	    isSleep = true;
					else
					    isSleep = false;
	    	    }
	    	    else
	    	    {    	    	    	    	    	    	    	  
	    	        if (cmpTime((Time*)&date.minute, &sleepTime.start) == -1 ||
	    	    	    cmpTime(&sleepTime.end, (Time*)&date.minute)   == -1)
	    	    	    isSleep = true;
					else
					    isSleep = false;
	    	    }
	    	}
			// 鬧鐘
			if (alarm.enable)
			{
			    if (cmpTime((Time*)&date.minute, &alarm.time) == 0)
				{
					ISD_PowerUp(); //ISD 上電
					delayms(25);
				    ISD_SetPlay(0);
	                ISD_Play();
					isSleep = false;
				}
			}

			if (/*effectEnable && */!isSleep && mode == &timeMode)
			{
			    mode->onExit();
				cursorPosition = 2;
				mode = effectModes[rand() % EFFECTCOUNT];
			}
	    }
	}
}

void memcpyA(byte* dest, byte* sour, byte count)
{
    while (count--)
	    *dest++ = *sour++;
}

idata byte uartInData[UART_IN_DATA_MAXSIZE], uartInType = 0xFF, uartInSize = 0xFF, uartInIndex = 0;
idata byte* uartOutData;
idata byte uartOutType = 0xFF, uartOutSize = 0xFF, uartOutOutdex = 0xFF;
void UART_int() interrupt 4
{
    ModeLink* newMode;
    if (RI)
    {
    	if (uartInType == 0xFF)
    	{
    	    if (SBUF == 0)
    	    {
    	        RI = 0;
    	    	uartOutType = 0;
    	    	uartOutSize = 0xFF;
    	    	uartOutOutdex = 0xFF;
				TI = 1;
    	    	goto TIAction;
    	    }
    	    uartInType = SBUF;
    	    RI = 0;
    	    return;
    	}
    	if (uartInSize == 0xFF)
    	{
    	    uartInSize = SBUF;
    	    RI = 0;
    	    if (uartInSize == 0)
    	        goto uartAction;
    	    return;
    	}
    	if (uartInIndex < uartInSize)
    	{
    	    uartInData[uartInIndex++] = SBUF;
            RI = 0;
    	    if (uartInIndex < uartInSize)
    	    	return;
    	}
    uartAction:
	    newMode = mode;
        switch (uartInType)
        {
        case UART_SETDATETIME:
            DS1307_setDateTime((DateTime*)uartInData); // 寫時間
			memcpyA((byte*)&date, uartInData, 7);
            break;
    	case UART_UART:
    	    newMode = &uartMode;
    	    break;
        case UART_SETLIGHTNESS:
            defaultLightness = uartInData[0];
            DS1307_setBytes(DS1307_RAM(0), 1, &defaultLightness);
            break;
        case UART_SETCROSSFADE:
            crossfade = uartInData[0];
            DS1307_setBytes(DS1307_RAM(1), 1, &crossfade);
            break;
        case UART_SETALARM:
            DS1307_setBytes(DS1307_RAM(2), 3, uartInData);
			memcpyA((byte*)&alarm, uartInData, 3);
            break;
        case UART_SETSLEEPTIME:
            DS1307_setBytes(DS1307_RAM(5), 5, uartInData);
			memcpyA((byte*)&sleepTime, uartInData, 5);
            break;
        case UART_BUTTON1:
            mode->button1Press();
            goto noMode;
        case UART_BUTTON2:
            mode->button2Press();
            goto noMode;
        case UART_BUTTON3:
            mode->button3Press();
            goto noMode;
        case UART_TIME:
    	    newMode = &timeMode;
            break;
        case UART_DATE:
    	    newMode = &dateMode;
            break;
        case UART_STOPWATCH:   
    	    newMode = &stopwatchMode;
            break;
        case UART_WORLDLINE:     
    	    newMode = &worldLineMode;
            break;
        case UART_MARQUEE:    	
    	    newMode = &marqueeMode;
            break;
        case UART_TEMPERATURE:    
    	    newMode = &temperatureMode;
            break;
    	case UART_GETLIGHTNESS:
    	    uartOutType = UART_GETLIGHTNESS;
    	    uartOutSize = 1;
    	    uartOutData = &defaultLightness;
    	    TI = 1;
    	    break;
    	case UART_GETCROSSFADE:
    	    uartOutType = UART_GETCROSSFADE;
    	    uartOutSize = 1;
    	    uartOutData = &crossfade;
    	    TI = 1;
    	    break;			
    	case UART_GETALARM:
    	    uartOutType = UART_GETALARM;
    	    uartOutSize = 3;
    	    uartOutData = (byte*)&alarm;
    	    TI = 1;
    	    break;
    	case UART_GETSLEEPTIME:
    	    uartOutType = UART_GETSLEEPTIME;
    	    uartOutSize = 5;
    	    uartOutData = (byte*)&sleepTime;
    	    TI = 1;
    	    break;
        }
    	if (newMode != mode)
		    changeMode(newMode);
	noMode:
        uartInType = 0xFF;
    	uartInSize = 0xFF;
    	uartInIndex = 0;
    }
TIAction:
    if (TI)
    {
        TI = 0;
    	if (uartOutType != 0xFF)
    	{
    	    SBUF = uartOutType;
    	    uartOutType = 0xFF;
    	    return;
    	}
    	if (uartOutSize != 0xFF && uartOutOutdex == 0xFF)
    	{
    	    SBUF = uartOutSize;
    	    if (uartOutSize == 0)
    	        goto uartEnd;
    	    uartOutOutdex = 0;
    	    return;
    	}
    	if (uartOutOutdex < uartOutSize)
    	{
    	    SBUF = uartOutData[uartOutOutdex++];
    	    if (uartOutOutdex < uartOutSize)
    	    	return;
    	}
        uartOutOutdex = 0xFF;
    uartEnd:
    	uartOutSize = 0xFF;
    }
}