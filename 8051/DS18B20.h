#ifndef DS18B20_H
#define DS18B20_H

#include "Types.h"

void DetectTemperature();
float ReadTemperature();
void SetConfiguration(sbyte Th,sbyte Tl,byte config); 

#endif