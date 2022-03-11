/*
 * ringBuffer.h
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_RINGBUFFER_R_H_
#define SOURCE_DRIVER_RINGBUFFER_R_H_

#include "common.h"

#define RING_MAX		90
#define RING_ROW_MAX	40
#define RING_COL_MAX	RING_MAX

//Ring is for reception
#define FACE_R			ring->ringFace
#define TAIL_R			ring->ringTail
#define BUFFER_R		ring->ringBuffer
#define OVERFLOW_R		ring->ringOverflow
#define EMPTYFLAG_R		ring->ringBufferEmptyFlag

//Queue is for debug
#define FACE_Q			ringQueue->ringFace
#define TAIL_Q			ringQueue->ringTail
#define BUFFER_Q		ringQueue->ringBuffer
#define OVERFLOW_Q		ringQueue->ringOverflow
#define EMPTYFLAG_Q		ringQueue->ringQueueEmptyFlag

#define ringRetrieveByte(x,y)	ringRetrieve_ByteArray(x,y,1)

typedef struct{
	uint32_t ringFace;
	uint32_t ringTail;
	uint8_t ringBuffer[RING_MAX];
	bool ringOverflow;
	bool ringBufferEmptyFlag;
}ringStruct_s;


typedef enum{
	RING_QUEUE_NORMAL,
	RING_QUEUE_OVERFLOW
}ringQueueStatus_t;

typedef struct{
	uint32_t ringFace;
	uint32_t ringTail;
	uint8_t ringBuffer[RING_ROW_MAX][RING_COL_MAX];
	bool ringOverflow;
	bool ringQueueEmptyFlag;
}ringQueue_s;


uint8_t ringRetrieve_ByteArray(ringStruct_s *ring, uint8_t* byteArray,uint8_t arraySize);
void ringStore_Byte(ringStruct_s *ring, uint8_t dataToStore);
void ringBufferInit(ringStruct_s *ring);

uint8_t QueueRetrieve_ByteArray(ringQueue_s *ringQueue, uint8_t* byteArray);
ringQueueStatus_t ringQueueStore(ringQueue_s *ringQueue, char* dataToStore);
void ringQueueInit(ringQueue_s *ringQueue);
uint32_t getRingQueueCurrentSize(ringQueue_s *ringQueue);



#endif /* SOURCE_DRIVER_RINGBUFFER_R_H_ */
