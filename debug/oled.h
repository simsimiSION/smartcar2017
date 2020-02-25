#ifndef _OELD_H
#define _OELD_H

#include "stdint.h"
//#include "Soft_I2C.h"

#define OLED_DO_OUT_LOW             GPIO_ResetBit(HW_GPIOB, 23) 
#define OLED_DO_OUT_HIGH             GPIO_SetBit(HW_GPIOB, 23)

#define OLED_DI_OUT_LOW             GPIO_ResetBit(HW_GPIOB, 21) 
#define OLED_DI_OUT_HIGH            GPIO_SetBit(HW_GPIOB, 21)   

#define OLED_RST_OUT_LOW            GPIO_ResetBit(HW_GPIOB, 17)  
#define OLED_RST_OUT_HIGH           GPIO_SetBit(HW_GPIOB, 17)    

#define OLED_DC_OUT_LOW             GPIO_ResetBit(HW_GPIOB, 11)  
#define OLED_DC_OUT_HIGH            GPIO_SetBit(HW_GPIOB, 11)    

#define OLED_CS_OUT_LOW             GPIO_ResetBit(HW_GPIOB, 9)  
#define OLED_CS_OUT_HIGH            GPIO_SetBit(HW_GPIOB, 9)     

typedef uint8_t u8;
typedef uint16_t u16;
/********************************************************************/
/*-----------------------------------------------------------------------
LCD_init          : 5110LCD��ʼ��

��д����          ��2012-11-01
����޸�����      ��2012-11-01
-----------------------------------------------------------------------*/
 extern u8 lanzhou96x64[768];
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(u8 x,u8 y,u8 ch[]);
 void LCD_P8x16Str(u8 x,u8 y,u8 ch[]);
 void LCD_P14x16Str(u8 x,u8 y,u8 ch[]);
 void LCD_Print(u8 x, u8 y, u8 ch[]);
 void LCD_PutPixel(u8 x,u8 y);
 void LCD_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]); 
 void LCD_Fill(u8 dat);
 void Dly_ms(u16 ms);
 void LCD_write_char(u8 ch);

 extern void LCD_WrDat(u8 data);
 void LCD_write_char(u8 ch);
 void LCD_write_int(int num);
 void LCD_write_english(char *s);
 void LCD_set_XY(u8 x, u8 y);
 void LCD_Set_Pos(u8 x, u8 y);
 void LCD_write_float( float num, uint8_t sig);
void OLED_init();
void LCD_clear();
void LCD_display_bit_image(uint8_t * image,uint8_t display_image_pos,uint8_t horizontal_length,uint8_t vertical_length);
/********************************************************************/

#endif
