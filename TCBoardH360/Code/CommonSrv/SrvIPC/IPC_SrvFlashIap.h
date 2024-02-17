/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_FLASH_IAP_H_
#define __IPC_SRV_FLASH_IAP_H_
#include "IPC_CommonSrvBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvFlashIapInit(void);
//清空IPC信道
ERROR_SUB IPC_SrvFlashIapClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvFlashIapClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvFlashIapWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvFlashIapWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvFlashIapQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvFlashIapQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvFlashIapCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvFlashIapQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvFlashIapCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvFlashIapCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);



#endif





