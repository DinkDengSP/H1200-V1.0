/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 10:44:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-03 15:48:30 +0800
************************************************************************************************/ 
#ifndef __IPC_MOD_UTIL_H_
#define __IPC_MOD_UTIL_H_
#include "IPC_ModBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_ModUtilInit(void);
//清空IPC信道
ERROR_SUB IPC_ModUtilClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_ModUtilClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_ModUtilWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_ModUtilWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_ModUtilQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_ModUtilQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_ModUtilCommunicationTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//握手,设置下位机运行模式
void IPC_ModUtilHandShakeWhileAck(SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr);
//急停,设置下位机急停
void IPC_ModUtilEmergencyStopWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_ModUtilCommunicationTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//握手,设置下位机运行模式
void IPC_ModUtilHandShakeWhileReturn(SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//急停,设置下位机急停
void IPC_ModUtilEmergencyStopWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr); 

#endif
