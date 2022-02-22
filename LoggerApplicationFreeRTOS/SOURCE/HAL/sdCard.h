/*
 * sdCard.h
 *
 *  Created on: 19-Feb-2022
 *      Author: denzo
 */

#ifndef SDCARD_H_
#define SDCARD_H_


#include "common.h"
#include "debug.h"


#define SDHAL_PRINT(x,...)	debugTransmit(SDHAL_TASKMSG,x)
#define SDHAL_PRINTF(x,...)	debugTransmit(SDHAL_TASKMSG,x,__VA_ARGS__)

void SD_Host_Init(cy_israddress SD_Host_CardDetect_Isr , cy_israddress cardDetectTimer_ISR);
cy_en_sd_host_status_t cardInit();
void SD_printSDHC_1_cardType();
void SD_printSDHC_1_cardCapacity();
void cardDetectTimerInit();




#endif /* SDCARD_H_ */
