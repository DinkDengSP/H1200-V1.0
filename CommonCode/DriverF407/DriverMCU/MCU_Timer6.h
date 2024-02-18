/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER6_H_
#define __MCU_TIMER6_H_
#include "DriverHeaderCore.h"
#include "DriverConfigTimer.h"

//定时器基础模式下,定时器周期中断函数回调
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE)
//分频系数和计数值 
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr);
//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax);
//定时器启动
void MCU_Timer6Start(void);
//定时器停止
void MCU_Timer6Stop(void);
//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void);

#endif

//DAC模式,只是输出,不带中断
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_DAC)

//分频系数和计数值 
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax);
//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax);
//定时器启动
void MCU_Timer6Start(void);
//定时器停止
void MCU_Timer6Stop(void);
//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void);

#endif




#endif





