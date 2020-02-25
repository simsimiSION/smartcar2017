#ifndef _RAMP_H_
#define _RAMP_H_

#include "base.h"
#include "straightLine.h"
extern int structRamp;

class ramp: public straightLine{
protected:
	int isRamp;
public:	
	ramp() : straightLine(){
		isRamp = 0;
	}
	int   get(char *str);
	int is();
	void Do();
};

#endif