

#ifndef RTC_DRIVER_H_
#define RTC_DRIVER_H_



#include "common.h"
#include "i2c_driver.h"
#include "debug.h"


#ifdef RTC_DRIVER_DEBUG_ENABLE
#define RTCDRIVER_PRINT(...)	debugTransmit(RTCDRIVER_TASKMSG,__VA_ARGS__)
#else
#define RTCDRIVER_PRINT(...)	
#endif

#define RTC_SLAVE_ADDRESS       0x68U
#define RTC_SECONDS_REGISTER    0x00U
#define RTC_MINUTES_REGISTER    0x01U
#define RTC_HOURS_REGISTER      0x02U
#define RTC_DAY_REGISTER        0x03U
#define RTC_DATE_REGISTER        0x04U
#define RTC_MONTH_REGISTER      0x05U
#define RTC_YEAR_REGISTER      0x06U




typedef struct{
    uint8_t seconds:4;
    uint8_t tenthSecond:4;
    uint8_t minutes:4;
    uint8_t tenthMinutes:4;
    uint8_t hour:4;
    uint8_t tenthHour:1;
    uint8_t am_pm_20:1;
    uint8_t hour_12_24:2;
    uint8_t day:3;
    uint8_t :5;
    uint8_t date:4;
    uint8_t tenthDate:4;
    uint8_t month:4;
    uint8_t tenthMonth:1;
    uint8_t :2;
    uint8_t century:1;
    uint8_t year:4;
    uint8_t tenthYear:4;
}RTCTimeRegisterStruct_s;

typedef struct{
    uint8_t minutes:4;
    uint8_t tenthMinutes:4;
}RTCMinutes_s;


typedef struct{
    uint8_t year:4;
    uint8_t tenthYear:4;
}RTCYear_s;

typedef struct{
    uint8_t Day:3;
    uint8_t :5;
}RTCDay_s;

typedef struct{
    uint8_t seconds:4;
    uint8_t tenthSeconds:4
}RTCSeconds_s;

typedef struct{
    uint8_t date:4;
    uint8_t tenthDate:2;
    uint8_t :2;
}RTCDate_s;

typedef struct{
    uint8_t month:4;
    uint8_t tenthMonth:1;
    uint8_t :2;
    uint8_t century:1;
}RTCMonth_s;


typedef struct{
    uint8_t hour:4;
    uint8_t tenthHour:1;
    uint8_t hour_20_am_pm:1;
    uint8_t hour_12_24_format:1;
    uint8_t :1;
}RTCHour_s;


void RTCDRIVER();
uint32_t rtc_getMonth();
uint32_t rtc_getYear();
uint8_t rtc_getDate();
uint8_t rtc_getDay();
uint8_t rtc_getHours();
uint8_t rtc_getMinutes();
uint8_t rtc_getSeconds();
uint8_t rtc_displayAllTime();
void rtc_setCustom(RTCTimeRegisterStruct_s customTime);
void rtc_getAllRegister(RTCTimeRegisterStruct_s *customTime);
void rtc_Reset();
uint8_t rtc_getHours();
void rtc_setTime(uint8_t yyyy,uint8_t mm, uint8_t dd,uint8_t d,uint8_t hr,uint8_t min,uint8_t sec);

#endif
