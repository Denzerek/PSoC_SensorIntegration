

#include "RTCTask.h"


#define checkForHardwareError() {\
                                    rtcProcessPrevState = rtcProcess;\
                                    rtcProcess = RTC_ERROR_CHECK;\
                                }
                                
#define RTC_RTCGlobalTime_NA		"RTC NA"
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
char RTCGlobalTime[20];

void RTCTask()
{

	xEventGroupWaitBits(
            xCreatedEventGroup,   /* The event group being tested. */
            DEBUG_TASK_EVENT_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* Bits should be cleared before returning. */
            pdFALSE,       /* Don't wait for all bits, either bit will do. */
            portMAX_DELAY );


    RTCTASK_PRINT("Initializing ...");
	sprintf( RTCGlobalTime,RTC_RTCGlobalTime_NA);

    rtcProcess = RTC_INIT;
    rtcProcess_t rtcProcessPrevState;
    while(1)
    {

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

    		sprintf( RTCGlobalTime,RTC_RTCGlobalTime_NA);
            checkI2CHardwareErrorStatus();
    		rtcProcess = RTC_IDLE;
        	break;
        case RTC_INIT:
    	    RTCTASK_PRINT("RTC_INIT");
        	if(i2c_driver_init() != CY_SCB_I2C_SUCCESS)
        	{
        		rtcProcess = RTC_INIT_FAILURE;
        		sprintf( RTCGlobalTime,RTC_RTCGlobalTime_NA);
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

        	rtc_get_mmddyyhhmmss_asString(RTCGlobalTime);
        	RTCTASK_PRINT(RTCGlobalTime);
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




