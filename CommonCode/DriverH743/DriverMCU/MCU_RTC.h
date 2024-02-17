/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_RTC_H_
#define __MCU_RTC_H_
#include "DriverHeaderCore.h"

//RTC相关数据
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

//关闭闹钟A
ERROR_SUB MCU_RTC_DisableAlarmA(void);

//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmB(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackB);

//关闭闹钟B
ERROR_SUB MCU_RTC_DisableAlarmB(void);

//周期性唤醒设定
ERROR_SUB MCU_RTC_SetPeriodicWakeUp(uint16_t second,MCU_RTC_CallBack wakeupCallBackPtr);

//关闭周期唤醒
ERROR_SUB MCU_RTC_DisablePeriodicWakeUp(void);


#endif












