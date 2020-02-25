#include "mpu6050.h"
#include "myI2C.h"
#include "common.h"


int mpu6050::init(){
	
	uint8 loop = 0;
	this->is = 0;
	kinetis_i2c_bus_init(&(this->bus));
	
	while( 0 == i2c_writeByte(&(this->device), MPU6050_Addr, PWR_MGMT_1, 0x00)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->device), MPU6050_Addr, SMPLRT_DIV, 0x07)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->device), MPU6050_Addr, CONFIG, 0x06)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->device), MPU6050_Addr, GYRO_CONFIG, 0x18)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->device), MPU6050_Addr, ACCEL_CONFIG, 0x01)) {
		loop++;
		if(loop == 5)
			return 0;
	}
	
	this->is = 1;
	return 1;
}

int16_t mpu6050::read_g_x(){
	uint8_t G_T_X1 = 0, G_T_X2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_XOUT_L,&G_T_X1);
	I2CreadByte(MPU6050_Addr,GYRO_XOUT_H,&G_T_X2);
	return (G_T_X2<<8)|G_T_X1;
}
int16_t mpu6050::read_g_y(){
	uint8_t G_T_Y1 = 0, G_T_Y2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_YOUT_L,&G_T_Y1);
	I2CreadByte(MPU6050_Addr,GYRO_YOUT_H,&G_T_Y2);
	return(G_T_Y2<<8)|G_T_Y1;
}
int16_t mpu6050::read_g_z(){
	uint8_t G_T_Z1 = 0, G_T_Z2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_ZOUT_L,&G_T_Z1) ;
	I2CreadByte(MPU6050_Addr,GYRO_ZOUT_H,&G_T_Z2) ;
	return(G_T_Z2<<8)|G_T_Z1; 
}
int16_t mpu6050::read_a_x(){
	uint8_t A_T_X1 = 0, A_T_X2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_XOUT_L,&A_T_X1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_XOUT_H,&A_T_X2) ;
	return(A_T_X2<<8)|A_T_X1;
}
int16_t mpu6050::read_a_y(){
	uint8_t A_T_Y1 = 0, A_T_Y2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_YOUT_L,&A_T_Y1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_YOUT_H,&A_T_Y2) ;
	return(A_T_Y2<<8)|A_T_Y1;
}
int16_t mpu6050::read_a_z(){
	uint8_t A_T_Z1 = 0, A_T_Z2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_ZOUT_L,&A_T_Z1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_ZOUT_H,&A_T_Z2) ;
	return(A_T_Z2<<8)|A_T_Z1;
}

param mpu6050::read(){
	this->mpu_param.a_x = this->read_a_x();
	this->mpu_param.a_y = this->read_a_y();
	this->mpu_param.a_z = this->read_a_z();
	this->mpu_param.g_x = this->read_g_x();
	this->mpu_param.g_y = this->read_g_y();
	this->mpu_param.g_z = this->read_g_z();
	
	return this->mpu_param;
}