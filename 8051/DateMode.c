#include "Display.h"
#include "DateMode.h"
#include "SetDateMode.h"
#include "TimeMode.h"
#include "MarqueeMode.h"
#include "TemperatureMode.h"
    	       
void date_onEnter();     
void date_onExit();
void scanDate();
void date_Button1Press();
void date_Button1Hold();
void date_Button2Press();
void date_Button3Press();

code ModeLink dateMode =
{
    scanDate,
    date_onEnter,
    date_onExit,
    date_Button1Press,
    date_Button1Hold,
    date_Button2Press,
    date_Button3Press,
	timeUpdate
};

void date_onEnter()
{
    DS1307_getDateTime(&date);
	preState = 0xFF;
	tCount =  100 - crossfade;
}

void date_onExit()
{
}

void scanDate()
{
	if (cmpPreState(date.second))
	{
	    nextStr[0] = date.date & 0x0F;
	    nextStr[1] = date.date >> 4;
	    nextStr[2] = date.month & 0x0F | RDOT;
	    nextStr[3] = date.month >> 4;
	    nextStr[4] = date.year & 0x0F | RDOT;
	    nextStr[5] = date.year >> 4;
	    nextStr[6] = 0;
	    nextStr[7] = 2;
	}
    scanCrossfade(tCount * 100 / 112);
}

void date_Button1Press()
{				 
    changeMode(&timeMode);
}

void date_Button1Hold()
{					 
    changeMode(&setDateMode);
}

void date_Button2Press()
{					 
    changeMode(&marqueeMode);
}

void date_Button3Press()
{					   
    changeMode(&temperatureMode);
}