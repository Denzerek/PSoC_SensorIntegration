#pragma once

#include "common.h"

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


uint8_t rtc_getSeconds();
uint8_t rtc_getMinutes();
uint8_t rtc_displayAllTime();
void rtc_setCustom(RTCTimeRegisterStruct_s customTime);
void rtc_getAllRegister(RTCTimeRegisterStruct_s *customTime);
void rtc_Reset();
uint8_t rtc_getHours();
void rtc_setTime(uint8_t yyyy,uint8_t mm, uint8_t dd,uint8_t d,uint8_t hr,uint8_t min,uint8_t sec);

