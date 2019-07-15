// https://github.com/kgamecarter/DS1307
#include <DS1307.h>

//#define RX_PIN PA10
//#define TX_PIN PA9
#define BUTTON_1_PIN PA8
#define BUTTON_2_PIN PA11
#define BUTTON_3_PIN PA12
#define TEMP_PIN PA13
#define SCL_PIN PA14
#define SDA_PIN PA15

#define SEL_NUM 8
const uint8_t selPins[SEL_NUM] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7 };


DS1307 rtc(SDA_PIN, SCL_PIN);

void setup()
{
	for (uint8_t i = 0; i < SEL_NUM; i++)
	{
		pinMode(selPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(selPins[i], LOW);
	}
	pinMode(LED_BUILTIN, OUTPUT);
	
	Serial.begin(9600);
	Serial.println("Ready");
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

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}
