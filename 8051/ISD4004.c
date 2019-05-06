/*�ާ@�����G1�B����K1��}�l�����A��}K1�䵲�������F�A����K1��A�}�l���ĤG�q�F�H�������F
   2�B���@�UK2��A�}�l�񭵡A���Ӭq�񭵵����A�~�򵥫ݡF�A���UK2��A��ĤG�q�A�H������
   PS:�C�@�q�������׭n��ʳ]�m�A����ۥD����������ס]�H�ݫ��^*/
#include <at89c51xd2.h>
#include <INTRINS.H> 
#include "ISD4004.h"
#include "Display.h"

//========ISD4004���O�w�q========================
#define POWER_UP  0x20 //�W�q���O
#define SET_PLAY  0xE0 //���w�񭵫��O
#define PLAY      0xF0 //��e�񭵫��O
#define SET_REC   0xA0 //���w�������O
#define REC       0xB0 //��e�������O
#define SET_MC    0xE8 //���w�ֶi���O
#define MC        0xF8 //�ֶi������O
#define STOP      0x30 //�����e�ާ@
#define STOP_WRDN 0x10 //�����e�ާ@�ñ��q
#define RINT      0x30 //Ū���A:OVF�MEOM
//=========ISD4004--c51���f�w�q=================
sbit ISD_SCLK = P2^0; //ISD4004 ����
sbit ISD_SS   = P2^1; //����
sbit ISD_MOSI = P2^2; //�ƾڿ�J
sbit ISD_MISO = P2^3; //�ƾڿ�X
sbit ISD_INT  = P2^4; //���X���_ 
sbit ISD_RAC  = P2^5; //��a�}����
//==========ISD4004��Ʃw�q===================
void ISD_SPI_Send8(byte isdx8); //spi ���o�e�l�{�ǡA8��ƾ�,�q�C�찪
void ISD_SPI_Send16(uint isdx16); //spi ���o�e�l�{�ǡA16��ƾ�.�q�C�찪
uint ISD_SPI_Radd(void);   //Ū���Ц�a�}

//========���ɨ��===========================


/******************************************************************
�W�١GPLAY_now(byte add_sect)
�\��G�����w�a�}�q�}�l����                                                   
���O�G
�եΡG�L
��^�G�L
/*****************************************************************/
/*void PLAY_now(byte add_sect)
{
 ISD_PowerUp(); //ISD �W�q
 delayms(25);
 ISD_SetPlay(ISD_ADDS1);

 ISD_Play(); //�o�e�񭵫��O 
}*/  
/******************************************************************
�W�١GREC_now(byte add_sect)
�\��G�����w�a�}�q�}�l����                                                   
���O�G
�եΡG�L
��^�G�L
/******************************************************************/
/*void REC_now(byte add_sect)
{
 ISD_PowerUp(); //ISD �W�q
 Delay1Ms(50); //�������
 ISD_PowerUp(); //ISD �W�q
 Delay1Ms(100); //�������
 switch (add_sect)
  {
  case 1: ISD_SetRec(ISD_ADDS1);break; //�o�e�a�}�� SetRec ���O
  case 2: ISD_SetRec(ISD_ADDS2);break; //�o�e�a�}�� SetRec ���O
  case 3: ISD_SetRec(ISD_ADDS3);break; //�o�e�a�}�� SetRec ���O
  case 4: ISD_SetRec(ISD_ADDS4);break; //�o�e�a�}�� SetRec ���O
  case 5: ISD_SetRec(ISD_ADDS5);break; //�o�e�a�}�� SetRec ���O
  //case 6:�K�K
  }
 ISD_Rec(); //�o�e rec ���O  
}*/

/******************************************************************
�W�١GISD_SPI_Send8(byte isdx)
�\��Gspi ���o�e�l�{�ǡA8 ��ƾ�                                                   
���O�G
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_SPI_Send8( byte isdx8 )
{ 
    byte i;
    
    ISD_SS = 0;   //�襤ISD4004
    ISD_SCLK = 0; 
    for (i = 0; i < 8; i++)    //���o�C��A�o����A�̦��o�e�C
    {
    	if ((isdx8 & 0x01) == 1) //�o�e�̧C��
    	    ISD_MOSI = 1; 
    	else 
    	    ISD_MOSI = 0;
    	
    	isdx8 >>= 1;   //�k���@��
    	
    	ISD_SCLK = 1;   //�����U���u�o�e
    	ISD_SCLK = 0; 
    } 
}
/******************************************************************
�W�١GISD_SPI_Send16(uint isdx16)
�\��Gspi ���o�e�l�{�ǡA16��ƾ�                                                   
���O�G
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_SPI_Send16( uint isdx16 )
{ 
    byte i;
    
    ISD_SS = 0;   //�襤ISD4004
    ISD_SCLK = 0; 
    for (i = 0; i < 16; i++) //���o�C��A�o����A�̦��o�e�C
    {
    	if ((isdx16&0x0001)==1) //�o�e�̧C��
    	    ISD_MOSI = 1; 
    	else 
    	    ISD_MOSI = 0;
    	
    	isdx16 = isdx16 >> 1;   //�k���@��
    	
    	ISD_SCLK=1;   //�����U���u�o�e
    	ISD_SCLK=0; 
    } 
}

/******************************************************************
�W�١GISD_SPI_Radd(void)
�\��GŪ��16���a�}                                                   
���O�G
�եΡG�L
��^�G�L
/******************************************************************/
uint ISD_SPI_Radd(void)
{ 
    byte i;
    uint addsig;
    ISD_SS = 0;   //�襤ISD4004
    ISD_SCLK = 0; 
    //==============Ū16��a�}================
    for(i=0;i<16;i++)   //Ū��a�}
    {
    	ISD_SCLK = 1;  //�����U���u�ƾڲ��XISD
    	ISD_SCLK = 0;
    	
    	if (ISD_MISO == 1) 
    	    addsig |= 0x8000; 
    	if (i<15)
    	    addsig >>= 1; //�̥�Ū�X���O�a�}���C��A�ҥH�n�����A�̫�@���a�}���Υ����A�_�h�a�}���X
    }
    
    ISD_Stop(); //�o�e stop ���O
    ISD_SS = 1; //���� spi �q�H��
    
    return addsig; //��^�a�}��  
}
/******************************************************************
�W�١GISD_Stop(void)
�\��G�o�e stop ���O                                                   
���O�GISD_SPI_Send8(byte isdx8);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_Stop(void)
{
    //ISD_SS = 0;
    ISD_SPI_Send8(STOP);  
    ISD_SS = 1;     //��������
}
/******************************************************************
�W�١GISD_PowerUp(void)
�\��G�o�e�W�q���O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_PowerUp(void)
{ 
    ISD_SS = 0;     //�襤ISD4004
    ISD_SPI_Send8(POWER_UP);
    ISD_SS = 1;
}
/******************************************************************
�W�١GISD_PowerDown(void)
�\��G�o�e���q���O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_PowerDown(void)
{
    ISD_SS = 0; 
    ISD_SPI_Send8(STOP_WRDN);
    ISD_SS = 1;
}
/******************************************************************
�W�١GISD_Play(void)
�\��G�o�eplay���O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_Play(void)
{ 
    ISD_SS = 0;
    ISD_SPI_Send8(PLAY); 
    ISD_SS = 1;
}
/******************************************************************
�W�١GISD_Rec(void)
�\��G�o�erec�������O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_Rec(void)
{ 
    ISD_SS = 0;
    ISD_SPI_Send8(REC);
    ISD_SS = 1;
}   
/******************************************************************
�W�١GISD_SetPlay(uint add)
�\��G�o�esetplay���O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
  ISD_SPI_Send16(uint isdx16);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_SetPlay(uint add)
{
    delayms(1);
    ISD_SPI_Send16(add);  //�o�e�񭵰_�l�a�}
    ISD_SPI_Send8(SET_PLAY); //�o�e setplay ���O�r�`
    ISD_SS = 1;
}
/******************************************************************
�W�١GISD_SetRec(uint add)
�\��G�o�esetrec���O�A�é��� 50ms                                                   
���O�GISD_SPI_Send8(byte isdx8);
  ISD_SPI_Send16(uint isdx16);
�եΡG�L
��^�G�L
/******************************************************************/
void ISD_SetRec(uint add)
{
    delayms(1);
    ISD_SPI_Send16(add); //�o�e�����_�l�a�}
    ISD_SPI_Send8(SET_REC); //�o�e setrec ���O�r�`
    ISD_SS = 1;
}
//************************************
//�ˬd����O�_���X(Ū OVF,�ê�^ OVF ��)
//************************************
byte ISD_Chk_Isdovf(void)
{
    ISD_SS=0; 
    ISD_SCLK=0;  
    ISD_SCLK=1; 
    ISD_SCLK=0; 
    
    if (ISD_MISO==1)
    { 
    	ISD_SCLK=0; 
    	ISD_SS =1; //���� spi �q�H��
    	ISD_Stop(); //�o�e stop ���O
    	return 1; //OVF �� 1�A��^ 1
    } 
    else
    { 
    	ISD_SCLK=0; 
    	ISD_SS =1; //���� spi �q�H��
    	ISD_Stop(); //�o�e stop ���O
    	return 0; //OVF �� 0�A��^ 0
    } 
}