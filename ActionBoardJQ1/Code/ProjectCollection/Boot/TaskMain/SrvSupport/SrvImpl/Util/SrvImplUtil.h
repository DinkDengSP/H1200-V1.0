/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 10:59:43 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_UTIL_H_
#define __SRV_IMPL_UTIL_H_
#include "SrvImplBase.h"

//任务全局对象初始化
void SrvImplUtilVarInit(void);

//任务接收消息处理
void SrvImplUtilMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplUtilFSM_Init(void);

//任务状态机周期运行
void SrvImplUtilFSM_RunPeriod(void);




#endif
























