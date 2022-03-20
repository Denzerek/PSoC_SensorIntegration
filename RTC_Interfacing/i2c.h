#pragma once

#include "common.h"
void i2c_readByte(uint8_t address,uint8_t registerAddress,uint8_t* readData);
void i2c_readBurst(uint8_t slaveAddress,uint8_t* registerAddressBuffer,uint8_t registerAddressSize,uint8_t* readDataBuffer,uint8_t readSize);

void i2c_writeByte(uint8_t address,uint8_t registerAddress,uint8_t writeData);
void i2c_writeBurst(uint8_t address,uint8_t registerAddress,uint8_t* writeData,uint8_t size);