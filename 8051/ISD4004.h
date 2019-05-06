#include "Types.h"	

void ISD_Stop(void);        //發送 stop 指令
void ISD_PowerUp(void);     //發送上電指令
void ISD_PowerDown(void);   //發送掉電指令

void ISD_Play(void);        //發送放音指令 
void ISD_SetPlay(uint add); //發送指定放音指令
void ISD_Rec(void);         //發送錄音指令 
void ISD_SetRec(uint add);  //發送指定錄音指令 
byte ISD_Chk_Isdovf(void);

//void PLAY_now(byte add_sect);//按指定地址開始放音
//void REC_now(byte add_sect); //按指定地址開始錄音