#pragma once

#include <stddef.h>
#include <functional>
#include <Arduino.h>

#include "Display.h"
#include "ISD4004.h"

class ModeManager;

#define MODES_COUNT 1

enum Modes : uint32_t
{
	TEST_MODE
};

class Mode
{
public:
	Mode(ModeManager* manager, Display* display);
	
	virtual Modes getMode();
	virtual void scan() = 0;
	virtual void update() = 0;
	virtual void button1Press() = 0;
	virtual void button2Press() = 0;
	virtual void button3Press() = 0;
	virtual void onEnter() {}
	virtual void onExit() {}
	
protected:
	ModeManager* manager;
	Display* display;
private:
	
};

class ModeManager
{
public:
	ModeManager()
		: mode(NULL)
		, nextMode(NULL)
	{}
	
	void scan();
	void update();
	void button1Press();
	void button2Press();
	void button3Press();
	void addMode(Modes m, Mode* mode);
	void setMode(Modes m);
	
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
	{}
	
	virtual Modes getMode()
	{ return Modes::TEST_MODE; }
	
	virtual void scan()
	{ this->display->scan(this->buffer); }
	
	virtual void update()
	{
		uint32_t t = millis();
		t /= 100;
		uint32_t v = t % 10;
		for (uint32_t i = 0; i < 8; i++)
			this->buffer[i] = (v + i) % 10;
		
		uint32_t p = t % 16;
		this->buffer[p / 2] |= p & 1 ? L_DOT : R_DOT;
		
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
