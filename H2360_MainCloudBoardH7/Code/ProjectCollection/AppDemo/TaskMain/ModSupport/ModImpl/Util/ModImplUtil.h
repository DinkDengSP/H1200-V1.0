/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_UTIL_H_
#define __MOD_IMPL_UTIL_H_
#include "ModImplBase.h"

//任务全局对象初始化
void ModImplUtilVarInit(void);

//任务接收消息处理
void ModImplUtilMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplUtilFSM_Init(void);

//任务状态机周期运行
void ModImplUtilFSM_RunPeriod(void);




#endif















