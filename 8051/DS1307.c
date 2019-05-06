#include <at89c51xd2.h>
#include <INTRINS.H>
#include "DS1307.h"

 
#define DS1307_WRITE 0xD0 
#define DS1307_READ  0xD1 

sbit SCL = P3^6;
sbit SDA = P3^7	;  
   
/**********************************************************************************  
(1) void StartIIC(void)            
(2) ��ƥ\��:�Ұ�IIC�`�u  
(3) �J�f�Ѽ�:void  
(4) �X�f�Ѽ�:void  
(5) �ק諸�~���ܶq(�����ܶq) ����  
(6) �ק�H      �ק���  
(7) �ק鷺�e  
*********************************************************************************/   
void StartIIC(void)   
{   
    SCL = 1;//�N�����u�m��   
    _nop_();   
    _nop_();//í�w�ɶ�2uS   
    SDA = 1;//�N�ƾڽu�m��   
    _nop_();   
    _nop_();//í�w�ɶ�2uS   
    SDA = 0;//�N�ƾڽu�m�C   
    _nop_();   
    _nop_();   
    _nop_();   
    _nop_();//�Ұʮɶ�4uS   
    SCL = 0;//�N�����u�m�C   
    _nop_();   
    _nop_();//í�w�ɶ�2uS   
}   
   
/**********************************************************************************  
(1) void StopIIC(void)            
(2) ��ƥ\��:����IIC�`�u  
(3) �J�f�Ѽ�:void  
(4) �X�f�Ѽ�:void  
(5) �ק諸�~���ܶq(�����ܶq) ����  
(6) �ק�H      �ק���  
(7) �ק鷺�e  
*********************************************************************************/   
void StopIIC(void)   
{   
	SDA=0;  /*�o�e�������󪺼ƾګH��*/   
	_nop_();   /*�o�e�������󪺮����H��*/   
	SCL=1;  /*��������إ߮ɶ��j��4�gs*/   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
	SDA=1;  /*�o�eI2C�`�u�����H��*/   
	_nop_();   
	_nop_();   
	_nop_();   
	_nop_();   
}   
   
/**********************************************************************************  
(1) void AckIIC(bit bAckFlag)            
(2) ��ƥ\��:�D���������H��  
(3) �J�f�Ѽ�:bAckFlag=0-ACK,bAckFlag=1-NACK.  
(4) �X�f�Ѽ�:void  
(5) �ק諸�~���ܶq(�����ܶq) ����  
(6) �ק�H      �ק���  
(7) �ק鷺�e  
*********************************************************************************/   
void AckIIC(bit bAckFlag)   
{   
    SDA = bAckFlag;   
    _nop_();//í�w�ɶ�1uS   
    SCL =1;//�����m��   
    _nop_();   
    _nop_();   
    _nop_();//í�w�ɶ�3uS   
    SCL =0;//�����m�C          
    _nop_();//í�w�ɶ�1uS   
    SDA = 1;//�N�ƾڽu�m��   
}   
   
/**********************************************************************************  
(1) byte SendOneByte(unsinged char ucByte)         
(2) ��ƥ\��:IIC�`�u�o�e�@�Ӧr�`  
(3) �J�f�Ѽ�:�n�g���r�`ucByte.  
(4) �X�f�Ѽ�:ACK-return 0,NACK-return 1.  
(5) �ק諸�~���ܶq(�����ܶq) ����  
(6) �ק�H      �ק���  
(7) �ק鷺�e  
*********************************************************************************/   
byte SendOneByte(byte ucSendByte)   
{   
    byte ucBitCount;   
    byte ucReturnByte;   
    for(ucBitCount=0; ucBitCount<8; ucBitCount++)   
    {   
        if(ucSendByte&0x80) SDA = 1;//�ƾڦ쬰��   
        else            SDA = 0;//�ƾڦ쬰�C   
        do   
        {   
            _nop_();//í�w�ɶ�1uS   
            SCL = 1;//�����m��   
            _nop_();//í�w�ɶ�1uS   
        }   
        while(!SCL);//�����H���Q�X��   
        _nop_();_nop_();//í�w�ɶ�2uS   
        SCL = 0;//�����m�C   
        _nop_();//í�w�ɶ�1uS   
        ucSendByte <<= 1;//�o�e�r�`�����@��   
    }   
    SDA = 1;//�ƾڽu�m��,�ǳ��˴������H��   
    _nop_();//í�w�ɶ�1uS   
    SCL = 1;//�����m��   
    _nop_();_nop_();//í�w�ɶ�2uS   
    if(SDA) ucReturnByte = 0x01;   
    else ucReturnByte = 0x00;   
    SCL = 0;//�����m�C   
    _nop_();_nop_();//í�w�ɶ�2uS   
    return (ucReturnByte);   
}   
   
   
/**********************************************************************************  
(1) byte ReceiveOneByte(void)            
(2) ��ƥ\��:IIC�`�u�����@�Ӧr�`  
(3) �J�f�Ѽ�:void  
(4) �X�f�Ѽ�:�����쪺�r�`  
(5) �ק諸�~���ܶq(�����ܶq) ����  
(6) �ק�H      �ק���  
(7) �ק鷺�e  
*********************************************************************************/   
byte ReceiveOneByte(void)   
{   
    byte ucBitCount;   
    byte ucReceiveByte;   
    SDA = 1;//�m�ƾڽu����J�覡   
    for(ucBitCount=0; ucBitCount<8; ucBitCount++)   
    {   
        SCL = 0;//�����m�C   
        _nop_();//í�w�ɶ�1uS   
        SCL = 1;//�����m��   
        _nop_();//í�w�ɶ�1uS   
        ucReceiveByte <<= 1;//�����@��,LSB=0.   
        if(SDA)  ucReceiveByte += 1;//�p�GSDA=1,LSB=1.   
    }   
    SCL = 0;//�����m�C   
    return(ucReceiveByte);   
}   

void DS1307_getBytes(byte address, byte uiDataLen, byte* pucDataBuff)
{
    byte uiCount;   
   
    StartIIC();//�Ұ��`�u   
    SendOneByte(DS1307_WRITE);//�o�e���a�}   
    SendOneByte(address);//�o�e�r�a�}   
   
    StartIIC();//�Ұ��`�u   
    if(SendOneByte(DS1307_READ))//�o�eŪ�R�O   
     return;   
       
    for(uiCount=0;uiCount<uiDataLen-1;uiCount++)   
    {   
        *pucDataBuff = ReceiveOneByte();//�����ƾ�   
        pucDataBuff ++;   
        AckIIC(0);   
    }   
    *pucDataBuff = ReceiveOneByte();   
    AckIIC(1);//�D����   
       
    StopIIC();//�����`�u   
}

void DS1307_setBytes(byte address, byte uiDataLen, byte* pucDataBuff) 
{
    byte uiCount;   
    StartIIC();//�Ұ��`�u   
    SendOneByte(DS1307_WRITE);//�o�e����g�a�}   
    SendOneByte(address);//�o�e�r�a�}   
    for(uiCount=0;uiCount<uiDataLen;uiCount++)   
    {   
        SendOneByte(*pucDataBuff);//�o�e�ƾ�   
        pucDataBuff++;   
    }   
    StopIIC();//�����`�u  
}

void DS1307_setDateTime(DateTime* dateTime)
{
    DS1307_setBytes(0x00, 7, (byte*)dateTime);
}

void DS1307_getDateTime(DateTime* dateTime)
{
    DS1307_getBytes(0x00, 7, (byte*)dateTime);
}
