/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 11:10:09 +0800
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

















