/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_UART6_RECV_H_
#define __SRV_IMPL_UART6_RECV_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplUart6RecvVarInit(void);

//任务接收消息处理
void SrvImplUart6RecvMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplUart6RecvFSM_Init(void);

//任务状态机周期运行
void SrvImplUart6RecvFSM_RunPeriod(void);




#endif



