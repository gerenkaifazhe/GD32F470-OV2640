#ifndef MyIIC_H
#define MyIIC_H

#include "gd32f4xx.h"

void MyI2C0_Init(void);
uint8_t MyI2C0_ReadReg(uint8_t SlaveADDR, uint8_t RegisterADDR);
void MyI2C0_WriteReg(uint8_t SlaveADDR, uint8_t RegisterADDR, uint8_t Data);

#endif
