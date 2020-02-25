#include "VCAN_camera.h"
#include "imgSth.h"
#include "Base.h"
#include "ramp.h"
#include "straightLine.h"
#include "zebarLine.h"
#include "cross.h"
#include "obstacle.h"

uint8_t imgbuff[CAMERA_SIZE];  
uint8_t img[CAMERA_W*CAMERA_H]; 
uint8 picture[60][160];     
uint8 *picturess = &picture[0][0];

/*========================================================================
*  函数名称:  LimitPr
*  功能说明：限制点的范围，使之不会超过左右边界
*  创建时间：2016
*  修改时间：2016
*  参数说明：
========================================================================*/
int LimitPr(int a)
{
	if (a > right)
		a = right;
	if (a < left)
		a = left;
	
	return a;
}

/*========================================================================
*  函数名称:  getTendency
*  功能说明：趋势检索
*
*  创建时间：2016
*  修改时间：2017
*  参数说明：左则返回 1
右则返回 2
========================================================================*/
#define LeftDir    1
#define RightDir    2
#define MiddleDir   3
float getTendency(uint8(*picture)[160], int effect, int Dir, int High, int Low)
{
	int i = 0;
	
	float AVE_X = 0;
	float AVE_Y = 0;
	float aa = 0;
	float bb = 0;
	float SLOPE = 0;
	
	if (effect > High)
		return 0;
	
	for (i = Low; i > High; i--)
		AVE_X += i;
	AVE_X = AVE_X / (Low - High + 1);
	
	switch (Dir)
	{
	case LeftDir:
		
		for (i = Low; i > High; i--)
			AVE_Y += lineinf[i].LeftPos;
		AVE_Y = AVE_Y / (Low - High + 1);
		
		for (i = Low; i > High; i--)
			aa += (i - AVE_X) * (lineinf[i].LeftPos - AVE_Y);
		
		for (i = Low; i > High; i--)
			bb += (i - AVE_X) * (i - AVE_X);
		
		break;
	case RightDir:
		
		for (i = Low; i > High; i--)
			AVE_Y += lineinf[i].RightPos;
		AVE_Y = AVE_Y / (Low - High + 1);
		
		for (i = Low; i > High; i--)
			aa += (i - AVE_X) * (lineinf[i].RightPos - AVE_Y);
		
		for (i = Low; i > High; i--)
			bb += (i - AVE_X) * (i - AVE_X);
		
		break;
	case MiddleDir:
		
		for (i = Low; i > High; i--)
			AVE_Y += lineinf[i].MiddlePos;
		AVE_Y = AVE_Y / (Low - High + 1);
		
		for (i = Low; i > High; i--)
			aa += (i - AVE_X) * (lineinf[i].MiddlePos - AVE_Y);
		
		for (i = Low; i > High; i--)
			bb += (i - AVE_X) * (i - AVE_X);
		
		break;
	default:
		return 0;
		break;
	}
	
	if (bb != 0)
		SLOPE = aa / bb;
	
	return SLOPE;
}

/*========================================================================
*  函数名称:  getFirstBoundaryFromDown
*  功能说明：获得边界点，从下方
*  创建时间：2017
*  修改时间：2017
*  参数说明：
========================================================================*/
int getFirstBoundaryFromDown(int dir,int isBoundary)
{
	int i = 0;
	switch(dir)
	{
	case 1:
		if(isBoundary){
			for(i = down; i >= effect; i--){
				if(lineinf[i].LeftPos == left){
					return i;
				}
				if(i == effect + 1)
					return 0;
			}
		}
		else{
			for(i = down; i >= effect; i--){
				if(lineinf[i].LeftPos != left){
					return i;
				}
				if(i == effect + 1)
					return 0;
			}
		}
		break;
	case 2:
		if(isBoundary){
			for(i = down; i >= effect; i--){
				if(lineinf[i].RightPos == right){
					return i;
				}
				if(i == effect + 1)
					return 0;
			}
		}
		else{
			for(i = down; i >= effect; i--){
				if(lineinf[i].RightPos != right){
					return i;
				}
				if(i == effect + 1)
					return 0;
			}
		}
		break;
	default:
		break;
	}
}

/*========================================================================
*  函数名称:  imgHandle
*  功能说明：限制点的范围，使之不会超过左右边界
*  创建时间：2016
*  修改时间：2016
*  参数说明：
========================================================================*/
/*相关类定义*/
extern base Base;
extern cross Cross;
extern obstacle Obstacle;
extern ramp Ramp;
extern straightLine Straightine;
extern zebarLine ZebarLine;

void imgHandle()
{
	/*相关变量定义*/
	int zebarDistance = 0;
	
	/*基础补线*/
	Base.Do();
	/*斑马线*/
	if(ZebarLine.is())
		zebarDistance = ZebarLine.getDistance();
	/*坡道*/
	Ramp.Do();
	/*障碍*/
	if(Obstacle.is())
		Obstacle.Do(); // 没写呢
	/*十字*/
	if(Cross.isNormal())
		Cross.patchNormal();
	if(Cross.isFeNormal())
		Cross.patchFeNormal();
	
	Base.setMiddleLine();
	
}

