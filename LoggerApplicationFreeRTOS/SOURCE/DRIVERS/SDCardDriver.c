#include "SDCardDriver.h"

volatile static uint8_t cardDetectFlag;
volatile static sdCardProcess_t cardStatus ;
volatile static uint8_t cardDetectTimerExpiredStatus = 1;
sdCardProcess_t sdCardProcessState = CARD_ABSENT;


void SD_Host_CardDetect_Isr()
{
	sdCardProcessState = CARD_DETECT_ACTIVITY;
	cardDetectFlag = 1;
	CARD_DETECT_TIMER_RUN();
	Cy_GPIO_ClearInterrupt(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN);
}


void cardDetectTimer_ISR()
{
	uint32_t interrupts = getCardDetectTimerInterrupt();
	if (0UL != (CY_TCPWM_INT_ON_TC & interrupts))
	{
		/* Handle the Terminal Count event */
		SDDRIVER_PRINT("Timer interrupt TC");
		sdCardProcessState = CARD_INSERT_CHECK;
		cardDetectTimerExpiredStatus = 1;
	}
	/* Clear the interrupt */
	clearCardDetectTimerInterrupts(interrupts);
}

void initSDCardProcess()
{
    SDDRIVER_PRINT("Initialilzing SD Host");
    SD_Host_Init(&SD_Host_CardDetect_Isr,&cardDetectTimer_ISR);
	sdCardProcessState=CARD_INSERT_CHECK;
}

void sdCardProcess()
{
	switch(sdCardProcessState)
	{
		case CARD_ABSENT:
			cardStatus = CARD_ABSENT;
            sdCardProcessState = CARD_IDLE;
			break;
        case CARD_IDLE:
        break;
		case CARD_DETECT_ACTIVITY:
			SDDRIVER_PRINT(" CARD_DETECT_ACTIVITY");
			CARD_DETECT_TIMER_RUN();
			sdCardProcessState = CARD_DETECT_DEBOUNC_WAIT;
			SDDRIVER_PRINT(" CARD_DETECT_DEBOUNC_WAIT");
			break;
		case CARD_DETECT_DEBOUNC_WAIT:
//			SDDRIVER_PRINT(" %d \r\n",Cy_TCPWM_Counter_GetCounter(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM));
			break;
		case CARD_INSERT_CHECK:
			SDDRIVER_PRINT(" CARD_INSERT_CHECK");
			if(Cy_GPIO_Read(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN))
			{
				sdCardProcessState = CARD_ABSENT ;
				SDDRIVER_PRINT(" CARD_ABSENT");
			}
			else
			{
				sdCardProcessState = CARD_INITIALIZE;
			}
			break;
		case CARD_INITIALIZE:
			SDDRIVER_PRINT(" CARD_INITIALIZE");
			if(cardInit() == CY_SD_HOST_SUCCESS)
			{
				sdCardProcessState = CARD_INSERTED_INITIALIZED;
			    cardStatus = CARD_INSERTED_INITIALIZED;
				SDDRIVER_PRINT(" CARD_INSERTED_INITIALIZED");
			}
			else
			{
				sdCardProcessState = CARD_ABSENT;
				SDDRIVER_PRINT(" CARD_ABSENT");
			}
			break;
		case CARD_INSERTED_INITIALIZED:
			break;
	}
            vTaskDelay(100);
}
