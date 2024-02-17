/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 09:55:52 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_STEP_MOTOR3_H_
#define __SRV_IMPL_STEP_MOTOR3_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplStepMotor3VarInit(void);

//任务接收消息处理
void SrvImplStepMotor3MsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplStepMotor3FSM_Init(void);

//任务状态机周期运行
void SrvImplStepMotor3FSM_RunPeriod(void);




#endif










