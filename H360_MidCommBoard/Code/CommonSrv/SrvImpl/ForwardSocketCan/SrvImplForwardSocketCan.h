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
#ifndef __SRV_IMPL_FORWARD_SOCKET_CAN_H_
#define __SRV_IMPL_FORWARD_SOCKET_CAN_H_
#include "CommonSrvImplBase.h"


//AckCmd数据转发
void SrvImplForwardSocketCanMsgAckCmd(PORT_CAN portCan,CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr);

//AckResult数据转发
void SrvImplForwardSocketCanMsgAckResult(PORT_CAN portCan,CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr);

//AckUpload数据转发
void SrvImplForwardSocketCanMsgAckUpload(PORT_CAN portCan,CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr);

//日志信息转发
void SrvImplForwardSocketCanMsgLogMsg(PORT_CAN portCan,CAN_PROT_LOG_PACK* logPackPtr);

//指令信息转发
void SrvImplForwardSocketCanMsgCmdMsg(PORT_CAN portCan,CAN_PROT_CMD_PACK* cmdPackPtr);

//结果信息转发
void SrvImplForwardSocketCanMsgResultMsg(PORT_CAN portCan,CAN_PROT_RESULT_PACK* resultPackPtr);

//主动上传信息转发
void SrvImplForwardSocketCanMsgUploadMsg(PORT_CAN portCan,CAN_PROT_UPLOAD_PACK* uploadPackPtr);

//任务全局对象初始化
void SrvImplForwardSocketCanVarInit(void);

//任务接收消息处理
void SrvImplForwardSocketCanMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplForwardSocketCanFSM_Init(void);

//任务状态机周期运行
void SrvImplForwardSocketCanFSM_RunPeriod(void);

#endif











