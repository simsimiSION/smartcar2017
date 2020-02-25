#include "cross.h"
#include "base.h"
#include "imgSth.h"
#include "string.h"

#define SLOPERIGHTDOWN    (   0)
#define SLOPERIGHTUP      (   3)
#define SLOPELEFTDOWN     (  -3)
#define SLOPELEFTUP       (   0)

/*==============================*/
cross Cross;
/*==============================*/
void cross::init(){
	isCross = 0;
	LCrossDown  = 0;
	RCrossDown  = 0;
	LCrossUp    = 0;
	RCrossUp    = 0;
	LCrossFePos = 0;
	RCrossFePos = 0;
}

int cross::get(char *str){
	if(strcmp(str, "isCross") == 0)	         return isCross;
	if(strcmp(str, "isNormalCross") == 0)    return isNormalCross;
	if(strcmp(str, "isFeNormalCross") == 0)  return isFeNormalCross;
	if(strcmp(str, "LCrossDown") == 0)       return LCrossDown;
	if(strcmp(str, "RCrossDown") == 0)		 return RCrossDown;
	if(strcmp(str, "LCrossUp") == 0)         return LCrossUp;
	if(strcmp(str, "RCrossUp") == 0)         return RCrossUp;
	if(strcmp(str, "LCrossFePos") == 0)      return LCrossFePos;
	if(strcmp(str, "RCrossFePos") == 0)      return RCrossFePos;	
			
}

void cross::set(char *str, int number){
	if(strcmp(str, "isCross") == 0)	         isCross = number;
	if(strcmp(str, "isNormalCross") == 0)    isNormalCross = number;
	if(strcmp(str, "isFeNormalCross") == 0)  isFeNormalCross = number;
	if(strcmp(str, "LCrossDown") == 0)       LCrossDown = number;
	if(strcmp(str, "RCrossDown") == 0)		 RCrossDown = number;
	if(strcmp(str, "LCrossUp") == 0)         LCrossUp = number;
	if(strcmp(str, "RCrossUp") == 0)         RCrossUp = number;
	if(strcmp(str, "LCrossFePos") == 0)      LCrossFePos = number;
	if(strcmp(str, "RCrossFePos") == 0)      RCrossFePos = number;
}

int cross::isBoundOver(char pn, int low, int high){
#define ISBOUNDOVERLIMITN   25
#define ISBOUNDOVERLIMITP   3
	
	int i = 0;
	switch(pn)
	{
	case 'n':
		for(i = low; i > high; i--){
			if(lineinf[i].LeftPos > (ISBOUNDOVERLIMITN + left) 
			   || lineinf[i].RightPos < (right - ISBOUNDOVERLIMITN))
				return 1;
		}
		return 0;
		break; 
	case 'p':
		for(i = low; i > high; i--)
		{
			if(lineinf[i].LeftPos < (ISBOUNDOVERLIMITP + left) 
			   || lineinf[i].RightPos > (right - ISBOUNDOVERLIMITP))
				return 1;
		}
		return 0;
		break;
	case 'l':
		for(i = low; i > high; i--)
		{
			if( lineinf[i].LeftPos == left )
				return 1;
		}
		return 0;
		break;
	case 'r':
		for(i = low; i > high; i--)
		{
			if(lineinf[i].RightPos == right)
				return 1;
		}
		return 0;
		break;
	default:
		break;
	}
}

int cross::isCurveBaseCross(int dir){
	
	int i = 0;
	int sum = 0;
	int sum2 = 0;
	int LRLPOS = 0;
	int RRLPOS = 0;
	
	switch(dir)
	{
	case 1:
		for(i = down; i > effect + 1; i--)
		{
			if(lineinf[i].TrueLeft > 120)
			{
				LRLPOS = i;
				break;
			}
		}
		for (i = down; i > getBigger(effect+1,LRLPOS); i--)
		{
			if(lineinf[i].LeftPos <= lineinf[i-1].LeftPos && lineinf[i].LeftPos >= lineinf[down].LeftPos)
				sum++;
			if(lineinf[i].TrueLeft == left && lineinf[i-1].TrueLeft == left && i >= 30)
				sum2++;
		}
		break;
	case 2:
		for(i = down; i > effect + 1; i--)
		{
			if(lineinf[i].TrueRight < 40)
			{
				RRLPOS = i;
				break;
			}
		}
		for (i = down; i > getBigger(effect+1,RRLPOS); i--)
		{
			if(lineinf[i].RightPos >= lineinf[i-1].RightPos && lineinf[i].RightPos <= lineinf[down].RightPos)
				sum++;
			if(lineinf[i].TrueRight == right && lineinf[i-1].TrueRight == right && i >= 30)
				sum2++;
		}
		break;
	default:
		break;
	}
	
	if(sum > (down - getBigger(getBigger(effect+1,RRLPOS),LRLPOS) - 2) && sum2 < 15)
		return 1;
	else
		return 0;
}

void cross::setMatrix(){
	int i = 0;
	for (i = down; i > up; i--)
	{
		LEFT[i] = left;
		RIGHT[i] = right;
		
	}
	for (i = down; i > effect; i--)
	{
		LEFT[i] = lineinf[i].LeftPos;
		RIGHT[i] = lineinf[i].RightPos;
	}
}

int cross::triFind(int dir){
#define TriFindLimit     (10)
	int i;
	triDown = 0;
	int triCount = 0;
	triFlag = 0;
	triFlag2 = 0;
	triPos = 0;
	int DDFlag = 0;
	
	switch (dir)
	{
	case 1:
		for (i = down; i > TriFindLimit; i--)
		{
			if (lineinf[i].LeftPos != left && DDFlag == 0)
				DDFlag = 1;
			if (lineinf[i].LeftPos == left && DDFlag == 1)
			{
				triDown = i;
				break;
			}
		}
		if (triDown != 0 && triDown != 59)
		{
			for (i = down; i > triDown; i--)
			{
				if (lineinf[i].LeftPos >= lineinf[i - 1].LeftPos)
					triCount++;
			}
			if (triCount >= getBigger(58 - triDown, 0))
				triFlag = 1;
			if ((lineinf[getSmaller(triDown + 1, 59)].LeftPos - lineinf[triDown].LeftPos >= 7)
				&& (lineinf[getSmaller(triDown + 2, 59)].LeftPos - lineinf[getSmaller(triDown + 1, 59)].LeftPos <= 6) && triFlag == 1)
			{
				triFlag = 0;
				triFlag2 = 1;
				triPos = triDown + 1;
			}
		}
		break;
		
	case 2:
		for (i = down; i > TriFindLimit; i--)
		{
			if (lineinf[i].RightPos != right && DDFlag == 0)
				DDFlag = 1;
			if (lineinf[i].RightPos == right&& DDFlag == 1)
			{
				triDown = i;
				break;
			}
		}
		if (triDown != 0 && triDown != 59)
		{
			for (i = down; i > triDown; i--)
			{
				if (lineinf[i].RightPos <= lineinf[i - 1].RightPos)
					triCount++;
			}
			if (triCount >= getBigger(58 - triDown, 0))
				triFlag = 1;
			if ((lineinf[triDown].RightPos - lineinf[getSmaller(triDown + 1, 59)].RightPos >= 7)
				&& (lineinf[getSmaller(triDown + 1, 59)].RightPos - lineinf[getSmaller(triDown + 2, 59)].RightPos <= 6) && triFlag == 1)
			{
				triFlag = 0;
				triFlag2 = 1;
				triPos = triDown + 1;
			}
		}
		break;
	default:
		break;
	}
	if (triFlag == 1)
		return 0;
	if (triFlag == 0 && triFlag2 == 1)
		return 0;
	if (triFlag == 0 && triFlag2 == 0)
		return 1;
}

float cross::curveSlope(int x, int pos[]){
	float SUM_XY = 0;
	float SUM_X = 0;
	float SUM_Y = 0;
	float SUM_XX = 0;
	float SUM_X_SUM_X = 0;
	float a = 0;
	
	if (x < 2)
		return 0;
	
	SUM_XY = x*pos[x] + (x - 1)*pos[x - 1] + (x - 2)*pos[x - 2];
	SUM_X = x + x - 1 + x - 2;
	SUM_Y = pos[x] + pos[x - 1] + pos[x - 2];
	SUM_XX = x*x + (x - 1)*(x - 1) + (x - 2)*(x - 2);
	SUM_X_SUM_X = SUM_X * SUM_X;
	
	if (3 * SUM_XX - SUM_X_SUM_X == 0)
		return 0;
	a = (float)((3 * SUM_XY - SUM_X * SUM_Y) / (3 * SUM_XX - SUM_X_SUM_X));
	
	return a;
}

int cross::isNormal(){
	
#define feature (3)
#define limit   (15)
#define LLLIMIT  (20)
#define RRLIMIT  (140)
	
	int i;
	int LCrossFlag = 0;
	int RCrossFlag = 0;
	int LCrossCount = 0;
	int RCrossCount = 0;
	isDoingL = 0;
	isDoingR = 0;
	
	if ( effect > 10 )                        
		return 0;
	if ( cross::isBoundOver('n',down,effect) == 1 || cross::isBoundOver('p',down,effect) == 0)
		return 0;
	if ( cross::isCurveBaseCross(1) == 1 || cross::isCurveBaseCross(2) == 1 )
		return 0;
	
	cross::setMatrix();
	
	/*左侧的十字找线程序*/
	for (i = down; i > getBigger(limit, effect + 3); i--)    //检测十字左下角的值
	{
		if ((((lineinf[i].LeftPos - lineinf[i - 1].LeftPos) <= 0) && ((lineinf[i - 1].LeftPos - lineinf[i - 2].LeftPos) > 0)))
		{
			LCrossDown = i - 1;
			LCrossFlag = 1;
			break;
		}
	}
	if(cross::isBoundOver('l',down,LCrossDown))
		LCrossFlag = 0;
	if (0 == cross::triFind(1) && LCrossFlag == 0)
	{
		LCrossFlag = 1;
		if (triFlag2 == 1)
			LCrossDown = triPos;
		else
			LCrossDown = triDown;
	}
	
	if(LCrossFlag == 1 && LCrossDown != triDown)          //检测十字左上角的值
	{
		for (i = LCrossDown - 3; i > effect + 3; i--)
		{
			if ( (cross::curveSlope(i, LEFT) > SLOPELEFTDOWN && cross::curveSlope(i, LEFT) < SLOPELEFTUP) )
			{
				LCrossUp = getBigger(effect, i - 2);
				break;
			}
			if (i == effect + 8)
			{
				LCrossUp = effect + 8;
				isDoingL = 1;
				break;
			}
		}
	}
	
	if (LCrossDown != 0 && LCrossUp != 0) //左侧十字计数程序
	{
		for (i = LCrossDown; i > LCrossUp; i--)
		{
			if (lineinf[i].LeftPos == left)
				LCrossCount++;
		}
	}
	
	/*左侧的十字找线程序*/
	for (i = down; i > getBigger(limit, effect + 3); i--)    //检测十字左右下角的值
	{
		if ((((lineinf[i].RightPos - lineinf[i - 1].RightPos) >= 0) && ((lineinf[i - 1].RightPos - lineinf[i - 2].RightPos) < 0)))
		{
			RCrossDown = i - 1;
			RCrossFlag = 1;
			break;
		}
	}
	if(cross::isBoundOver('r',down,RCrossDown))
		RCrossFlag = 0;
	if (0 == cross::triFind(2) && RCrossFlag == 0)
	{
		LCrossFlag = 1;
		if (triFlag2 == 1)
			RCrossDown = triPos;
		else
			RCrossDown = triDown;
	}
	
	if(RCrossFlag == 1 && RCrossDown != triDown)       //检测十字右上角的值
	{
		for (i = RCrossDown - 3; i > effect + 3; i--)
		{
			if ( (cross::curveSlope(i, RIGHT) > SLOPERIGHTDOWN && cross::curveSlope(i, RIGHT) < SLOPERIGHTUP) )
			{
				RCrossUp = getBigger(effect, i - 2);
				break;
			}
			if (i == effect + 8)
			{
				RCrossUp = effect + 8;
				isDoingR = 1;
				break;
			}
		}
	}
	
	if (LCrossDown != 0 && LCrossUp != 0) //右侧十字计数程序
	{
		for (i = LCrossDown; i > LCrossUp; i--)
		{
			if (lineinf[i].LeftPos == left)
				LCrossCount++;
		}
	}
	
	/*返回最终参数*/
	if ((RCrossCount > 3 && isDoingR == 0) || isDoingR == 1)
	{
		isNormalCross = 1;
		isCross = 1;
	}
	else if ((LCrossCount > 3 && isDoingL == 0) || isDoingL == 1)
	{
		isNormalCross = 1;
		isCross = 1;
	}
	else
		isNormalCross = 0;
	
	if(isNormalCross)
		return 1;
	else
		return 0;
}

int cross::babyFind(int CrossDown, int CrossDownPos, int dir){
	int i;
	int j;
	
	int LeftLimPos = 0;
	int LeftLim = 0;
	int RightLimPos = 0;
	int RightLim = 0;
	int BabyDown = 0;
	int BabyDownPos = 0;
	BabyDownPosBigger = 0;
	BabyDownBigger = 0;
	
	switch(dir)
	{
	case 1:
		for (i = CrossDown; i > up; i--) //LeftLim
		{
			LeftLimPos = CrossDownPos;
			if (picture[i][CrossDownPos] == 0 && picture[i - 1][CrossDownPos] == 255)
			{
				LeftLim = i - 1;
				break;
			}
			if (i == up + 8)
			{
				LeftLim = up + 8;
				break;
			}
		}
		if (LeftLim == effect)
		{
			lineinf[LeftLim].LeftPos = left;
			lineinf[LeftLim].RightPos = right;
		}
		if(getBigger(LeftLimPos + 1, lineinf[LeftLim].LeftPos + 1) >= lineinf[LeftLim].RightPos)
			RightLimPos = LeftLimPos;
		for (i = getBigger(LeftLimPos + 1, lineinf[LeftLim].LeftPos + 1); i < lineinf[LeftLim].RightPos; i++) //RightLim
		{
			if (picture[LeftLim][i] == 255 && picture[LeftLim][i + 1] == 0)
			{
				RightLimPos = i;
				break;
			}
			if (i == lineinf[LeftLim].RightPos - 1)
			{
				RightLimPos = lineinf[LeftLim].RightPos;
				break;
			}
		}
		for (i = LeftLimPos; i <= getSmaller(RightLimPos, 130); i++)
		{
			for (j = LeftLim; j < 45; j++)
			{
				if (picture[j][i] == 255 && picture[j + 1][i] == 0 && picture[LeftLim][i] == 255)
				{
					BabyDownPos = i;
					BabyDown = j;
					break;
				}
			}
			if (BabyDown >= BabyDownBigger)
			{
				BabyDownBigger = BabyDown;
				BabyDownPosBigger = BabyDownPos;
			}
		}
		break;
	case 2:
		for (i = CrossDown; i > up; i--) //LeftLim
		{
			RightLimPos = CrossDownPos;
			if (picture[i][CrossDownPos] == 0 && picture[i - 1][CrossDownPos] == 255)
			{
				RightLim = i - 1;
				break;
			}
			if (i == up + 8)
			{
				RightLim = up + 8;
				break;
			}
		}
		if (RightLim == effect)
		{
			lineinf[RightLim].LeftPos = left;
			lineinf[RightLim].RightPos = right;
		}
		if(getSmaller(RightLimPos - 1, lineinf[RightLim].RightPos - 1) <= lineinf[RightLim].LeftPos)
			LeftLimPos = RightLimPos;
		for (i = getSmaller(RightLimPos - 1, lineinf[RightLim].RightPos - 1); i > lineinf[RightLim].LeftPos; i--) //RightLim
		{
			if (picture[RightLim][i] == 255 && picture[RightLim][i - 1] == 0)
			{
				LeftLimPos = i;
				break;
			}
			if (i == lineinf[RightLim].LeftPos + 1)
			{
				LeftLimPos = lineinf[RightLim].LeftPos;
				break;
			}
		}
		for (i = RightLimPos; i >= getBigger(LeftLimPos, 30); i--)
		{
			for (j = RightLim; j < 45; j++)
			{
				if (picture[j][i] == 255 && picture[j + 1][i] == 0 && picture[RightLim][i] == 255)
				{
					BabyDownPos = i;
					BabyDown = j;
					break;
				}
			}
			if (BabyDown >= BabyDownBigger)
			{
				BabyDownBigger = BabyDown;
				BabyDownPosBigger = BabyDownPos;
			}
		}
		break;
	default:
		break;
	}
	
	if (BabyDownBigger < 30 && BabyDownBigger != 0 && BabyDownPosBigger != 0)
	{
		switch(dir)
		{
		case 1:
			if ((BabyDownPosBigger - lineinf[LCrossDown].LeftPos <= 70 || (lineinf[LCrossDown].LeftPos - BabyDownPosBigger < 4 && lineinf[LCrossDown].LeftPos - BabyDownPosBigger > 0)) && BabyDownPosBigger != right) //36
				return 1;
			else
				return 0;
			break;
		case 2:
			if ((lineinf[RCrossDown].RightPos - BabyDownPosBigger <= 70 || (BabyDownPosBigger - lineinf[RCrossDown].RightPos < 4 && BabyDownPosBigger - lineinf[RCrossDown].RightPos > 0)) && BabyDownPosBigger != left) //36
				return 1;
			else
				return 0;
			break;
		default:
			break;
		}
	}
	else
		return 0;
	
}

void cross::patchNormal(){
	
	int i = 0, j = 0;
	
	if(!isNormalCross)
		return;
	
	if (isDoingR == 0)
	{
		if (RCrossDown != RCrossUp)
			cross::patchRight(RCrossDown, lineinf[RCrossDown].RightPos, RCrossUp - 1, lineinf[RCrossUp - 1].RightPos);
	}
	if (isDoingL == 0)
	{
		if (LCrossDown != LCrossUp)
			cross::patchLeft(LCrossDown, lineinf[LCrossDown].LeftPos, LCrossUp - 1, lineinf[LCrossUp - 1].LeftPos);
	}
	if (isDoingL == 1)
	{
		if ( cross::babyFind(LCrossDown, lineinf[LCrossDown].LeftPos, 1) )
		{
			if (BabyDownBigger > effect + 1 && LCrossDown - BabyDownBigger < 45)
			{
				if ((getAbs(BabyDownPosBigger - lineinf[BabyDownBigger].RightPos) > 3) && BabyDownBigger != LCrossDown)
				{
					cross::patchLeft(LCrossDown, lineinf[LCrossDown].LeftPos, BabyDownBigger, BabyDownPosBigger);
					if (BabyDownPosBigger == right) {}
					else
					{
						lineinf[BabyDownBigger].LeftPos = BabyDownPosBigger;
						for (i = BabyDownBigger; i > effect + 1; i--)
						{
							if (picture[i - 1][lineinf[i].LeftPos] == 255)
							{
								for (j = lineinf[i].LeftPos; j < right; j++)
								{
									if (picture[i][j] == 255 && picture[i][j + 1] == 0)
									{
										lineinf[i - 1].LeftPos = j;
										break;
									}
									if (j == (right - 1))
										lineinf[i - 1].LeftPos = right;
								}
							}
						}
						if (LCrossDown != BabyDownBigger - 2)
							cross::patchLeft(LCrossDown, lineinf[LCrossDown].LeftPos, BabyDownBigger - 2, lineinf[BabyDownBigger - 2].LeftPos);
					}
				}
			}
		}
	}
	if (isDoingR == 1)
	{
		if ( cross::babyFind(RCrossDown, lineinf[RCrossDown].RightPos, 2) )
		{
			if (BabyDownBigger > effect + 1 && RCrossDown - BabyDownBigger < 45)
			{
				if ((getAbs(BabyDownPosBigger - lineinf[BabyDownBigger].LeftPos) > 3) && (BabyDownBigger != RCrossDown))
				{
					cross::patchRight(RCrossDown, lineinf[RCrossDown].RightPos, BabyDownBigger, BabyDownPosBigger);
					if (BabyDownPosBigger == left) {}
					else
					{
						lineinf[BabyDownBigger].RightPos = BabyDownPosBigger;
						for (i = BabyDownBigger; i > effect + 1; i--)
						{
							if (picture[i - 1][lineinf[i].RightPos] == 255)
							{
								for (j = lineinf[i].RightPos; j > left; j--)
								{
									if (picture[i][j] == 255 && picture[i][j - 1] == 0)
									{
										lineinf[i - 1].RightPos = j;
										break;
									}
									if (j == (left + 1))
										lineinf[i - 1].RightPos = left;
								}
							}
						}
						if (RCrossDown != BabyDownBigger - 2)
							cross::patchRight(RCrossDown, lineinf[RCrossDown].RightPos, BabyDownBigger - 2, lineinf[BabyDownBigger - 2].RightPos);
					}
				}
				
			}
		}
	}
	
	
	
	
}

int cross::isFeNormal(){
	
	int i;
	
	int RCount = 0;           //右侧下几个点计数
	int LCount = 0;           //左侧下几个点计数
	LCrossFePos = down;       //我想要的左侧标志点
	RCrossFePos = down;       //我想要的右侧标志点
	
	if (effect > 18)
		return 0;
	
	for (i = down; i > down - 20; i--)
	{
		if (lineinf[i].LeftPos == left && (lineinf[down].LeftPos == left || lineinf[down - 4].LeftPos == left))
			LCount++;
		if (lineinf[i].RightPos == right && (lineinf[down].RightPos == right || lineinf[down - 4].RightPos == right))
			RCount++;
	}
	
	if (LCount == 0 && RCount == 0)
		return 0;
	
	
	if (LCount > 1)  //寻找左侧标志点
	{
		for (i = down; i > getBigger(13, effect + 3); i--)
		{
			if (cross::curveSlope(i, LEFT) > SLOPELEFTDOWN && cross::curveSlope(i, LEFT) < SLOPELEFTUP && lineinf[i].LeftPos != left )
			{
				LCrossFePos = i - 1;
				break;
			}
		}
	}
	if (RCount > 1)  //寻找右侧标志点
	{
		for (i = down; i > getBigger(13, effect + 3); i--)
		{
			if (cross::curveSlope(i, RIGHT) > SLOPERIGHTDOWN && cross::curveSlope(i, RIGHT) < SLOPERIGHTUP && lineinf[i].RightPos != right )
			{
				RCrossFePos = i - 1;
				break;
			}
		}
	}
	
	if(RCrossFePos != down || LCrossFePos != down)
	{
		isFeNormalCross = 1;
		isCross = 1;
		return 1;
	}
	else
	{
		isFeNormalCross = 0;
		return 0;
	}
}

void cross::patchFeNormal(){
	
	if(!isFeNormalCross)
		return;
	
	if (LCrossFePos != down)   //左侧补线
		cross::patchLeft(down, lineinf[down].LeftPos, LCrossFePos, lineinf[LCrossFePos].LeftPos);
	if (RCrossFePos != down)   //右侧补线
		cross::patchRight(down, lineinf[down].RightPos, RCrossFePos, lineinf[RCrossFePos].RightPos);
	
}

void cross::Do(){
	cross::init();
	if(cross::isNormal())
		cross::patchNormal();
	if(cross::isFeNormal())
		cross::patchFeNormal();
}


