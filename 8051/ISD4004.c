/*操作說明：1、按住K1鍵開始錄音，放開K1鍵結束錄音；再按住K1鍵，開始錄第二段；以此類推；
   2、按一下K2鍵，開始放音，等該段放音結束，繼續等待；再按下K2鍵，放第二段，以此類推
   PS:每一段錄音長度要手動設置，不能自主控制錄音長度（以待後改）*/
#include <at89c51xd2.h>
#include <INTRINS.H> 
#include "ISD4004.h"
#include "Display.h"

//========ISD4004指令定義========================
#define POWER_UP  0x20 //上電指令
#define SET_PLAY  0xE0 //指定放音指令
#define PLAY      0xF0 //當前放音指令
#define SET_REC   0xA0 //指定錄音指令
#define REC       0xB0 //當前錄音指令
#define SET_MC    0xE8 //指定快進指令
#define MC        0xF8 //快進執行指令
#define STOP      0x30 //停止當前操作
#define STOP_WRDN 0x10 //停止當前操作並掉電
#define RINT      0x30 //讀狀態:OVF和EOM
//=========ISD4004--c51接口定義=================
sbit ISD_SCLK = P2^0; //ISD4004 時鐘
sbit ISD_SS   = P2^1; //片選
sbit ISD_MOSI = P2^2; //數據輸入
sbit ISD_MISO = P2^3; //數據輸出
sbit ISD_INT  = P2^4; //溢出中斷 
sbit ISD_RAC  = P2^5; //行地址時鐘
//==========ISD4004函數定義===================
void ISD_SPI_Send8(byte isdx8); //spi 串行發送子程序，8位數據,從低到高
void ISD_SPI_Send16(uint isdx16); //spi 串行發送子程序，16位數據.從低到高
uint ISD_SPI_Radd(void);   //讀取標行地址

//========延時函數===========================


/******************************************************************
名稱：PLAY_now(byte add_sect)
功能：按指定地址段開始播放                                                   
指令：
調用：無
返回：無
/*****************************************************************/
/*void PLAY_now(byte add_sect)
{
 ISD_PowerUp(); //ISD 上電
 delayms(25);
 ISD_SetPlay(ISD_ADDS1);

 ISD_Play(); //發送放音指令 
}*/  
/******************************************************************
名稱：REC_now(byte add_sect)
功能：按指定地址段開始錄音                                                   
指令：
調用：無
返回：無
/******************************************************************/
/*void REC_now(byte add_sect)
{
 ISD_PowerUp(); //ISD 上電
 Delay1Ms(50); //延遲錄音
 ISD_PowerUp(); //ISD 上電
 Delay1Ms(100); //延遲錄音
 switch (add_sect)
  {
  case 1: ISD_SetRec(ISD_ADDS1);break; //發送地址的 SetRec 指令
  case 2: ISD_SetRec(ISD_ADDS2);break; //發送地址的 SetRec 指令
  case 3: ISD_SetRec(ISD_ADDS3);break; //發送地址的 SetRec 指令
  case 4: ISD_SetRec(ISD_ADDS4);break; //發送地址的 SetRec 指令
  case 5: ISD_SetRec(ISD_ADDS5);break; //發送地址的 SetRec 指令
  //case 6:……
  }
 ISD_Rec(); //發送 rec 指令  
}*/

/******************************************************************
名稱：ISD_SPI_Send8(byte isdx)
功能：spi 串行發送子程序，8 位數據                                                   
指令：
調用：無
返回：無
/******************************************************************/
void ISD_SPI_Send8( byte isdx8 )
{ 
    byte i;
    
    ISD_SS = 0;   //選中ISD4004
    ISD_SCLK = 0; 
    for (i = 0; i < 8; i++)    //先發低位再發高位，依次發送。
    {
    	if ((isdx8 & 0x01) == 1) //發送最低位
    	    ISD_MOSI = 1; 
    	else 
    	    ISD_MOSI = 0;
    	
    	isdx8 >>= 1;   //右移一位
    	
    	ISD_SCLK = 1;   //時鐘下降沿發送
    	ISD_SCLK = 0; 
    } 
}
/******************************************************************
名稱：ISD_SPI_Send16(uint isdx16)
功能：spi 串行發送子程序，16位數據                                                   
指令：
調用：無
返回：無
/******************************************************************/
void ISD_SPI_Send16( uint isdx16 )
{ 
    byte i;
    
    ISD_SS = 0;   //選中ISD4004
    ISD_SCLK = 0; 
    for (i = 0; i < 16; i++) //先發低位再發高位，依次發送。
    {
    	if ((isdx16&0x0001)==1) //發送最低位
    	    ISD_MOSI = 1; 
    	else 
    	    ISD_MOSI = 0;
    	
    	isdx16 = isdx16 >> 1;   //右移一位
    	
    	ISD_SCLK=1;   //時鐘下降沿發送
    	ISD_SCLK=0; 
    } 
}

/******************************************************************
名稱：ISD_SPI_Radd(void)
功能：讀取16位行地址                                                   
指令：
調用：無
返回：無
/******************************************************************/
uint ISD_SPI_Radd(void)
{ 
    byte i;
    uint addsig;
    ISD_SS = 0;   //選中ISD4004
    ISD_SCLK = 0; 
    //==============讀16位地址================
    for(i=0;i<16;i++)   //讀行地址
    {
    	ISD_SCLK = 1;  //時鐘下降沿數據移出ISD
    	ISD_SCLK = 0;
    	
    	if (ISD_MISO == 1) 
    	    addsig |= 0x8000; 
    	if (i<15)
    	    addsig >>= 1; //最先讀出的是地址的低位，所以要左移，最後一次地址不用左移，否則地址溢出
    }
    
    ISD_Stop(); //發送 stop 指令
    ISD_SS = 1; //關閉 spi 通信端
    
    return addsig; //返回地址值  
}
/******************************************************************
名稱：ISD_Stop(void)
功能：發送 stop 指令                                                   
指令：ISD_SPI_Send8(byte isdx8);
調用：無
返回：無
/******************************************************************/
void ISD_Stop(void)
{
    //ISD_SS = 0;
    ISD_SPI_Send8(STOP);  
    ISD_SS = 1;     //關閉片選
}
/******************************************************************
名稱：ISD_PowerUp(void)
功能：發送上電指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
調用：無
返回：無
/******************************************************************/
void ISD_PowerUp(void)
{ 
    ISD_SS = 0;     //選中ISD4004
    ISD_SPI_Send8(POWER_UP);
    ISD_SS = 1;
}
/******************************************************************
名稱：ISD_PowerDown(void)
功能：發送掉電指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
調用：無
返回：無
/******************************************************************/
void ISD_PowerDown(void)
{
    ISD_SS = 0; 
    ISD_SPI_Send8(STOP_WRDN);
    ISD_SS = 1;
}
/******************************************************************
名稱：ISD_Play(void)
功能：發送play指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
調用：無
返回：無
/******************************************************************/
void ISD_Play(void)
{ 
    ISD_SS = 0;
    ISD_SPI_Send8(PLAY); 
    ISD_SS = 1;
}
/******************************************************************
名稱：ISD_Rec(void)
功能：發送rec錄音指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
調用：無
返回：無
/******************************************************************/
void ISD_Rec(void)
{ 
    ISD_SS = 0;
    ISD_SPI_Send8(REC);
    ISD_SS = 1;
}   
/******************************************************************
名稱：ISD_SetPlay(uint add)
功能：發送setplay指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
  ISD_SPI_Send16(uint isdx16);
調用：無
返回：無
/******************************************************************/
void ISD_SetPlay(uint add)
{
    delayms(1);
    ISD_SPI_Send16(add);  //發送放音起始地址
    ISD_SPI_Send8(SET_PLAY); //發送 setplay 指令字節
    ISD_SS = 1;
}
/******************************************************************
名稱：ISD_SetRec(uint add)
功能：發送setrec指令，並延遲 50ms                                                   
指令：ISD_SPI_Send8(byte isdx8);
  ISD_SPI_Send16(uint isdx16);
調用：無
返回：無
/******************************************************************/
void ISD_SetRec(uint add)
{
    delayms(1);
    ISD_SPI_Send16(add); //發送錄音起始地址
    ISD_SPI_Send8(SET_REC); //發送 setrec 指令字節
    ISD_SS = 1;
}
//************************************
//檢查芯片是否溢出(讀 OVF,並返回 OVF 值)
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
    	ISD_SS =1; //關閉 spi 通信端
    	ISD_Stop(); //發送 stop 指令
    	return 1; //OVF 為 1，返回 1
    } 
    else
    { 
    	ISD_SCLK=0; 
    	ISD_SS =1; //關閉 spi 通信端
    	ISD_Stop(); //發送 stop 指令
    	return 0; //OVF 為 0，返回 0
    } 
}