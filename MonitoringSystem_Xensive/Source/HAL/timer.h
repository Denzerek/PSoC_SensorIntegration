/*
 * timer.h
 *
 *  Created on: 19-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_HAL_TIMER_H_
#define SOURCE_HAL_TIMER_H_

#include "common.h"


void timerInit(cyisraddress Timer_Isr);
void clearTimerInterrupt(uint32_t intrSource);

#endif /* SOURCE_HAL_TIMER_H_ */
