#include <Arduino.h>

#include "Display.h"

Display::Display(const uint8_t* _selPins, const uint8_t* _lightPins)
	: brightness(160)
	, selPins(_selPins)
	, lightPins(_lightPins)
{ }

Display::~Display()
{
}

void Display::begin()
{
	for (int i = 0; i < SEL_PIN_COUNT; i++)
	{
		pinMode(selPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(selPins[i], LOW);
	}
	for (int i = 0; i < LIGHT_PIN_COUNT; i++)
	{
		pinMode(lightPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(lightPins[i], LOW);
	}
}

uint16_t Display::getBrightness()
{
	return this->brightness;
}

void Display::setBrightness(uint16_t value)
{
	if (value > MAX_BRIGHTNESS)
	{
		Serial.print("Invalid brightness ");
		Serial.println(value);
		return;
	}
	this->brightness = value;
}

void Display::scan(const uint8_t* str)
{
	for (int i = 0; i < SEL_PIN_COUNT; i++)
	{
		if (str[i] == OVER_CHAR)
		{
			/* delay until 1.6ms */
			delayMicroseconds((SEL_PIN_COUNT - i) * MAX_BRIGHTNESS);  
			break;
		}
		if (str[i] == NO_NUM)
		{
			delayMicroseconds(MAX_BRIGHTNESS);
			continue;
		}
		digitalWrite(this->selPins[i], HIGH);

		this->show(str[i]);
		delayMicroseconds(brightness);

		this->show(NO_NUM);
		digitalWrite(this->selPins[i], LOW);
		delayMicroseconds(MAX_BRIGHTNESS - brightness);
	}
}

void Display::show(uint8_t v)
{
	for (int i = 0; i < LIGHT_PIN_COUNT; i++)
	{
		digitalWrite(this->lightPins[i], v & 1);
		v >>= 1;
	}
}
