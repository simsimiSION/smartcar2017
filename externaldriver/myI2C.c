#include "myI2C.h"
#include "IIC.h"



static int kinetis_i2c_read(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data){
	return I2CreadBytes(dev, reg, length, data);
}

static int kinetis_i2c_write(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data){
	return I2CwriteBytes(dev, reg, length, data);
}

static uint8_t kinetis_i2c_readByte(uint8_t dev, uint8_t reg, uint8_t data){
    return I2CreadByte(dev, reg, &data);
}

static uint8_t kinetis_i2c_writeByte(uint8_t dev, uint8_t reg, uint8_t data){
	return I2CwriteByte(dev, reg, data);
}

const static struct i2c_ops kinetis_i2c_ops = {
    kinetis_i2c_read,
    kinetis_i2c_write,
	kinetis_i2c_readByte,
    kinetis_i2c_writeByte,
};

int kinetis_i2c_bus_init(struct i2c_bus* bus){
	I2C_init();
	return i2c_bus_register(bus, &kinetis_i2c_ops);
}

