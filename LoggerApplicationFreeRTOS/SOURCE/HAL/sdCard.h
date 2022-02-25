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


typedef struct{
    uint32_t sdCardEvent;
    char* sdCardMessage;
}sdCardEvents_s;

void SD_Host_Init(cy_israddress SD_Host_CardDetect_Isr , cy_israddress cardDetectTimer_ISR);
cy_en_sd_host_status_t cardInit();
void SD_printSDHC_1_cardType();
void SD_printSDHC_1_cardCapacity();
void cardDetectTimerInit();
cy_en_sd_host_status_t sdHostWrite(cy_stc_sd_host_write_read_config_t* data);
cy_en_sd_host_status_t sdHostRead(cy_stc_sd_host_write_read_config_t* data);




#endif /* SDCARD_H_ */
