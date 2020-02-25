#ifndef _IMGSTH_H_
#define _IMGSTH_H_

#include "base.h"
#include "ramp.h"
#include "straightLine.h"
#include "zebarLine.h"
#include "VCAN_camera.h"


//template<typename figure>
//figure getBigger(figure a, figure b);
//template<typename figure>
//figure getSmaller(figure a, figure b);
#define getBigger(a,b)   ((a) > (b) ? (a) : (b))
#define getSmaller(a,b)  ((a) > (b) ? (b) : (a))
#define getAbs(a)        ((a) > (0) ? (a) : (-a)) 

extern int LimitPr(int a);
extern int getFirstBoundaryFromDown(int dir,int isBoundary);
extern float getTendency(uint8(*picture)[160], int effect, int Dir, int High, int Low);

extern void imgHandle();

extern uint8 picture[60][160];  
extern uint8 *picturess;
extern uint8_t imgbuff[CAMERA_SIZE];  
extern uint8_t img[CAMERA_W*CAMERA_H]; 

#endif