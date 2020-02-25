#include "straightLine.h"
#include "base.h"
#include "imgSth.h"
#include "string.h"
int straightArray[60] =
{ 30,30,30,30,29,29,29,29,28,28,
28,28,28,27,27,27,26,26,26,25,
25,25,24,24,23,23,22,22,22,21,
22,21,21,21,20,19,18,17,17,16,
16,16,15,15,15,14,14,13,12,12,
12,11,10,10, 9, 8, 6, 5, 4, 3 };

/*==============================*/
straightLine StriaghtLine;
/*==============================*/
int straightLine::get(char *str){
	if(strcmp(str, "isStraightLine") == 0)       return isStraightLine;
}

int straightLine::is(int distance){
	
	int i = 0;
	
	int LStraight = 0;
	int RStraight = 0;
	int StraightZero = 0;
	
	for (i = down; i > distance; i--)
	{
		if (lineinf[i].LeftPos == left || lineinf[i].RightPos == right)
		{
			StraightZero = 1;
			break;
		}
	}
	for (i = down; i > distance; i--)
	{
		if (lineinf[i].LeftPos <= lineinf[i - 1].LeftPos && lineinf[i - 1].LeftPos - lineinf[i].LeftPos < 4)
			LStraight++;
		if (lineinf[i].RightPos >= lineinf[i - 1].RightPos && lineinf[i].RightPos - lineinf[i - 1].RightPos < 4)
			RStraight++;
	}
	
	if (LStraight >(down - distance - 1) && RStraight >(down - distance - 1) && effect < (distance + 3)
		&& StraightZero == 0)
	{
		isStraightLine = 1;
		return 1;
	}
	else
	{
		isStraightLine = 0;
		return 0;
	}
}

void straightLine::patch(int dir){
	
	int i = 0;
	
	switch (dir)
	{
	case 1:
		for (i = down; i > effect; i--)
			lineinf[i].MiddlePos = lineinf[i].LeftPos + straightArray[down - i];
		break;
	case 2:
		for (i = down; i > effect; i--)
			lineinf[i].MiddlePos = lineinf[i].RightPos - straightArray[down - i];
		break;
	default:
		break;
	}	
}
