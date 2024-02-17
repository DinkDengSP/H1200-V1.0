/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER7_H_
#define __MCU_TIMER7_H_
#include "DriverHeaderCore.h"
#include "DriverConfigTimer.h"
#include "MCU_TimerBase.h"

//定时器基础模式下,定时器周期中断函数回调
#if(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_BASE)

//分频系数和计数值 
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr);
//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax);
//定时器启动
void MCU_Timer7Start(void);
//定时器停止
void MCU_Timer7Stop(void);
//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void);

#endif

//DAC模式,只是输出,不带中断
#if(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_DAC)

//分频系数和计数值 
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax);
//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax);
//定时器启动
void MCU_Timer7Start(void);
//定时器停止
void MCU_Timer7Stop(void);
//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void);
#endif



//独立定时器下的测试用例
#if((MCU_TIMER7_TEST_CASE_USE != 0)&&(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_BASE))
//测试用例初始化
void MCU_Timer7_TestCaseInit(void);
//测试用例周期运转
void MCU_Timer7_TestCaseRunPeriod(void);

#endif

#endif












