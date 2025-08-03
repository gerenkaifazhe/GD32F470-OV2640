// #define COLOR_BLUE    (0x001F)
// #define LCD_W 240
// #define LCD_H 280

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"
#include "Lcd.h"
#include "Gui.h"
#include "MyDMA.h"
#include "pic.h"
#include "DCI.h"
#include "MyIIC.h"
#include "TIMER.h"

uint32_t Pic_Image[33600];
uint8_t Pic_Image8[134400];

int main(void)
{

    systick_config();
    Lcd_Init();
    OV2640_Init();
    My_DCI_Init();
    LCD_Fill(0,0,LCD_W,LCD_H,COLOR_DARKBLUE);

    MyDMA_DCI_IRQHandler_Init((uint32_t)Pic_Image,33600);
    MyDMA_DCI_IRQHandler_Transfer();

    dci_enable();
    dci_capture_enable();

    Timer_Init();

    while(1)
	{
        switch (ov_frameEND)
        {
        case 1:
            LCD_Show32DCI_Picture(0,0,240,280,Pic_Image);
            LCD_ShowIntNum(1,64,(OV2640_Frame*1),2,COLOR_WHITE,COLOR_BROWN,32);
            ov_frameEND = 0;
            // dci_enable();
            break;
        default:
            break;
        }
    }
}
