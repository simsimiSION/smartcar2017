#ifndef _BASE_H_
#define _BASE_H_

#define up     0
#define down   59
#define left   0
#define right 159

#define img_col    (down - up + 1)
#define img_row    (right - left + 1)

typedef unsigned char uint8;

typedef struct LINE
{
	int LeftPos;      
	int RightPos;   
	int MiddlePos;    
	int TrueLeft;
	int TrueRight;
}LineType;
extern LineType lineinf[60];
extern int effect;

class base{
public:

	base();
	void Do();
	LineType getLineinf();
	void setLineinf(LineType line[]);
	void firstLine();
	void otherLine(int effect);
	int  getEffect();
	void setTrueRL();
	void setMiddleLine();
	
	void patchLeft(int Down, int DownPos, int Up, int UpPos);
	void patchMiddle(int Down, int DownPos, int Up, int UpPos);
	void patchRight(int Down, int DownPos, int Up, int UpPos);
	
};

#endif