/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 16:54:39
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_SOCKET_NET_8700_H_
#define __SRV_IMPL_SOCKET_NET_8700_H_
#include "SrvImplBase.h"

/*----------------------------------外部辅助函数--------------------------------------------------*/
//获取主通讯网口状态
ULONG SrvImplSocketLAN8700MainGetState(void);
//获取日志通讯网口状态
ULONG SrvImplSocketLAN8700LogGetState(void);

//主通讯网口数据发送
uint32_t SrvImplSocketLAN8700MainSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplSocketLAN8700LogSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

//任务全局对象初始化
void SrvImplSocketNet8700VarInit(void);

//任务接收消息处理
void SrvImplSocketNet8700MsgProcess(void* taskMsgPtr);

//ACK消息发送处理
void SrvImplSocketNet8700AckSendMsgProcess(void* socketAckMsgSendPtr);

//任务状态机初始化
void SrvImplSocketNet8700FSM_Init(void);

//任务状态机周期运行
void SrvImplSocketNet8700FSM_RunPeriod(void);




#endif



