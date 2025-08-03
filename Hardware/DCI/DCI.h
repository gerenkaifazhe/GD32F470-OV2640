#ifndef DCI_H
#define DCI_H

#include "gd32f4xx.h"
#include "gd32f4xx_dci.h"
#include "TIMER.h"
#include "main.h"

extern uint16_t ov_frameEND;
extern uint8_t OV2640_FrameCount;
extern uint8_t OV2640_Frame;

void My_DCI_Init(void);

#endif
