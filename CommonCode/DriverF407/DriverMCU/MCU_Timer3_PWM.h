/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER3_PWM_H_
#define __MCU_TIMER3_PWM_H_
#include "DriverHeaderCore.h"
#include "MCU_TimerBase.h"

//定时器PWM输出初始化
void MCU_Timer3Ch2_Init(MCU_TimerBaseCallBack callBackFuncPtr);

//设置脉冲频率
void MCU_Timer3Ch2_SetFreq(uint32_t freq);

//设置转速
void MCU_Timer3Ch2_SetRpm(float rpm);

//启动定时器输出
void MCU_Timer3Ch2_StartOutput(void);

//关闭定时器输出
void MCU_Timer3Ch2_StopOutput(void);



#endif









