#ifndef _BALL_H_
#define _BALL_H_ 

#define LEFTBOUNDARY    13
#define RIGHTBOUNDARY  125
#define UPBOUNDARY      15
#define DOWNBOUNDARY    63

#define LEFTUPMOVE       1
#define RIGHTUPMOVE      2
#define LEFTDOWNMOVE     3
#define RIGHTDOWNMOVE    4

#define MOVE_X          14
#define MOVE_Y          16
         
class ball{
public:
    int x;
    int y;
	int id;
	int moveStatus;
	int ballCount;
	
public:
	ball(int _x, int _y, int _id);
	void print();
	void moveLeftUp();
	void moveRightUp();
	void moveLeftDown();
	void moveRightDown();
	int isTouchLeft();
	int isTouchRight();
	int isTouchUp();
	int	isTouchDown();
	void play();
};


extern int ballNumber;
extern ball ballList[4];
#endif