#include "zebarline.h"
#include "base.h"
#include "imgSth.h"
#include "string.h"
zebarLine::zebarLine(){
	is();
}
/*==============================*/
zebarLine ZebarLine;
/*==============================*/
int zebarLine::get(char *str){
	if(strcmp(str, "distance") == 0 )          return distance;
	if(strcmp(str, "isZebarLine") == 0)       return isZebarLine;
}

int zebarLine::is(){
#define ZEBARLIMIT  4
	
	int ZebarCount = 0;
	int ZECOUNT = 0;
	
	for (int i = down; i > effect + 10; i--) 
	{
		if (lineinf[i].LeftPos == right || lineinf[i].RightPos == left || lineinf[i].LeftPos >= lineinf[i].RightPos
			|| lineinf[i].LeftPos == left || lineinf[i].RightPos == right) {}
		else {
			for (int j = lineinf[i].LeftPos + 1; j < lineinf[i].RightPos - 1; j++)
			{
				if (picture[i][j] != picture[i][j + 1])
					ZebarCount++;
			}
			if (ZEBARLIMIT < ZebarCount / 2)
			{
				distance = down - i;
				isZebarLine = 1;
				return 1;
			}
			else
				ZebarCount = 0;
			
		}
	}
	
	isZebarLine = 0;
	return 0;
	
}
int zebarLine::getDistance(){
	return distance;
}