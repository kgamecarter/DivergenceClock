#ifndef DS1307_H
#define DS1307_H

#include "Types.h"

#define DS1307_SECOND  0x00

#define DS1307_MINUTE  0x01

#define DS1307_HOUR    0x02

#define DS1307_DAY     0x03

#define DS1307_DATE    0x04

#define DS1307_MONTH   0x05

#define DS1307_YEAR    0x06

#define DS1307_CONTROL 0x07

#define DS1307_RAM(X)  (0x08+(X))


typedef struct _dateTime
{
    byte second;
    byte minute;
    byte hour;
    byte day;
    byte date;
    byte month;
    byte year;
} DateTime;

void DS1307_setBytes(byte address, byte uiDataLen, byte* pucDataBuff);
void DS1307_getBytes(byte address, byte uiDataLen, byte* pucDataBuff);
void DS1307_setDateTime(DateTime*);
void DS1307_getDateTime(DateTime*);


#endif