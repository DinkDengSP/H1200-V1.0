/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 10:44:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** AppifyRecord1 :    
** AppifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 21:01:28 +0800
************************************************************************************************/ 
#ifndef __IPC_APP_DEMO_H_
#define __IPC_APP_DEMO_H_
#include "IPC_AppBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_AppDemoInit(void);
//清空IPC信道
ERROR_SUB IPC_AppDemoClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_AppDemoClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_AppDemoWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_AppDemoWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_AppDemoQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_AppDemoQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_AppDemoCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_AppDemoCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);



#endif

