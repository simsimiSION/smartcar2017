#ifndef _CROSS_H_
#define _CROSS_H_
#include "base.h"
#include "imgSth.h"
class cross : public base{
protected:
	int isCross;
	int isNormalCross;
	int isFeNormalCross;
	int LCrossDown;
	int RCrossDown;
	int LCrossUp;
	int RCrossUp;
	int LCrossFePos;
	int RCrossFePos;
	int LEFT[60];
	int RIGHT[60];
	int triFlag;
	int triFlag2;
	int triPos;
	int triDown;
	int isDoingL;
	int isDoingR;
	int BabyDownBigger;
	int BabyDownPosBigger;
public:	
	cross() : base(){
		init();
	}
	void  init();
	int   get(char *str);
	void  set(char *str, int number);
	int   isBoundOver(char pn, int low, int high);
	int   isCurveBaseCross(int dir);
	void  setMatrix();
	int   triFind(int dir);
	float curveSlope(int x, int pos[]);
	int   isNormal();
	int   babyFind(int CrossDown, int CrossDownPos, int dir);
	void  patchNormal();
	int   isFeNormal();
	void  patchFeNormal();
	void  Do();
	
};


#endif