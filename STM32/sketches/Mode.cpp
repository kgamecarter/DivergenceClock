#include "Mode.h"

#include <Arduino.h>

Mode::Mode(Modes m, ModeManager* manager, Display* display)
	: manager(manager)
	, display(display)
{
	manager->addMode(m, this);
}

void Mode::scan()
{
	this->display->scan(this->buffer);
}

ModeManager::ModeManager()
		: mode(NULL)
		, nextMode(NULL)
		, modes { NULL }
{
}

void ModeManager::scan()
{
	if (this->nextMode != NULL)
	{
		if (this->mode != NULL)
			this->mode->onExit();
		this->mode = this->nextMode;
		this->nextMode = NULL;
		this->mode->onEnter();
		return;
	}
	if (this->mode != NULL)
		this->mode->scan();
}

void ModeManager::update()
{
	if (this->mode != NULL)
		this->mode->update();
}

void ModeManager::button1Press()
{
	if (this->mode != NULL)
		this->mode->button1Press();
}

void ModeManager::button2Press()
{
	if (this->mode != NULL)
		this->mode->button2Press();
}

void ModeManager::button3Press()
{
	if (this->mode != NULL)
		this->mode->button3Press();
}

void ModeManager::addMode(Modes m, Mode* mode)
{
	if (m >= MODES_COUNT)
	{
		Serial.print("Invalid mode ");
		Serial.println((uint32_t)m);
		return;
	}
	this->modes[m] = mode;
}

void ModeManager::setMode(Modes m)
{
	if (m >= MODES_COUNT || modes[m] == NULL)
	{
		Serial.print("Invalid mode ");
		Serial.println((uint32_t)m);
		return;
	}
	this->nextMode = modes[m];
}
