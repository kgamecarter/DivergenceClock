#include "BCDCompute.h"

byte BCDaddOne(byte BCDvalue, byte BCDmin, byte BCDmax)
{
    if (BCDvalue >= BCDmax)
        return BCDmin;
    if (BCDvalue < BCDmin)
        return BCDmin;
    if ((BCDvalue & 0xF) != 9)
        BCDvalue++;
    else
        BCDvalue = (BCDvalue & 0xF0) + 0x10;
    return BCDvalue;
}

byte BCDsubOne(byte BCDvalue, byte BCDmin, byte BCDmax)
{
    if (BCDvalue > BCDmax)
        return BCDmax;
    if (BCDvalue <= BCDmin)
        return BCDmax;
    if ((BCDvalue & 0xF) != 0)
        BCDvalue--;
    else
        BCDvalue = (BCDvalue & 0xF0) - 0x10 + 0x09;
    return BCDvalue;
}