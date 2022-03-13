

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
	RTC_TIME_VALIDITY_CHECK,
    RTC_ERROR_CHECK,
	RTC_READ_APPLICATION_TIME,
	RTC_SET_VALID_TIME,
}rtcProcess_t;


void setRTCToBuildTime();
static rtcProcess_t rtcProcess = RTC_IDLE;
char RTCGlobalTime[20];


typedef enum{
	Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec,MONTH_MAX
}months_e;

typedef struct {
	months_e month;
	char string[4];
}months_s;
months_s monthArray[] = {
		{Jan,"Jan"},{Feb,"Feb"},{Mar,"Mar"},{Apr,"Apr"},{May,"May"},
		{Jun,"Jun"},{Jul,"Jul"},{Aug,"Aug"},{Sep,"Sep"},{Oct,"Oct"},{Nov,"Nov"},
		{Dec,"Dec"},{MONTH_MAX,"\0"},
};

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
            taskYIELD();
            if(rtcProcessPrevState == RTC_COMM_ERROR)
            {
                clearI2CErrorFlag();
                rtcProcess = RTC_TIME_VALIDITY_CHECK;
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
            rtcProcess = RTC_TIME_VALIDITY_CHECK;
            break;
        case RTC_INIT_FAILURE:
    	    RTCTASK_PRINT("RTC_INIT_FAILURE");
    		rtcProcess = RTC_IDLE;
        	break;
        case RTC_TIME_VALIDITY_CHECK:
    	    RTCTASK_PRINT("RTC_TIME_VALIDITY_CHECK");
        	if(rtc_getMonth() == 0 || rtc_getDate() == 0 || rtc_getYear == 2000)
        	{
        		rtcProcess = RTC_SET_VALID_TIME;
        	}
        	else
        	{
        		rtcProcess = RTC_READ_APPLICATION_TIME;
        	}
            checkForHardwareError();

        	break;
        case RTC_SET_VALID_TIME:
    	    RTCTASK_PRINT("RTC_SET_VALID_TIME");
        	setRTCToBuildTime();
    		rtcProcess = RTC_READ_APPLICATION_TIME;
        	break;
        case RTC_READ_APPLICATION_TIME:
        	rtc_get_mmddyyhhmmss_asString(RTCGlobalTime);
        	//RTCTASK_PRINT(RTCGlobalTime);
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


void setRTCToBuildTime()
{
	RTCTimeRegisterStruct_s RTCTimeRegisterStruct;
	char temp[5] = __DATE__;
	temp[3] = '\0';
	for(int i = 0; monthArray[i].month < MONTH_MAX ; i++ )
	{
		// Set to build date
		if(!strcmp(monthArray[i].string, temp))
		{
			RTCTimeRegisterStruct.month = (monthArray[i].month + 1) % 10;
			RTCTimeRegisterStruct.tenthMonth = (monthArray[i].month + 1) / 10;
		}
	}
	strcpy(temp,&(__DATE__[4]));
	temp[1] = '\0';
	RTCTimeRegisterStruct.tenthDate = atoi(temp);
	strcpy(temp,&(__DATE__[5]));
	temp[1] = '\0';
	RTCTimeRegisterStruct.date = atoi(temp);
	strcpy(temp,&(__DATE__[9]));
	temp[1] = '\0';
	RTCTimeRegisterStruct.year = atoi(temp);
	strcpy(temp,&(__DATE__[10]));
	temp[1] = '\0';
	RTCTimeRegisterStruct.tenthYear = atoi(temp);


	strcpy(temp,&(__TIME__));
	temp[2] = '\0';
	RTCTimeRegisterStruct.tenthHour = atoi(temp);
	strcpy(temp,&(__TIME__[1]));
	temp[2] = '\0';
	RTCTimeRegisterStruct.hour = atoi(temp);


	strcpy(temp,&(__TIME__[3]));
	temp[2] = '\0';
	RTCTimeRegisterStruct.tenthMinutes = atoi(temp);
	strcpy(temp,&(__TIME__[4]));
	temp[2] = '\0';
	RTCTimeRegisterStruct.minutes = atoi(temp);

	strcpy(temp,&(__TIME__[6]));
	temp[2] = '\0';
	RTCTimeRegisterStruct.tenthSecond = atoi(temp);
	strcpy(temp,&(__TIME__[7]));
	temp[2] = '\0';
	RTCTimeRegisterStruct.seconds = atoi(temp);

	rtc_setCustom(RTCTimeRegisterStruct);

}


