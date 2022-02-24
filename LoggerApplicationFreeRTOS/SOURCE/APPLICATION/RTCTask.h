#ifndef RTC_TASK_H_
#define RTC_TASK_H_

#include "common.h"
#include "RTC_driver.h"
#include "debug.h"

#define RTC_TASK_DEBUG_ENABLE

#ifdef RTC_TASK_DEBUG_ENABLE
#define RTCTASK_PRINT(x,...)	debugTransmit(RTCTASK_TASKMSG,x)
#define RTCTASK_PRINTF(x,...)	debugTransmit(RTCTASK_TASKMSG,x,__VA_ARGS__)
#else
#define RTCTASK_PRINT(x,...)	
#define RTCTASK_PRINTF(x,...)
#endif


void RTCTask();
#endif