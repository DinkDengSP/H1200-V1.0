/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 11:02:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-14 13:56:39 +0800
************************************************************************************************/ 
#ifndef __IPC_MOD_RACK_STORE_H_
#define __IPC_MOD_RACK_STORE_H_
#include "IPC_ModBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_ModRackStoreInit(void);
//清空IPC信道
ERROR_SUB IPC_ModRackStoreClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_ModRackStoreClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
ERROR_SUB IPC_ModRackStoreWaitCmdReturn(uint32_t commandCode,SYS_RESULT_PACK* resultPackPtr);
//等待上一次发送的指令结果返回
ERROR_SUB IPC_ModRackStoreWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
ERROR_SUB IPC_ModRackStoreQueryCmdState(uint32_t commandCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//查询上一次发送的指令的状态
ERROR_SUB IPC_ModRackStoreQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
ERROR_SUB IPC_ModRackStoreCommunicationTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//读取样本架仓指定仓位单个传感器状态,WAIT
ERROR_SUB IPC_ModRackStoreReadSingleSensorByPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);
//读取样本架仓指定仓位全部传感器状态,WAIT
ERROR_SUB IPC_ModRackStoreReadAllSensorStateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_STORE_SENSOR_SET* sensorStateSetPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
ERROR_SUB IPC_ModRackStoreQueryCommunicationTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//读取样本架仓指定仓位单个传感器状态,QUERY
ERROR_SUB IPC_ModRackStoreQueryReadSingleSensorByPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                            PIN_STATE* sensorStatePtr);
//读取样本架仓指定仓位全部传感器状态,QUERY
ERROR_SUB IPC_ModRackStoreQueryReadAllSensorStateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                        RACK_STORE_SENSOR_SET* sensorStateSetPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
ERROR_SUB IPC_ModRackStoreCommunicationTestWhileAck(void);
//读取样本架仓指定仓位单个传感器状态,ACK
ERROR_SUB IPC_ModRackStoreReadSingleSensorByPosWhileAck(RACK_STORE_HOUSE rackHousePos);
//读取样本架仓指定仓位全部传感器状态,ACK
ERROR_SUB IPC_ModRackStoreReadAllSensorStateWhileAck(void);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
ERROR_SUB IPC_ModRackStoreCommunicationTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//读取样本架仓指定仓位单个传感器状态,RETURN
ERROR_SUB IPC_ModRackStoreReadSingleSensorByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,
                                                            PIN_STATE* sensorStatePtr);
//读取样本架仓指定仓位全部传感器状态,RETURN
ERROR_SUB IPC_ModRackStoreReadAllSensorStateWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_STORE_SENSOR_SET* sensorStateSetPtr);




#endif














