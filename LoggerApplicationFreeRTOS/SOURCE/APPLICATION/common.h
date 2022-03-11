/*
 * common.h
 *
 *  Created on: 21-Feb-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_COMMON_H_
#define SOURCE_APPLICATION_COMMON_H_




#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdarg.h>
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"


#define UART_TRANSMIT(x)		serialDebugTransmit(x)

#define serialPrint(x)			UART_TRANSMIT(x);

#define false 0
#define true 1

//#define SERIAL_DEBUG_WITH_DMA

#define DEBUG_TASK_EVENT_BIT        ( 1 << 0 )


typedef enum
{
	DEBUG_TASKMSG,
	SDCARD_TASKMSG,
	SDDRIVER_TASKMSG,
	SDHAL_TASKMSG,
	RTCDRIVER_TASKMSG,
	I2CHAL_TASKMSG,
	I2CDRIVER_TASKMSG,
	RTCTASK_TASKMSG
}dbgHeader_t;

typedef struct {
	dbgHeader_t dbgHeader;
	char stringHeader[20];
}taskMsgStruct_s;

extern EventGroupHandle_t xCreatedEventGroup;

#endif /* SOURCE_APPLICATION_COMMON_H_ */
