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
#ifndef __SRV_IMPL_SOCKET_NET_8700_H_
#define __SRV_IMPL_SOCKET_NET_8700_H_
#include "CommonSrvImplBase.h"

/*----------------------------------外部辅助函数--------------------------------------------------*/
//获取主通讯网口状态
ULONG SrvImplSocketNetLAN8700MainGetState(void);
//获取日志通讯网口状态
ULONG SrvImplSocketNetLAN8700LogGetState(void);
//获取探针通讯网口状态
ULONG SrvImplSocketNetLAN8700ProbeGetState(void);
//检测SOCKET MAIN连接情况
FlagStatus SrvImplSocketLAN8700CheckConnectSta(void);

//主通讯网口数据发送
uint32_t SrvImplSocketNetLAN8700MainSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplSocketNetLAN8700LogSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//探针网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplSocketNetLAN8700ProbeSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

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











