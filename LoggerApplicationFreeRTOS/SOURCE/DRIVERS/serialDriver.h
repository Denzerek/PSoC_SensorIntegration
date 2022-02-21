/*
 * serialDriver.h
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_SERIALDRIVER_H_
#define SOURCE_DRIVER_SERIALDRIVER_H_


#include "common.h"


#define RECEPTION_BUFFER_MAX	50
#define _WORKING_SINGLE_SHOT


typedef struct{
	char receptionData[RECEPTION_BUFFER_MAX];
	char previousByte[11];
	uint8_t index;
	uint8_t length;
}reception_s;



void serialDebugInit(void);
void serialDebugTransmit(char* transmitData);
uint8_t serialDebug_RetrieveReceptionData(uint8_t* retrievalBuffer);
void SerialDebug_TASK();

#endif /* SOURCE_DRIVER_SERIALDRIVER_H_ */
