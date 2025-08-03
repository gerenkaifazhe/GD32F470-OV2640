#include "Lcd.h"

LcdOrientation lcd_Orientation = lcdOrientation_Portrait;

void delay(int t)
{
	while(t--);
}

void LCD_Writ_Bus(uint8_t dat)
{
	  LCD_CS_L;
    Lcd_SpiWrite(dat);
	  delay(4);
    LCD_CS_H;
}

void LCD_Writ_NewBus(uint8_t dat)
{
	  LCD_CS_L;
    // Spi4_NewWrite(dat);
    REG32((SPI4) + 0x0CU) = (uint32_t)dat;
    while ((SPI_STAT(SPI4) & SPI_FLAG_TBE) == 0);
	  delay(1);
    LCD_CS_H;
}

void Lcd_WriteData(uint8_t data)
{
    LCD_Writ_Bus(data);

}
void Lcd_WriteData16(uint16_t data)
{
    LCD_Writ_Bus(data >> 8);
    LCD_Writ_Bus(data);
}
void Lcd_WriteCom(uint8_t com)
{
    LCD_DC_L;
    LCD_Writ_Bus(com);
    LCD_DC_H;
}

uint8_t Lcd_Read()
{
    return Lcd_SpiRead();
}

static void lcd_gpio_init(void)
{
  	rcu_periph_clock_enable(LCD_RST_RCU);
	  gpio_mode_set(LCD_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_RST_PIN);
	  gpio_output_options_set(LCD_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_RST_PIN);

	  rcu_periph_clock_enable(LCD_DC_RCU);
	  gpio_mode_set(LCD_DC_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_DC_PIN);
	  gpio_output_options_set(LCD_DC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_DC_PIN);

	  rcu_periph_clock_enable(LCD_CS_RCU);
	  gpio_mode_set(LCD_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS_PIN);
	  gpio_output_options_set(LCD_CS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_CS_PIN);

	  rcu_periph_clock_enable(LCD_BLK_RCU);
	  gpio_mode_set(LCD_BLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_BLK_PIN);
	  gpio_output_options_set(LCD_BLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LCD_BLK_PIN);

}

void Lcd_Init()
{
    Spi4_Init();

    lcd_gpio_init();

    LCD_RST_L; //复位
    delay_1ms(100);
    LCD_RST_H;
    delay_1ms(100);
    LCD_BLK_H; //打开背光

    Lcd_WriteCom(0x11); // Sleep out
    delay_1ms(120);      // Delay 120ms

    Lcd_SetOrientation(lcdOrientation_Portrait);

    Lcd_WriteCom(0x3A);
    Lcd_WriteData(0x05);

    Lcd_WriteCom(0xB2);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x33);

    Lcd_WriteCom(0xB7);
    Lcd_WriteData(0x35);

    Lcd_WriteCom(0xBB);
    Lcd_WriteData(0x32); // Vcom=1.35V

    Lcd_WriteCom(0xC2);
    Lcd_WriteData(0x01);

    Lcd_WriteCom(0xC3);
    Lcd_WriteData(0x15); // GVDD=4.8V  颜色深度

    Lcd_WriteCom(0xC4);
    Lcd_WriteData(0x20); // VDV, 0x20:0v

    Lcd_WriteCom(0xC6);
    Lcd_WriteData(0x0F); // 0x0F:60Hz

    Lcd_WriteCom(0xD0);
    Lcd_WriteData(0xA4);
    Lcd_WriteData(0xA1);

    Lcd_WriteCom(0xE0);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x0E);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x05);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x48);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x34);

    Lcd_WriteCom(0xE1);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x08);
    Lcd_WriteData(0x0E);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x09);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x48);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x14);
    Lcd_WriteData(0x15);
    Lcd_WriteData(0x31);
    Lcd_WriteData(0x34);
    Lcd_WriteCom(0x21);

    Lcd_WriteCom(0x29);

    delay_1ms(120); // Delay 120ms
}

/**
 * @brief  Lcd_SetOrientation: 设置LCD屏幕的视角方向
 * @param [in] lcdOrientation: 视角方向
 * @return void
 */

void Lcd_SetOrientation(LcdOrientation lcdOrientation)
{
    lcd_Orientation = lcdOrientation;
    Lcd_WriteCom(0x36);
    Lcd_WriteData(lcd_Orientation);
}

/**
 * @brief  Lcd_GetW: 得到当前视角的屏幕宽度
 * @return      uint16_t: 得到屏幕宽度的像素值
 */
uint16_t Lcd_GetW()
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        return LCD_W;
    }
    return LCD_H;
}
/**
 * @brief  Lcd_GetW: 得到当前视角的屏幕宽度
 * @return      uint16_t: 得到屏幕高度的像素值
 */
uint16_t Lcd_GetH()
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        return LCD_H;
    }
    return LCD_W;
}

/**
 * @brief Lcd_PushStart: 开始写入屏幕数据
 * @param        [in] x: 写入屏幕数据的起始x坐标
 * @param        [in] y: 起始y坐标
 * @param        [in] w: 宽度
 * @param        [in] h: 高度
 * @return void
 */
void Lcd_PushStart(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (lcd_Orientation == lcdOrientation_Portrait || lcd_Orientation == lcdOrientation_APortrait)
    {
        y1 += 20;
			  y2 += 20;
    }
    else if (lcd_Orientation == lcdOrientation_Left || lcd_Orientation == lcdOrientation_Right)
    {
        x1 += 20;
			  x2 += 20;
    }

    Lcd_WriteCom(0x2a); //列地址设置
    Lcd_WriteData16(x1);
    Lcd_WriteData16(x2);
    Lcd_WriteCom(0x2b); //行地址设置
    Lcd_WriteData16(y1);
    Lcd_WriteData16(y2);
    Lcd_WriteCom(0x2c); //储存器写
}
