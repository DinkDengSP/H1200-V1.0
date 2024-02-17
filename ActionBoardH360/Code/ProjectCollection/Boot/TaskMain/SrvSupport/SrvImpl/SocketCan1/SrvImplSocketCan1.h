/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 10:25:38 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_SOCKET_CAN1_H_
#define __SRV_IMPL_SOCKET_CAN1_H_
#include "SrvImplBase.h"

//任务全局对象初始化
void SrvImplSocketCan1VarInit(void);

//任务接收消息处理
void SrvImplSocketCan1MsgProcess(void* taskMsgPtr);

//ACK消息发送处理
void SrvImplSocketCan1AckSendMsgProcess(void* socketAckMsgSendPtr);

//任务状态机初始化
void SrvImplSocketCan1FSM_Init(void);

//任务状态机周期运行
void SrvImplSocketCan1FSM_RunPeriod(void);




#endif
































