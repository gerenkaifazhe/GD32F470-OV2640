#ifndef Lcd_H
#define Lcd_H

#include "gd32f4xx.h"
#include "systick.h"
#include "Spi4.h"

#define LCD_W 240
#define LCD_H 280

#define LCD_RST_RCU    RCU_GPIOA
#define LCD_RST_PORT   GPIOA
#define LCD_RST_PIN    GPIO_PIN_1

#define LCD_DC_RCU     RCU_GPIOC
#define LCD_DC_PORT    GPIOC
#define LCD_DC_PIN     GPIO_PIN_3

#define LCD_CS_RCU     RCU_GPIOA
#define LCD_CS_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_0

#define LCD_BLK_RCU    RCU_GPIOA
#define LCD_BLK_PORT   GPIOA
#define LCD_BLK_PIN    GPIO_PIN_2


#define LCD_RST_L  gpio_bit_reset(LCD_RST_PORT,LCD_RST_PIN)//RST  PF10
#define LCD_RST_H  gpio_bit_set(LCD_RST_PORT,LCD_RST_PIN)

#define LCD_DC_L   gpio_bit_reset(LCD_DC_PORT,LCD_DC_PIN)//DC  PA6
#define LCD_DC_H   gpio_bit_set(LCD_DC_PORT,LCD_DC_PIN)

#define LCD_CS_L   gpio_bit_reset(LCD_CS_PORT,LCD_CS_PIN)//CS  PA4
#define LCD_CS_H   gpio_bit_set(LCD_CS_PORT,LCD_CS_PIN)

#define LCD_BLK_L  gpio_bit_reset(LCD_BLK_PORT,LCD_BLK_PIN)//BLK  PA7
#define LCD_BLK_H  gpio_bit_set(LCD_BLK_PORT,LCD_BLK_PIN)


#define Lcd_SpiRead   Spi4_Read
#define Lcd_SpiWrite  Spi4_Write

typedef enum
{
    lcdOrientation_Portrait = 0x00,  //ÊúÆÁ
    lcdOrientation_APortrait = 0xC0, //ÊúÆÁ£¬ÊÖ»úµ¹¹ýÀ´
    lcdOrientation_Left = 0x70,      //ºáÆÁ£¬ÆÁÄ»ÔÚhome¼ü×ó±ß£¨±È½Ï³£ÓÃ£©
    lcdOrientation_Right = 0xA0      //ºáÆÁ£¬ÆÁÄ»ÔÚhome¼üÓÒ±ß
} LcdOrientation;




void Lcd_Init();
void Lcd_SetOrientation(LcdOrientation lcdOrientation);
uint16_t Lcd_GetW();
uint16_t Lcd_GetH();
void Lcd_PushStart(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

void Lcd_WriteData(uint8_t data);
void LCD_Writ_NewBus(uint8_t dat);
void Lcd_WriteData16(uint16_t data);
#endif
