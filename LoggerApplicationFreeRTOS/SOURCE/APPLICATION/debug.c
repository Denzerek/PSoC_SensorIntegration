/*
 * debug.c
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */




#include "debug.h"
#include "serialDriver.h"




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
}



void debugTask()
{
	TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
	serialDebugInit();

	serialDebugTransmit("\x1b[2J\x1b[;H");
	DEBUG_PRINT("Serial Debug initialized");

	while(!SerialDebug_TASK()){}

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

