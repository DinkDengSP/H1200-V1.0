/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-30 13:58:01 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_PORT_IN_H_
#define __IPC_SRV_PORT_IN_H_
#include "IPC_CommonBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvPortInInit(void);
//清空IPC信道
ERROR_SUB IPC_SrvPortInClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvPortInClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvPortInWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvPortInWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvPortInQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                    IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvPortInQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvPortInCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//单个读取,WAIT
void IPC_SrvPortInReadSingleWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr);
//区间读取,WAIT
void IPC_SrvPortInReadRangeWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr);
//批量读取,WAIT
void IPC_SrvPortInReadAnyWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvPortInQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//单个读取,QUERY
void IPC_SrvPortInQueryReadSingleState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr);
//区间读取,QUERY
void IPC_SrvPortInQueryReadRangeState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr);
//批量读取,QUERY
void IPC_SrvPortInQueryReadAnyState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvPortInCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//单个读取,ACK
void IPC_SrvPortInReadSingleWhileAck(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr);
//区间读取,ACK
void IPC_SrvPortInReadRangeWhileAck(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr);
//批量读取,ACK
void IPC_SrvPortInReadAnyWhileAck(uint8_t readSingleCount,uint8_t* pinIndexArray,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvPortInCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//单个读取,RETURN
void IPC_SrvPortInReadSingleWhileReturn(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr);
//区间读取,RETURN
void IPC_SrvPortInReadRangeWhileReturn(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState** pinStateArrayPtr);
//批量读取,RETURN
void IPC_SrvPortInReadAnyWhileReturn(uint8_t readSingleCount,uint8_t* pinIndexArray,SYS_RESULT_PACK* resultPackPtr,
                                            GPIO_PinState** pinStateArrayPtr);



#endif















