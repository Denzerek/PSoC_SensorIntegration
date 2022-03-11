/*
 * dma.h
 *
 *  Created on: 11-Mar-2022
 *      Author: AustinA
 */

#ifndef SOURCE_APPLICATION_DMA_H_
#define SOURCE_APPLICATION_DMA_H_

#include "common.h"

typedef enum
{
	queue_A,
	queue_B
}serialDebugPingPong_t;


void dmaInit();


#endif /* SOURCE_APPLICATION_DMA_H_ */
