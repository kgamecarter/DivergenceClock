#include "Display.h"

Display::Display(const uint8_t* _selPins, const uint8_t* _lightPins)
	: defaultLightness(160)
	, selPins(_selPins)
	, lightPins(_lightPins)
{ }

Display::~Display()
{
}

void Display::begin()
{
	for (uint8_t i = 0; i < SEL_NUM; i++)
	{
		pinMode(selPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(selPins[i], LOW);
	}
	for (uint8_t i = 0; i < LIGHT_PIN_NUM; i++)
	{
		pinMode(lightPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(lightPins[i], LOW);
	}
}

void Display::scan(const uint8_t* str)
{
	uint8_t i;
	for (i = 0; i < SEL_NUM; i++)
	{
		if (str[i] == OVERCHAR)
			break;
		if (str[i] == NONUM)
		{
			delay(2);
			continue;
		}
		digitalWrite(this->selPins[i], HIGH);

		this->show(str[i]);
		delayMicroseconds(defaultLightness);

		this->show(NONUM);
		digitalWrite(this->selPins[i], LOW);
		delayMicroseconds(200 - defaultLightness);
	}
	this->show(NONUM);
	// delayº¡1.6ms
	if (i < SEL_NUM)
		delay((SEL_NUM - i) * 200);  
}

void Display::show(uint8_t v)
{
	for (uint8_t i = 0; i < LIGHT_PIN_NUM; i++)
	{
		digitalWrite(this->lightPins[i], v & 1);
		v >>= 1;
	}
}
