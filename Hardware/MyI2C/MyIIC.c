#include "MyIIC.h"

#define			MyI2C0_SCL_RCUGPIO		RCU_GPIOB
#define			MyI2C0_SDA_RCUGPIO		RCU_GPIOB

#define			MyI2C0_SCL_GPIO			GPIOB
#define			MyI2C0_SDA_GPIO			GPIOB
#define			MyI2C0_SCL_PIN			GPIO_PIN_6
#define			MyI2C0_SDA_PIN			GPIO_PIN_9

#define			Operate_Address			OV2640_Address
#define			OV2640_Address			0x60


void MyI2C0_Init(void)
{
	rcu_periph_clock_enable(MyI2C0_SCL_RCUGPIO);
	rcu_periph_clock_enable(MyI2C0_SDA_RCUGPIO);
	rcu_periph_clock_enable(RCU_I2C0);

	gpio_af_set(MyI2C0_SCL_GPIO,GPIO_AF_4,MyI2C0_SCL_PIN);
	gpio_af_set(MyI2C0_SDA_GPIO,GPIO_AF_4,MyI2C0_SDA_PIN);

	gpio_mode_set(MyI2C0_SCL_GPIO,GPIO_MODE_AF,GPIO_PUPD_NONE,MyI2C0_SCL_PIN);
	gpio_mode_set(MyI2C0_SDA_GPIO,GPIO_MODE_AF,GPIO_PUPD_NONE,MyI2C0_SDA_PIN);

	gpio_output_options_set(MyI2C0_SCL_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,MyI2C0_SCL_PIN);
	gpio_output_options_set(MyI2C0_SDA_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,MyI2C0_SDA_PIN);

	i2c_deinit(I2C0);
	i2c_clock_config(I2C0,100000, I2C_DTCY_2);
	i2c_mode_addr_config(I2C0,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,Operate_Address);

	i2c_enable(I2C0);
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);

}


uint8_t MyI2C0_ReadReg(uint8_t SlaveADDR, uint8_t RegisterADDR)
{
	i2c_start_on_bus(I2C0);										//发送起始信号
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//等待起始信号发送完成，等待接收方接收到地址

    i2c_master_addressing(I2C0, SlaveADDR, I2C_TRANSMITTER);		//发送器件地址
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);			//等待地址发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//地址发送完成后，I2C_FLAG_ADDSEND标志位置1，清除I2C_FLAG_ADDSEND标志位

	i2c_data_transmit(I2C0,RegisterADDR);						//发送寄存器地址
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//等待数据发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//数据发送完成后，I2C_FLAG_TBE标志位置1，清除I2C_FLAG_TBE标志位

	i2c_start_on_bus(I2C0);										//重新发送起始信号
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//等待起始信号发送完成，等待接收方接收到地址

	i2c_master_addressing(I2C0, SlaveADDR, I2C_RECEIVER);		//发送器件地址
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);		//等待地址发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//地址发送完成后，I2C_FLAG_ADDSEND标志位置1，清除I2C_FLAG_ADDSEND标志位

    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));					//等待字节发送完成
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);					//不发送应答

    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		//非真时循环，就是I2C_DATA寄存器为空时循环，（I2C_FLAG_RBNE为真时，I2C_DATA寄存器有数据）
													//等待数据接收完成

    uint8_t Temp = i2c_data_receive(I2C0);			//接收数据

    i2c_stop_on_bus(I2C0);							//发送停止信号
    while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);			//等待停止信号
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);			//使能应答

	return Temp;

}

void MyI2C0_WriteReg(uint8_t SlaveADDR, uint8_t RegisterADDR, uint8_t Data)
{
	i2c_start_on_bus(I2C0);										//发送起始信号
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//等待起始信号发送完成，等待接收方接收到地址

    i2c_master_addressing(I2C0, SlaveADDR, I2C_TRANSMITTER);		//发送器件地址
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);			//等待地址发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//地址发送完成后，I2C_FLAG_ADDSEND标志位置1，清除I2C_FLAG_ADDSEND标志位

	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	i2c_data_transmit(I2C0, RegisterADDR);						//发送寄存器地址
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//等待数据发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//数据发送完成后，I2C_FLAG_TBE标志位置1，清除I2C_FLAG_TBE标志位

	i2c_data_transmit(I2C0, Data);						//发送寄存器地址
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//等待数据发送完成
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//数据发送完成后，I2C_FLAG_TBE标志位置1，清除I2C_FLAG_TBE标志位

    i2c_stop_on_bus(I2C0);
    while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);

}


