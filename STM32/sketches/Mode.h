#pragma once

#include <stddef.h>
#include <functional>
#include <Arduino.h>

#include "Display.h"
#include "ISD4004.h"

class ModeManager;

#define MODES_COUNT 1

enum Modes : uint8_t
{
	TEST_MODE
};

class Mode
{
public:
	Mode(ModeManager* manager, Display* display)
		: manager(manager)
		, display(display)
	{ }
	
	virtual void scan() = 0;
	virtual void update() = 0;
	virtual void button1Press() = 0;
	virtual void button2Press() = 0;
	virtual void button3Press() = 0;

protected:
	ModeManager* manager;
	Display* display;
private:
	
};

class ModeManager
{
public:
	ModeManager()
		: mode(0)
		, nextMode(0)
	{ }
	
	void scan()
	{
		if (this->nextMode != NULL)
		{
			// TODO change mode
			this->mode = this->nextMode;
			this->nextMode = NULL;
		}
		if (this->mode != NULL)
			this->mode->scan();
	}
	
	void update()
	{
		if (this->mode != NULL)
			this->mode->update();
	}
	
	void button1Press()
	{
		if (this->mode != NULL)
			this->mode->button1Press();
	}
	
	void button2Press()
	{
		if (this->mode != NULL)
			this->mode->button2Press();
	}
	
	void button3Press()
	{
		if (this->mode != NULL)
			this->mode->button3Press();
	}
	
	void addMode(Modes m, Mode* mode)
	{
		if (m < 0 || MODES_COUNT <= m)
		{
			Serial.print("Invalid mode");
			Serial.println((uint8_t)m);
			return;
		}
		this->modes[m] = mode;
	}
	
	void setMode(Modes m)
	{
		if (m < 0 || MODES_COUNT <= m)
		{
			Serial.print("Invalid mode");
			Serial.println((uint8_t)m);
			return;
		}
		this->nextMode = modes[m];
	}
	
private:
	Mode* modes[MODES_COUNT];
	Mode* mode;
	Mode* nextMode;
};

class TestMode
	: public Mode
{
public:
	TestMode(ModeManager* manager, Display* display, ISD4004* isd, std::function<void(void)> btn1, std::function<void(void)> btn2)
		: Mode(manager, display)
		, isd(isd)
		, btn1(btn1)
		, btn2(btn2)
	{ }
	
	virtual void scan()
	{
		this->display->scan(buffer);
	}
	
	virtual void update()
	{
		auto t = millis();
		t /= 100;
		uint8_t v = t % 10;
		for (uint8_t i = 0; i < 8; i++)
			buffer[i] = v;
		
		uint8_t p = t % 16;
		buffer[p / 2] |= p & 1 ? L_DOT : R_DOT;
		
		delay(100);
	}
	
	virtual void button1Press()
	{
		this->btn1();
	}
	
	virtual void button2Press()
	{
		this->btn2();
	}
	
	virtual void button3Press()
	{
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
		digitalWrite(LED_BUILTIN, HIGH);
		isd->powerUp();
		delay(25);
		isd->setPlay(0);
		isd->play();
	}

private:
	ISD4004* isd;
	std::function<void(void)> btn1, btn2;
	uint8_t buffer[8];
};
