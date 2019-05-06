#include <AT89X52.H>
#include "DS1302.h"

void reset_3w();
byte rbyte_3w();
void wbyte_3w(byte);

sbit SCLK = P2^2;
sbit IO   = P2^1;
sbit CE   = P2^0;


void reset_3w()    /* ----- reset and enable the 3-wire interface ------ */
{
   SCLK = 0;
   CE = 0;
   CE = 1;
}

byte rbyte_3w()    /* ------- read one byte from the device -------- */
{
    byte i;
    byte R_Byte = 0x00;
    IO = 1;
    for (i = 0; i < 8; i++)
    {
    	SCLK = 1;
    	SCLK = 0;
    	R_Byte >>= 1;
    	R_Byte |= (byte)IO << 7; 
    }
    return R_Byte;
}

void wbyte_3w(byte W_Byte)    /* ------ write one byte to the device ------- */
{
    byte i;
    for (i = 0; i < 8; ++i)
    {
        IO = 0;
    	if (W_Byte & 0x01)
    	    IO = 1;    /* set port pin high to read data */
    	SCLK = 0;
    	SCLK = 1;
    	W_Byte >>= 1;
    }
}

void writebyte(byte ClkAdd, byte ClkData)
{
    reset_3w();
    wbyte_3w(ClkAdd);
    wbyte_3w(ClkData);
    reset_3w();
}


byte readbyte(byte ClkAdd)
{
    byte value;
    reset_3w();
    wbyte_3w(ClkAdd);
    value = rbyte_3w();
    reset_3w();
    return value;
}

void read_clk_regs(DateTime* date)
{
    reset_3w();
    wbyte_3w(READ_CLOCK_BURST); /* clock burst */
    date->second = rbyte_3w();
    date->minute = rbyte_3w();
    date->hour   = rbyte_3w();
    date->date   = rbyte_3w();
    date->month  = rbyte_3w();
    date->day    = rbyte_3w();
    date->year   = rbyte_3w();
    reset_3w();
}

void write_clk_regs(DateTime* date)    /* --- initialize time & date from user entries --- */
/* Note: NO error checking is done on the user entries! */
{
    reset_3w();
    wbyte_3w(WRITE_WRITE_PROTECT);    /* control register */
    wbyte_3w(0);    	/* disable write protect */
    //reset_3w();
    //wbyte_3w(0x90);    /* trickle charger register */
    //wbyte_3w(0xAB);    /* enable, 2 diodes, 8K resistor */
    reset_3w();
    wbyte_3w(WRITE_CLOCK_BURST);    /* clock burst write (eight registers) */
    wbyte_3w(date->second);
    wbyte_3w(date->minute);
    wbyte_3w(date->hour);
    wbyte_3w(date->date);
    wbyte_3w(date->month);
    wbyte_3w(date->day);
    wbyte_3w(date->year);
    wbyte_3w(0);    	/* must write control register in burst mode */
    reset_3w();
}

void burstramrd(byte* ptr, byte length)    /* ----------- read RAM in burst mode --------------- */
{
    byte i;
    reset_3w();
    wbyte_3w(READ_RAM_BURST);    /* RAM burst read */
    length = length <= 31 ? length : 31;
    for (i = 0; i < length; i++)
    	ptr[i] = rbyte_3w();
    reset_3w();
}

void burstramwr(byte* ptr, byte length)    /* ---- write one value entire RAM in burst mode ---- */
{
    byte i;
    reset_3w();
    wbyte_3w(WRITE_RAM_BURST);    /* RAM burst write */
    length = length <= 31 ? length : 31;
    for (i = 0; i < length; i++)
    	wbyte_3w(ptr[i]);
    reset_3w();
}
