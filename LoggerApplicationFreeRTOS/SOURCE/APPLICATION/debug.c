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
};



void debug_ReceptionData_Handler(uint8_t* receptionData)
{
	DEBUG_PRINTF("Processing Command : %s",receptionData);
}



void debugTask()
{

	serialDebugInit();

	serialDebugTransmit("\x1b[2J\x1b[;H");
	DEBUG_PRINT("Serial Debug initialized");

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
	va_list argptr;
	for(int i = 0; i < (sizeof(taskMsgCollection)/sizeof(taskMsgStruct_s)); i++)
	{
		if(taskMsgCollection[i].dbgHeader == dbgHeader)
		{
			serialDebugTransmit(taskMsgCollection[i].stringHeader);
			va_start(argptr, message);
			vsprintf(myData, message, argptr);
			va_end(argptr);
			strcat(myData,"\r\n");
			serialDebugTransmit(myData);
			return;
		}
	}
	serialDebugTransmit("[ UNKNOWN ]\r\n");
}


