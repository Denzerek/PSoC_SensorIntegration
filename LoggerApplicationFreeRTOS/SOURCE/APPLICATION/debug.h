/*
 * debug.h
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_DEBUG_H_
#define SOURCE_APPLICATION_DEBUG_H_

#include "common.h"



#define DEBUG_PRINT(x,...)	debugTransmit(DEBUG_TASKMSG,x)
#define DEBUG_PRINTF(x,...)	debugTransmit(DEBUG_TASKMSG,x,__VA_ARGS__)


void debugTransmit(dbgHeader_t dbgHeader,uint8_t* message, ...);
void debugTask();

#endif /* SOURCE_APPLICATION_DEBUG_H_ */
