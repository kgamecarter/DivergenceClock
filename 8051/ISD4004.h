#include "Types.h"	

void ISD_Stop(void);        //�o�e stop ���O
void ISD_PowerUp(void);     //�o�e�W�q���O
void ISD_PowerDown(void);   //�o�e���q���O

void ISD_Play(void);        //�o�e�񭵫��O 
void ISD_SetPlay(uint add); //�o�e���w�񭵫��O
void ISD_Rec(void);         //�o�e�������O 
void ISD_SetRec(uint add);  //�o�e���w�������O 
byte ISD_Chk_Isdovf(void);

//void PLAY_now(byte add_sect);//�����w�a�}�}�l��
//void REC_now(byte add_sect); //�����w�a�}�}�l����