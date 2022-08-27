/*
 * debug.c
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */




#include "debug.h"
#include "serialDriver.h"
#include "dma.h"
#include "RTC_driver.h"
extern char RTCGlobalTime[20];


uint8_t retrievalBuffer[200];


taskMsgStruct_s taskMsgCollection[] = {
		{DEBUG_TASKMSG,			"[ DEBUG ] : "		},
		{SDCARD_TASKMSG,		"[ SD TASK ] : "	},
		{SDDRIVER_TASKMSG,		"[ SD DRIVER ] : "	},
		{SDHAL_TASKMSG,			"[ SD HAL ] : "		},
		{RTCDRIVER_TASKMSG,		"[ RTC DRIVER ] : "	},
		{I2CHAL_TASKMSG,		"[ I2C HAL] : "		},
		{I2CDRIVER_TASKMSG,		"[ I2C DRIVER ] : "	},
		{RTCTASK_TASKMSG,		"[ RTC TASK ] : "	}
};




typedef struct{
	char command[20];
	uint8_t (*commandHandler)();
}commandStruct_s;

commandStruct_s commandHolder[]={
		{"rtc_reset",rtc_Reset}
};


void debug_ReceptionData_Handler(uint8_t* receptionData)
{
	int i;
	if(*receptionData == '\0' || *receptionData == '\r' || *receptionData == '\n')
		return;
	for(i = 0;receptionData[i] != '\0' && receptionData[i] != '\r' && receptionData[i] != '\n';i++){}
	receptionData[i] = '\0';

	DEBUG_PRINT("Command = %s",receptionData);
	for(int i = 0 ; i < sizeof(commandHolder)/sizeof(commandStruct_s);i++)
	{
		if(!strcmp(commandHolder[i].command,receptionData))
		{
			DEBUG_PRINT("Processing...");

			if(commandHolder[i].commandHandler())
				DEBUG_PRINT("FAILED");
			else
				DEBUG_PRINT("DONE");
		}
	}
	*receptionData = '\0';
	dmaStartTransfer();
	switchCurrentSerialQueue();
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
	DEBUG_PRINT("BUILD DATE : %s",__DATE__);
	DEBUG_PRINT("BUILD TIME : %s",__TIME__);
	DEBUG_PRINT("*******************************************");
	DEBUG_PRINT("**********  Logger Application ************");
	DEBUG_PRINT("*******************************************");


	DEBUG_PRINT("");

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


#ifdef TEST

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

#else
void debugTransmit(dbgHeader_t dbgHeader, ...)
{

	uint8_t myData[100];
	uint8_t myData1[100];
	va_list argptr;
	va_start(argptr, dbgHeader);
	for(int i = 0; i < (sizeof(taskMsgCollection)/sizeof(taskMsgStruct_s)); i++)
	{
		if(taskMsgCollection[i].dbgHeader == dbgHeader)
		{
			vsprintf(myData, va_arg(argptr,char *), argptr);
			va_end(argptr);
#ifdef DEBUG_DISPLAY_DATE_TIME
			sprintf(myData1,"%s %s%s\r\n",RTCGlobalTime,taskMsgCollection[i].stringHeader,myData);
#else
			sprintf(myData1,"%s%s\r\n",taskMsgCollection[i].stringHeader,myData);
#endif
			serialDebugTransmit(myData1);
			return;

		}
	}
	serialDebugTransmit("[ UNKNOWN ]\r\n");
}


#endif
