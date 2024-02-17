/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 17:38:08 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 16:04:27 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_TEST_FLOW_H_
#define __APP_IMPL_TEST_FLOW_H_
#include "AppImplBaseTestFlow.h"

//任务全局对象初始化
void AppImplTestFlowVarInit(void);

//任务接收消息处理
void AppImplTestFlowMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void AppImplTestFlowFSM_Init(void);

//任务状态机周期运行
void AppImplTestFlowFSM_RunPeriod(void);




#endif




