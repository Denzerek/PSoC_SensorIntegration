/*
 * timerDriver.h
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_TIMERDRIVER_H_
#define SOURCE_DRIVER_TIMERDRIVER_H_



#include "common.h"
#include "timer.h"


bool getScreenUpdateTimerStatus();
void startScreenUpdateTimer();

bool getrandomTimerStatus();
void startrandomTimer(uint32_t timeInMs);

void timerDriver_Init();

#endif /* SOURCE_DRIVER_TIMERDRIVER_H_ */
