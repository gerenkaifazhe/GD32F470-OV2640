#ifndef Spi4_H
#define Spi4_H

#include "gd32f4xx.h"
#include "systick.h"

void     Spi4_Init(void);
void     Spi4_Write(uint8_t dat);
void Spi4_NewWrite(uint8_t dat);
uint8_t  Spi4_Read(void);

#endif
