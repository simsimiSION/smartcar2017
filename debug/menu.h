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



#include "common.h"
#include "gpio.h"

typedef struct
{
	uint8 KeyStateIndex; 				//  当前状态索引号
	uint8 KeyEscState;				//  按下“退回”键时转向的状态索引号
	uint32_t MenuSubstate;
	uint32_t MenuSetup;
	void (*WindowProc)( );		//  当前状态应该执行的功能操作
}
KeyMenuTableStruct;

typedef struct
{
	
	//  motor speed
	uint32_t	para_speed_pid_p;
	uint32_t	para_speed_pid_i;
	uint32_t	para_speed_pid_d;
	
	//  servo
	uint32_t	para_servo_midval;
	//  display mode.
	uint8	para_display_mode;
}SmartCarParameter;


#define SPEED_MENU_LEN	(3)
#define	SERVO_MENU_LEN	(1)
#define	VISIONDBG_MENU_LEN	(1)

extern int RUN;
extern int batt;
extern uint32_t CNSTVALUE;
extern float SpeedPIDParam[SPEED_MENU_LEN];
extern uint32_t ServoParam[VISIONDBG_MENU_LEN];

extern SmartCarParameter theparameter;

#define	KEY_BOARD_HISTORY_LEN	(5)
#define	KEY_BOARD_NUM		(4)
extern uint8_t key_value;

void init_kbd(void);

void keyboard_scan(void);

void display_proc(void);
