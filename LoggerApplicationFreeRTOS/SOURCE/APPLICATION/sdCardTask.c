#include "sdCardTask.h"


void sdCardTask()
{
    
	xEventGroupWaitBits(
            xCreatedEventGroup,   /* The event group being tested. */
            DEBUG_TASK_EVENT_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* Bits should be cleared before returning. */
            pdFALSE,       /* Don't wait for all bits, either bit will do. */
            portMAX_DELAY );


    SDCARD_PRINT("Initializing ...");
    initSDCardProcess();
    
    SDCARD_PRINT("Running ... ");

    while(1)
    {        
        sdCardProcess();
    }
}
