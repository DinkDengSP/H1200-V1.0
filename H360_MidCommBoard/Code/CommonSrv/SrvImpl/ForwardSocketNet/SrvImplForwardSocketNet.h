/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 11:53:32
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_FORWARD_SOCKET_NET_H_
#define __SRV_IMPL_FORWARD_SOCKET_NET_H_
#include "CommonSrvImplBase.h"

//任务消息转发函数
//AckCmd数据转发
void SrvImplForwardSocketNetMsgAckCmd(NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr);
//AckResult数据转发
void SrvImplForwardSocketNetMsgAckResult(NET_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr);
//AckUpload数据转发
void SrvImplForwardSocketNetMsgAckUpload(NET_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr);
//日志信息转发
void SrvImplForwardSocketNetMsgLogMsg(NET_PROT_LOG_PACK_RECV* logPackPtr);
//指令信息转发
void SrvImplForwardSocketNetMsgCmdMsg(NET_PROT_CMD_PACK_RECV* cmdPackPtr);
//结果信息转发
void SrvImplForwardSocketNetMsgResultMsg(NET_PROT_RESULT_PACK_RECV* resultPackPtr);
//主动上传信息转发
void SrvImplForwardSocketNetMsgUploadMsg(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);


//任务全局对象初始化
void SrvImplForwardSocketNetVarInit(void);

//任务接收消息处理
void SrvImplForwardSocketNetMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplForwardSocketNetFSM_Init(void);

//任务状态机周期运行
void SrvImplForwardSocketNetFSM_RunPeriod(void);

#endif











