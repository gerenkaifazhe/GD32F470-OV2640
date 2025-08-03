#include "OV2640.h"

#define				OV2640_PWDN_GPIO			GPIOC
#define				OV2640_PWDN_PIN				GPIO_PIN_9
#define				OV2640_RESET_GPIO			GPIOA
#define				OV2640_RESET_PIN			GPIO_PIN_8
#define				OV2640_Address				0x60


#define				OV2640_PWDN_BitSet(x)			gpio_bit_write(OV2640_PWDN_GPIO,OV2640_PWDN_PIN,x)
#define				OV2640_RESET_BitSet(x)			gpio_bit_write(OV2640_RESET_GPIO,OV2640_RESET_PIN,x)




void OV2640_GPIO_Pin_Init(void)
{
    My_DCI_Init();          //DCI引脚的初始化,IIC引脚未初始化

	rcu_periph_clock_enable(OV2640_PWDN_GPIO);
	rcu_periph_clock_enable(OV2640_RESET_GPIO);

	gpio_mode_set(OV2640_PWDN_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,OV2640_PWDN_PIN);
	gpio_output_options_set(OV2640_PWDN_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,OV2640_PWDN_PIN);

	gpio_mode_set(OV2640_RESET_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,OV2640_RESET_PIN);
	gpio_output_options_set(OV2640_RESET_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,OV2640_RESET_PIN);

}

void OV2640_PWDNState(uint8_t state)
{
	OV2640_PWDN_BitSet(state);
}

void OV2640_Reset(uint8_t state)
{
	OV2640_RESET_BitSet(state);
}

uint8_t OV2640_ReadRegister(uint8_t RegisterADDR)
{
	uint8_t Data = MyI2C0_ReadReg(OV2640_Address,RegisterADDR);

	return Data;
}

void OV2640_WriteRegister(uint8_t RegisterADDR,uint8_t Data)
{
	MyI2C0_WriteReg(OV2640_Address,RegisterADDR,Data);
}

uint8_t ov2640_outsize_set(uint16_t width,uint16_t height)
{
    uint16_t outh;
    uint16_t outw;
    uint8_t temp;
    if(width%4)return 0xFF;
    if(height%4)return 0xFF;
    outw=width/4;
    outh=height/4;
    OV2640_WriteRegister(0xFF,0x00);
    OV2640_WriteRegister(0xE0,0x04);
    OV2640_WriteRegister(0x5A,outw&0xFF);
    OV2640_WriteRegister(0x5B,outh&0xFF);
    temp=(outw>>8)&0x03;
    temp|=(outh>>6)&0x04;
    OV2640_WriteRegister(0x5C,temp);
    OV2640_WriteRegister(0xE0,0x00);
    return 0;
}

uint16_t OV2640_Init(void)
{
	uint16_t PMID = 0;
	uint8_t i = 0;
	OV2640_GPIO_Pin_Init();			//初始化引脚
	OV2640_Reset(0);			//复位
	OV2640_Reset(1);			//取消复位
	delay_1ms(1);
	MyI2C0_Init();

	OV2640_PWDNState(0);		//0:允许I2C访问

	PMID = OV2640_ReadRegister(0x1C);
	PMID = (PMID << 8) | OV2640_ReadRegister(0x1D);
	// PMID = (PMID << 8) | OV2640_ReadRegister(0x0A);
	// PMID = (PMID << 8) | OV2640_ReadRegister(0x0B);

	OV2640_WriteRegister(0xFF,0x01);		//配置Sensor address
	delay_1ms(10);
	for(i=0;i<sizeof(ov2640_svga_init_reg_tbl)/2;i++)
	{
		OV2640_WriteRegister(ov2640_svga_init_reg_tbl[i][0],ov2640_svga_init_reg_tbl[i][1]);
	}

	delay_1ms(100);
	for(i=0;i<(sizeof(ov2640_rgb565_reg_tbl)/2);i++)
	{
		OV2640_WriteRegister(ov2640_rgb565_reg_tbl[i][0],ov2640_rgb565_reg_tbl[i][1]);
	}

	delay_1ms(100);

		for(i=0;i<(sizeof(OV2640_30FPS)/2);i++)
	{
		OV2640_WriteRegister(OV2640_30FPS[i][0],OV2640_30FPS[i][1]);
	}

	delay_1ms(100);


	ov2640_outsize_set(240,280);

	return PMID;

}





