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
	i2c_start_on_bus(I2C0);										//������ʼ�ź�
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//�ȴ���ʼ�źŷ�����ɣ��ȴ����շ����յ���ַ

    i2c_master_addressing(I2C0, SlaveADDR, I2C_TRANSMITTER);		//����������ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);			//�ȴ���ַ�������
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//��ַ������ɺ�I2C_FLAG_ADDSEND��־λ��1�����I2C_FLAG_ADDSEND��־λ

	i2c_data_transmit(I2C0,RegisterADDR);						//���ͼĴ�����ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//�ȴ����ݷ������
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//���ݷ�����ɺ�I2C_FLAG_TBE��־λ��1�����I2C_FLAG_TBE��־λ

	i2c_start_on_bus(I2C0);										//���·�����ʼ�ź�
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//�ȴ���ʼ�źŷ�����ɣ��ȴ����շ����յ���ַ

	i2c_master_addressing(I2C0, SlaveADDR, I2C_RECEIVER);		//����������ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);		//�ȴ���ַ�������
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//��ַ������ɺ�I2C_FLAG_ADDSEND��־λ��1�����I2C_FLAG_ADDSEND��־λ

    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));					//�ȴ��ֽڷ������
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);					//������Ӧ��

    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));		//����ʱѭ��������I2C_DATA�Ĵ���Ϊ��ʱѭ������I2C_FLAG_RBNEΪ��ʱ��I2C_DATA�Ĵ��������ݣ�
													//�ȴ����ݽ������

    uint8_t Temp = i2c_data_receive(I2C0);			//��������

    i2c_stop_on_bus(I2C0);							//����ֹͣ�ź�
    while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);			//�ȴ�ֹͣ�ź�
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);			//ʹ��Ӧ��

	return Temp;

}

void MyI2C0_WriteReg(uint8_t SlaveADDR, uint8_t RegisterADDR, uint8_t Data)
{
	i2c_start_on_bus(I2C0);										//������ʼ�ź�
    while(i2c_flag_get(I2C0,I2C_FLAG_SBSEND) == RESET);			//�ȴ���ʼ�źŷ�����ɣ��ȴ����շ����յ���ַ

    i2c_master_addressing(I2C0, SlaveADDR, I2C_TRANSMITTER);		//����������ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_ADDSEND) == RESET);			//�ȴ���ַ�������
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);						//��ַ������ɺ�I2C_FLAG_ADDSEND��־λ��1�����I2C_FLAG_ADDSEND��־λ

	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	i2c_data_transmit(I2C0, RegisterADDR);						//���ͼĴ�����ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//�ȴ����ݷ������
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//���ݷ�����ɺ�I2C_FLAG_TBE��־λ��1�����I2C_FLAG_TBE��־λ

	i2c_data_transmit(I2C0, Data);						//���ͼĴ�����ַ
    while(i2c_flag_get(I2C0, I2C_FLAG_TBE) == RESET);			//�ȴ����ݷ������
    i2c_flag_clear(I2C0, I2C_FLAG_TBE);						//���ݷ�����ɺ�I2C_FLAG_TBE��־λ��1�����I2C_FLAG_TBE��־λ

    i2c_stop_on_bus(I2C0);
    while(I2C_CTL0(I2C0) & I2C_CTL0_STOP);

}


