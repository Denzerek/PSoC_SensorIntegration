#ifndef SD_CARD_TASK_H_
#define SD_CARD_TASK_H_

#include "common.h"
#include "debug.h"
#include "SDCardDriver.h"


#define SDCARD_DEBUG_ENABLE

#ifdef SDCARD_DEBUG_ENABLE
#define SDCARD_PRINT(x,...)	    debugTransmit(SDCARD_TASKMSG,x)
#define SDCARD_PRINTF(x,...)	debugTransmit(SDCARD_TASKMSG,x,__VA_ARGS__)
#else
#define SDCARD_PRINT(x,...)
#define SDCARD_PRINTF(x,...)
#endif


void sdCardTask();

#endif