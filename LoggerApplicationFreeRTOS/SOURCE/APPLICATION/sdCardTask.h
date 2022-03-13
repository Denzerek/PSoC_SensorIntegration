#ifndef SD_CARD_TASK_H_
#define SD_CARD_TASK_H_

#include "common.h"
#include "debug.h"
#include "SDCardDriver.h"



#ifdef SDCARD_DEBUG_ENABLE
#define SDCARD_PRINT(...)	debugTransmit(SDCARD_TASKMSG,__VA_ARGS__)
#else
#define SDCARD_PRINT(...)
#define SDCARD_PRINT(...)
#endif


void sdCardTask();

#endif