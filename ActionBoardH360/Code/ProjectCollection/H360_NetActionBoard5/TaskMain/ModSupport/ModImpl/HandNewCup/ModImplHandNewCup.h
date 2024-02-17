/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 11:08:17 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_HAND_NEW_CUP_H_
#define __MOD_IMPL_HAND_NEW_CUP_H_
#include "ModImplBaseHandNewCup.h"

//任务全局对象初始化
void ModImplHandNewCupVarInit(void);

//任务接收消息处理
void ModImplHandNewCupMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplHandNewCupFSM_Init(void);

//任务状态机周期运行
void ModImplHandNewCupFSM_RunPeriod(void);




#endif











