#pragma once

#include "common.h"
#include "debug.h"
#include "i2c.h"

#define I2C_DRIVER_DEBUG_ENABLE

#ifdef I2C_DRIVER_DEBUG_ENABLE
#define I2CDRIVER_PRINT(x,...)	debugTransmit(I2CDRIVER_TASKMSG,x)
#define I2CDRIVER_PRINTF(x,...)	debugTransmit(I2CDRIVER_TASKMSG,x,__VA_ARGS__)
#else
#define I2CDRIVER_PRINT(x,...)	
#define I2CDRIVER_PRINTF(x,...)
#endif


typedef enum
{
    I2C_SCB3
}scbI2Chw_t;

void i2c_driver_init();

void i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData);
void i2c_readBurst(uint8_t slaveAddress,uint8_t* registerAddressBuffer,uint8_t registerAddressSize,uint8_t* readDataBuffer,uint8_t readSize);

void i2c_writeByte(uint8_t address,uint8_t registerAddress,uint8_t writeData);
void i2c_writeBurst(uint8_t address,uint8_t registerAddress,uint8_t* writeData,uint8_t size);



