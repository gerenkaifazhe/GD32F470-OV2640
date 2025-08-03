#ifndef TIMER_H
#define TIMER_H

#include "gd32f4xx.h"
#include "DCI.h"

extern uint8_t FrameState;
extern uint16_t FrameCount;

void Timer_Init(void);

#endif
