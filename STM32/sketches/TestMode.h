#pragma once

#include <functional>

#include "Mode.h"

class TestMode
	: public Mode
{
public:
	TestMode(ModeManager* manager, Display* display, ISD4004* isd, std::function<void(void)> btn1, std::function<void(void)> btn2);
	
	Modes getMode() override;
	void scan() override;
	void update() override;
	void button1Press() override;
	void button2Press() override;
	void button3Press() override;

private:
	ISD4004* isd;
	std::function<void(void)> btn1, btn2;
	uint8_t buffer[8];
};
