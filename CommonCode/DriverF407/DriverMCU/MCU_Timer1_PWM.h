/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 20:55:39 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER1_PWM_H_
#define __MCU_TIMER1_PWM_H_
#include "DriverHeaderCore.h"
#include "MCU_TimerBase.h"

//定时器PWM输出初始化
void MCU_Timer1Ch1_Init(MCU_TimerBaseCallBack callBackFuncPtr);

//设置脉冲频率
void MCU_Timer1Ch1_SetFreq(uint32_t freq);

//设置转速
void MCU_Timer1Ch1_SetRpm(float rpm);

//启动定时器输出
void MCU_Timer1Ch1_StartOutput(void);

//关闭定时器输出
void MCU_Timer1Ch1_StopOutput(void);



#endif












