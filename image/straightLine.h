#ifndef _STRAIGHTLINE_H_
#define _STRAIGHTLINE_H_

#include "base.h"

class straightLine{
protected:
	int isStraightLine;
public:	
	int   get(char *str);
	int is(int distance);
	void patch(int dir);
};

#endif