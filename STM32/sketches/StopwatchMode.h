#pragma once

#include "Mode.h"

class StopwatchMode
	: public Mode
{
public:
	StopwatchMode(ModeManager* manager, Display* display);

	Modes getMode() override;
	void update() override;
	void button1Press() override;
	void button2Press() override;
	void button3Press() override;
	
	void clear();

private:
	bool isStart;
	uint32_t start, saved;
};
