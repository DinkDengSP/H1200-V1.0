/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-30 15:13:10 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_PORT_OUT_H_
#define __IPC_SRV_PORT_OUT_H_
#include "IPC_CommonBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvPortOutInit(void);
//清空IPC信道
ERROR_SUB IPC_SrvPortOutClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvPortOutClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvPortOutWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvPortOutWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvPortOutQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvPortOutQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvPortOutCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//单个输出写入,WAIT
void IPC_SrvPortOutWriteSingleWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//批量输出写入,WAIT
void IPC_SrvPortOutWriteAnyWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvPortOutQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//单个输出写入,QUERY
void IPC_SrvPortOutQueryWriteSingleState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//批量输出写入,QUERY
void IPC_SrvPortOutQueryWriteAnyState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvPortOutCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//单个输出写入,ACK
void IPC_SrvPortOutWriteSingleWhileAck(uint8_t writeIndex,GPIO_PinState setState,SYS_RESULT_PACK* resultPackPtr);
//批量输出写入,ACK
void IPC_SrvPortOutWriteAnyWhileAck(uint8_t* writeDataPairArrayPtr,uint8_t pairCount,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvPortOutCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//单个输出写入,RETURN
void IPC_SrvPortOutWriteSingleWhileReturn(uint8_t writeIndex,GPIO_PinState setState,SYS_RESULT_PACK* resultPackPtr);
//批量输出写入,RETURN
void IPC_SrvPortOutWriteAnyWhileReturn(uint8_t* writeDataPairArrayPtr,uint8_t pairCount,SYS_RESULT_PACK* resultPackPtr);



#endif
















