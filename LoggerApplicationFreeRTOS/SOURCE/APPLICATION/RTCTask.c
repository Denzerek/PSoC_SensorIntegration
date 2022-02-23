

#include "RTCTask.h"



uint8_t readBuffer[20];
uint8_t writeBuffer[20];


typedef union{
    RTCTimeRegisterStruct_s time;
    uint8_t timeBuffer[sizeof(RTCTimeRegisterStruct_s)];
}time_u;


void RTCTask()
{
    
	xEventGroupWaitBits(
            xCreatedEventGroup,   /* The event group being tested. */
            DEBUG_TASK_EVENT_BIT, /* The bits within the event group to wait for. */
            pdFALSE,        /* Bits should be cleared before returning. */
            pdFALSE,       /* Don't wait for all bits, either bit will do. */
            portMAX_DELAY );


    RTCTASK_PRINT("Initializing ...");
    i2c_driver_init();

    while(1)
    {
        vTaskDelay(1000);
        RTCTASK_PRINTF("\r%d Year %d month %d Date %d Day %d hr %d min %d  sec",rtc_getYear(),rtc_getMonth(),rtc_getDate(),rtc_getDay(),rtc_getHours(),rtc_getMinutes(),rtc_getSeconds());
        vTaskDelay(1000);
    }
}

uint8_t rtc_getSeconds()
{
    RTCSeconds_s RTCSeconds;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_SECONDS_REGISTER,&RTCSeconds);
    return (RTCSeconds.tenthSeconds* 10+ RTCSeconds.seconds );
}

uint8_t rtc_getMinutes()
{
    RTCMinutes_s RTCMinutes;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_MINUTES_REGISTER,&RTCMinutes);
    return (RTCMinutes.tenthMinutes* 10+ RTCMinutes.minutes );
}

uint8_t rtc_getHours()
{
    RTCHour_s RTCHour;
    uint8_t hourTemp;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_HOURS_REGISTER,&RTCHour);
    hourTemp = RTCHour.tenthHour;
    if((RTCHour.hour_20_am_pm) && !(RTCHour.hour_12_24_format))
        hourTemp = 2;
    return ( hourTemp* 10 + RTCHour.hour);
}

uint8_t rtc_getDay()
{
    RTCDay_s RTCDay;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_DAY_REGISTER,&RTCDay);
    return RTCDay.Day;
}

uint8_t rtc_getDate()
{
    RTCDate_s RTCDate;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_DATE_REGISTER,&RTCDate);
    return ( RTCDate.tenthDate * 10 + RTCDate.date);
}

uint32_t rtc_getYear()
{
    RTCYear_s RTCYear;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_YEAR_REGISTER,&RTCYear);
    return ( 2000 + RTCYear.tenthYear * 10 + RTCYear.year);
}

uint32_t rtc_getMonth()
{
    RTCMonth_s RTCMonth;
    i2c_readByte(RTC_SLAVE_ADDRESS,RTC_MONTH_REGISTER,&RTCMonth);
    return ( RTCMonth.tenthMonth * 10 + RTCMonth.month);
}


void rtc_getAllRegister(RTCTimeRegisterStruct_s *customTime)
{
    i2c_readBurst(RTC_SLAVE_ADDRESS,writeBuffer,1,customTime,sizeof(RTCTimeRegisterStruct_s));
}
void rtc_setCustom(RTCTimeRegisterStruct_s customTime)
{
    i2c_writeBurst(RTC_SLAVE_ADDRESS,RTC_SECONDS_REGISTER,&customTime,sizeof(customTime));
}

void rtc_setTime(uint8_t yyyy,uint8_t mm, uint8_t dd,uint8_t d,uint8_t hr,uint8_t min,uint8_t sec)
{
    
    RTCTimeRegisterStruct_s customTime;
    rtc_getAllRegister(&customTime);

    customTime.hour = hr %10;
    customTime.tenthHour = hr /10;

    customTime.minutes = min %10;
    customTime.tenthMinutes = min /10;

    customTime.seconds = sec %10;
    customTime.tenthSecond = sec /10;

    // customTime.hour_12_24 = 0;
    // customTime.am_pm_20 = 1;

    customTime.month = mm %10;
    customTime.tenthMonth = mm/10;

    customTime.day = d;

    customTime.date = dd %10;
    customTime.tenthDate = dd/10;

    yyyy -= 2000;
    customTime.year = yyyy % 10;
    customTime.tenthYear = yyyy / 10 ;
    rtc_setCustom(customTime);
}

void rtc_Reset()
{
    static int onceflag;
    if(onceflag) return;
    onceflag ++;
    RTCTimeRegisterStruct_s time = {
    .seconds= 0,
    .tenthSecond= 0,
    .minutes= 0,
    .tenthMinutes= 0,
    .hour= 0,
    .tenthHour= 0,
    .am_pm_20= 0,
    .day=0,
    .date= 0,
    .tenthDate= 0,
    .month= 0,
    .tenthMonth= 0,
    .century= 0,
    };
    writeBuffer[0] = RTC_SECONDS_REGISTER;
    i2c_writeBurst(RTC_SLAVE_ADDRESS,RTC_SECONDS_REGISTER,&time,sizeof(time));
}

uint8_t rtc_displayAllTime()
{
    RTCTimeRegisterStruct_s time;
    writeBuffer[0] = RTC_SECONDS_REGISTER;
    i2c_readBurst(RTC_SLAVE_ADDRESS,writeBuffer,1,(uint8_t*)&time,sizeof(time));

    // printf("\r %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",time.century,
    //         time.tenthMonth,time.month,time.tenthDate,time.date,time.day,time.hour_12_24,
    //         time.am_pm,time.tenthHour,time.hour,time.tenthMinutes,time.minutes,
    //         time.tenthSecond,time.seconds);
            
    RTCTASK_PRINTF("%d %d %d %d %d %d %d %d       Time  = %d%d : %d%d : %d%d  ",time.century,
            time.tenthMonth,time.month,time.tenthDate,time.date,time.day,time.hour_12_24,
            time.am_pm_20,   time.tenthHour,time.hour,time.tenthMinutes,time.minutes,
            time.tenthSecond,time.seconds);
            return 0;

}



// uint8_t rtc_getSeconds()
// {
//     uint8_t tenthSec;
//     uint8_t Sec;

//     writeBuffer[0] = RTC_SECONDS_REGISTER;
//     i2c_readBurst(0x68,writeBuffer,1,readBuffer,4);

//     Sec = readBuffer[0] %10;
//     tenthSec = ((readBuffer[0] >> 4 ) * 10 );
//     return (tenthSec+Sec);
// }

// uint8_t rtc_getMinutes()
// {
//     uint8_t tenthhour;
//     uint8_t hour;

//     typedef union{
//         typedef struct{
//             uint8_t 
//         }
//     };

//     writeBuffer[0] = RTC_SECONDS_REGISTER;
//     i2c_readBurst(0x68,writeBuffer,1,readBuffer,4);

//     Sec = readBuffer[0] %10;
//     tenthSec = ((readBuffer[0] >> 4 ) * 10 );
//     return (tenthSec+Sec);
// }

