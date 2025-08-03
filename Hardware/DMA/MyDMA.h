#ifndef MyDMA_H
#define MyDMA_H

extern uint8_t TransData;
extern uint8_t ReceiveData;

extern uint8_t Finish;
extern uint8_t State;
extern uint32_t Temp;
extern uint8_t TransFinish_Display;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB);
void MyDMA_IRQHandler_Init(uint32_t AddrA, uint32_t AddrB, uint32_t Number);
void MyDMA_DCI_IRQHandler_Init(uint32_t AddrB, uint32_t Number);
void MyDMA_SPI_IRQHandler_Init(uint32_t AddrB, uint32_t Number);
void MyDMA_Transfer(void);
void MyDMA_IRQHandler_Transfer(void);
void MyDMA_DCI_IRQHandler_Transfer(void);
void MyDMA_SPI_IRQHandler_Transfer(void);


#endif
