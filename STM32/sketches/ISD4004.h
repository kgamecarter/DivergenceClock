#pragma once

#include <Arduino.h>

class ISD4004
{
public:
	ISD4004(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t ss)
		: mosi(mosi)
		, miso(miso)
		, sclk(sclk)
		, ss(ss)
	{ }
	
	void begin();
	void stop();
	void powerUp();
	void powerDown();
	void play();
	void setPlay(uint16_t address);
	void rec();
	void setRec(uint16_t address);
	bool checkIsdOverflow();
	
	~ISD4004();

private:
	uint8_t mosi, miso, sclk, ss;
	void spiSend(uint8_t data);
	void spiSend(uint16_t data);
	void sendCommand(uint8_t cmd);
};