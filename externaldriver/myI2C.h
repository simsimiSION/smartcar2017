#ifndef _MYI2C_H_
#define _MYI2C_H_

#include "IIC.h"

#define I2C_EOK         (0x00)
#define I2C_ERROR       (0x01)

#define MY_I2C_HW   HW_GPIOC
#define MY_I2C_SCL  7
#define MY_I2C_SDA  5


typedef struct i2c_bus *i2c_bus_t;
struct i2c_bus{
	const struct i2c_ops *ops;
	struct i2c_device *owner;
};

struct i2c_ops
{
    int (*read)(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
    int (*write)( uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
	uint8_t (*readByte)(uint8_t dev, uint8_t reg, uint8_t data);
	uint8_t (*writeByte)(uint8_t dev, uint8_t reg, uint8_t data);
};

static inline int i2c_bus_register(struct i2c_bus *bus, const struct i2c_ops *ops){
    if(bus && ops)
    {
        bus->ops = ops;
        return I2C_EOK;
    }
    return I2C_ERROR;
}

static inline int i2c_read(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data){
	uint32_t ret;
	
   	ret = bus->ops->read(dev, reg, length, data);
    return ret;
}

static inline int i2c_write(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data){
	uint32_t ret;

   	ret = bus->ops->write(dev, reg, length, data);
    return ret;
}

static inline uint8_t i2c_readByte(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t data){
	uint32_t ret;

   	ret = bus->ops->readByte(dev, reg, data);
    return ret;
}

static inline uint8_t i2c_writeByte(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t data){
	uint32_t ret;

   	ret = bus->ops->writeByte(dev, reg, data);
    return ret;
}


/*
 * API
*/
int i2c_bus_register(struct i2c_bus *bus, const struct i2c_ops *ops);
static inline int  i2c_read(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
static inline int  i2c_write(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
static inline uint8_t  i2c_readByte(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t data); 
static inline uint8_t  i2c_writeByte(i2c_bus_t bus, uint8_t dev, uint8_t reg, uint8_t data);
extern int kinetis_i2c_bus_init(struct i2c_bus* bus);
#endif