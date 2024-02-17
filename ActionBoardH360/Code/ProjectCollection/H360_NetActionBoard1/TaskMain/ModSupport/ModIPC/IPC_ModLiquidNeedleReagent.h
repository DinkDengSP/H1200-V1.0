/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:17:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:13:01 +0800
************************************************************************************************/ 
#ifndef __IPC_MOD_LIQUID_NEEDLE_REAGENT_H_
#define __IPC_MOD_LIQUID_NEEDLE_REAGENT_H_
#include "IPC_ModBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_ModLiquidNeedleReagentInit(void);
//清空IPC信道
ERROR_SUB IPC_ModLiquidNeedleReagentClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_ModLiquidNeedleReagentClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_ModLiquidNeedleReagentWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_ModLiquidNeedleReagentWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_ModLiquidNeedleReagentQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_ModLiquidNeedleReagentQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_ModLiquidNeedleReagentCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_ModLiquidNeedleReagentCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);



#endif

