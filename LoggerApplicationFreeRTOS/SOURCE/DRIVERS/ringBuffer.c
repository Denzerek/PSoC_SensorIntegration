/*
 * ringBuffer.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */


#include "ringBuffer.h"
#include "stdlib.h"


/*
 * ==================================================================================================================
 * =                                         Queue / 2-D RING BUFFER
 * ==================================================================================================================
 */

void ringQueueInit(ringQueue_s *ringQueue)
{
	FACE_Q = 0;
	TAIL_Q = 0;
	OVERFLOW_Q = false;
	EMPTYFLAG_Q = true;
}

ringQueueStatus_t ringQueueStore(ringQueue_s *ringQueue, char* dataToStore)
{

	strcpy( BUFFER_Q[FACE_Q] , dataToStore);
	FACE_Q ++;
	EMPTYFLAG_Q = false;
	
	if( OVERFLOW_Q && (FACE_Q == TAIL_Q ))
	{
		return RING_QUEUE_OVERFLOW;
	}


	if(FACE_Q >= RING_ROW_MAX)
	{
		if(TAIL_Q == 0)
		{
			FACE_Q = RING_ROW_MAX - 1;
			return RING_QUEUE_OVERFLOW;
		}
		FACE_Q = 0;
		if(FACE_Q < TAIL_Q)
			OVERFLOW_Q = true;
	}

	return RING_QUEUE_NORMAL;

}




uint8_t QueueRetrieve_ByteArray(ringQueue_s *ringQueue, uint8_t* byteArray)
{
	if(EMPTYFLAG_Q == true || ( FACE_Q == 0 && TAIL_Q == 0))
	{
		serialPrint("[WARNING] : Ring Queue underflow1\r\n");
		return 0;
	}

//	if(OVERFLOW_Q)
//	{
//
//	}
//	else
	{
		strcpy(byteArray,BUFFER_Q[TAIL_Q]);
		TAIL_Q ++;
	}

	if((TAIL_Q == FACE_Q ) && ( OVERFLOW_Q == false))
	{
		TAIL_Q = FACE_Q = 0;
		
		EMPTYFLAG_Q = true;
	}

	if(TAIL_Q >= RING_ROW_MAX  )
	{
		TAIL_Q = 0;
		if(FACE_Q == 0)
		{
			EMPTYFLAG_Q = true;
		}
		else if(TAIL_Q < FACE_Q)
		OVERFLOW_Q = false;
	}
}

/*
 * ==================================================================================================================
 * =                                         1-D RING BUFFER
 * ==================================================================================================================
 */

#if 1
void ringBufferInit(ringStruct_s *ring)
{
	FACE_R = 0;
	TAIL_R = 0;
	OVERFLOW_R = false;
	EMPTYFLAG_R = true;
}


void ringStore_Byte(ringStruct_s *ring, uint8_t dataToStore)
{
	if( OVERFLOW_R && (FACE_R > TAIL_R) )
	{
		serialPrint("[WARNING] : Ring Buffer overflow\r\n");
		return;
	}

	BUFFER_R[FACE_R] = dataToStore;
	FACE_R++;

	EMPTYFLAG_R = false;

	if(FACE_R >= RING_MAX)
	{
		FACE_R = 0;
		OVERFLOW_R = true;
	}

}

/*
 * Test Cases
 *
 * RETRIEVAL
 * 		arraySize	Tail	FACE_R	OVERFLOW_R	EMPTYFLAG_R	RingMax		Status		Output
 * 1.	5			0		0		0			1			15			Correct		Underflow warning
 * 2.	5			0		1		0			0			15			Correct		returns 1 data
 * 3.	5			13		2		1			0			15			Correct
 * 3.	5			13		5		1			0			15			Correct
 * 4.	5			7		5		1			0			15			Correct
 * 5.	1			0		1		0			1			15
 *
 */
uint8_t ringRetrieve_ByteArray(ringStruct_s *ring, uint8_t* byteArray,uint8_t arraySize)
{
	if(EMPTYFLAG_R == true || ( FACE_R == 0 && TAIL_R == 0))
	{
		serialPrint("[WARNING] : Ring Buffer underflow\r\n");
		return 0;
	}

	if(!arraySize || arraySize >= RING_MAX ||  (!OVERFLOW_R && (( FACE_R - TAIL_R ) <= arraySize)))
	{
		if(OVERFLOW_R)
		{
			arraySize = FACE_R - TAIL_R + RING_MAX;

			memcpy(byteArray,&BUFFER_R[TAIL_R],RING_MAX);

			if(FACE_R)
			{
				memcpy(&byteArray[RING_MAX - TAIL_R],&BUFFER_R[0],FACE_R);
			}
		}
		else
		{
			arraySize = FACE_R - TAIL_R;
			memcpy(byteArray,&BUFFER_R[TAIL_R],arraySize);
		}
		EMPTYFLAG_R = true;
		OVERFLOW_R = false;
		TAIL_R = FACE_R = 0;
		return arraySize ;
	}
	if(OVERFLOW_R)
	{

		if(arraySize > RING_MAX - TAIL_R)
		{
			memcpy(byteArray,&BUFFER_R[TAIL_R],(RING_MAX - TAIL_R));
			if((TAIL_R + arraySize - RING_MAX) > FACE_R)
			{
				memcpy(&byteArray[RING_MAX - TAIL_R],&BUFFER_R[0],FACE_R);
				FACE_R = TAIL_R = 0;
				OVERFLOW_R = false;
				EMPTYFLAG_R = true;
				return arraySize;
			}
			else
			{
				memcpy(&byteArray[RING_MAX - TAIL_R],&BUFFER_R[0],TAIL_R + arraySize - RING_MAX);
			}
			OVERFLOW_R = false;
			TAIL_R = TAIL_R + arraySize - RING_MAX;
			return arraySize;
		}
		else
		{
			memcpy(byteArray,&BUFFER_R[TAIL_R],arraySize);
			TAIL_R = arraySize + TAIL_R;
			return arraySize;
		}

	}
	memcpy(byteArray,&BUFFER_R[TAIL_R],arraySize);
	TAIL_R = arraySize + TAIL_R;
	return arraySize;
}

#endif