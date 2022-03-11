
#include "softwareTimers.h"
#include "serialDriver.h"


TimerHandle_t debugDmaTimer;




void debugDmaTimer_Handler( TimerHandle_t xTimer )
 {
    const uint32_t ulMaxExpiryCountBeforeStopping = 10;
    uint32_t ulCount;

    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT( xTimer );

    
    switchCurrentSerialQueue();
    dmaStartTransfer();
 }



void softwareTimers_Init()
{
    debugDmaTimer =  xTimerCreate
                 ( "SOFTWARE TIMER 1",
                   pdMS_TO_TICKS( 2000 ),
                   pdTRUE,
                   (void *) 0,
                   debugDmaTimer_Handler );

    if( debugDmaTimer == NULL )
    {
      /* The timer was not created. */
    }
    else
    {
        /* Start the timer.  No block time is specified, and
        even if one was it would be ignored because the RTOS
        scheduler has not yet been started. */
        if( xTimerStart(debugDmaTimer, 0 ) != pdPASS )
        {
            /* The timer could not be set into the Active
            state. */
        }
    }
}