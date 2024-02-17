/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 08:49:23 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_FLASH_IAP_H_
#define __SRV_IMPL_FLASH_IAP_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplFlashIapVarInit(void);

//任务接收消息处理
void SrvImplFlashIapMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplFlashIapFSM_Init(void);

//任务状态机周期运行
void SrvImplFlashIapFSM_RunPeriod(void);




#endif















