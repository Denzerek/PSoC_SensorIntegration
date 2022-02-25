#include "sdCardTask.h"

typedef enum{
    SD_CARD_IDLE,
    SD_CARD_CONNECTIVITY,
    SD_CARD_RW_STATE
}sdTaskState_t;

sdTaskState_t sdTaskState;

void sdCardTask()
{
    
	xEventGroupWaitBits(
            xCreatedEventGroup,   /* The event group being tested. */
            DEBUG_TASK_EVENT_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* Bits should be cleared before returning. */
            pdFALSE,       /* Don't wait for all bits, either bit will do. */
            portMAX_DELAY );


    SDCARD_PRINT("Initializing ...");
    init_sd_card_driver();
            uint32_t test[30] = {1,2,3,4,9,7,5,4,2,4,5,45,4,2,2,3,4};
            uint32_t test1[30];

    sdTaskState = SD_CARD_CONNECTIVITY;
    
    while(1)
    {       
        switch(sdTaskState)
        {
            case SD_CARD_IDLE:
            break;
            case SD_CARD_CONNECTIVITY:
                if(sdCardConnectivityProcess() == CARD_INSERTED_INITIALIZED)
                {
                    sdTaskState = SD_CARD_RW_STATE;
                }
                vTaskDelay(100);
            break;
            case SD_CARD_RW_STATE:
                SDCARD_PRINT("Writing");
                vTaskDelay(2000);
                sdCardWrite(0,test);
                vTaskDelay(2000);
                SDCARD_PRINT("Reading");
                sdCardRead(0,test1);

                // testCard();
                
                sdTaskState = SD_CARD_CONNECTIVITY;

            break;
        } 
    }
}


