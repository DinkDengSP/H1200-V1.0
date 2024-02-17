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
#ifndef __IPC_SRV_HEART_H_
#define __IPC_SRV_HEART_H_
#include "IPC_SrvBase.h"


/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvHeartInit(void);
//清空IPC信道
ERROR_SUB IPC_SrvHeartClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvHeartClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvHeartWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvHeartWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvHeartQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvHeartQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvHeartCommunicationTestWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvHeartCommunicationTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);




#endif
