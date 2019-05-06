#include <at89c51xd2.h>
#include <INTRINS.H>
#include "DS1307.h"

 
#define DS1307_WRITE 0xD0 
#define DS1307_READ  0xD1 

sbit SCL = P3^6;
sbit SDA = P3^7	;  
   
/**********************************************************************************  
(1) void StartIIC(void)            
(2) 函數功能:啟動IIC總線  
(3) 入口參數:void  
(4) 出口參數:void  
(5) 修改的外部變量(全局變量) 說明  
(6) 修改人      修改日期  
(7) 修改內容  
*********************************************************************************/   
void StartIIC(void)   
{   
    SCL = 1;//將時鐘線置高   
    _nop_();   
    _nop_();//穩定時間2uS   
    SDA = 1;//將數據線置高   
    _nop_();   
    _nop_();//穩定時間2uS   
    SDA = 0;//將數據線置低   
    _nop_();   
    _nop_();   
    _nop_();   
    _nop_();//啟動時間4uS   
    SCL = 0;//將時鐘線置低   
    _nop_();   
    _nop_();//穩定時間2uS   
}   
   
/**********************************************************************************  
(1) void StopIIC(void)            
(2) 函數功能:停止IIC總線  
(3) 入口參數:void  
(4) 出口參數:void  
(5) 修改的外部變量(全局變量) 說明  
(6) 修改人      修改日期  
(7) 修改內容  
*********************************************************************************/   
void StopIIC(void)   
{   
	SDA=0;  /*發送結束條件的數據信號*/   
	_nop_();   /*發送結束條件的時鐘信號*/   
	SCL=1;  /*結束條件建立時間大於4μs*/   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
	SDA=1;  /*發送I2C總線結束信號*/   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
}   
   
/**********************************************************************************  
(1) void AckIIC(bit bAckFlag)            
(2) 函數功能:主控器應答信號  
(3) 入口參數:bAckFlag=0-ACK,bAckFlag=1-NACK.  
(4) 出口參數:void  
(5) 修改的外部變量(全局變量) 說明  
(6) 修改人      修改日期  
(7) 修改內容  
*********************************************************************************/   
void AckIIC(bit bAckFlag)   
{   
    SDA = bAckFlag;   
    _nop_();//穩定時間1uS   
    SCL =1;//時鐘置高   
    _nop_();   
    _nop_();   
    _nop_();//穩定時間3uS   
    SCL =0;//時鐘置低          
    _nop_();//穩定時間1uS   
    SDA = 1;//將數據線置高   
}   
   
/**********************************************************************************  
(1) byte SendOneByte(unsinged char ucByte)         
(2) 函數功能:IIC總線發送一個字節  
(3) 入口參數:要寫的字節ucByte.  
(4) 出口參數:ACK-return 0,NACK-return 1.  
(5) 修改的外部變量(全局變量) 說明  
(6) 修改人      修改日期  
(7) 修改內容  
*********************************************************************************/   
byte SendOneByte(byte ucSendByte)   
{   
    byte ucBitCount;   
    byte ucReturnByte;   
    for(ucBitCount=0; ucBitCount<8; ucBitCount++)   
    {   
        if(ucSendByte&0x80) SDA = 1;//數據位為高   
        else            SDA = 0;//數據位為低   
        do   
        {   
            _nop_();//穩定時間1uS   
            SCL = 1;//時鐘置高   
            _nop_();//穩定時間1uS   
        }   
        while(!SCL);//時鐘信號被鉗位   
        _nop_();_nop_();//穩定時間2uS   
        SCL = 0;//時鐘置低   
        _nop_();//穩定時間1uS   
        ucSendByte <<= 1;//發送字節左移一位   
    }   
    SDA = 1;//數據線置高,準備檢測應答信號   
    _nop_();//穩定時間1uS   
    SCL = 1;//時鐘置高   
    _nop_();_nop_();//穩定時間2uS   
    if(SDA) ucReturnByte = 0x01;   
    else ucReturnByte = 0x00;   
    SCL = 0;//時鐘置低   
    _nop_();_nop_();//穩定時間2uS   
    return (ucReturnByte);   
}   
   
   
/**********************************************************************************  
(1) byte ReceiveOneByte(void)            
(2) 函數功能:IIC總線接收一個字節  
(3) 入口參數:void  
(4) 出口參數:接收到的字節  
(5) 修改的外部變量(全局變量) 說明  
(6) 修改人      修改日期  
(7) 修改內容  
*********************************************************************************/   
byte ReceiveOneByte(void)   
{   
    byte ucBitCount;   
    byte ucReceiveByte;   
    SDA = 1;//置數據線為輸入方式   
    for(ucBitCount=0; ucBitCount<8; ucBitCount++)   
    {   
        SCL = 0;//時鐘置低   
        _nop_();//穩定時間1uS   
        SCL = 1;//時鐘置高   
        _nop_();//穩定時間1uS   
        ucReceiveByte <<= 1;//左移一位,LSB=0.   
        if(SDA)  ucReceiveByte += 1;//如果SDA=1,LSB=1.   
    }   
    SCL = 0;//時鐘置低   
    return(ucReceiveByte);   
}   

void DS1307_getBytes(byte address, byte uiDataLen, byte* pucDataBuff)
{
    byte uiCount;   
   
    StartIIC();//啟動總線   
    SendOneByte(DS1307_WRITE);//發送頁地址   
    SendOneByte(address);//發送字地址   
   
    StartIIC();//啟動總線   
    if(SendOneByte(DS1307_READ))//發送讀命令   
     return;   
       
    for(uiCount=0;uiCount<uiDataLen-1;uiCount++)   
    {   
        *pucDataBuff = ReceiveOneByte();//接收數據   
        pucDataBuff ++;   
        AckIIC(0);   
    }   
    *pucDataBuff = ReceiveOneByte();   
    AckIIC(1);//非應答   
       
    StopIIC();//停止總線   
}

void DS1307_setBytes(byte address, byte uiDataLen, byte* pucDataBuff) 
{
    byte uiCount;   
    StartIIC();//啟動總線   
    SendOneByte(DS1307_WRITE);//發送器件寫地址   
    SendOneByte(address);//發送字地址   
    for(uiCount=0;uiCount<uiDataLen;uiCount++)   
    {   
        SendOneByte(*pucDataBuff);//發送數據   
        pucDataBuff++;   
    }   
    StopIIC();//停止總線  
}

void DS1307_setDateTime(DateTime* dateTime)
{
    DS1307_setBytes(0x00, 7, (byte*)dateTime);
}

void DS1307_getDateTime(DateTime* dateTime)
{
    DS1307_getBytes(0x00, 7, (byte*)dateTime);
}
