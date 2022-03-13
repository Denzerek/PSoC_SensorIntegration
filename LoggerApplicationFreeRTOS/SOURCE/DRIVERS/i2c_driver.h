#pragma once

#include "common.h"
#include "debug.h"
#include "i2c.h"


#ifdef I2C_DRIVER_DEBUG_ENABLE
#define I2CDRIVER_PRINT(...)	debugTransmit(I2CDRIVER_TASKMSG,__VA_ARGS__)
#else
#define I2CDRIVER_PRINT(...)
#endif


typedef enum
{
    I2C_SCB3
}scbI2Chw_t;

typedef enum{
    I2C_DRIVER_RW_SUCCESS,
    I2C_DRIVER_RW_FAILED = 0xFF,
}i2cDriverState_t;


cy_en_scb_i2c_status_t i2c_driver_init();

bool getI2CErrorFlag();
void clearI2CErrorFlag();
uint32_t checkI2CHardwareErrorStatus();


i2cDriverState_t i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData);
i2cDriverState_t i2c_readBurst(uint8_t slaveAddress,uint8_t* registerAddressBuffer,uint8_t registerAddressSize,uint8_t* readDataBuffer,uint8_t readSize);

i2cDriverState_t i2c_writeByte(uint8_t address,uint8_t registerAddress,uint8_t writeData);
i2cDriverState_t i2c_writeBurst(uint8_t address,uint8_t registerAddress,uint8_t* writeData,uint8_t size);



