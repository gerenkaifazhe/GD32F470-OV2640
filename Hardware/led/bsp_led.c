#include "bsp_led.h"


void led_gpio_config(void)
{
	/* ����GPIOD�Ķ˿�ʱ�� */
	BSP_RCU_AHB1EN |= (0x01 << 3);
/* ����GPIOD��ģʽ */
	BSP_GPIOD_CTL &= ~(0x03 << (2*7));
	BSP_GPIOD_CTL |= (0x01 << (2*7));
	
	BSP_GPIOD_PUD &= ~(0x03 << (2*7));
/* ����GPIO����� */
	BSP_GPIOD_OMODE &= ~(0x01 << 7);
	
	BSP_GPIOD_OSPD &= ~(0x03 << (2*7));
	BSP_GPIOD_OSPD |= (0x02 << (2*7));
}