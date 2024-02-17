/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 16:13:00 +0800
************************************************************************************************/ 
#ifndef __MCU_RTC_H_
#define __MCU_RTC_H_
#include "DriverHeaderCore.h"

typedef struct RTC_TIMER
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekDay;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
	uint16_t subSecond;
}RTC_TIMER;
#define LENGTH_RTC_TIMER        (sizeof(RTC_TIMER)/sizeof(uint8_t))

//RTC中断回调函数
typedef void (*MCU_RTC_CallBack)(void);

//芯片RTC初始化
ERROR_SUB MCU_RTC_Init(void);

//RTC同步UTC标准时间
ERROR_SUB MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC);

//RTC同步RTC时间
ERROR_SUB MCU_RTC_SyncTimeByRTC(RTC_TIMER* rtcTimerPtr);

//获取RTC内部时间
ERROR_SUB MCU_RTC_GetTimeCurrent(RTC_TIMER* rtcTimerPtr);

//获取RTC内部时间
ERROR_SUB MCU_RTC_GetTimeCurrentUTC(uint32_t* rtcTimerPtr);

//获取驱动中保存的当前时间,获取之前会去读一次
ERROR_SUB MCU_RTC_GetTimeRecordSync(RTC_TIMER** rtcTimerPtrPtr);

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackA);

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmB(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackB);

//周期性唤醒设定
ERROR_SUB MCU_RTC_SetPeriodicWakeUp(uint16_t second,MCU_RTC_CallBack wakeupCallBackPtr);

#endif










