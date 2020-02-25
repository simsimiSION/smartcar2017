#ifndef _ZEBARLINE_H_
#define _ZEBARLINE_H_
#include "base.h"

class zebarLine{
protected:
	int distance;
	int isZebarLine;
public:	
	zebarLine();
	int   get(char *str);
	int is();
	int getDistance();
	
	
};


#endif