/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 11:25:26 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_CUP_TRAY_MANAGE_H_
#define __MOD_IMPL_CUP_TRAY_MANAGE_H_
#include "ModImplBaseCupTrayManage.h"

//任务全局对象初始化
void ModImplCupTrayManageVarInit(void);

//任务接收消息处理
void ModImplCupTrayManageMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplCupTrayManageFSM_Init(void);

//任务状态机周期运行
void ModImplCupTrayManageFSM_RunPeriod(void);




#endif












