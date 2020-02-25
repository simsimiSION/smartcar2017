#ifndef _LCDSCREEN_H_
#define _LCDSCREEN_H_
#include "common.h"
#include "gpio.h"

#define LCD_PORT  HW_GPIOB
#define SCL_PIN   23
#define SDA_PIN   21
#define RST_PIN   17
#define DC_PIN    11
#define CS_PIN    9

#define SCL_LOW    GPIO_ResetBit(LCD_PORT, SCL_PIN) 
#define SCL_HIGH   GPIO_SetBit(LCD_PORT, SCL_PIN)

#define SDA_LOW    GPIO_ResetBit(LCD_PORT, SDA_PIN) 
#define SDA_HIGH   GPIO_SetBit(LCD_PORT, SDA_PIN)

#define RST_LOW    GPIO_ResetBit(LCD_PORT, RST_PIN)  
#define RST_HIGH   GPIO_SetBit(LCD_PORT, RST_PIN)    

#define DC_LOW     GPIO_ResetBit(LCD_PORT, DC_PIN) 
#define DC_HIGH    GPIO_SetBit(LCD_PORT, DC_PIN)

#define CS_LOW     GPIO_ResetBit(LCD_PORT, CS_PIN)
#define CS_HIGH    GPIO_SetBit(LCD_PORT, CS_PIN) 

#define X_WIDTH 128
#define Y_WIDTH 64

typedef unsigned short uint16;

class lcd{
public:
	lcd();
	
	void writeData(uint8 data);
	void writeCmd(uint8 cmd);
	void setXY(uint8 x, uint8 y);  //我们俩不一样
	void setPos(uint8 x, uint8 y); //我们俩不一样
	void delayMs(uint16 ms);
	void fill(uint8 bmp);
	void clear();
	void printPos(uint8 x, uint8 y);
	void printVerLine(uint8 x, uint8 y, uint8 number);
	void writeChar(uint8 ch);
	void writeInt(int num);
	void wtireString(char* str);
	void writeFloat(float num, int sig);
	int  getStringCode(char* str);	
	void writeChinese(uint8 x, uint8 y, char* str);
};

#endif