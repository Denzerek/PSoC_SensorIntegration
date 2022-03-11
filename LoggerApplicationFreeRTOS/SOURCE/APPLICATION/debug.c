/*
 * debug.c
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */




#include "debug.h"
#include "serialDriver.h"
#include "dma.h"




uint8_t retrievalBuffer[200];


taskMsgStruct_s taskMsgCollection[] = {
		{DEBUG_TASKMSG,"[ DEBUG ] : "},
		{SDCARD_TASKMSG,"[ SD TASK ] : "},
		{SDDRIVER_TASKMSG,"[ SD DRIVER ] : "},
		{SDHAL_TASKMSG,"[ SD HAL ] : "},
		{RTCDRIVER_TASKMSG,"[ RTC DRIVER ] : "},
		{I2CHAL_TASKMSG,"[ I2C HAL] : "},
		{I2CDRIVER_TASKMSG,"[ I2C DRIVER ] : "},
		{RTCTASK_TASKMSG,"[ RTC TASK ] : "}
};



void debug_ReceptionData_Handler(uint8_t* receptionData)
{
	DEBUG_PRINTF("Processing Command : %s",receptionData);
		switchCurrentSerialQueue();
		dmaStartTransfer();
}

char myBuffer[50];
void uartDmaSend(char* data)
{

    sprintf(myBuffer,"\r%s",data);
    dmaStartTransfer();
}


void debugTask()
{
	TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
	serialDebugInit();
#ifdef SERIAL_DEBUG_WITH_DMA
	dmaDriver_init();
#endif

	DEBUG_PRINT("\x1b[2J\x1b[;H");
	DEBUG_PRINT("Serial Debug initialized");
	DEBUG_PRINTF("BUILD DATE : %s",__DATE__);
	DEBUG_PRINTF("BUILD TIME : %s",__TIME__);
	DEBUG_PRINT("*******************************************");
	DEBUG_PRINT("**********  Logger Application ************");
	DEBUG_PRINT("*******************************************");


	DEBUG_PRINTF("Current Ring size %d",getCurrentSerialQueueSize());


	// vTaskDelay(2000);
	xEventGroupSetBits(
				xCreatedEventGroup,    /* The event group being updated. */
				DEBUG_TASK_EVENT_BIT );



    while(1)
    {
    	SerialDebug_TASK();

    	if(serialDebug_RetrieveReceptionData(retrievalBuffer))
    	{
    		debug_ReceptionData_Handler(retrievalBuffer);
    	}
    }
}



void debugTransmit(dbgHeader_t dbgHeader,uint8_t* message, ...)
{

	uint8_t myData[100];
	uint8_t myData1[100];
	va_list argptr;
	for(int i = 0; i < (sizeof(taskMsgCollection)/sizeof(taskMsgStruct_s)); i++)
	{
		if(taskMsgCollection[i].dbgHeader == dbgHeader)
		{
			va_start(argptr, message);
			vsprintf(myData, message, argptr);
			va_end(argptr);

			sprintf(myData1,"%s%s\r\n",taskMsgCollection[i].stringHeader,myData);
			serialDebugTransmit(myData1);
			return;

		}
	}
	serialDebugTransmit("[ UNKNOWN ]\r\n");
}


