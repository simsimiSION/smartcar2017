#include "ramp.h"
#include "base.h"
#include "imgSth.h"
#include "string.h"

int structRamp = 0;
/*==============================*/
ramp Ramp;
/*==============================*/
int ramp::get(char *str){
	if(strcmp(str, "isRamp") == 0)          return isRamp;
}


int ramp::is(){
#define RAMPLIMMIT   35
	
	int temp1 = 0;
	int temp2 = 0;
	int temp3 = 0;
	if (effect > 2)
		return 0;
	if (straightLine::is(1) == 0 && structRamp == 0)
		return 0;
	
	temp1 = lineinf[1].RightPos - lineinf[1].LeftPos;
	temp2 = lineinf[2].RightPos - lineinf[2].LeftPos;
	temp3 = lineinf[3].RightPos - lineinf[3].LeftPos;
	
	if (temp1 > RAMPLIMMIT && temp2 > RAMPLIMMIT && temp3 > RAMPLIMMIT)
	{
		this->isRamp = 1;
		return 1;
	}
	else
	{
		this->isRamp = 0;
		return 0;
	}
}

void ramp::Do(){
	
	this->is();
	
	if (structRamp == 0 && isRamp == 1) //识别坡道
		structRamp = 1;
	if (structRamp == 1 && isRamp == 0) 				//上坡
		structRamp = 2;
	if (structRamp == 2 && isRamp == 1) 				//下坡
		structRamp = 3;
	if (structRamp == 3)  								//清标志位
		structRamp = 0;
	if (lineinf[effect+5].LeftPos > 80 || lineinf[effect+5].RightPos < 80)
		structRamp = 0;
}