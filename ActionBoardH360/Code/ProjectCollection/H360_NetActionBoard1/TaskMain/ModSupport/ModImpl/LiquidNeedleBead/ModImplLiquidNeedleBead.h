/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:45:56 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_LIQUID_NEEDLE_BEAD_H_
#define __MOD_IMPL_LIQUID_NEEDLE_BEAD_H_
#include "ModImplBaseLiquidNeedleBead.h"

//任务全局对象初始化
void ModImplLiquidNeedleBeadVarInit(void);

//任务接收消息处理
void ModImplLiquidNeedleBeadMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplLiquidNeedleBeadFSM_Init(void);

//任务状态机周期运行
void ModImplLiquidNeedleBeadFSM_RunPeriod(void);


#endif



