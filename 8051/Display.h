#ifndef DISPLAY_H
#define DISPLAY_H

#include "Types.h"

#define RDOT     0x10
#define LDOT     0x20
#define NONUM      10
#define OVERCHAR 0xFF

void watchDog();
bool cmpPreState(byte);
void scan(byte*);
void scanCrossfade(byte);
void scanLightness(byte*, byte*);
void displayOff();
void delayms(byte);


extern byte cursorPosition;
extern byte twinkleCount;
extern byte defaultLightness;
extern byte crossfade;
extern byte preStr[8], nextStr[8], preState; 
#endif