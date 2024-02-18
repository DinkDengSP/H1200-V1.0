/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 19:31:25 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_ALARM_H_
#define __STEP_MOTOR_ALARM_H_
#include "StepMotorTable.h"

//急停中断回调函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorEmergencyStopByIntSingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];

//步进电机辅助急停1中断函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt1SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];

//步进电机辅助急停2中断函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt2SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];


#endif




