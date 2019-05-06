#include "Display.h"
#include "UartMode.h"
#include "TimeMode.h"
   
void uartMode_onEnter();	
void uartMode_onExit();
void scanUart();
void uartMode_Button1Press();
void uartMode_Button1Hold();
void uartMode_Button2Press();
void uartMode_Button3Press();

code ModeLink uartMode =
{
    scanUart,
    uartMode_onEnter,
	uartMode_onExit,
	uartMode_Button1Press,
	uartMode_Button1Hold,
	uartMode_Button2Press,
	uartMode_Button3Press
};

void uartMode_onEnter()
{
}						 

void uartMode_onExit()
{
}

void scanUart()
{
	scanLightness(uartInData, uartInData + 8);
}

void uartMode_Button1Press()
{			  
    changeMode(&timeMode);
}

void uartMode_Button1Hold()
{
}

void uartMode_Button2Press()
{
}

void uartMode_Button3Press()
{
}