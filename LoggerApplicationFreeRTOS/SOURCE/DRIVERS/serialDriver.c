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

static volatile bool bUartTxCmpltFlag = true;
ringStruct_s serialRing;
reception_s	receivedData;

#ifdef SERIAL_DEBUG_WITH_DMA


static ringQueue_s ringQueueHandle_A;
static ringQueue_s ringQueueHandle_B;
static ringQueue_s* ringQueueHandle_Current = NULL;
static ringQueue_s* ringQueueHandle_Previous = NULL;

#else
static ringQueue_s ringQueueHandle;
#endif

uint8_t temp[200];




void serialPreviousQueueReset()
{
#ifdef SERIAL_DEBUG_WITH_DMA
	ringQueueReset(ringQueueHandle_Previous);
#endif
}

void switchCurrentSerialQueue()
{
#ifdef SERIAL_DEBUG_WITH_DMA
	ringQueueHandle_Previous = ringQueueHandle_Current;
	if(ringQueueHandle_Current == &ringQueueHandle_A)
	{
		ringQueueHandle_Current = &ringQueueHandle_B;
	}
	else{
		ringQueueHandle_Current = &ringQueueHandle_A;
	}
#endif
}



uint32_t getCurrentSerialQueueSize()
{
#ifdef SERIAL_DEBUG_WITH_DMA
	return getRingQueueCurrentSize(ringQueueHandle_Current);
#else
	return getRingQueueCurrentSize(&ringQueueHandle);
#endif
}



uint32_t* getSerialQueueSourceAddress()
{
#ifdef SERIAL_DEBUG_WITH_DMA
	return &(ringQueueHandle_Current->ringBuffer[0][0]);
#else
	return &(ringQueueHandle.ringBuffer[0][0]);
#endif
}

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

#ifdef SERIAL_DEBUG_WITH_DMA
	ringQueueInit(&ringQueueHandle_A);
	ringQueueInit(&ringQueueHandle_B);
#else
	ringQueueInit(&ringQueueHandle);
#endif

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
				//serialPrint(temp);
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


uint8_t SerialDebug_TASK()
{
	uint8_t allTransmissionComplete = false;
#ifndef SERIAL_DEBUG_WITH_DMA
	if(bUartTxCmpltFlag == true && ringQueueHandle.ringQueueEmptyFlag == false)
	{
		serialDebugTransmit(NULL);
	}
	else
	if (ringQueueHandle.ringQueueEmptyFlag == true)
	{
		allTransmissionComplete = true;
	}
	return allTransmissionComplete;
#endif
}



void serialDebugTransmit(char* transmitData)
{
#ifdef SERIAL_DEBUG_WITH_DMA

	if(ringQueueHandle_Current == NULL)
	{
		switchCurrentSerialQueue();
	}
	if(transmitData != NULL)
	{
		ringQueueStore(ringQueueHandle_Current,transmitData);
	}
	if(getCurrentSerialQueueSize() > 10)// RING_ROW_MAX)
	{
		switchCurrentSerialQueue();
		dmaStartTransfer();
	}

#else
	if(bUartTxCmpltFlag == false)
	{

		if(ringQueueStore(&ringQueueHandle,transmitData) == RING_QUEUE_OVERFLOW)
		{
			while(1)
			{
				if(bUartTxCmpltFlag == true)
				{
					/* Start transmit operation (do not check status) */
					UART_Transmit("[ WARNING ] : OVERFLOW DETECTED in debug\r\n");

					/*set the falg to false after transmission*/
					bUartTxCmpltFlag = false;
					break;
				}
			}
			while(ringQueueHandle.ringQueueEmptyFlag == false)
			{
				if(bUartTxCmpltFlag == true)
				{
					char tempBuffer[RING_MAX];
					QueueRetrieve_ByteArray(&ringQueueHandle,tempBuffer);
					/* Start transmit operation (do not check status) */
					UART_Transmit(tempBuffer);

					/*set the falg to false after transmission*/
					bUartTxCmpltFlag = false;
				}
			}
		}
		return;
	}

	char tempBuffer[RING_MAX];
	if(transmitData != NULL)
	{
		ringQueueStore(&ringQueueHandle,transmitData);
	}
	if(QueueRetrieve_ByteArray(&ringQueueHandle,tempBuffer))
	{
		/* Start transmit operation (do not check status) */
		UART_Transmit(tempBuffer);

		/*set the falg to false after transmission*/
		bUartTxCmpltFlag = false;
	}
#endif
}
