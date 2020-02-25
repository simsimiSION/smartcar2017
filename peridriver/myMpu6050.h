#ifndef _MYMPU6050_H_
#define _MYMPU6050_H_

#include "myI2C.h"

// MPU6050
//****************************************
#define	SMPLRT_DIV		0x19	
#define	CONFIG			0x1A	
#define	GYRO_CONFIG		0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	
#define	WHO_AM_I		0x75	

#define	MPU6050_Addr   0xD0
//****************************************

typedef struct parameter{
  float g_x;
  float g_y;
  float g_z;
  float a_x;
  float a_y;
  float a_z;
	
}param;

typedef struct angle_param{
  float dot_velocity_x;
  float angle_x;
  float dot_velocity_y;
  float angle_y;
  float dot_velocity_z;
  float angle_z;
}angleParam;


class mpu6050{
public:
	int is;
	struct i2c_bus I2C_bus;
	param mpu_param;
	angleParam filterParam;

public:
	int init();
	int16_t read_g_x();
	int16_t read_g_y();
	int16_t read_g_z();
	int16_t read_a_x();
	int16_t read_a_y();
	int16_t read_a_z();
	param read();
	angleParam filter();
};



#endif