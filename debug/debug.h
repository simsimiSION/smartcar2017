#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "stdio.h"
#include "assert.h"
#include "common.h"
#include "gpio.h"

#define BLACK   255
#define WHITE     0

#define PRINT(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)

/*LED*/
#define LED_COUNT    0
#define LED_PORT_BASE    {NULL, NULL, NULL}
#define LED_PIN_BASE     {NULL, NULL, NULL}
static PORT_Type * const LED_PORT[] = LED_PORT_BASE;
static int * const LED_PIN[]  = LED_PIN_BASE;

/*BUZZER*/
#define BUZZER_PORT HW_GPIOC
#define BUZZER_PIN  5

/*UART*/
#define UARTINIT     UART4_RX_PC14_TX_PC15
#define UARTDEBUG    HW_UART4

class debug{
public:
	
	int isLed[3];
	int isBuzzer;
	
	debug();
	~debug();
	/*system*/
	void print(char *str);
	void assertDebug(int expression);
	void delayUs(uint32_t us);
	void delayMs(uint32_t ms);
	/*image*/
	void pictureTurnColor(int ver, int hor, int color);
	void pictureClear(int color);
	void showLine();
	void showLineOnly();
	/*gpio*/
	void ledOn(int number);
	void ledOff(int number);
	void buzzerOn();
	void buzzerOff();
	/*uart*/
	void sendBuffer(uint8_t *buff, uint32_t len);
	void sendWave(void *wareaddr, uint32_t waresize);
	void sendImage(void *imgaddr, uint32_t imgsize);
	void sendImageZhu(void *imgaddr, uint32_t imgsize);
	void extractImage(uint8_t *dst, uint8_t *src, uint32_t srclen);
	void reduceImage(uint8_t *aim,uint8_t *get,uint32_t length);
	/*led*/
	void ledSeeImage();
};


#endif