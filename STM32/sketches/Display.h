#pragma once

#include <Arduino.h>

#define RDOT     0x10
#define LDOT     0x20
#define NONUM      10
#define OVERCHAR 0xFF

#define SEL_NUM 8
#define LIGHT_PIN_NUM 6

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
