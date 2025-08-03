#ifndef Gui_H
#define Gui_H

#include "gd32f4xx.h"
#include "systick.h"


#define COLOR_GOLDEN   (0XFEA0)//��ɫ
#define COLOR_SILVER   (0XC618)//��ɫ
#define COLOR_BRONZE   (0XBB86)//��ͭ
#define COLOR_WHITE   (0xFFFF)
#define COLOR_BLACK   (0x0000)
#define COLOR_BLUE    (0x001F)
#define COLOR_BRED    (0XF81F)
#define COLOR_GRED    (0XFFE0)
#define COLOR_GBLUE   (0X07FF)
#define COLOR_RED     (0xF800)
#define COLOR_MAGENTA (0xF81F)
#define COLOR_GREEN   (0x07E0)
#define COLOR_CYAN    (0x7FFF)
#define COLOR_YELLOW  (0xFFE0)
#define COLOR_BROWN   (0XBC40)      //��ɫ
#define COLOR_BRRED   (0XFC07)      //�غ�ɫ
#define COLOR_GRAY    (0X8430)       //��ɫ
#define COLOR_DARKBLUE (0X01CF)   //����ɫ
#define COLOR_LIGHTBLUE (0X7D7C)  //ǳ��ɫ
#define COLOR_GRAYBLUE (0X5458)   //����ɫ
#define COLOR_LIGHTGREEN (0X841F) //ǳ��ɫ
#define COLOR_LGRAY (0XC618)      //ǳ��ɫ(PANNEL),���屳��ɫ
#define COLOR_LGRAYBLUE (0XA651)  //ǳ����ɫ(�м����ɫ)
#define COLOR_LBBLUE (0X2B12)     //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//ָ�����������ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//��ָ��λ�û�һ������
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ���ִ�
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����12x12����
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ����32x32����

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾһ���ַ�
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);//��ʾ�ַ���
uint32_t mypow(uint8_t m,uint8_t n);//����
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//��ʾͼƬ


void DrawEclipsePoint(uint16_t x0, uint16_t y0,uint16_t x,uint16_t y,uint16_t color);
void MiddlePiontEclipse(uint16_t x0,uint16_t y0,uint16_t a,uint16_t b,uint16_t color);
void Draw_Solid_Circle(uint16_t x, uint16_t y, uint16_t  r,uint16_t color);
void LCD_Show32Picture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint32_t pic[]);
void LCD_Show32DCI_Picture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint32_t pic[]);


#endif
