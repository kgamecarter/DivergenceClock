#include "DS18B20.h"
#include <at89c51xd2.h>

sbit DQ = P3^5; //�ƾڶǿ�u�޸} 
//----------------------------------------------------------------------- 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
//----------------------------------------------------------------------- 
//-----------------------�������---------------------------------------- 
void delay_temp(uint n); 
void Init_DS18B20(); 
byte ReadOneByte(); 
void WriteOneByte(byte dat);
//----------------------------------------------------------------------- 
//------------���ɤl�{��------------- 
void delay_temp(uint n) 
{ 
    while(n--);  
}
 
//------��l�Ƶ{��-------------------- 
void Init_DS18B20() 
{
    byte x = 0; 
    DQ = 1; // DQ���m�� 
    delay_temp(8); //�y���� 
    DQ = 0; //�o�e�_��߽� 
    delay_temp(85); //���ɡ]>480ms) 
    DQ = 1; //�԰��ƾڽu 
    delay_temp(14); //���ݡ]15~60ms) 
    x = DQ; //��X���ȨӧP�_��l�Ʀ��S�����\�A18B20�s�b����X=0�A�_�hX=1 
    delay_temp(20); 
}
 
//------Ū�@�Ӧr�`---------------------------------------------------------------- 
byte ReadOneByte() //�D���ƾڽu���q���ԦܧC�q��1ms�H�W�A�A�ϼƾڽu�ɬ����q���A�q�Ӳ���Ū�H�� 
{
    byte i = 0; //�C��Ū�g���̵u������ɶ���60ms�A�U��Ū�g������������1ms�H�W�����q����_�� 
    byte dat = 0; 
    for(i = 8; i > 0; i--) //�@�Ӧr�`��8�� 
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
 
//------�g�@�Ӧr�`---------------------------------------------------------------- 
void WriteOneByte(byte dat) 
{
    byte i = 0; //�ƾڽu�q���q���ԦܧC�q���A���ͼg�_�l�H���C15ms�����N�һݼg����e��ƾڽu�W�A 
    for(i = 8; i > 0; i--) //�b15~60ms������ƾڽu�i��ļˡA�p�G�O���q���N�g1�A�C�g0�o�͡C 
    {
    	DQ = 0; //�b�}�l�t�@�Ӽg�g���e������1ms�H�W�����q����_���C 
    	DQ = dat & 0x01; 
    	delay_temp(5); 
    	DQ = 1; 
    	dat >>= 1; 
    } 
    delay_temp(4); 
}

void SetConfiguration(sbyte Th,sbyte Tl,byte config)
{
    Init_DS18B20(); //��l�� 
    WriteOneByte(0x4E);  
    WriteOneByte(Th);  
    WriteOneByte(Tl); 
    WriteOneByte(0x1F | (config << 5)); //�ū��ഫ��T��
} 
//-------Ū�ū׭ȡ]�C���tempL;�����tempH;�ū׭Ȧs�btemperature.)--------------- 

void DetectTemperature()
{ 
    Init_DS18B20(); //��l�� 
    WriteOneByte(0xCC); //���LŪ�ǦC�����ާ@ 
    WriteOneByte(0x44); //�Ұʷū��ഫ
}

float ReadTemperature() 
{
    byte tempL = 0;
    byte tempH = 0; 
    float temperature; //�ū׭ȫO�s�btemperature��
    Init_DS18B20(); //��l�� 
    WriteOneByte(0xCC); //���LŪ�ǦC�����ާ@ 
    WriteOneByte(0xBE); //Ū�ūױH�s���]�Y��ӭȤ��O���ūת��C��M����^ 
    tempL = ReadOneByte(); //Ū�X�ūת��C��LSB 
    tempH = ReadOneByte(); //Ū�X�ūת�����MSB  
    temperature = (((int)tempH << 8) | tempL) * 0.0625f; //�ū��ഫ�A�Ⱚ�C�찵�������B����Ƭ���ڷū�
    return temperature; 
}