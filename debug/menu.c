#include "common.h"
#include "VCAN_OV7725_Eagle.h"
#include "OLED.h"
#include "common.h"
#include "menu.h"
#include "imgSth.h"
#include "debug.h"

#define KBGPIO HW_GPIOB
#define ESC   22
#define UP    16
#define DOWN  10
#define OK    20

#define PIDCHANGE              0.1 
#define SERVOCHANGE            1

#define PID  (FLASH_SECTOR_NUM-1)

#define SIZE_OF_KEYBD_MENU	6

SmartCarParameter theparameter;

static uint8 key_value = 0;            //按键参数

static uint32_t keymenu_curr_state = 0;  //菜单当前状态

int RUN;

void main_window_proc(void);
void run_window_proc(void);
void vision_parameter_proc(void);
void speed_pid_window_proc(void);
void servo_window_proc(void);
void see_value(void);
void see_picture_in_oled(void);
void send(void);

extern debug Debug;

KeyMenuTableStruct KeyMenuTable[SIZE_OF_KEYBD_MENU] =
{
	{0,0,0,0,main_window_proc},	
	{1,0,0,0,see_value},
	{2,0,0,0,see_picture_in_oled},
	{3,0,0,0,send},

};

#define	MAIN_MENU_LEN	(4)
char * MainMenuStrings[MAIN_MENU_LEN] = {"LET'GO", "SEE VALUE","OLED IMAGE","SEND"};

char * SeeValueStrings[] = {"SeeValue"};

uint32_t run_state = 0;

#define SEND_LEN   (1)
int SendMode = 0;
int SENDMODE = 10;
int SendSetup[SEND_LEN] = {0};
char * SendSth[] = {"SEND IMAGE"};

#define CNST_LEN   (1)
char *setCnst[] = {"CNST:"};
uint32_t CNSTVALUE= 0X40;
uint32_t CNSTMin=0X00;
uint32_t CNSTMax=0XFF;
uint32_t CNSTAdj=1;
////////////////////////////////////////////////////////////////////////////////
//          主屏幕
////////////////////////////////////////////////////////////////////////////////
void main_window_proc(void)
{
	uint8_t i;
	
	switch (key_value)
	{
	case 0x01:
		break;
	case 0x04:
		{
			KeyMenuTable[keymenu_curr_state].MenuSubstate++;
			if (KeyMenuTable[keymenu_curr_state].MenuSubstate > (MAIN_MENU_LEN-1))
				KeyMenuTable[keymenu_curr_state].MenuSubstate = 0;
		}
		break;
	case 0x02:
		{
			if (KeyMenuTable[keymenu_curr_state].MenuSubstate == 0)
				KeyMenuTable[keymenu_curr_state].MenuSubstate = MAIN_MENU_LEN;			
			KeyMenuTable[keymenu_curr_state].MenuSubstate--;
		}
		break;
	case 0x08:
		{
			if(KeyMenuTable[keymenu_curr_state].MenuSubstate == 0)
				RUN = 1;
			
			
			keymenu_curr_state = KeyMenuTable[keymenu_curr_state].MenuSubstate;
			LCD_clear();
		}
		break;
	default:
		{
			
			LCD_set_XY(8, 0);
			LCD_write_english("**** SMARTCAR ****");
			
			for (i=0; i<MAIN_MENU_LEN; i++)
			{
				LCD_set_XY(8, i+2);
				LCD_write_english(MainMenuStrings[i]);
			}
			for (i=0; i<MAIN_MENU_LEN; i++)
			{
				LCD_set_XY(0, i+2);
				if (KeyMenuTable[keymenu_curr_state].MenuSubstate == i)
					LCD_write_english(">");
				else
					LCD_write_english(" ");
			}	
			
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//          观测数值
////////////////////////////////////////////////////////////////////////////////

void see_value(void)
{
	
	switch (key_value)
	{
	case 0x01:
		{
			keymenu_curr_state = KeyMenuTable[keymenu_curr_state].KeyEscState;
			LCD_clear();
		}
		break;
	case 0x04:
		{}
		break;
	case 0x02:
		{}
		break;
	case 0x08:
		{}
		break;
	default:
		{
			LCD_set_XY(8, 0);
			LCD_write_english(" SET VALUE ");
			/*
			在这里输入想要查看的变量值
			*/
			
			
			//			LCD_set_XY(8,8);
			//			LCD_write_english("steer_error:");
			//			LCD_write_int(steer_error);
			
			
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//          oled显示图像
////////////////////////////////////////////////////////////////////////////////
int SeeByOled = 0;
int IsAdjust = 0;
char buffer[2];

void Dec2Hex(int num)
{
    char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    if(num < 16)
    {
        buffer[0] = '0';
        buffer[1] = hex[num];
    }
    else
    {
        buffer[0] = hex[num/16];
        buffer[1] = hex[num%16];
    }
}


void see_picture_in_oled(void)
{
	switch (key_value)
	{
	case 0x01:
		{
			keymenu_curr_state = KeyMenuTable[keymenu_curr_state].KeyEscState;
			KeyMenuTable[keymenu_curr_state].MenuSetup = 0;
			LCD_clear();
		}
		break;
	case 0x04:
		{
			if (KeyMenuTable[keymenu_curr_state].MenuSetup)
			{
				if (CNSTVALUE > CNSTMin )
					CNSTVALUE -= CNSTAdj;
			}
			
		}
		break;
	case 0x02:
		{
			if (KeyMenuTable[keymenu_curr_state].MenuSetup)
			{
				if (CNSTVALUE < CNSTMax )
					CNSTVALUE += CNSTAdj;	
			}
			
		}
		break;            
	case 0x08:
		{
			KeyMenuTable[keymenu_curr_state].MenuSetup = !KeyMenuTable[keymenu_curr_state].MenuSetup;
		}
		break;
	default:
		{
			if(IsAdjust == 0 && SeeByOled == 0)
			{
				camera_init(imgbuff);
				SeeByOled = 1;
			}
			
			camera_get_img();        
			Debug.extractImage(picturess, imgbuff, 1200);  
			imgHandle();
			Debug.showLine();
	        Debug.ledSeeImage();
			
			LCD_set_XY(8, 8);
			LCD_write_english(setCnst[0]);
			LCD_set_XY(96, 8);
			Dec2Hex(CNSTVALUE);
			LCD_write_english("0x");
			LCD_write_english(buffer);
			LCD_set_XY(0, 8);
			
			if (0 == KeyMenuTable[keymenu_curr_state].MenuSetup)
			{
				LCD_write_english(">");
				IsAdjust = 0;
			}
			else
			{
				LCD_write_english("*");
				IsAdjust = 1;
			}
			
			if(IsAdjust)
				SeeByOled = 0;
		}
		break;
	}
}

int SendByBT = 0;
void send(void)
{
	
	uint8_t i;
	
	switch (key_value)
	{
	case 0x01:
		{
			keymenu_curr_state = KeyMenuTable[keymenu_curr_state].KeyEscState;
			SendSetup[SendMode] = 0;
			LCD_clear();
		}
		break;
	case 0x04:
		{
			SendMode++;
			if (SendMode > (SEND_LEN-1))
				SendMode = 0;
		}
		break;
	case 0x02:
		{
			if (SendMode == 0)
				SendMode = SEND_LEN;			
			SendMode--;
		}
		break;
	case 0x08:
		{
			SENDMODE = SendMode;
			SendSetup[SendMode] = !SendSetup[SendMode];
		}
		break;
	default:
		{
			
			LCD_set_XY(8, 0);
			LCD_write_english("****** SEND *****");
			
			for (i=0; i<SEND_LEN; i++)
			{
				LCD_set_XY(8, i+2);
				LCD_write_english(SendSth[i]);
			}
			for (i=0; i<SEND_LEN; i++)
			{
				LCD_set_XY(0, i+2);
				
				if (SendMode == i)
				{
					if (0 == SendSetup[SendMode])
						LCD_write_english(">");
					else
						LCD_write_english("*");
				}
				else
					LCD_write_english(" ");
			}	
			
			if(SendSetup[SENDMODE] == 0)
				SENDMODE = 10;
			
			switch(SENDMODE)
			{
			case 0 :
				if(!SendByBT)
				{
					camera_init(imgbuff);
					SendByBT = 1;
				}

			camera_get_img();        
			Debug.extractImage(picturess, imgbuff, 1200);  
			imgHandle();
			Debug.showLine();
			Debug.reduceImage(picturess, imgbuff, 9600);
			Debug.sendImage(imgbuff, CAMERA_SIZE);  
				break;
			default:
				break;
			}
			
		}
		break;
	}
	
}


void init_kbd(void)
{
	GPIO_QuickInit(KBGPIO,  UP    ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KBGPIO,  DOWN  ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KBGPIO,  ESC   ,kGPIO_Mode_IPU);
	GPIO_QuickInit(KBGPIO,  OK    ,kGPIO_Mode_IPU);
}

void keyboard_scan(void)
{
	uint8_t ckey = 0;
	if (!GPIO_ReadBit(KBGPIO, ESC))	
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KBGPIO, ESC));
		ckey |= 0x01;
	}
	if (!GPIO_ReadBit(KBGPIO, UP))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KBGPIO, UP));
		ckey |= 0x02;
	}
	if (!GPIO_ReadBit(KBGPIO, DOWN))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KBGPIO, DOWN));
		ckey |= 0x04;
	}
	if (!GPIO_ReadBit(KBGPIO, OK))
	{
		DelayMs(200);
		while(!GPIO_ReadBit(KBGPIO, OK));
		ckey |= 0x08;
	}
	key_value = ckey;
	
	
}

void display_proc(void)
{
	KeyMenuTable[keymenu_curr_state].WindowProc();
}
