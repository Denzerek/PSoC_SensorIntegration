/*
 * sdCard.c
 *
 *  Created on: 19-Feb-2022
 *      Author: denzo
 */
#include "sdCard.h"



/* Assign SDHC_1_HW interrupt number and priority */
#define SD_Host_INTR_PRIORITY   (3UL)


/* Allocate context for SD Host operation */
cy_stc_sd_host_context_t sdHostContext;


/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t sdHostIntrConfig =
{
            #if (CY_CPU_CORTEX_M0P)
                 /* .intrSrc */ NvicMux4_IRQn,
                /* .cm0pSrc */ SDHC_1_IRQ,
            #else
                 /* .intrSrc */ SDHC_1_IRQ, /* SD Host interrupt number (non M0 core)*/
            #endif
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};



/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t sdHostCardDetectIntrConfig =
{
		 /* .intrSrc */ SDHC1_CARD_DETECT_N_NUM_IRQ,
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};

/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t cardDetectTimerIntr =
{
		 /* .intrSrc */ CARD_DETECT_TIMER_IRQ,
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};









bool Cy_SD_Host_IsCardConnected(SDHC_Type const * 	base	)
{
	return true;
}


void SD_Host_User_Isr(void)
{
    volatile uint32_t normalStatus;
    volatile uint32_t errorStatus;

    normalStatus = Cy_SD_Host_GetNormalInterruptStatus(SDHC_1_HW);

    /* Check the Error event */
    switch(normalStatus)
    {
		case CY_SD_HOST_CARD_INSERTION:
			SDHAL_PRINT(" SD Card Inserted");
			break;
		case CY_SD_HOST_CARD_REMOVAL:
			SDHAL_PRINT(" SD Card Removed");
			break;
		default:
			SDHAL_PRINT(" Unknown Interrupt %d\r\n",normalStatus);

			break;
    }

    if (0u < normalStatus)
    {
        /* Clear the normalStatus event */
        Cy_SD_Host_ClearNormalInterruptStatus(SDHC_1_HW, normalStatus);
    }

    errorStatus = Cy_SD_Host_GetErrorInterruptStatus(SDHC_1_HW);
    /* Check the Error event */
    switch(errorStatus)
   {
		default:
			SDHAL_PRINT(" Unknown Interrupt %ud\r\n",errorStatus);
			break;
   }
    if (0u < errorStatus)
    {
        /* Clear the Error event */
        Cy_SD_Host_ClearErrorInterruptStatus(SDHC_1_HW, errorStatus);
    }

    /* Add the use code here. */
}

void SD_Host_Init(cy_israddress SD_Host_CardDetect_Isr , cy_israddress cardDetectTimer_ISR)
{
	cy_en_sd_host_status_t status;

	 Cy_SD_Host_Enable(SDHC_1_HW);

	 /* Generate an interrupt on SD card insertion or removal */
	 Cy_SD_Host_SetNormalInterruptMask(SDHC_1_HW, (CY_SD_HOST_CARD_INSERTION | CY_SD_HOST_CARD_REMOVAL));

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&sdHostCardDetectIntrConfig, SD_Host_CardDetect_Isr);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(sdHostCardDetectIntrConfig.intrSrc);
	#endif

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&sdHostIntrConfig, &SD_Host_User_Isr);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(sdHostIntrConfig.intrSrc);
	#endif

	/* Configure SD Host to operate */
	status = Cy_SD_Host_Init(SDHC_1_HW, &SDHC_1_config, &sdHostContext);

	if(status != CY_SD_HOST_SUCCESS)
	{
		SDHAL_PRINT(" SD Host Init Failure %d\r\n",status);
		return;
	}

	cardDetectTimerInit(cardDetectTimer_ISR);
}



void cardDetectTimerInit(cy_israddress cardDetectTimer_ISR)
{
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM, &CARD_DETECT_TIMER_config))
    {
        /* Handle possible errors */
    }


	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&cardDetectTimerIntr, cardDetectTimer_ISR);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(cardDetectTimerIntr.intrSrc);
	#endif

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM);
}



void decode(cy_en_sd_host_status_t status)
{

	if((status & CY_SD_HOST_ERROR)==CY_SD_HOST_ERROR)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR ");
	}
	if((status & CY_SD_HOST_ERROR_INVALID_PARAMETER)==CY_SD_HOST_ERROR_INVALID_PARAMETER)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_INVALID_PARAMETER ");

	}
	if((status & CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS)==CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS ");

	}
	if((status & CY_SD_HOST_ERROR_UNINITIALIZED)==CY_SD_HOST_ERROR_UNINITIALIZED)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_UNINITIALIZED ");

	}
	if((status & CY_SD_HOST_ERROR_TIMEOUT)==CY_SD_HOST_ERROR_TIMEOUT)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_TIMEOUT ");
	}
	if((status & CY_SD_HOST_OPERATION_INPROGRESS)==CY_SD_HOST_OPERATION_INPROGRESS)
	{
		SDHAL_PRINT("CY_SD_HOST_OPERATION_INPROGRESS ");
	}
	if((status & CY_SD_HOST_ERROR_UNUSABLE_CARD)==CY_SD_HOST_ERROR_UNUSABLE_CARD)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_UNUSABLE_CARD ");
	}
	if((status & CY_SD_HOST_ERROR_DISCONNECTED)== CY_SD_HOST_ERROR_DISCONNECTED)
	{
		SDHAL_PRINT("CY_SD_HOST_ERROR_DISCONNECTED ");
	}
}


cy_en_sd_host_status_t cardInit()
{
	cy_en_sd_host_status_t status;

	/* Initialize the card */
	status = Cy_SD_Host_InitCard(SDHC_1_HW, &SDHC_1_card_cfg, &sdHostContext);
	if(status != CY_SD_HOST_SUCCESS)
	{
		SDHAL_PRINT(" SDCard initialization Error %d",status);
		decode(status);
		return status;
	}

	// SD_printSDHC_1_cardType();
	// SD_printSDHC_1_cardCapacity();
	SDHAL_PRINT(" SDCard initialized");
	return status;



}



void SD_printSDHC_1_cardCapacity()
{
	switch(*SDHC_1_card_cfg.cardCapacity)
	{
		case CY_SD_HOST_SDSC:
			SDHAL_PRINT(" Card Capacity :  SDSC - Secure Digital Standard Capacity (up to 2 GB).");
				break;
	//		case CY_SD_HOST_SDHC:
	//			SDHAL_PRINT(" Card Capacity :  SDHC - Secure Digital High Capacity (up to 32 GB).");break;
	//		case CY_SD_HOST_EMMC_LESS_2G:
	//			SDHAL_PRINT(" Card Capacity :  The eMMC block addressing for less than 2GB.");break;
		case CY_SD_HOST_EMMC_GREATER_2G:
			SDHAL_PRINT(" Card Capacity :  The eMMC block addressing for greater than 2GB.");break;
		case CY_SD_HOST_SDXC:
			SDHAL_PRINT(" Card Capacity :  SDXC - Secure Digital Extended Capacity (up to 2 TB).");break;
		case CY_SD_HOST_UNSUPPORTED:
			SDHAL_PRINT(" Card Capacity :  Not supported.");break;
	}
}

void SD_printSDHC_1_cardType()
{
	switch(*SDHC_1_card_cfg.cardType)
	{
		case CY_SD_HOST_SD:
			SDHAL_PRINT(" Card Type : The Secure Digital card (SD).");break;
		case CY_SD_HOST_SDIO:
			SDHAL_PRINT(" Card Type : The CD Input Output card (SDIO).");break;
		case CY_SD_HOST_EMMC:
			SDHAL_PRINT(" Card Type : The Embedded Multimedia card (eMMC).");break;
		case CY_SD_HOST_COMBO:
			SDHAL_PRINT(" Card Type : The Combo card (SD + SDIO).");break;
		case CY_SD_HOST_UNUSABLE:
			SDHAL_PRINT(" Card Type : The unusable or not supported.");break;
		case CY_SD_HOST_NOT_EMMC:
			SDHAL_PRINT(" Card Type : The card is not eMMC.");break;
	}
}









#if 0
void checkCardStatus()
{
	if(cardDetectFlag  && cardDetectTimerExpiredStatus)
	{
		cardDetectFlag = 0;
		cardDetectTimerExpiredStatus = 0;

		if(Cy_GPIO_Read(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN))
		{
			SDHAL_PRINT(" Card Removed");
			cardStatus = 0;
		}
		else
		{
			SDHAL_PRINT(" Card Inserted");
			if(cardInit() == CY_SD_HOST_SUCCESS)
			{
				cardStatus = 1;
			}
			else
			{
				cardStatus = 0;
				SDHAL_PRINT("In error");
			}
		}
	}

	if(cardStatus)
	{

		/* Note: SD Host and the card must be initialized in ADMA2 or SDMA
		 * mode before using the code below.
		 */
		#define DATA       (7U)  /* Data to write. */
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
		SDHAL_PRINT("Starting Write operation ");
		ret = Cy_SD_Host_Write(SDHC1, &data, &sdHostContext);  /* Write data to the card. */
		if (CY_SD_HOST_SUCCESS == ret)
		{
		    while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		    {
		        /* Wait for the data-transaction complete event. */
		    }
		}
		SDHAL_PRINT("SD card write operation complete");
		SDHAL_PRINT("Starting read operation ");
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

		SDHAL_PRINT("Read operation complete");
		SDHAL_PRINT("Data Read Back :");
		for(int i=0;i<CY_SD_HOST_BLOCK_SIZE;i++)
		{
			SDHAL_PRINT("%d ",rxBuff[i]);
		}
		SDHAL_PRINT("\r\n\n");
		CyDelay(2000);
	}

#if 0
	uint32_t count;
	SDHAL_PRINT("CARD_DETECT_DEBOUNCE %d\r\n",CARD_DETECT_DEBOUNCE);
	CyDelay(100);
#endif
}

#endif