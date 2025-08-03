#include "TIMER.h"

uint8_t FrameState = 0;

uint16_t FrameCount = 0;

void Timer_Init(void)
{
    rcu_periph_clock_enable(RCU_TIMER9);
    // rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);
    // rcu_system_clock_source_config(RCU_CKSYSSRC_HXTAL);

    timer_parameter_struct timer9_parameter_structure;
    timer9_parameter_structure.prescaler = 20000;
    timer9_parameter_structure.alignedmode = TIMER_COUNTER_CENTER_UP;
    timer9_parameter_structure.counterdirection = TIMER_COUNTER_UP;
    timer9_parameter_structure.period = 10000;
    timer9_parameter_structure.clockdivision = TIMER_CKDIV_DIV1;
    timer9_parameter_structure.repetitioncounter = 0;
    timer_init(TIMER9,&timer9_parameter_structure);

    nvic_irq_enable(TIMER0_UP_TIMER9_IRQn,2,3);
    timer_interrupt_enable(TIMER9,TIMER_INT_UP);

    timer_enable(TIMER9);
}

void TIMER0_UP_TIMER9_IRQHandler(void)
{
    if(timer_interrupt_flag_get(TIMER9,TIMER_INT_UP) != RESET)
    {
        FrameState++;
        OV2640_Frame = OV2640_FrameCount;
        OV2640_FrameCount = 0;
        timer_interrupt_flag_clear(TIMER9,TIMER_INT_UP);
    }
}

