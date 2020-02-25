#include "debug.h"
#include "base.h"
#include "imgSth.h"
#include "gpio.h"
#include "uart.h"
#include "lcdScreen.h"

/*****************/
extern lcd Lcd;
/*****************/

debug::debug(){
	debug::assertDebug(   BUZZER_PORT == HW_GPIOA || BUZZER_PORT == HW_GPIOB || BUZZER_PORT == HW_GPIOC 
					   || BUZZER_PORT == HW_GPIOD || BUZZER_PORT == HW_GPIOE || BUZZER_PORT == NULL);
	
#if (LED_COUNT == 1)
	this->isLed[0] = 1;
	this->isLed[1] = 0;
	this->isLed[2] = 0;
	GPIO_QuickInit(LED_PORT[0]   , LED_PIN[0]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[0]   , LED_PIN[0]   , 0);
#elif (LED_COUNT == 2)
	this->isLed[0] = 1;
	this->isLed[1] = 1;
	this->isLed[2] = 0;
	GPIO_QuickInit(LED_PORT[0]   , LED_PIN[0]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[0]   , LED_PIN[0]   , 0);
	GPIO_QuickInit(LED_PORT[1]   , LED_PIN[1]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[1]   , LED_PIN[1]   , 0);
#elif (LED_COUNT == 3)
	this->isLed[0] = 1;
	this->isLed[1] = 1;
	this->isLed[2] = 1;
	GPIO_QuickInit(LED_PORT[0]   , LED_PIN[0]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[0]   , LED_PIN[0]   , 0);
	GPIO_QuickInit(LED_PORT[1]   , LED_PIN[1]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[1]   , LED_PIN[1]   , 0);
	GPIO_QuickInit(LED_PORT[2]   , LED_PIN[2]   , kGPIO_Mode_OPP);
	GPIO_WriteBit (LED_PORT[2]   , LED_PIN[2]   , 0);
#else
	this->isLed[0] = 0;
	this->isLed[1] = 0;
	this->isLed[2] = 0;
#endif
	
	if(BUZZER_PORT != NULL && BUZZER_PIN != NULL){
		this->isBuzzer = 1;
		GPIO_QuickInit(BUZZER_PORT, BUZZER_PIN, kGPIO_Mode_OPP);
		GPIO_WriteBit (BUZZER_PORT, BUZZER_PIN, 0);
	}
	
	DelayInit();                            
	UART_QuickInit(UARTINIT, 115200);
}

debug::~debug(){
	delete isLed;
}

/*********************************/
debug Debug;
/*********************************/


/*=====================================================================================
�����ķָ��� ------- system
======================================================================================*/

void debug::print(char *str){
	PRINT("%s",str);
}

void debug::assertDebug(int expression){
	assert(expression);
}

void debug::delayUs(uint32_t us){
	 DWT_DelayUs(us);
}

void debug::delayMs(uint32_t ms){
	DWT_DelayMs(ms);
}
/*=====================================================================================
�����ķָ��� ------- image
======================================================================================*/
void debug::pictureTurnColor(int ver, int hor, int color){
	switch(color){
	case BLACK:
		picture[ver][hor] = BLACK;
		break;
	case WHITE:
		picture[ver][hor] = WHITE;
		break;
	default:
		break;
	}
}

void debug::pictureClear(int color){
	switch(color){
	case BLACK:
		for(int i = down; i >= up; i--){
			for(int j = left; j <= right; j++){
				debug::pictureTurnColor( i, j, BLACK);
			}
		}
	case WHITE:
		for(int i = down; i >= up; i--){
			for(int j = left; j <= right; j++){
				debug::pictureTurnColor( i, j, WHITE);
			}
		}
		break;
	default:
		break;
	}
}

void debug::showLine(){
	for(int i = down; i >= effect; i--){
		debug::pictureTurnColor(i, lineinf[i].LeftPos  , BLACK);
		debug::pictureTurnColor(i, lineinf[i].RightPos , BLACK);
		debug::pictureTurnColor(i, lineinf[i].MiddlePos, BLACK);
	}
}

void debug::showLineOnly(){
	debug::pictureClear(WHITE);
	debug::showLine();
}

/*=====================================================================================
�����ķָ��� ------- gpio
======================================================================================*/

void debug::ledOn(int number){
	debug::assertDebug((number == 1 || number == 2 || number == 0) && number <= LED_COUNT);	
	
#if (LED_COUNT == 1)
	GPIO_WriteBit(LED_PORT[0], LED_PIN[0], 1);
#elif (LED_COUNT == 2)
	GPIO_WriteBit(LED_PORT[number], LED_PIN[number], 1);
#elif (LED_COUNT == 3)
	GPIO_WriteBit(LED_PORT[number], LED_PIN[number], 1);
#else
#endif
}

void debug::ledOff(int number){
	debug::assertDebug((number == 1 || number == 2 || number == 0) && number <= LED_COUNT);	
	
#if (LED_COUNT == 1)
	GPIO_WriteBit(LED_PORT[0], LED_PIN[0], 0);
#elif (LED_COUNT == 2)
	GPIO_WriteBit(LED_PORT[number], LED_PIN[number], 0);
#elif (LED_COUNT == 3)
	GPIO_WriteBit(LED_PORT[number], LED_PIN[number], 0);
#else
#endif
}

void debug::buzzerOn(){
	debug::assertDebug(this->isBuzzer);
	GPIO_WriteBit(BUZZER_PORT, BUZZER_PIN, 1);
}

void debug::buzzerOff(){
	debug::assertDebug(this->isBuzzer);
	GPIO_WriteBit(BUZZER_PORT, BUZZER_PIN, 0);
}

/*=====================================================================================
�����ķָ��� ------- uart
======================================================================================*/
void debug::sendBuffer(uint8_t *buff, uint32_t len){
	while(len--)
	{
		UART_WriteByte(UARTDEBUG, *buff);
		buff++;
	}
}

void debug::sendWave(void *wareaddr, uint32_t waresize){
#define CMD_WARE     3
	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����
	
	debug::sendBuffer(cmdf               ,  sizeof(cmdf));
	debug::sendBuffer((uint8_t *)wareaddr,  waresize);
	debug::sendBuffer(cmdr               ,  sizeof(cmdf));
	
}

void debug::sendImage(void *imgaddr, uint32_t imgsize){
#define CMD_IMG     1
	uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
	uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����
	
	debug::sendBuffer(cmdf               ,  sizeof(cmdf));
	debug::sendBuffer((uint8_t *)imgaddr,  imgsize);
	debug::sendBuffer(cmdr               ,  sizeof(cmdf));  
}

void debug::sendImageZhu(void *imgaddr, uint32_t imgsize){
	uint8 frame_head[]={1,254,254,1};
	debug::sendBuffer(frame_head               , 4);
	debug::sendBuffer((uint8_t *)imgaddr       , imgsize);
}

void debug::extractImage(uint8_t *dst, uint8_t *src, uint32_t srclen){
	uint8_t colour[2] = {0, 255}; 
	//uint8_t colour[2] = {255, 0}; 
	uint8_t * mdst = dst;
	uint8_t * msrc = src;
	
	uint8_t tmpsrc;
	while(srclen --)
	{
		tmpsrc = *msrc++;
		*mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];    
		*mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
		*mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
	}	
}

void debug::reduceImage(uint8_t *aim,uint8_t *get,uint32_t length){
	while(length>0)
	{
		*get = 0;
		*get += ((*aim++) & 0x80) >> 0;
		*get += ((*aim++) & 0x80) >> 1;
		*get += ((*aim++) & 0x80) >> 2;
		*get += ((*aim++) & 0x80) >> 3;
		*get += ((*aim++) & 0x80) >> 4;
		*get += ((*aim++) & 0x80) >> 5;
		*get += ((*aim++) & 0x80) >> 6;
		*get += ((*aim++) & 0x80) >> 7;
		//*get =~ *get;
		get++;
		length -= 8;
	}
	
}

/*=====================================================================================
�����ķָ��� ------- led
======================================================================================*/
void debug::ledSeeImage(){
	int i = 0;
	int j = 0;
	
	int temp = 0;
	
	for(i = 0; i < 8; i++ )
	{
		Lcd.setPos(0, i);
		
		if(i < 7)
		{
			for(j = 20; j < 140; j++)  //20
			{
				temp = 0;
				if(!picture[i*8 + 0][j])     temp|=0x01;  //�� 1 ��ͼ��
				if(!picture[i*8 + 1][j])     temp|=0x02;  //�� 2 ��ͼ��
				if(!picture[i*8 + 2][j])     temp|=0x04;  //�� 3 ��ͼ��
				if(!picture[i*8 + 3][j])     temp|=0x08;  //�� 4 ��ͼ��
				if(!picture[i*8 + 4][j])     temp|=0x10;  //�� 5 ��ͼ��
				if(!picture[i*8 + 5][j])     temp|=0x20;  //�� 6 ��ͼ��
				if(!picture[i*8 + 6][j])     temp|=0x40;  //�� 7 ��ͼ��
				if(!picture[i*8 + 7][j])     temp|=0x80;  //�� 8 ��ͼ��
				Lcd.writeData(temp);

			}
		}
		if(i == 7)
			
		{
			for(j = 20; j < 140; j++)
			{
				temp = 0;
				if(!picture[i*8 + 0][j])     temp|=0x01;  //�� 57 ��ͼ��
				if(!picture[i*8 + 1][j])     temp|=0x02;  //�� 58 ��ͼ��
				if(!picture[i*8 + 2][j])     temp|=0x04;  //�� 59 ��ͼ��
				if(!picture[i*8 + 3][j])     temp|=0x08;  //�� 60 ��ͼ��
				Lcd.writeData(temp);
			}
		}
	}
}