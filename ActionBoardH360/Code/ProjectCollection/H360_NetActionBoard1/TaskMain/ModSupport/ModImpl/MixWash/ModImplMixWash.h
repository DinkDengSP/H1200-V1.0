/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:45:56 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_MIX_WASH_H_
#define __MOD_IMPL_MIX_WASH_H_
#include "ModImplBaseMixWash.h"

//任务全局对象初始化
void ModImplMixWashVarInit(void);

//任务接收消息处理
void ModImplMixWashMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplMixWashFSM_Init(void);

//任务状态机周期运行
void ModImplMixWashFSM_RunPeriod(void);


#endif



