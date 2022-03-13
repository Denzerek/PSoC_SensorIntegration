/*
 * debug.h
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_DEBUG_H_
#define SOURCE_APPLICATION_DEBUG_H_

#include "common.h"


#define DEBUG_PRINT(...)	debugTransmit(DEBUG_TASKMSG,__VA_ARGS__)

void debugTask();
void debugTransmit(dbgHeader_t dbgHeader, ...);

#endif /* SOURCE_APPLICATION_DEBUG_H_ */
