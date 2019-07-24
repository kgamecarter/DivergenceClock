#pragma once

#include "Display.h"
#include "ISD4004.h"

class ModeManager;

#define MODES_COUNT 2

enum Modes : uint32_t
{
	TEST_MODE,
	STOPWATCH_MODE
};

class Mode
{
public:
	Mode(Modes m, ModeManager* manager, Display* display);
	
	virtual Modes getMode() = 0;
	virtual void scan();
	virtual void update() = 0;
	virtual void button1Press() = 0;
	virtual void button2Press() = 0;
	virtual void button3Press() = 0;
	virtual void onEnter() {}
	virtual void onExit() {}
	
protected:
	ModeManager* manager;
	Display* display;
	uint8_t buffer[8];
private:
	
};

class ModeManager
{
public:
	ModeManager();
	
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
