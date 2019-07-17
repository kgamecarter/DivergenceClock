// https://github.com/kgamecarter/DS1307
#include <DS1307.h>
// https://github.com/kgamecarter/JK_Button
#include <JK_Button.h>
// https://github.com/MajenkoLibraries/SoftSPI
#include <SoftSPI.h>

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

#define SEL_NUM 8
const uint8_t selPins[SEL_NUM] = { PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7 };

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

SoftSPI mySPI(ISD_MOSI, ISD_MISO, ISD_SCLK);

void setup()
{
	for (uint8_t i = 0; i < SEL_NUM; i++)
	{
		pinMode(selPins[i], OUTPUT_OPEN_DRAIN);
		digitalWrite(selPins[i], LOW);
	}
	pinMode(BUTTON_1_PIN, INPUT_PULLUP);
	pinMode(BUTTON_2_PIN, INPUT_PULLUP);
	pinMode(BUTTON_3_PIN, INPUT_PULLUP);
	pinMode(LED_BUILTIN, OUTPUT_OPEN_DRAIN);
	
	mySPI.begin();
	
	Serial.begin(UART_BAUD);
	Serial.println("Ready");
	printTime();
}

void loop()
{
	scanButton();
}

void scanButton()
{
	btn1.read();
	btn2.read();
	btn3.read();
	if (btn1.wasPressed())
	{
		printTime();
	}
	if (btn2.wasPressed())
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);
	}
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
