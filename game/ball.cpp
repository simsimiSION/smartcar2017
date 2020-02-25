#include "ball.h"
#include "lcdScreen.h"
#include "common.h"
/***************/
int ballNumber = 4;

extern lcd Lcd;
ball ballList[4] = {
	ball(13, 15, 1),
	ball(83, 31, 2),
	ball(55, 63, 3),
	ball(97, 47, 4),
};
/***************/

ball::ball(int _x, int _y, int _id){
	this->x = _x;
	this->y = _y;
	this->id = _id;
	this->moveStatus = LEFTUPMOVE;
}

#define DELAYMS  30
void ball::print(){
	switch(this->id){
	case 1:
		Lcd.writeChinese(this->x - 13, (this->y - 15)/16, "东");
		break;
	case 2:
		Lcd.writeChinese(this->x - 13, (this->y - 15)/16, "北");
		break;
	case 3:
		Lcd.writeChinese(this->x - 13, (this->y - 15)/16, "大");
		break;
	case 4:
		Lcd.writeChinese(this->x - 13, (this->y - 15)/16, "学");
		break;
	default:
		break;
		
	}
	DWT_DelayMs(DELAYMS);	
}
void ball::moveLeftUp(){
	this->x -= MOVE_X;
	this->y -= MOVE_Y;
}
void ball::moveRightUp(){
	this->x += MOVE_X;
	this->y -= MOVE_Y;
}
void ball::moveLeftDown(){
	this->x -= MOVE_X;
	this->y += MOVE_Y;
}
void ball::moveRightDown(){
	this->x += MOVE_X;
	this->y += MOVE_Y;
}
int ball::isTouchLeft(){
	if(this->x == LEFTBOUNDARY)
		return 1;
	else
		return 0;
}
int ball::isTouchRight(){
	if(this->x == RIGHTBOUNDARY)
		return 1;
	else
		return 0;
}
int ball::isTouchUp(){
	if(this->y == UPBOUNDARY)
		return 1;
	else
		return 0;
}
int	ball::isTouchDown(){
	if(this->y == DOWNBOUNDARY)
		return 1;
	else
		return 0;
}


void ball::play(){
	
		this->ballCount++;
		if(this->ballCount > (30 / ballNumber))
		{
			this->ballCount = 0;
			Lcd.clear();
			if(this->isTouchLeft() && this->moveStatus == LEFTUPMOVE)
				this->moveStatus = RIGHTUPMOVE;
			if(this->isTouchLeft() && this->moveStatus == LEFTDOWNMOVE)
				this->moveStatus = RIGHTDOWNMOVE;
			if(this->isTouchRight() && this->moveStatus == RIGHTUPMOVE)
				this->moveStatus = LEFTUPMOVE;
			if(this->isTouchRight() && this->moveStatus == RIGHTDOWNMOVE)
				this->moveStatus = LEFTDOWNMOVE;
			if(this->isTouchUp() && this->moveStatus == LEFTUPMOVE)
				this->moveStatus = LEFTDOWNMOVE;
			if(this->isTouchUp() && this->moveStatus == RIGHTUPMOVE)
				this->moveStatus = RIGHTDOWNMOVE;
			if(this->isTouchDown() && this->moveStatus == RIGHTDOWNMOVE)
				this->moveStatus = RIGHTUPMOVE;
			if(this->isTouchDown() && this->moveStatus == LEFTDOWNMOVE)
				this->moveStatus = LEFTUPMOVE;
			
			switch(this->moveStatus){
			case LEFTDOWNMOVE:
				this->moveLeftDown();
				break;
			case LEFTUPMOVE:
				this->moveLeftUp();
				break;
			case RIGHTDOWNMOVE:
				this->moveRightDown();
				break;
			case RIGHTUPMOVE:
				this->moveRightUp();
				break;
			}
		}
		
		this->print();
	
}