#ifndef RTC_TASK_H_
#define RTC_TASK_H_

#include "common.h"
#include "RTC_driver.h"
#include "debug.h"


#ifdef RTC_TASK_DEBUG_ENABLE
#define RTCTASK_PRINT(...)	debugTransmit(RTCTASK_TASKMSG,__VA_ARGS__)
#else
#define RTCTASK_PRINT(...)	
#endif


void RTCTask();
#endif