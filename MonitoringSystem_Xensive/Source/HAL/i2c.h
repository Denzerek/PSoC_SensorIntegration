/*
 * i2c.h
 *
 *  Created on: 06-Jul-2022
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_I2C_H_
#define SOURCE_DRIVER_I2C_H_

#include "common.h"


/*******************************************************************************
* Macros
*******************************************************************************/
#define OVERSAMPLING            7
#define I2C_MASTER_FREQUENCY    400000

void i2cInit(cyhal_i2c_t *);


#endif /* SOURCE_DRIVER_I2C_H_ */
