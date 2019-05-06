#include "DS18B20.h"
#include <at89c51xd2.h>

sbit DQ = P3^5; //數據傳輸線引腳 
//----------------------------------------------------------------------- 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
//----------------------------------------------------------------------- 
//-----------------------內部函數---------------------------------------- 
void delay_temp(uint n); 
void Init_DS18B20(); 
byte ReadOneByte(); 
void WriteOneByte(byte dat);
//----------------------------------------------------------------------- 
//------------延時子程序------------- 
void delay_temp(uint n) 
{ 
    while(n--);  
}
 
//------初始化程序-------------------- 
void Init_DS18B20() 
{
    byte x = 0; 
    DQ = 1; // DQ先置高 
    delay_temp(8); //稍延時 
    DQ = 0; //發送復位脈衝 
    delay_temp(85); //延時（>480ms) 
    DQ = 1; //拉高數據線 
    delay_temp(14); //等待（15~60ms) 
    x = DQ; //用X的值來判斷初始化有沒有成功，18B20存在的話X=0，否則X=1 
    delay_temp(20); 
}
 
//------讀一個字節---------------------------------------------------------------- 
byte ReadOneByte() //主機數據線先從高拉至低電平1ms以上，再使數據線升為高電平，從而產生讀信號 
{
    byte i = 0; //每個讀週期最短的持續時間為60ms，各個讀週期之間必須有1ms以上的高電平恢復期 
    byte dat = 0; 
    for(i = 8; i > 0; i--) //一個字節有8位 
    {    DQ = 1; 
    	delay_temp(1); 
    	DQ = 0; 
    	dat >>= 1; 
    	DQ = 1; 
    	if(DQ)
    	    dat |= 0x80; 
    	delay_temp(4); 
    } 
    return dat; 
}
 
//------寫一個字節---------------------------------------------------------------- 
void WriteOneByte(byte dat) 
{
    byte i = 0; //數據線從高電平拉至低電平，產生寫起始信號。15ms之內將所需寫的位送到數據線上， 
    for(i = 8; i > 0; i--) //在15~60ms之間對數據線進行採樣，如果是高電平就寫1，低寫0發生。 
    {
    	DQ = 0; //在開始另一個寫週期前必須有1ms以上的高電平恢復期。 
    	DQ = dat & 0x01; 
    	delay_temp(5); 
    	DQ = 1; 
    	dat >>= 1; 
    } 
    delay_temp(4); 
}

void SetConfiguration(sbyte Th,sbyte Tl,byte config)
{
    Init_DS18B20(); //初始化 
    WriteOneByte(0x4E);  
    WriteOneByte(Th);  
    WriteOneByte(Tl); 
    WriteOneByte(0x1F | (config << 5)); //溫度轉換精確度
} 
//-------讀溫度值（低位放tempL;高位放tempH;溫度值存在temperature.)--------------- 

void DetectTemperature()
{ 
    Init_DS18B20(); //初始化 
    WriteOneByte(0xCC); //跳過讀序列號的操作 
    WriteOneByte(0x44); //啟動溫度轉換
}

float ReadTemperature() 
{
    byte tempL = 0;
    byte tempH = 0; 
    float temperature; //溫度值保存在temperature裡
    Init_DS18B20(); //初始化 
    WriteOneByte(0xCC); //跳過讀序列號的操作 
    WriteOneByte(0xBE); //讀溫度寄存器（頭兩個值分別為溫度的低位和高位） 
    tempL = ReadOneByte(); //讀出溫度的低位LSB 
    tempH = ReadOneByte(); //讀出溫度的高位MSB  
    temperature = (((int)tempH << 8) | tempL) * 0.0625f; //溫度轉換，把高低位做相應的運算轉化為實際溫度
    return temperature; 
}