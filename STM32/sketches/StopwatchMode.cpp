#include "StopwatchMode.h"

#include <Arduino.h>

#define SW_DELAY 33

StopwatchMode::StopwatchMode(ModeManager* manager, Display* display)
	: Mode(this->getMode(), manager, display)
{
	this->clear();
}

Modes StopwatchMode::getMode()
{
	return Modes::STOPWATCH_MODE;
}

void StopwatchMode::update()
{
	if (this->isStart)
	{
		uint32_t t = millis() - start;
		t /= 10;
		buffer[0] = t % 10;
		t /= 10;
		buffer[1] = t % 10;
		t /= 10;
		uint32_t s = t % 60;
		buffer[2] = (s % 10) | R_DOT;
		buffer[3] = s / 10;
		t /= 60;
		uint32_t m = t % 60;
		buffer[4] = (m % 10) | R_DOT;
		buffer[5] = m / 10;
		t /= 60;
		uint32_t h = t % 60;
		buffer[6] = (h % 10) | R_DOT;
		buffer[7] = h / 10;
	}
	delay(SW_DELAY);
}

void StopwatchMode::button1Press()
{
	if (this->isStart)
	{
		this->saved = millis() - this->start;
		this->isStart = false;
	}
	else
	{
		this->start = millis() - this->saved;
		this->isStart = true;
	}
}

void StopwatchMode::button2Press()
{
	if (!this->isStart)
		clear();
}

void StopwatchMode::button3Press()
{
}

void StopwatchMode::clear()
{
	this->start = 0;
	this->saved = 0;
	this->isStart = false;
	for (int i = 0; i < 8; i++)
	{
		buffer[i] = 0;
	}
	buffer[2] |= R_DOT;
	buffer[4] |= R_DOT;
	buffer[6] |= R_DOT;
}