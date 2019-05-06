#ifndef DS1302_H
#define DS1302_H
// RTC´¹¤ù DS1302

#define byte unsigned char

#define WRITE_SECOND 0x80
#define READ_SECOND  0x81

#define WRITE_MINUTE 0x82
#define READ_MINUTE  0x83

#define WRITE_HOUR   0x84
#define READ_HOUR    0x85

#define WRITE_DATE   0x86
#define READ_DATE    0x87

#define WRITE_MONTH  0x88
#define READ_MONTH   0x89

#define WRITE_DAY    0x8A
#define READ_DAY     0x8B

#define WRITE_YEAR   0x8C
#define READ_YEAR    0x8D

#define WRITE_RAM(X) (0xC0+((X)<<1))
#define READ_RAM(X)  (0xC1+((X)<<1))

#define WRITE_RAM_BURST   0xFE
#define READ_RAM_BURST    0xFF

#define WRITE_CLOCK_BURST 0xBE
#define READ_CLOCK_BURST  0xBF

#define WRITE_WRITE_PROTECT 0x8E
#define READ_WRITE_PROTECT  0x8F

typedef struct _dateTime
{
    byte second;
    byte minute;
    byte hour;
    byte date;
    byte month;
    byte day;
    byte year;
} DateTime;

void writebyte(byte, byte);
byte readbyte(byte);
void read_clk_regs(DateTime*);
void write_clk_regs(DateTime*);

#endif