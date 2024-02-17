/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-30 15:47:31 +0800
************************************************************************************************/ 
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

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvUtilCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//联机,WAIT
void IPC_SrvUtilOnLineWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//脱机,WAIT
void IPC_SrvUtilOffLineWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//UTC同步,WAIT
void IPC_SrvUtilUtcSyncWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//心跳获取,WAIT
void IPC_SrvUtilHeartGetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr);
//急停,WAIT
void IPC_SrvUtilEmStopWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//获取版本号,WAIT
void IPC_SrvUtilGetVersionWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                            BOARD_SOFT_VERSION* appVersionPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvUtilQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//联机,QUERY
void IPC_SrvUtilQueryOnLineState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//脱机,QUERY
void IPC_SrvUtilQueryOffLineState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//UTC同步,QUERY
void IPC_SrvUtilQueryUtcSyncState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//心跳获取,QUERY
void IPC_SrvUtilQueryHeartGetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr);
//急停,QUERY
void IPC_SrvUtilQueryEmStopState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//获取版本号,QUERY
void IPC_SrvUtilQueryGetVersionState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,
                                        BOARD_SOFT_VERSION* srvVersionPtr,BOARD_SOFT_VERSION* appVersionPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvUtilCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
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
void IPC_SrvUtilCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//联机,RETURN
void IPC_SrvUtilOnLineWhileReturn(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//脱机,RETURN
void IPC_SrvUtilOffLineWhileReturn(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardModePtr);
//UTC同步,RETURN
void IPC_SrvUtilUtcSyncWhileReturn(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr);
//心跳获取,RETURN
void IPC_SrvUtilHeartGetWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint64_t* utcTimeStampPtr,BOARD_RUN_MODE* boardModePtr);
//急停,RETURN
void IPC_SrvUtilEmStopWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//获取版本号,RETURN
void IPC_SrvUtilGetVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,BOARD_SOFT_VERSION* bootVesionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                        BOARD_SOFT_VERSION* appVersionPtr);



#endif














