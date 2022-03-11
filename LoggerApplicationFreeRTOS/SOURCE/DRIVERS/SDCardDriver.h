

#ifndef SDCARDDRIVER_H_
#define SDCARDDRIVER_H_



#include "common.h"
#include "sdCard.h"
#include "debug.h"


#define SDCARD_DRIVER_DEBUG_ENABLE

#ifdef SDCARD_DRIVER_DEBUG_ENABLE
#define SDDRIVER_PRINT(x,...)	debugTransmit(SDDRIVER_TASKMSG,x)
#define SDDRIVER_PRINTF(x,...)	debugTransmit(SDDRIVER_TASKMSG,x,__VA_ARGS__)
#else
#define SDDRIVER_PRINT(x,...)	
#define SDDRIVER_PRINTF(x,...)
#endif


///* Then start the counter */
#define CARD_DETECT_TIMER_RUN()		Cy_TCPWM_TriggerStart_Single(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM)

#define getCardDetectTimerInterrupt()	 Cy_TCPWM_GetInterruptStatusMasked(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM);

#define clearCardDetectTimerInterrupts(x)	 Cy_TCPWM_ClearInterrupt(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM, x)

typedef enum{
	CARD_ABSENT,
    CARD_IDLE,
	CARD_DETECT_DEBOUNC_WAIT,
	CARD_DETECT_ACTIVITY,
	CARD_INSERT_CHECK,
	CARD_INITIALIZE,
	CARD_INSERTED_INITIALIZED,
}sdCardConnectivityProcess_t;

void init_sd_card_driver();
sdCardConnectivityProcess_t sdCardConnectivityProcess();
void testCard();
void sdCardRead(uint32_t readStartAddress,uint32_t* dataToRead);
void sdCardWrite(uint32_t writeStartAddress,uint32_t* dataToWrite);

#endif