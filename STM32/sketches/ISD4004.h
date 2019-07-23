#pragma once

#include <Arduino.h>
//========ISD4004指令定義========================
#define ISD_POWER_UP  (uint8_t)0x20 //上電指令
#define ISD_SET_PLAY  (uint8_t)0xE0 //指定放音指令
#define ISD_PLAY      (uint8_t)0xF0 //當前放音指令
#define ISD_SET_REC   (uint8_t)0xA0 //指定錄音指令
#define ISD_REC       (uint8_t)0xB0 //當前錄音指令
#define ISD_SET_MC    (uint8_t)0xE8 //指定快進指令
#define ISD_MC        (uint8_t)0xF8 //快進執行指令
#define ISD_STOP      (uint8_t)0x30 //停止當前操作
#define ISD_STOP_WRDN (uint8_t)0x10 //停止當前操作並掉電
#define ISD_RINT      (uint8_t)0x30 //讀狀態:OVF和EOM

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
	void sendCommand(uint8_t cmd);

private:
	uint8_t mosi, miso, sclk, ss;
	void spiSend(uint8_t data);
	void spiSend(uint16_t data);
};