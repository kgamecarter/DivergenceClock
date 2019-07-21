#include <IWatchdog.h>
// https://github.com/PaulStoffregen/OneWire
#include <OneWire.h>
// https://github.com/kgamecarter/DS1307
#include <DS1307.h>
// https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <DallasTemperature.h>
// https://github.com/kgamecarter/JK_Button
#include <JK_Button.h>
// https://github.com/kgamecarter/Arduino-Scheduler
#include <Scheduler.h>

#include "ISD4004.h"
#include "Display.h"
#include "Mode.h"

#define UART_BAUD 9600

//#define RX_PIN PA10
//#define TX_PIN PA9
#define BUTTON_1_PIN PA8
#define BUTTON_2_PIN PA11
#define BUTTON_3_PIN PA12
#define TEMP_PIN PA13
#define SCL_PIN PA14
#define SDA_PIN PA15

#define ISD_SCLK PB0
#define ISD_SS   PB1
#define ISD_MOSI PB2
#define ISD_MISO PB3
#define ISD_INT  PB4 
#define ISD_RAC  PB5

const uint8_t selPins[SEL_NUM] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7 };
const uint8_t lightPins[LIGHT_PIN_NUM] = { PB8, PB9, PB10, PB11, PB12, PB13 };

Button btn1([]() -> bool {
	return digitalRead(BUTTON_1_PIN);
});
Button btn2([]() -> bool {
	return digitalRead(BUTTON_2_PIN);
});
Button btn3([]() -> bool {
	return digitalRead(BUTTON_3_PIN);
});

DS1307 rtc(SDA_PIN, SCL_PIN);

ISD4004 isd(ISD_MOSI, ISD_MISO, ISD_SCLK, ISD_SS);

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensorAddress;

Display display(selPins, lightPins);

ModeManager manager;

void printTime();
void printTemp();

TestMode testMode(&manager, &display, &isd, printTime, printTemp);

void setup()
{
	display.begin();
	pinMode(BUTTON_1_PIN, INPUT_PULLUP);
	pinMode(BUTTON_2_PIN, INPUT_PULLUP);
	pinMode(BUTTON_3_PIN, INPUT_PULLUP);
	pinMode(LED_BUILTIN, OUTPUT_OPEN_DRAIN);
	digitalWrite(LED_BUILTIN, LOW);

	/* 如果是因為 Watchdog 而 reset 則閃燈 */
	if (IWatchdog.isReset(true))
	{
		for (uint8_t i = 0; i < 5; i++)
		{
			digitalWrite(LED_BUILTIN, LOW);
			delay(100);
			digitalWrite(LED_BUILTIN, HIGH);
			delay(100);
		}
	}
	// Init the watchdog timer with 2 seconds timeout
	IWatchdog.begin(2000000);
	if (!IWatchdog.isEnabled())
	{
		/* LED blinks indefinitely */
		while (1)
		{
			digitalWrite(LED_BUILTIN, LOW);
			delay(500);
			digitalWrite(LED_BUILTIN, HIGH);
			delay(500);
		}
	}
	
	pinMode(ISD_INT, INPUT);
	// TODO ISD_INT interrupt
	pinMode(ISD_RAC, INPUT);
	
	pinMode(PB2, OUTPUT_OPEN_DRAIN);
	digitalWrite(PB2, HIGH);
	isd.begin();
	isd.powerDown();
	
	sensors.begin();
	sensors.setWaitForConversion(false);
	sensors.getAddress(tempSensorAddress, 0);
	
	Serial.begin(UART_BAUD);
	Serial.println("Ready");
	printTime();
	printTemp();
	digitalWrite(LED_BUILTIN, HIGH);
	
	manager.setMode(&testMode);
	Scheduler.startLoop(scanButtonLoop);
	Scheduler.startLoop([]() -> void
	{
		manager.update();
		yield();
	});
}

void loop()
{
	IWatchdog.reload();
	manager.scan();
	yield();
}

void scanButtonLoop()
{
	btn1.read();
	btn2.read();
	btn3.read();
	if (btn1.wasPressed())
		manager.button1Press();
	if (btn2.wasPressed())
		manager.button2Press();
	if (btn3.wasPressed())
		manager.button3Press();
	delay(5);
}

void printTime()
{
	auto time = rtc.getTime();
	Serial.print(time.year);
	Serial.print("/");
	Serial.print(time.mon);
	Serial.print("/");
	Serial.print(time.date);
	Serial.print(" ");
	Serial.print(time.hour);
	Serial.print(":");
	Serial.print(time.min);
	Serial.print(":");
	Serial.println(time.sec);
}

void printTemp()
{
	// 取得上次的值
	float tempC = sensors.getTempC(tempSensorAddress);
	delay(5);
	Serial.print("Temp: ");
	if (tempC != DEVICE_DISCONNECTED_C) 
	{
		Serial.println(tempC);
	} 
	else
	{
		Serial.println("Error");
	}
	delay(5);
	// 非同步請求
	sensors.requestTemperatures();
}