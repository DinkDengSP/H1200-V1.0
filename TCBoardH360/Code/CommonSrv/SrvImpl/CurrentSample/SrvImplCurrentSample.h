/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_CURRENT_SAMPLE_H_
#define __SRV_IMPL_CURRENT_SAMPLE_H_
#include "CommonSrvImplBase.h"
#include "SrvImplBaseCurrentSample.h"

//任务全局对象初始化
void SrvImplCurrentSampleVarInit(void);

//任务接收消息处理
void SrvImplCurrentSampleMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplCurrentSampleFSM_Init(void);

//任务状态机周期运行
void SrvImplCurrentSampleFSM_RunPeriod(void);




#endif





