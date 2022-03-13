#include "SDCardDriver.h"

volatile static uint8_t cardDetectFlag;
volatile static sdCardConnectivityProcess_t cardStatus ;
volatile static uint8_t cardDetectTimerExpiredStatus = 1;
sdCardConnectivityProcess_t sdCardConnectivityProcessState = CARD_ABSENT;


void SD_Host_CardDetect_Isr()
{
    if(sdCardConnectivityProcessState != CARD_DETECT_DEBOUNC_WAIT)
	{
        sdCardConnectivityProcessState = CARD_DETECT_ACTIVITY;
    }
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
		sdCardConnectivityProcessState = CARD_INSERT_CHECK;
		cardDetectTimerExpiredStatus = 1;
	}
	/* Clear the interrupt */
	clearCardDetectTimerInterrupts(interrupts);
}

void init_sd_card_driver()
{
    SDDRIVER_PRINT("Initialilzing SD Host");
    SD_Host_Init(&SD_Host_CardDetect_Isr,&cardDetectTimer_ISR);
	sdCardConnectivityProcessState=CARD_INSERT_CHECK;
}

sdCardConnectivityProcess_t sdCardConnectivityProcess()
{
	switch(sdCardConnectivityProcessState)
	{
		case CARD_ABSENT:
			cardStatus = CARD_ABSENT;
            sdCardConnectivityProcessState = CARD_IDLE;
			break;
        case CARD_IDLE:
            taskYIELD();
        break;
		case CARD_DETECT_ACTIVITY:
			SDDRIVER_PRINT(" CARD_DETECT_ACTIVITY");
			CARD_DETECT_TIMER_RUN();
			sdCardConnectivityProcessState = CARD_DETECT_DEBOUNC_WAIT;
			SDDRIVER_PRINT(" CARD_DETECT_DEBOUNC_WAIT");
			break;
		case CARD_DETECT_DEBOUNC_WAIT:
//			SDDRIVER_PRINT(" %d \r\n",Cy_TCPWM_Counter_GetCounter(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM));
			break;
		case CARD_INSERT_CHECK:
			SDDRIVER_PRINT(" CARD_INSERT_CHECK");
			if(Cy_GPIO_Read(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN))
			{
				sdCardConnectivityProcessState = CARD_ABSENT ;
				SDDRIVER_PRINT(" CARD_ABSENT");
			}
			else
			{
				sdCardConnectivityProcessState = CARD_INITIALIZE;
			}
			break;
		case CARD_INITIALIZE:
			SDDRIVER_PRINT(" CARD_INITIALIZE");
			if(cardInit() == CY_SD_HOST_SUCCESS)
			{
                sdCardConnectivityProcessState = CARD_INSERTED_INITIALIZED;
			    cardStatus = CARD_INSERTED_INITIALIZED;
				SDDRIVER_PRINT(" CARD_INSERTED_INITIALIZED");
			}
			else
			{
				sdCardConnectivityProcessState = CARD_ABSENT;
				SDDRIVER_PRINT(" CARD_ABSENT");
			}
			break;
		case CARD_INSERTED_INITIALIZED:
            taskYIELD();
			break;
	}
	return sdCardConnectivityProcessState;
}


void sdCardWrite(uint32_t writeStartAddress,uint32_t* dataToWrite)
{
	/* Note: SD Host and the card must be initialized in ADMA2 or SDMA
		 * mode before using the code below.
		 */
		cy_stc_sd_host_write_read_config_t data;
		uint8_t txBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to write. */
		memset(txBuff, 123, sizeof(txBuff));  /* Fill the array with data to write. */
		data.address = writeStartAddress;         /* The address to write/read data on the card or eMMC. */
		data.numberOfBlocks = 1UL;  /* The number of blocks to write/read (Single block write/read). */
		data.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
		data.dataTimeout = 12UL;     /* The timeout value for the transfer. */
		data.enReliableWrite = false; /* For EMMC cards enable reliable write. */
		data.enableDma = true;  /* Enable DMA mode. */
		data.data = (uint32_t*)txBuff;  /* The pointer to data to write. */
		SDDRIVER_PRINT("Starting Write operation \r\n");
		sdHostWrite(&data);  /* Write data to the card. */
		SDDRIVER_PRINT("SD card write operation complete\r\n");
}



void sdCardRead(uint32_t readStartAddress,uint32_t* dataToRead)
{
	/* Note: SD Host and the card must be initialized in ADMA2 or SDMA
		 * mode before using the code below.
		 */
		cy_stc_sd_host_write_read_config_t data;
		uint8_t rxBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to read. */
		uint8_t tempBuff[CY_SD_HOST_BLOCK_SIZE * 3] = "";   /* Data to write. */
		uint8_t test[5];
		data.address = readStartAddress;         /* The address to write/read data on the card or eMMC. */
		data.numberOfBlocks = 1UL;  /* The number of blocks to write/read (Single block write/read). */
		data.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
		data.dataTimeout = 12UL;     /* The timeout value for the transfer. */
		data.enReliableWrite = false; /* For EMMC cards enable reliable write. */
		data.enableDma = true;  /* Enable DMA mode. */
		data.data = (uint32_t*)rxBuff;  /* The pointer to data to write. */
		SDDRIVER_PRINT("Starting Read operation \r\n");
		sdHostRead(&data);  /* Write data to the card. */
		SDDRIVER_PRINT("SD card read operation complete\r\n");
		SDDRIVER_PRINT("Data Read Back :");
		
		for(int i=0;i<30;i++)
		{
			sprintf(test,"%d ",rxBuff[i]);
			strcat(tempBuff,test);
			if((i%10) == 0)
			{
				SDDRIVER_PRINT(tempBuff);
				strcpy(tempBuff,"");
				vTaskDelay(400);
			}
		}
		SDDRIVER_PRINT("\r\n\n");
}

extern cy_stc_sd_host_context_t sdHostContext;

void testCard()
{
		/* Note: SD Host and the card must be initialized in ADMA2 or SDMA
		 * mode before using the code below.
		 */
		#define DATA       (7U)  /* Data to write. */
		uint8_t tempBuff[CY_SD_HOST_BLOCK_SIZE * 3] = "";   /* Data to write. */
		uint8_t test[5];
		cy_stc_sd_host_write_read_config_t data;
		cy_en_sd_host_status_t ret;
		uint8_t rxBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to read. */
		uint8_t txBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to write. */
		memset(txBuff, 123, sizeof(txBuff));  /* Fill the array with data to write. */
		data.address = 0UL;         /* The address to write/read data on the card or eMMC. */
		data.numberOfBlocks = 1UL;  /* The number of blocks to write/read (Single block write/read). */
		data.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
		data.dataTimeout = 12UL;     /* The timeout value for the transfer. */
		data.enReliableWrite = false; /* For EMMC cards enable reliable write. */
		data.enableDma = true;  /* Enable DMA mode. */
		data.data = (uint32_t*)txBuff;  /* The pointer to data to write. */
		SDDRIVER_PRINT("Starting Write operation \r\n");
		ret = Cy_SD_Host_Write(SDHC1, &data, &sdHostContext);  /* Write data to the card. */
		if (CY_SD_HOST_SUCCESS == ret)
		{
		    while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		    {
		        /* Wait for the data-transaction complete event. */
		    }
		}
		SDDRIVER_PRINT("SD card write operation complete\r\n");
		SDDRIVER_PRINT("Starting read operation \r\n");
		data.data = (uint32_t*)rxBuff;  /* The pointer to data to read. */
		ret = Cy_SD_Host_Read(SDHC1, &data, &sdHostContext);   /* Read data from the card. */
		if (CY_SD_HOST_SUCCESS == ret)
		{
		    while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		    {
		        /* Wait for the data-transaction complete event. */
		    }

		    /* Clear the data-transaction complete event. */
		    Cy_SD_Host_ClearNormalInterruptStatus(SDHC1, CY_SD_HOST_XFER_COMPLETE);
		}

		SDDRIVER_PRINT("Read operation complete\r\n");
		SDDRIVER_PRINT("Data Read Back :");
				vTaskDelay(100);
		// for(int i=0;i<CY_SD_HOST_BLOCK_SIZE;i++)
		// {
		// 	SDDRIVER_PRINT("%d ",rxBuff[i]);
		// 	if((i%10) == 0)
		// 	{
		// 		vTaskDelay(100);
		// 	}
		// }
		// SDDRIVER_PRINT("\r\n\n");


		
		for(int i=0;i<CY_SD_HOST_BLOCK_SIZE;i++)
		{
			sprintf(test,"%d ",rxBuff[i]);
			strcat(tempBuff,test);
			if((i%10) == 0)
			{
				SDDRIVER_PRINT(tempBuff);
				strcpy(tempBuff,"");
				vTaskDelay(400);
			}
		}
		SDDRIVER_PRINT("\r\n\n");
	}
