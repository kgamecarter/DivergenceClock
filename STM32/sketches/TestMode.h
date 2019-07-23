#pragma once

#include <functional>

#include "Mode.h"

class TestMode
	: public Mode
{
public:
	TestMode(ModeManager* manager, Display* display, ISD4004* isd, std::function<void(void)> btn1, std::function<void(void)> btn2);
	
	virtual Modes getMode();
	virtual void scan();
	virtual void update();
	virtual void button1Press();
	virtual void button2Press();
	virtual void button3Press();

private:
	ISD4004* isd;
	std::function<void(void)> btn1, btn2;
	uint8_t buffer[8];
};
