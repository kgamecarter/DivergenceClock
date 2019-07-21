#include "ISD4004.h"

#define ISD_DELAY 1
//========ISD4004指令定義========================
#define POWER_UP  (uint8_t)0x20 //上電指令
#define SET_PLAY  (uint8_t)0xE0 //指定放音指令
#define PLAY      (uint8_t)0xF0 //當前放音指令
#define SET_REC   (uint8_t)0xA0 //指定錄音指令
#define REC       (uint8_t)0xB0 //當前錄音指令
#define SET_MC    (uint8_t)0xE8 //指定快進指令
#define MC        (uint8_t)0xF8 //快進執行指令
#define STOP      (uint8_t)0x30 //停止當前操作
#define STOP_WRDN (uint8_t)0x10 //停止當前操作並掉電
#define RINT      (uint8_t)0x30 //讀狀態:OVF和EOM

void ISD4004::begin()
{
	pinMode(this->mosi, OUTPUT_OPEN_DRAIN);
	digitalWrite(this->mosi, HIGH);
	pinMode(this->miso, INPUT);
	pinMode(this->sclk, OUTPUT_OPEN_DRAIN);
	digitalWrite(this->sclk, LOW);
	pinMode(this->ss, OUTPUT_OPEN_DRAIN);
	digitalWrite(this->ss, HIGH);
}

void ISD4004::stop()
{ this->sendCommand(STOP); }

void ISD4004::powerUp()
{ this->sendCommand(POWER_UP); }

void ISD4004::powerDown()
{ this->sendCommand(STOP_WRDN); }

void ISD4004::play()
{ this->sendCommand(PLAY); }

void ISD4004::setPlay(uint16_t address)
{
	this->spiSend(address);
	this->sendCommand(SET_PLAY);
}

void ISD4004::rec()
{ this->sendCommand(REC); }

void ISD4004::setRec(uint16_t address)
{
	this->spiSend(address);
	this->sendCommand(SET_REC);
}

bool ISD4004::checkIsdOverflow()
{
	digitalWrite(this->ss, LOW);
	digitalWrite(this->sclk, LOW);
	digitalWrite(this->sclk, HIGH);
	digitalWrite(this->sclk, LOW);
    
	if (digitalRead(this->miso) == HIGH)
	{
		digitalWrite(this->sclk, LOW);
		digitalWrite(this->ss, HIGH); //關閉 spi 通信端
		this->stop(); //發送 stop 指令
		return true; //OVF 為 1，返回 true
	}
	else
	{
		digitalWrite(this->sclk, LOW);
		digitalWrite(this->ss, HIGH);   //關閉 spi 通信端
		this->stop(); //發送 stop 指令
		return false; //OVF 為 0，返回 false
	}
}

void ISD4004::spiSend(uint8_t data)
{
	digitalWrite(this->ss, LOW);
	digitalWrite(this->sclk, LOW);
	delayMicroseconds(ISD_DELAY); // Tsss
	//先發低位再發高位，依次發送。
	for (int i = 0; i < 8; i++)
	{
		//發送最低位
		digitalWrite(this->mosi, data & 1);
		delayMicroseconds(ISD_DELAY); // Tdis
		//右移一位
		data >>= 1;
    	//時鐘下降沿發送
		digitalWrite(this->sclk, HIGH);
		delayMicroseconds(ISD_DELAY); // TsckHigh
		digitalWrite(this->sclk, LOW);
		delayMicroseconds(ISD_DELAY); // TsckLow
	}
}

void ISD4004::spiSend(uint16_t data)
{
	register uint8_t* ptr = (uint8_t*)&data;
	this->spiSend(ptr[0]);
	delayMicroseconds(ISD_DELAY);
	this->spiSend(ptr[1]);
	delayMicroseconds(ISD_DELAY);
}

void ISD4004::sendCommand(uint8_t cmd)
{
	this->spiSend(cmd);
	delayMicroseconds(ISD_DELAY);  // Tssh
	digitalWrite(this->ss, HIGH);
	delayMicroseconds(ISD_DELAY);  // Tssmin
}

ISD4004::~ISD4004()
{
}
