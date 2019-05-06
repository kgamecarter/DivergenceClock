#include "Display.h"
#include "TemperatureMode.h"
#include "DS18B20.h"
#include "DateMode.h"
#include "ISD4004.h"
                     
void temperatureMode_onEnter();
void temperatureMode_onExit();
void scanTemperature();
void temperatureMode_Button1Press();
void temperatureMode_Button1Hold();
void temperatureMode_Button2Press();
void temperatureMode_Button3Press();
void temperatureMode_timerInterrupt();
code ModeLink temperatureMode =
{
    scanTemperature,
    temperatureMode_onEnter,
    temperatureMode_onExit,
    temperatureMode_Button1Press,
    temperatureMode_Button1Hold,
    temperatureMode_Button2Press,
    temperatureMode_Button3Press,
	temperatureMode_timerInterrupt
};

float temperature;

void temperatureMode_onEnter()
{
    twinkleCount = 33;

	ISD_PowerUp();
}

void temperatureMode_onExit()
{
    twinkleCount = 0;
	
	ISD_PowerDown();
}

void temperatureMode_timerInterrupt()
{
    if (twinkleCount >= 33)
    {
	    displayOff();
        twinkleCount = 0;
        temperature = ReadTemperature();
        DetectTemperature();
    }
    else
        twinkleCount++;
}

void scanTemperature()
{
    uint tmp;
    nextStr[7] = OVERCHAR;
    nextStr[6] = NONUM;
    if (temperature < 0.0f)
    {
        nextStr[6] = NONUM | LDOT | RDOT;
        temperature = -temperature;
    }
    tmp = temperature * 1000.0f;
    nextStr[0] = NONUM;

    nextStr[1] = tmp % 10;
    tmp /= 10;
    nextStr[2] = tmp % 10;
    tmp /= 10;
    nextStr[3] = tmp % 10;
    tmp = (int)temperature;
    nextStr[4] = tmp % 10 | RDOT;
    tmp /= 10;
    nextStr[5] = tmp % 10;
    tmp /= 10;
    if (tmp != 0)
        nextStr[6] = tmp;
    scan(nextStr);
}


void temperatureMode_Button1Press()
{				 
    changeMode(&dateMode);
}

void temperatureMode_Button1Hold()
{
}

void temperatureMode_Button2Press()
{
	ISD_PowerUp(); //ISD 上電
	delayms(25);
	ISD_PowerUp(); //ISD 上電
	ISD_SetRec(0);
	ISD_Rec();
}

void temperatureMode_Button3Press()
{
    ISD_Stop();
}