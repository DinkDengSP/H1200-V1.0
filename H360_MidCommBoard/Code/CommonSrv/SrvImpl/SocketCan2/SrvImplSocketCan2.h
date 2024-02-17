/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_SOCKET_CAN2_H_
#define __SRV_IMPL_SOCKET_CAN2_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplSocketCan2VarInit(void);

//任务接收消息处理
void SrvImplSocketCan2MsgProcess(void* taskMsgPtr);

//ACK消息发送处理
void SrvImplSocketCan2AckSendMsgProcess(void* socketAckMsgSendPtr);

//任务状态机初始化
void SrvImplSocketCan2FSM_Init(void);

//任务状态机周期运行
void SrvImplSocketCan2FSM_RunPeriod(void);

#endif











