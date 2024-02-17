/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_HEART_H_
#define __SRV_IMPL_BASE_HEART_H_
#include "SrvImplBase.h"


#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != CONFIG_FUNC_DISABLE)
//线程信息统计任务初始化
void SrvImplHeartSubThreadPerformanceStateInit(void);
//线程信息统计任务周期运转
void SrvImplHeartSubThreadPerformanceStateRunPeriod(void);
#endif

#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//系统RTC时间显示初始化
void SrvImplHeartSubRtcTimerShowInit(void);
//系统RTC时间显示周期运行
void SrvImplHeartSubRtcTimerShowRunPeriod(void);
#endif

#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//系统内存使用率展示初始化
void SrvImplHeartSubMemUsageShowInit(void);
//系统内存使用率展示周期运行
void SrvImplHeartSubMemUsageShowRunPeriod(void);
#endif

#if(SRV_HEART_IWDG_ENABLE != CONFIG_FUNC_DISABLE)
//运行时独立看门狗初始化
void SrvImplHeartSubIwdgInit(void);
//独立看门狗定时喂狗
void SrvImplHeartSubIwdgFeedRunPeriod(void);
#endif

#if(SRV_HEART_WWDG_ENABLE != CONFIG_FUNC_DISABLE)
//运行时窗口看门狗初始化
void SrvImplHeartSubWwdgInit(void);
//运行时窗口看门狗定时喂狗
void SrvImplHeartSubWwdgFeedRunPeriod(void);
#endif

#if(SRV_HEART_LED_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//初始化LED定时展示
void SrvImplHeartSubLedShowInit(void);
//LED定时闪烁
void SrvImplHeartSubLedShowRunPeriod(void);
#endif


#endif








