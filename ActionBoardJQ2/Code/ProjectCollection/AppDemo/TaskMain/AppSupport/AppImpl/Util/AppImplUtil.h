/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 11:29:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 19:20:27 +0800
************************************************************************************************/ 
#ifndef __APP_IMPL_UTIL_H_
#define __APP_IMPL_UTIL_H_
#include "AppImplBase.h"

//任务全局对象初始化
void AppImplUtilVarInit(void);

//任务接收消息处理
void AppImplUtilMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void AppImplUtilFSM_Init(void);

//任务状态机周期运行
void AppImplUtilFSM_RunPeriod(void);




#endif




