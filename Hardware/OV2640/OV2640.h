#ifndef _OV2640_H
#define _OV2640_H

#include "gd32f4xx.h"
#include "OV2640_Register.h"
#include "DCI.h"
#include "MyIIC.h"
#include "systick.h"



void OV2640_GPIO_Pin_Init(void);
void OV2640_PWDNState(uint8_t state);
void OV2640_Reset(uint8_t state);
uint16_t OV2640_Init(void);

#endif
