#include "DCI.h"

uint16_t ov_frameEND = 0;
uint8_t OV2640_Frame = 0;
uint8_t OV2640_FrameCount = 0;

/*
    OV2640引脚连接
    D0->PA9
    D1->PA10
    D2->PC8
    D3->PG11
    D4->PC11
    D5->PD3
    D6->PB8
    D7->PE6
    VSYNC->PB7
    HSYNC->PA4
    PIXCLK->PA6
    RESET->PA8
    PWDN->PC9
    SCL->PB6
    SDA->PB9
*/


void My_DCI_Init(void)
{
    rcu_periph_clock_enable(RCU_DCI);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOG);


    gpio_af_set(GPIOA,GPIO_AF_13,GPIO_PIN_9);
    gpio_af_set(GPIOA,GPIO_AF_13,GPIO_PIN_10);
    gpio_af_set(GPIOB,GPIO_AF_13,GPIO_PIN_8);
    gpio_af_set(GPIOC,GPIO_AF_13,GPIO_PIN_8);
    gpio_af_set(GPIOC,GPIO_AF_13,GPIO_PIN_11);
    gpio_af_set(GPIOD,GPIO_AF_13,GPIO_PIN_3);
    gpio_af_set(GPIOE,GPIO_AF_13,GPIO_PIN_6);
    gpio_af_set(GPIOG,GPIO_AF_13,GPIO_PIN_11);

    gpio_af_set(GPIOB,GPIO_AF_13,GPIO_PIN_7);           //VSYNC
    gpio_af_set(GPIOA,GPIO_AF_13,GPIO_PIN_4);           //HSYNC
    gpio_af_set(GPIOA,GPIO_AF_13,GPIO_PIN_6);           //PIXCLK

    gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_9);
    gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_10);
    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_8);
    gpio_mode_set(GPIOC,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_8);
    gpio_mode_set(GPIOC,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_11);
    gpio_mode_set(GPIOD,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_3);
    gpio_mode_set(GPIOE,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_6);
    gpio_mode_set(GPIOG,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_11);

    gpio_mode_set(GPIOB,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_7);
    gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_4);
    gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO_PIN_6);

    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_9);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_10);
    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_8);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_8);
    gpio_output_options_set(GPIOC,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_11);
    gpio_output_options_set(GPIOD,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_3);
    gpio_output_options_set(GPIOE,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_6);
    gpio_output_options_set(GPIOG,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_11);

    gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_7);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_4);
    gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_6);

    dci_deinit();

    dci_parameter_struct dci_init_struct;
    dci_init_struct.capture_mode = DCI_CAPTURE_MODE_CONTINUOUS;
    dci_init_struct.clock_polarity = DCI_CK_POLARITY_RISING;
    dci_init_struct.hsync_polarity = DCI_HSYNC_POLARITY_LOW;
    dci_init_struct.vsync_polarity = DCI_VSYNC_POLARITY_LOW;
    dci_init_struct.frame_rate = DCI_FRAME_RATE_ALL;
    dci_init_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS;

    nvic_irq_enable(DCI_IRQn,1,1);

    dci_interrupt_enable(DCI_INT_EF);

    dci_init(&dci_init_struct);
}



void DCI_IRQHandler(void)
{
    if( dci_interrupt_flag_get(DCI_INT_FLAG_EF) == SET )        //帧结束中断
    {
        // dci_disable();
        dci_interrupt_flag_clear(DCI_INT_EF);
        OV2640_FrameCount++;
        ov_frameEND = 1;
    }
}
