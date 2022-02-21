/*
 * serialDriver.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */



#include "common.h"
#include "serialDriver.h"
#include "ringBuffer.h"
#include "uart.h"

bool bUartTxCmpltFlag = true;
ringStruct_s serialRing;
reception_s	receivedData;
ringQueue_s ringQueueHandle;
uint8_t temp[200];

void ReceptionCallback(uint32_t event)
{

	switch(event)
	{
	case CY_SCB_UART_RECEIVE_NOT_EMTPY:
			ringStore_Byte(&serialRing,(uint8_t)UART_Receive());
		break;
		case CY_SCB_UART_RECEIVE_DONE_EVENT :
		break;
		case CY_SCB_UART_TRANSMIT_DONE_EVENT:


			//Set the transmission complete flag
			bUartTxCmpltFlag = true;
			break;
	}

}

void serialDebugInit()
{
	UART_Init(ReceptionCallback);

	ringBufferInit(&serialRing);

	ringQueueInit(&ringQueueHandle);

}


uint8_t serialDebug_RetrieveReceptionData(uint8_t* retrievalBuffer)
{
	uint8_t commandReceived = false;

	if(serialRing.ringBufferEmptyFlag == false)
	{
#ifdef _WORKING_SINGLE_SHOT
		receivedData.length = ringRetrieve_ByteArray(&serialRing,&receivedData.previousByte,10);
		for(int index = 0;index < receivedData.length; index++)
		{
			if(receivedData.previousByte[index] != '\r' || receivedData.previousByte[index] != '\n')
			{
				sprintf(temp,"%c",receivedData.previousByte[index]);
				serialPrint(temp);
			}
			receivedData.receptionData[receivedData.index++] = receivedData.previousByte[index];
			if(receivedData.previousByte[index] == '\r' || receivedData.previousByte[index] == '\n' || receivedData.index >= (RECEPTION_BUFFER_MAX - 2))
			{
				receivedData.receptionData[receivedData.index++] = (char)'\r';
				receivedData.receptionData[receivedData.index++] = (char)'\n';
				receivedData.receptionData[receivedData.index] = (char)'\0';
//				serialPrint((char*)&receivedData.receptionData);
				strcpy(retrievalBuffer,receivedData.receptionData);
				receivedData.index = 0;
				receivedData.receptionData[receivedData.index] = (char)'\0';
				commandReceived = true;
			}
		}
#endif
	}
	return commandReceived;

}

void SerialDebug_TASK()
{

	if(bUartTxCmpltFlag == true && ringQueueHandle.ringQueueEmptyFlag == false)
	{
		char tempBuffer[50];
		QueueRetrieve_ByteArray(&ringQueueHandle,tempBuffer);
		serialDebugTransmit(tempBuffer);
	}


}


void serialDebugTransmit(char* transmitData)
{
	if(bUartTxCmpltFlag == false)
	{
		ringQueueStore(&ringQueueHandle,transmitData);
		return;
	}

	/* Start transmit operation (do not check status) */
	UART_Transmit(transmitData);

	/*set the falg to false after transmission*/
	bUartTxCmpltFlag = false;
}
