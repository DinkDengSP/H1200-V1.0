/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 10:51:00
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_SOCKET_NET_5500_H_
#define __SRV_IMPL_SOCKET_NET_5500_H_
#include "CommonSrvImplBase.h"

/*----------------------------------外部辅助函数--------------------------------------------------*/
//获取主通讯网口状态
ULONG SrvImplSocketNetW5500MainGetState(void);
//获取探针网口状态
ULONG SrvImplSocketNetW5500ProbeGetState(void);
//获取日志通讯网口状态
ULONG SrvImplSocketNetW5500LogGetState(void);

//主通讯网口数据发送
uint32_t SrvImplSocketNetW5500MainSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//探针网口数据发送
uint32_t SrvImplSocketNetW5500ProbeSendBuf(uint8_t* bufferPtr,uint32_t bufLength);
//日志网口数据发送,日志网口里面输出讯息必须是底层打印,否则会形成递归调用
uint32_t SrvImplSocketNetW5500LogSendBuf(uint8_t* bufferPtr,uint32_t bufLength);

/*----------------------------------外部扩展函数--------------------------------------------------*/
//外扩主端口消息处理程序
void SrvImplExternSocket5500MainMsgProcess(NET_RECV_PACK* netPackPtr);
//外扩日志端口消息处理程序
void SrvImplExternSocket5500LogMsgProcess(NET_RECV_PACK* netPackPtr);
//外扩探针端口消息处理程序
void SrvImplExternSocket5500ProbeMsgProcess(NET_RECV_PACK* netPackPtr);
//外扩消息处理程序
void SrvImplExternSocket5500AckSendMsgProcess(void* socketAckMsgSendPtr);
//扩展定时处理程序
void SrvImplSocketNetW5500TimerPeriod(uint32_t currentTimeTick);

//任务全局对象初始化
void SrvImplSocketNet5500VarInit(void);

//任务接收消息处理
void SrvImplSocketNet5500MsgProcess(void* taskMsgPtr);

//ACK消息发送处理
void SrvImplSocketNet5500AckSendMsgProcess(void* socketAckMsgSendPtr);

//任务状态机初始化
void SrvImplSocketNet5500FSM_Init(void);

//任务状态机周期运行
void SrvImplSocketNet5500FSM_RunPeriod(void);

#endif











