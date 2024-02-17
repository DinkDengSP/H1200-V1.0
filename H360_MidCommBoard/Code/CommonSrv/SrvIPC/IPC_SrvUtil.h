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
#ifndef __IPC_SRV_UTIL_H_
#define __IPC_SRV_UTIL_H_
#include "IPC_CommonBase.h"


/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvUtilInit(void);
//清空IPC信道
ERROR_SUB IPC_SrvUtilClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvUtilClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvUtilWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvUtilWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvUtilQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvUtilQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvUtilCommunicationTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//联机,ACK
void IPC_SrvUtilOnLineWhileAck(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr);
//脱机,ACK
void IPC_SrvUtilOffLineWhileAck(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr);
//UTC同步,ACK
void IPC_SrvUtilUtcSyncWhileAck(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr);
//心跳获取,ACK
void IPC_SrvUtilHeartGetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//急停,ACK
void IPC_SrvUtilEmStopWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取版本号,ACK
void IPC_SrvUtilGetVersionWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvUtilCommunicationTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//联机,RETURN
void IPC_SrvUtilOnLineWhileReturn(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//脱机,RETURN
void IPC_SrvUtilOffLineWhileReturn(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//UTC同步,RETURN
void IPC_SrvUtilUtcSyncWhileReturn(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//心跳获取,RETURN
void IPC_SrvUtilHeartGetWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,IPC_RETURN_DATA* returnDataPtr);
//急停,RETURN
void IPC_SrvUtilEmStopWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//获取版本号,RETURN
void IPC_SrvUtilGetVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);


#endif












