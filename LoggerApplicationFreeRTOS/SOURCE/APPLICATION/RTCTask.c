

#include "RTCTask.h"


#define checkForHardwareError() {\
                                    rtcProcessPrevState = rtcProcess;\
                                    rtcProcess = RTC_ERROR_CHECK;\
                                }
                                

typedef enum
{
	RTC_IDLE,
	RTC_COMM_ERROR,
	RTC_INIT,
	RTC_INIT_FAILURE,
	RTC_INIT_SUCCESS,
	RTC_READ,
    RTC_ERROR_CHECK,
}rtcProcess_t;


static rtcProcess_t rtcProcess = RTC_IDLE;

void RTCTask()
{
    
	xEventGroupWaitBits(
            xCreatedEventGroup,   /* The event group being tested. */
            DEBUG_TASK_EVENT_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* Bits should be cleared before returning. */
            pdFALSE,       /* Don't wait for all bits, either bit will do. */
            portMAX_DELAY );


    RTCTASK_PRINT("Initializing ...");

    rtcProcess = RTC_INIT;
    rtcProcess_t rtcProcessPrevState;
    while(1)
    {
        // vTaskDelay(1000);
        // RTCTASK_PRINTF("%d Year %d month %d Date %d Day %d hr %d min %d  sec",rtc_getYear(),rtc_getMonth(),rtc_getDate(),rtc_getDay(),rtc_getHours(),rtc_getMinutes(),rtc_getSeconds());
        // rtc_displayAllTime();
        // vTaskDelay(1000);

        switch(rtcProcess)
        {
        case RTC_IDLE:
            vTaskDelay(100);
            if(rtcProcessPrevState == RTC_COMM_ERROR)
            {
                clearI2CErrorFlag();
                rtcProcess = RTC_READ;
            }
        	break;
        case RTC_COMM_ERROR:
    	    RTCTASK_PRINT("RTC_COMM_ERROR:Please check hardware connections");
            checkI2CHardwareErrorStatus();
    		rtcProcess = RTC_IDLE;
        	break;
        case RTC_INIT:
    	    RTCTASK_PRINT("RTC_INIT");
        	if(i2c_driver_init() != CY_SCB_I2C_SUCCESS)
        	{
        		rtcProcess = RTC_INIT_FAILURE;
                break;
        	}
            rtcProcess = RTC_INIT_SUCCESS;
        	break;
        case RTC_INIT_SUCCESS:
    	    RTCTASK_PRINT("RTC_INIT_SUCCESS");
            rtcProcess = RTC_READ;
            break;
        case RTC_INIT_FAILURE:
    	    RTCTASK_PRINT("RTC_INIT_FAILURE");
    		rtcProcess = RTC_IDLE;
        	break;
        case RTC_READ:

            rtc_displayAllTime();
            checkForHardwareError();
            vTaskDelay(1000);
        	break;
        case RTC_ERROR_CHECK:
            if(getI2CErrorFlag())
            {
    		    rtcProcess = rtcProcessPrevState = RTC_COMM_ERROR;
                break;
            }
            rtcProcess = rtcProcessPrevState;
            break;

        }
    }
}




