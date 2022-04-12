/*
 * dps3xxDriver.h
 *
 *  Created on: 28-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_DPS3XXDRIVER_H_
#define SOURCE_DRIVER_DPS3XXDRIVER_H_

#include "common.h"


uint8_t getDps3xx_Temp_pressure(float * temperature, float * pressure);
uint8_t getDPS3xxRevision();
void dps3xx_Init();

#endif /* SOURCE_DRIVER_DPS3XXDRIVER_H_ */
