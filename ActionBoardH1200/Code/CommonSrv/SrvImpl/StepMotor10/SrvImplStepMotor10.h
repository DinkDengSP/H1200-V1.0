/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:28:40
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_STEP_MOTOR10_H_
#define __SRV_IMPL_STEP_MOTOR10_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplStepMotor10VarInit(void);

//任务接收消息处理
void SrvImplStepMotor10MsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplStepMotor10FSM_Init(void);

//任务状态机周期运行
void SrvImplStepMotor10FSM_RunPeriod(void);





#endif











