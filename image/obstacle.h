#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include "base.h"

extern int structObstacle;

class obstacle{
protected:
	int isObstacle;
	int isJumpOb;
	int canObstacle;
	int dir;
public:
	obstacle();
	int   get(char *str);
	int is();
	void patch();
	int isJump();
	void Do();

	
};


#endif