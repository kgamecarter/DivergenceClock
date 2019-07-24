#pragma once

#include <DallasTemperature.h>

#include "Mode.h"

class TemperatureMode
	: public Mode
{
public:
	TemperatureMode(ModeManager* manager, Display* display, DallasTemperature* sensors);

	Modes getMode() override;
	void update() override;
	void button1Press() override;
	void button2Press() override;
	void button3Press() override;
	
	void setAddress(uint8_t* address);

private:
	DallasTemperature* sensors;
	uint8_t* address;
};

TemperatureMode::TemperatureMode(ModeManager* manager, Display* display, DallasTemperature* sensors)
	: Mode(this->getMode(), manager, display)
	, sensors(sensors)
{
}

Modes TemperatureMode::getMode()
{
	return Modes::TEMPERATURE_MODE;
}

void TemperatureMode::update()
{
	float tempC = this->sensors->getTempC(this->address);
	delay(250);
	
    buffer[7] = OVER_CHAR;
	buffer[6] = NO_NUM;
	if (tempC < 0.0f)
    {
	    buffer[6] = NO_NUM | L_DOT | R_DOT;
	    tempC = -tempC;
    }
	uint32_t tmp = tempC * 1000.0f;
	buffer[0] = NO_NUM;

	buffer[1] = tmp % 10;
    tmp /= 10;
	buffer[2] = tmp % 10;
    tmp /= 10;
	buffer[3] = tmp % 10;
	tmp = (uint32_t)tempC;
	buffer[4] = tmp % 10 | R_DOT;
    tmp /= 10;
	buffer[5] = tmp % 10;
    tmp /= 10;
    if (tmp != 0)
		buffer[6] = tmp;
	
	delay(250);
	this->sensors->requestTemperatures();
	delay(500);
}

void TemperatureMode::button1Press()
{
}

void TemperatureMode::button2Press()
{
}

void TemperatureMode::button3Press()
{
}

void TemperatureMode::setAddress(uint8_t* address)
{
	this->address = address;
}