#include "TestMode.h"

TestMode::TestMode(ModeManager* manager, Display* display, ISD4004* isd, std::function<void(void)> btn1, std::function<void(void)> btn2)
	: Mode(manager, display)
	, isd(isd)
	, btn1(btn1)
	, btn2(btn2)
{
}

Modes TestMode::getMode()
{
	return Modes::TEST_MODE;
}

void TestMode::scan()
{
	this->display->scan(this->buffer);
}

void TestMode::update()
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
	
void TestMode::button1Press()
{
	this->btn1();
}
	
void TestMode::button2Press()
{
	this->btn2();
}
	
void TestMode::button3Press()
{
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
	digitalWrite(LED_BUILTIN, HIGH);
	isd->powerUp();
	delay(25);
	isd->setPlay(0);
	isd->play();
}
