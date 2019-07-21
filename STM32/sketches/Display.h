#pragma once

#include <Arduino.h>

#define R_DOT     0x10
#define L_DOT     0x20
#define NO_NUM      10
#define OVER_CHAR 0xFF

#define SEL_PIN_COUNT 8
#define LIGHT_PIN_COUNT 6

class Display
{
public:
	Display(const uint8_t* _selPins, const uint8_t* _lightPins);
	~Display();
	
	void begin();
	void scan(const uint8_t* str);

private:
	const uint8_t* selPins;
	const uint8_t* lightPins;
	uint8_t defaultLightness;
	
	void show(uint8_t v);
};
