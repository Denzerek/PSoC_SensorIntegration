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
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Initializing ...");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Initializing ...");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Initializing ...");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    SDCARD_PRINT("Running ... ");
    SDCARD_PRINT("Running ...1 ");
    SDCARD_PRINT("Running ...2 ");
    SDCARD_PRINT("Running ...3 ");
    SDCARD_PRINT("Running ...4 ");
    initSDCardProcess();
    

    while(1)
    {        
        sdCardProcess();
    }
}
