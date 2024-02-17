/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 20:11:08 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_STEP_MOTOR_4_H_
#define __SRV_IMPL_STEP_MOTOR_4_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplStepMotor4VarInit(void);

//任务接收消息处理
void SrvImplStepMotor4MsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplStepMotor4FSM_Init(void);

//任务状态机周期运行
void SrvImplStepMotor4FSM_RunPeriod(void);




#endif















