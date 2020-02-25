#include "myMpu6050.h"
#include "myI2C.h"
#include "common.h"

/***************************************************************************/
/*看不懂的一堆参数，先放着吧*/
float PX[2][2] = {{ 1, 0 },
{ 0, 1 }};
float PY[2][2] = {{ 1, 0 },
{ 0, 1 }};
float PdotX[4] ={ 0,0,0,0};
float PdotY[4] ={ 0,0,0,0};
float Q_angle_x=200, Q_gyro_x=0.005; //角度数据置信度,角速度数据置信度
float R_angle_x=300 ,C_0_x = 1; 
float Q_angle_y=200, Q_gyro_y=0.005; //角度数据置信度,角速度数据置信度
float R_angle_y=300 ,C_0_y = 1; 
float q_bias_x, angle_err_x, PCt_0_x, PCt_1_x, E_x, K_0_x, K_1_x, t_0_x, t_1_x;
float q_bias_y, angle_err_y, PCt_0_y, PCt_1_y, E_y, K_0_y, K_1_y, t_0_y, t_1_y;
float dt=0.005;
/*  >_<  */
/***************************************************************************/


/*
*   mpu6050 初始化
*/
int mpu6050::init(){
	
	uint8 loop = 0;
	this->is = 0;
	kinetis_i2c_bus_init(&(this->I2C_bus));
	
	while( 0 == i2c_writeByte(&(this->I2C_bus), MPU6050_Addr, PWR_MGMT_1, 0x00)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->I2C_bus), MPU6050_Addr, SMPLRT_DIV, 0x07)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->I2C_bus), MPU6050_Addr, CONFIG, 0x06)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->I2C_bus), MPU6050_Addr, GYRO_CONFIG, 0x18)){
		loop++;
		if(loop == 5)
			return 0;
	}
	loop = 0;
	while( 0 == i2c_writeByte(&(this->I2C_bus), MPU6050_Addr, ACCEL_CONFIG, 0x01)) {
		loop++;
		if(loop == 5)
			return 0;
	}
	
	this->is = 1;
	return 1;
}

/*
*   mpu6050 g_x
*/
int16_t mpu6050::read_g_x(){
	uint8_t G_T_X1 = 0, G_T_X2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_XOUT_L,&G_T_X1);
	I2CreadByte(MPU6050_Addr,GYRO_XOUT_H,&G_T_X2);
	return (G_T_X2<<8)|G_T_X1;
}

/*
*   mpu6050 g_y
*/
int16_t mpu6050::read_g_y(){
	uint8_t G_T_Y1 = 0, G_T_Y2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_YOUT_L,&G_T_Y1);
	I2CreadByte(MPU6050_Addr,GYRO_YOUT_H,&G_T_Y2);
	return(G_T_Y2<<8)|G_T_Y1;
}

/*
*   mpu6050 g_z
*/
int16_t mpu6050::read_g_z(){
	uint8_t G_T_Z1 = 0, G_T_Z2 = 0;
	I2CreadByte(MPU6050_Addr,GYRO_ZOUT_L,&G_T_Z1) ;
	I2CreadByte(MPU6050_Addr,GYRO_ZOUT_H,&G_T_Z2) ;
	return(G_T_Z2<<8)|G_T_Z1; 
}

/*
*   mpu6050 a_x
*/
int16_t mpu6050::read_a_x(){
	uint8_t A_T_X1 = 0, A_T_X2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_XOUT_L,&A_T_X1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_XOUT_H,&A_T_X2) ;
	return(A_T_X2<<8)|A_T_X1;
}

/*
*   mpu6050 a_y
*/
int16_t mpu6050::read_a_y(){
	uint8_t A_T_Y1 = 0, A_T_Y2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_YOUT_L,&A_T_Y1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_YOUT_H,&A_T_Y2) ;
	return(A_T_Y2<<8)|A_T_Y1;
}

/*
*   mpu6050 a_z
*/
int16_t mpu6050::read_a_z(){
	uint8_t A_T_Z1 = 0, A_T_Z2 = 0;
	I2CreadByte(MPU6050_Addr,ACCEL_ZOUT_L,&A_T_Z1) ;
	I2CreadByte(MPU6050_Addr,ACCEL_ZOUT_H,&A_T_Z2) ;
	return(A_T_Z2<<8)|A_T_Z1;
}

/*
*   mpu6050 初始参数读取
*/
param mpu6050::read(){
	this->mpu_param.a_x = this->read_a_x();
	this->mpu_param.a_y = this->read_a_y();
	this->mpu_param.a_z = this->read_a_z();
	this->mpu_param.g_x = this->read_g_x();
	this->mpu_param.g_y = this->read_g_y();
	this->mpu_param.g_z = this->read_g_z();
	
	return this->mpu_param;
}

/*
*   mpu6050 卡尔曼滤波
*/
angleParam mpu6050::filter(){
	float G_X_S= (float)this->mpu_param.g_x / 18;
	float G_Y_S= (float)this->mpu_param.g_y / 18;
	
	this->filterParam.angle_x += (G_X_S - q_bias_x) ;//* dt;
	angle_err_x = this->mpu_param.a_x - this->filterParam.angle_x;
	PdotX[0]=Q_angle_x - PX[0][1] - PX[1][0];
	PdotX[1]=- PX[1][1];
	PdotX[2]=- PX[1][1];
	PdotX[3]=Q_gyro_x;
	PX[0][0] += PdotX[0] * dt;
	PX[0][1] += PdotX[1] * dt;
	PX[1][0] += PdotX[2] * dt;
	PX[1][1] += PdotX[3] * dt;
	PCt_0_x = C_0_x * PX[0][0];
	PCt_1_x = C_0_x * PX[1][0];
	E_x = R_angle_x + C_0_x * PCt_0_x;
	K_0_x = PCt_0_x / E_x;
	K_1_x = PCt_1_x / E_x;
	t_0_x = PCt_0_x;
	t_1_x = C_0_x * PX[0][1];
	PX[0][0] -= K_0_x * t_0_x;
	PX[0][1] -= K_0_x * t_1_x;
	PX[1][0] -= K_1_x * t_0_x;
	PX[1][1] -= K_1_x * t_1_x;
	this->filterParam.angle_x += K_0_x * angle_err_x; //最优角度
	q_bias_x += K_1_x * angle_err_x;
	this->filterParam.dot_velocity_x = G_X_S - q_bias_x;//最优角速度
	
	this->filterParam.angle_y += (G_Y_S - q_bias_y) ;//* dt;
	angle_err_y = this->mpu_param.a_y - this->filterParam.angle_y;
	PdotY[0]=Q_angle_y - PY[0][1] - PY[1][0];
	PdotY[1]=- PY[1][1];
	PdotY[2]=- PY[1][1];
	PdotY[3]=Q_gyro_y;
	PY[0][0] += PdotY[0] * dt;
	PY[0][1] += PdotY[1] * dt;
	PY[1][0] += PdotY[2] * dt;
	PY[1][1] += PdotY[3] * dt;
	PCt_0_y = C_0_y * PY[0][0];
	PCt_1_y = C_0_y * PY[1][0];
	E_y = R_angle_y + C_0_y * PCt_0_y;
	K_0_y = PCt_0_y / E_y;
	K_1_y = PCt_1_y / E_y;
	t_0_y = PCt_0_y;
	t_1_y = C_0_y * PY[0][1];
	PY[0][0] -= K_0_y * t_0_y;
	PY[0][1] -= K_0_y * t_1_y;
	PY[1][0] -= K_1_y * t_0_y;
	PY[1][1] -= K_1_y * t_1_y;
	this->filterParam.angle_y += K_0_y * angle_err_y; //最优角度
	q_bias_y += K_1_y * angle_err_y;
	this->filterParam.dot_velocity_x = G_Y_S - q_bias_y;//最优角速度
}