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


#define UART_TRANSMIT(x)		serialDebugTransmit(x)

#define serialPrint(x)			UART_TRANSMIT(x);

#define false 0
#define true 1


typedef enum
{
	DEBUG_TASKMSG
}dbgHeader_t;

typedef struct {
	dbgHeader_t dbgHeader;
	char stringHeader[20];
}taskMsgStruct_s;




#endif /* SOURCE_APPLICATION_COMMON_H_ */
