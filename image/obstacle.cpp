#include "obstacle.h"
#include "base.h"
#include "imgSth.h"
#include "cross.h"
#include "string.h"

int structObstacle = 0;

const int StraightLine[60] ={ 
	30,30,30,30,29,29,29,29,28,28,
	28,28,28,27,27,27,26,26,26,25,
	25,25,24,24,23,23,22,22,22,21,
	22,21,21,21,20,19,18,17,17,16,
	16,16,15,15,15,14,14,13,12,12,
	12,11,10,10, 9, 8, 6, 5, 4, 3 };


extern cross Cross;

obstacle::obstacle(){
	isObstacle = 0;
	canObstacle = 0;
	isJumpOb = 0;
	dir = 0;
}
/*==============================*/
obstacle Obstacle;
/*==============================*/
int obstacle::get(char *str){
	if(strcmp(str,"isObstacle") == 0)   return isObstacle;
	if(strcmp(str, "dir") == 0)          return dir;
}


int obstacle::is(){
	int i = 0;
	int j = 0;
	int k = 0;
	
	int wrong = 0;
	int FirstOb = 0;
	int OBBlack = 0;
	int RIGHTOBS = 0;
	int LEFTOBS = 0;
	int ObstacleLeft[60] = { 0 };
	int ObstacleRight[60] = { 0 };
	int IsObstacleLeft = 0;
	int IsObstacleRight = 0;
	int NotObstacle = 0;
	int LeftOb = 0;
	int RightOb = 0;
	
	if (effect > 7)
		return 0;
	if((Cross.get("LCrossFePos") != down && lineinf[Cross.get("LCrossFePos")].LeftPos > 20) 
	   || (Cross.get("RCrossFePos") != down && lineinf[Cross.get("RCrossFePos")].RightPos < 140))
		return 0;
	
	LeftOb  = getFirstBoundaryFromDown(1,0);
	RightOb = getFirstBoundaryFromDown(2,0);
	
	if(getFirstBoundaryFromDown(1,1) != 0 || getFirstBoundaryFromDown(2,1) != 0)
		NotObstacle = 1; 
	
	for (i = down; i > effect + 10; i--)
	{
		if (lineinf[i].LeftPos - lineinf[i - 1].LeftPos > 20)
		{
			wrong = 1;
			break;
		}
		if (lineinf[i - 1].RightPos - lineinf[i].RightPos > 20)
		{
			wrong = 1;
			break;
		}
	}
	
	for (i = down; i > effect + 2; i--)
	{
		for (j = lineinf[i].LeftPos + 1; j < lineinf[i].RightPos - 2; j++)
		{
			if (picture[i][j] == 0 && picture[i][j + 1] == 255 && picture[i][j + 2] == 255)
			{
				ObstacleLeft[i] = j + 1 - lineinf[i].LeftPos;
				LEFTOBS = j;
				for (k = lineinf[i].RightPos - 1; k > lineinf[i].LeftPos + ObstacleLeft[i]; k--)
				{
					if (picture[i][k] == 255 && picture[i][k + 1] == 0 && picture[i][k + 2] == 0)
					{
						ObstacleRight[i] = lineinf[i].RightPos - k;
						RIGHTOBS = k;
						if (ObstacleLeft[i] != 0)
						{
							if ((float)(ObstacleRight[i] / ObstacleLeft[i]) > 3 && ObstacleLeft[i] < 13 && ((ObstacleRight[i] + ObstacleLeft[i]) > (RIGHTOBS - LEFTOBS)))
							{
								IsObstacleLeft++;
								if (picture[i][80] == 255)
									OBBlack++;
								if (picture[i][80] == 0 && FirstOb == 0)
									FirstOb = i;
							}
							if ((float)(ObstacleRight[i] / ObstacleLeft[i]) < 0.3333333 && ObstacleRight[i] < 13 && ((ObstacleRight[i] + ObstacleLeft[i]) > (RIGHTOBS - LEFTOBS)))
							{
								IsObstacleRight++;
								if (picture[i][80] == 255)
									OBBlack++;
								if (picture[i][80] == 0 && FirstOb == 0)
									FirstOb = i;
							}
						}
						break;
					}
				}
				break;
				
			}
		}
	}
	
	if (IsObstacleLeft > 2 && NotObstacle == 0 && FirstOb > 8 && wrong == 0){
		isObstacle = 1;
		dir = 2;
		return 2;
	}
	if (IsObstacleRight > 2 && NotObstacle == 0 && FirstOb > 8 && wrong == 0){
		isObstacle = 1;
		dir = 1;
		return 1;
	}
	
	return 0;
}

int obstacle::isJump(){
	int Is = 0;
	for (int i = down; i > effect + 20; i--)
	{
		if (getAbs(lineinf[i].LeftPos - lineinf[i - 5].LeftPos) > 15 || getAbs(lineinf[i].RightPos - lineinf[i - 5].RightPos) > 15)
		{
			Is = 1;
			break;
		}
	}
	if (Is)
		return 1;
	else
		return 0;
}

void obstacle::patch(){
	int i = 0;
	switch (this->dir)
	{
	case 1:
		for (i = down; i > effect; i--)
			lineinf[i].MiddlePos = lineinf[i].LeftPos + StraightLine[down - i] ;
		break;
	case 2:
		for (i = down; i > effect; i--)
			lineinf[i].MiddlePos = lineinf[i].RightPos - StraightLine[down - i] ;
		break;
	default:
		break;
	}
}

void obstacle::Do(){
	this->isObstacle = obstacle::is();
	if (this->isObstacle != 0)
	{
		if (this->isObstacle == 1)
			structObstacle = 1;
		else if (this->isObstacle == 2)
			structObstacle = 2;
	}
	if (structObstacle != 0)
	{
		this->isJumpOb = obstacle::isJump();
		obstacle::patch();
	}
	if (this->isObstacle == 0 && structObstacle != 0 && this->isJumpOb == 1)
		this->canObstacle = 1;

	if (this->canObstacle == 1 && this->isJumpOb == 0)
	{
		this->canObstacle = 0;
		structObstacle = 0;
	}
}