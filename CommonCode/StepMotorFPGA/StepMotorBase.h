/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:13:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-10 15:42:26 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_BASE_H_
#define __STEP_MOTOR_BASE_H_
#include "StepMotorCurve.h"

//获取步进电机访问信号量
ERROR_SUB StepMotorBaseGetLock(STEP_MOTOR_NO motorNo);

//释放步进电机访问信号量
void StepMotorBaseReleaseLock(STEP_MOTOR_NO motorNo);

//获取步进电机数据集合
STEP_MOTOR_DATA_SET* StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO motorNo);

//电机按照属性使能
ERROR_SUB StepMotorBaseEnableByAttriSingle(STEP_MOTOR_NO motorNo);

//RTT中断显示电机停止讯息
void StepMotorRTT_ShowIntMsgMotorState(STEP_MOTOR_NO motorNo,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason);

#endif




