/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 10:25:38 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_MIX_BEAD_H_
#define __MOD_IMPL_MIX_BEAD_H_
#include "ModImplBaseMixBead.h"

//任务全局对象初始化
void ModImplMixBeadVarInit(void);

//任务接收消息处理
void ModImplMixBeadMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplMixBeadFSM_Init(void);

//任务状态机周期运行
void ModImplMixBeadFSM_RunPeriod(void);




#endif






















