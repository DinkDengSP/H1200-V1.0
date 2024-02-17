/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_SOCKET_NET_H_
#define __SRV_IMPL_SOCKET_NET_H_
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
void SrvImplSocketNetVarInit(void);

//任务接收消息处理
void SrvImplSocketNetMsgProcess(void* taskMsgPtr);

//ACK消息发送处理
void SrvImplSocketNetAckSendMsgProcess(void* socketAckMsgSendPtr);

//任务状态机初始化
void SrvImplSocketNetFSM_Init(void);

//任务状态机周期运行
void SrvImplSocketNetFSM_RunPeriod(void);




#endif

















