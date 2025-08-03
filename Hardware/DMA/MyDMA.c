#include "gd32f4xx.h"                   // Device header
#include "MyDMA.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_dci.h"

#define USART_RECEIVE_LENGTH  1
#define DCI_DR_ADDRESS     (0x50050028U)
#define SPI_DR_ADDRESS     (0x4001500CU)

uint8_t TransFinish_Display = 0;
uint32_t MyDMA_IRQHandler_Number = 0;

uint32_t Temp = 0;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB)
{
    // rcu_deinit();
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1,DMA_CH0);
    dma_channel_disable(DMA1,DMA_CH0);							//ʹCHENΪ0������1
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FTF);					//���FTFIE0
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FEE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_SDE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_TAE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_HTF);

    dma_multi_data_parameter_struct DMA_multihannel_InitStruct;
    DMA_multihannel_InitStruct.periph_addr = AddrA;
    DMA_multihannel_InitStruct.periph_width = DMA_PERIPH_WIDTH_32BIT;		//�����ȡ�����5
    DMA_multihannel_InitStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;	//�����ַ�����㷨������5
    DMA_multihannel_InitStruct.memory0_addr = AddrB;
    DMA_multihannel_InitStruct.memory_width = DMA_MEMORY_WIDTH_32BIT;		//�洢����ȡ�����5
    DMA_multihannel_InitStruct.memory_inc = DMA_MEMORY_INCREASE_DISABLE;	//�洢����ַ�����㷨������5

    DMA_multihannel_InitStruct.memory_burst_width = DMA_MEMORY_BURST_SINGLE;		//���ô洢��ͻ�����͡�����5
    DMA_multihannel_InitStruct.periph_burst_width = DMA_PERIPH_BURST_SINGLE;		//��������ͻ�����͡�����5
    DMA_multihannel_InitStruct.critical_value = DMA_FIFO_1_WORD;
    DMA_multihannel_InitStruct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;		//����ѭ��ģʽ������5
    DMA_multihannel_InitStruct.direction = DMA_MEMORY_TO_MEMORY;					//TMλ�����ݴ��䷽ʽ������3
    DMA_multihannel_InitStruct.number = 1;								//��10��
    DMA_multihannel_InitStruct.priority = DMA_PRIORITY_ULTRA_HIGH;

	dma_switch_buffer_mode_enable(DMA1,DMA_CH0,DISABLE);					//SBMENλ����ʹ�ô洢�л�ģʽ������5
    dma_switch_buffer_mode_config(DMA1,DMA_CH0,AddrB,DMA_MEMORY_0);			//MBSλ��������ѡ�񡣲���5
    nvic_irq_enable(DMA1_Channel0_IRQn,1,1);		//����ͨ�����ȼ�������5
    dma_flow_controller_config(DMA1,DMA_CH0,DMA_FLOW_CONTROLLER_DMA);		//TFCSλ�����ô�������������5

    dma_periph_address_config(DMA1,DMA_CH0,AddrB);					//��8��
    dma_memory_address_config(DMA1,DMA_CH0,DMA_MEMORY_0,AddrB);		//��9��
    dma_multi_data_mode_init(DMA1,DMA_CH0,&DMA_multihannel_InitStruct);		//��ʼ��DMA�������ݴ���ģʽ
}

void MyDMA_IRQHandler_Init(uint32_t AddrA, uint32_t AddrB, uint32_t Number)
{
    // rcu_deinit();
    MyDMA_IRQHandler_Number = Number;
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1,DMA_CH0);
    dma_channel_disable(DMA1,DMA_CH0);							//ʹCHENΪ0������1
    dma_interrupt_disable(DMA1,DMA_CH0,DMA_CHXCTL_FTFIE);
    dma_interrupt_flag_clear(DMA1,DMA_CH0,DMA_INT_FLAG_FTF);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FTF);					//���FTFIE0
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FEE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_SDE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_TAE);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_HTF);

    dma_multi_data_parameter_struct DMA_multihannel_InitStruct;
    DMA_multihannel_InitStruct.periph_addr = AddrA;
    DMA_multihannel_InitStruct.periph_width = DMA_PERIPH_WIDTH_32BIT;		//�����ȡ�����5
    DMA_multihannel_InitStruct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;	//�����ַ�����㷨������5
    DMA_multihannel_InitStruct.memory0_addr = AddrB;
    DMA_multihannel_InitStruct.memory_width = DMA_MEMORY_WIDTH_32BIT;		//�洢����ȡ�����5
    DMA_multihannel_InitStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;	//�洢����ַ�����㷨������5

    DMA_multihannel_InitStruct.memory_burst_width = DMA_MEMORY_BURST_SINGLE;		//���ô洢��ͻ�����͡�����5
    DMA_multihannel_InitStruct.periph_burst_width = DMA_PERIPH_BURST_SINGLE;		//��������ͻ�����͡�����5
    DMA_multihannel_InitStruct.critical_value = DMA_FIFO_4_WORD;
    DMA_multihannel_InitStruct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;		//����ѭ��ģʽ������5
    DMA_multihannel_InitStruct.direction = DMA_MEMORY_TO_MEMORY;					//TMλ�����ݴ��䷽ʽ������3
    DMA_multihannel_InitStruct.number = Number;								//��10��

    DMA_multihannel_InitStruct.priority = DMA_PRIORITY_ULTRA_HIGH;

	dma_switch_buffer_mode_enable(DMA1,DMA_CH0,DISABLE);					//SBMENλ����ʹ�ô洢�л�ģʽ������5
    dma_switch_buffer_mode_config(DMA1,DMA_CH0,AddrB,DMA_MEMORY_0);			//MBSλ��������ѡ�񡣲���5
    nvic_irq_enable(DMA1_Channel0_IRQn,2,2);		//����ͨ�����ȼ�������5
    dma_flow_controller_config(DMA1,DMA_CH0,DMA_FLOW_CONTROLLER_DMA);		//TFCSλ�����ô�������������5

	dma_interrupt_enable(DMA1,DMA_CH0,DMA_CHXCTL_FTFIE);

	dma_periph_address_config(DMA1,DMA_CH0,AddrB);					//��8��
    dma_memory_address_config(DMA1,DMA_CH0,DMA_MEMORY_0,AddrB);		//��9��
    dma_multi_data_mode_init(DMA1,DMA_CH0,&DMA_multihannel_InitStruct);		//��ʼ��DMA�������ݴ���ģʽ
}

void MyDMA_DCI_IRQHandler_Init(uint32_t AddrB, uint32_t Number)
{
    MyDMA_IRQHandler_Number = Number;
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1,DMA_CH1);
    dma_channel_disable(DMA1,DMA_CH1);							//ʹCHENΪ0������1
    dma_interrupt_disable(DMA1,DMA_CH1,DMA_CHXCTL_FTFIE);
    dma_interrupt_flag_clear(DMA1,DMA_CH1,DMA_INT_FLAG_FTF);
    dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_FTF);					//���FTFIE0
    dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_FEE);
    dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_SDE);
    dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_TAE);
    dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_HTF);

    dma_multi_data_parameter_struct DMA_multihannel_InitStruct;
    DMA_multihannel_InitStruct.periph_addr = (uint32_t)DCI_DR_ADDRESS;
    DMA_multihannel_InitStruct.periph_width = DMA_PERIPH_WIDTH_32BIT;		//�����ȡ�����5
    DMA_multihannel_InitStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;	//�����ַ�����㷨������5
    DMA_multihannel_InitStruct.memory0_addr = AddrB;
    DMA_multihannel_InitStruct.memory_width = DMA_MEMORY_WIDTH_32BIT;		//�洢����ȡ�����5
    DMA_multihannel_InitStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;	//�洢����ַ�����㷨������5

    DMA_multihannel_InitStruct.memory_burst_width = DMA_MEMORY_BURST_SINGLE;		//���ô洢��ͻ�����͡�����5
    DMA_multihannel_InitStruct.periph_burst_width = DMA_PERIPH_BURST_SINGLE;		//��������ͻ�����͡�����5
    DMA_multihannel_InitStruct.critical_value = DMA_FIFO_4_WORD;
    DMA_multihannel_InitStruct.circular_mode = DMA_CIRCULAR_MODE_ENABLE;		//����ѭ��ģʽ������5
    DMA_multihannel_InitStruct.direction = DMA_PERIPH_TO_MEMORY;					//TMλ�����ݴ��䷽ʽ������3
    DMA_multihannel_InitStruct.number = Number;								//��10��

    DMA_multihannel_InitStruct.priority = DMA_PRIORITY_ULTRA_HIGH;

	dma_switch_buffer_mode_enable(DMA1,DMA_CH1,DISABLE);					//SBMENλ����ʹ�ô洢�л�ģʽ������5
    dma_switch_buffer_mode_config(DMA1,DMA_CH1,AddrB,DMA_MEMORY_0);			//MBSλ��������ѡ�񡣲���5
    dma_flow_controller_config(DMA1,DMA_CH1,DMA_FLOW_CONTROLLER_DMA);		//TFCSλ�����ô�������������5
    dma_channel_subperipheral_select(DMA1, DMA_CH1, DMA_SUBPERI1);

	// dma_periph_address_config(DMA1,DMA_CH1,AddrB);					//��8��
    // dma_memory_address_config(DMA1,DMA_CH1,DMA_MEMORY_0,AddrB);		//��9��
    dma_multi_data_mode_init(DMA1,DMA_CH1,&DMA_multihannel_InitStruct);		//��ʼ��DMA�������ݴ���ģʽ
}

void MyDMA_SPI_IRQHandler_Init(uint32_t AddrB, uint32_t Number)
{
    MyDMA_IRQHandler_Number = Number;
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1,DMA_CH6);
    dma_channel_disable(DMA1,DMA_CH6);							//ʹCHENΪ0������1
    dma_interrupt_disable(DMA1,DMA_CH6,DMA_CHXCTL_FTFIE);
    dma_interrupt_flag_clear(DMA1,DMA_CH6,DMA_INT_FLAG_FTF);
    dma_flag_clear(DMA1,DMA_CH6,DMA_FLAG_FTF);					//���FTFIE0
    dma_flag_clear(DMA1,DMA_CH6,DMA_FLAG_FEE);
    dma_flag_clear(DMA1,DMA_CH6,DMA_FLAG_SDE);
    dma_flag_clear(DMA1,DMA_CH6,DMA_FLAG_TAE);
    dma_flag_clear(DMA1,DMA_CH6,DMA_FLAG_HTF);

    dma_multi_data_parameter_struct DMA_multihannel_InitStruct;
    DMA_multihannel_InitStruct.periph_addr = (uint32_t)SPI_DR_ADDRESS;
    DMA_multihannel_InitStruct.periph_width = DMA_PERIPH_WIDTH_16BIT;		//�����ȡ�����5
    DMA_multihannel_InitStruct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;	//�����ַ�����㷨������5
    DMA_multihannel_InitStruct.memory0_addr = AddrB;
    DMA_multihannel_InitStruct.memory_width = DMA_MEMORY_WIDTH_32BIT;		//�洢����ȡ�����5
    DMA_multihannel_InitStruct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;	//�洢����ַ�����㷨������5

    DMA_multihannel_InitStruct.memory_burst_width = DMA_MEMORY_BURST_SINGLE;		//���ô洢��ͻ�����͡�����5
    DMA_multihannel_InitStruct.periph_burst_width = DMA_PERIPH_BURST_SINGLE;		//��������ͻ�����͡�����5
    DMA_multihannel_InitStruct.critical_value = DMA_FIFO_4_WORD;
    DMA_multihannel_InitStruct.circular_mode = DMA_CIRCULAR_MODE_ENABLE;		//����ѭ��ģʽ������5
    DMA_multihannel_InitStruct.direction = DMA_MEMORY_TO_PERIPH;					//TMλ�����ݴ��䷽ʽ������3
    DMA_multihannel_InitStruct.number = Number;								//��10��

    DMA_multihannel_InitStruct.priority = DMA_PRIORITY_HIGH;

	dma_switch_buffer_mode_enable(DMA1,DMA_CH6,DISABLE);					//SBMENλ����ʹ�ô洢�л�ģʽ������5
    dma_switch_buffer_mode_config(DMA1,DMA_CH6,AddrB,DMA_MEMORY_0);			//MBSλ��������ѡ�񡣲���5
    dma_flow_controller_config(DMA1,DMA_CH6,DMA_FLOW_CONTROLLER_DMA);		//TFCSλ�����ô�������������5
    dma_channel_subperipheral_select(DMA1, DMA_CH6, DMA_SUBPERI7);
    spi_dma_enable(SPI4,SPI_DMA_TRANSMIT);

    dma_multi_data_mode_init(DMA1,DMA_CH6,&DMA_multihannel_InitStruct);		//��ʼ��DMA�������ݴ���ģʽ
}

void MyDMA_Transfer(void)
{
    dma_channel_enable(DMA1,DMA_CH0);
    while(dma_flag_get(DMA1,DMA_CH0,DMA_FLAG_FTF) == RESET);
    // LCD_ShowIntNum(1,128,5,2,COLOR_WHITE,COLOR_LGRAYBLUE,32);
    dma_channel_disable(DMA1,DMA_CH0);
    dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FTF);
    dma_transfer_number_config(DMA1,DMA_CH0,2);
}

void MyDMA_IRQHandler_Transfer(void)
{
    dma_channel_enable(DMA1,DMA_CH0);
}

void MyDMA_DCI_IRQHandler_Transfer(void)
{
    dma_channel_enable(DMA1,DMA_CH1);
}
void MyDMA_SPI_IRQHandler_Transfer(void)
{
    dma_channel_enable(DMA1,DMA_CH6);
}

void DMA1_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH0,DMA_INT_FLAG_FTF) == SET)
    {
        Temp = dma_transfer_number_get(DMA1,DMA_CH0);
        dma_channel_disable(DMA1,DMA_CH0);
        dma_flag_clear(DMA1,DMA_CH0,DMA_FLAG_FTF);
        dma_transfer_number_config(DMA1,DMA_CH0,MyDMA_IRQHandler_Number);
        dma_interrupt_flag_clear(DMA1,DMA_CH0,DMA_INT_FLAG_FTF);
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH1,DMA_INT_FLAG_FTF) == SET)
    {
        Temp = dma_transfer_number_get(DMA1,DMA_CH1);
        dma_channel_disable(DMA1,DMA_CH1);
        dma_flag_clear(DMA1,DMA_CH1,DMA_FLAG_FTF);
        dma_transfer_number_config(DMA1,DMA_CH1,MyDMA_IRQHandler_Number);

        TransFinish_Display = 1;
        dma_interrupt_flag_clear(DMA1,DMA_CH1,DMA_INT_FLAG_FTF);
    }
}
