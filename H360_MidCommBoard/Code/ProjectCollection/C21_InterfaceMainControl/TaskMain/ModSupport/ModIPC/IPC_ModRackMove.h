/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 11:02:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-14 13:10:36 +0800
************************************************************************************************/ 
#ifndef __IPC_MOD_RACK_MOVE_H_
#define __IPC_MOD_RACK_MOVE_H_
#include "IPC_ModBase.h"


/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_ModRackMoveInit(void);
//清空IPC信道
ERROR_SUB IPC_ModRackMoveClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_ModRackMoveClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
ERROR_SUB IPC_ModRackMoveWaitCmdReturn(uint32_t commandCode,SYS_RESULT_PACK* resultPackPtr);
//等待上一次发送的指令结果返回
ERROR_SUB IPC_ModRackMoveWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
ERROR_SUB IPC_ModRackMoveCmdState(uint32_t commandCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//查询上一次发送的指令的状态
ERROR_SUB IPC_ModRackMoveLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
ERROR_SUB IPC_ModRackMoveCommunicationTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元电机复位,WAIT
ERROR_SUB IPC_ModRackMoveMotorResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机走步数,WAIT
ERROR_SUB IPC_ModRackMoveMotorRunStepWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机状态读取,WAIT
ERROR_SUB IPC_ModRackMoveMotorStateReadWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元传感器信号单个读取,WAIT
ERROR_SUB IPC_ModRackMoveSensorReadSingleWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);
//试管架移动单元传感器信号批量读取,WAIT
ERROR_SUB IPC_ModRackMoveSensorReadAllWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//获取试管架移动单元当前位置,WAIT
ERROR_SUB IPC_ModRackMoveGetCurrentPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//试管架移动单元复位,WAIT
ERROR_SUB IPC_ModRackMoveModuleResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//试管架移动单元到空闲位置,WAIT
ERROR_SUB IPC_ModRackMoveModuleToIdleWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//横移电机移动到指定试管架仓位置,WAIT
ERROR_SUB IPC_ModRackMoveLaterallyToStoreByPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//横移电机移动到C21接口位置,WAIT
ERROR_SUB IPC_ModRackMoveLaterallyToC21WaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载试管架仓位置,WAIT
ERROR_SUB IPC_ModRackMoveRunToLoadRackStorePosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载试管架仓位置,WAIT
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackStorePosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载C21位置,WAIT
ERROR_SUB IPC_ModRackMoveRunToLoadRackC21PosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载C21位置,WAIT
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackC21WaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//在当前位置执行试管架仓样本架装载,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreCurrentWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行试管架仓样本架放入,WAIT
ERROR_SUB IPC_ModRackMovePushOutRackToStoreCurrentWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架装载,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21CurrentWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架放入,WAIT
ERROR_SUB IPC_ModRackMovePushOutRackToC21CurrentWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管架仓中装载一个试管架,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往试管架仓中放入一个试管架,WAIT
ERROR_SUB IPC_ModRackMovePushOutRackToStoreByPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从C21接口装载一个试管架,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21WaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往C21接口放入一个试管架,WAIT
ERROR_SUB IPC_ModRackMovePushOutRackToC21WaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToStoreByPosWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入C21中,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToC21WaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//从C21中取出一个试管架放入试管架仓中,WAIT
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21ToRackStoreWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
ERROR_SUB IPC_ModRackMoveQueryCommunicationTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元电机复位,QUERY
ERROR_SUB IPC_ModRackMoveQueryMotorResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机走步数,QUERY
ERROR_SUB IPC_ModRackMoveQueryMotorRunStepState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机状态读取,QUERY
ERROR_SUB IPC_ModRackMoveQueryMotorStateReadState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元传感器信号单个读取,QUERY
ERROR_SUB IPC_ModRackMoveQuerySensorReadSingleState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);
//试管架移动单元传感器信号批量读取,QUERY
ERROR_SUB IPC_ModRackMoveQuerySensorReadAllState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//获取试管架移动单元当前位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryGetCurrentPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//试管架移动单元复位,QUERY
ERROR_SUB IPC_ModRackMoveQueryModuleResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//试管架移动单元到空闲位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryModuleToIdleState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//横移电机移动到指定试管架仓位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryLaterallyToStoreByPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//横移电机移动到C21接口位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryLaterallyToC21State(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载试管架仓位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryRunToLoadRackStorePosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载试管架仓位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryRunToUnLoadRackStorePosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载C21位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryRunToLoadRackC21PosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载C21位置,QUERY
ERROR_SUB IPC_ModRackMoveQueryRunToUnLoadRackC21State(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//在当前位置执行试管架仓样本架装载,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormStoreCurrentState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行试管架仓样本架放入,QUERY
ERROR_SUB IPC_ModRackMoveQueryPushOutRackToStoreCurrentState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架装载,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormC21CurrentState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架放入,QUERY
ERROR_SUB IPC_ModRackMoveQueryPushOutRackToC21CurrentState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管架仓中装载一个试管架,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormStoreByPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往试管架仓中放入一个试管架,QUERY
ERROR_SUB IPC_ModRackMoveQueryPushOutRackToStoreByPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从C21接口装载一个试管架,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormC21State(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往C21接口放入一个试管架,QUERY
ERROR_SUB IPC_ModRackMoveQueryPushOutRackToC21State(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormStoreByPosToStoreByPosState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入C21中,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormStoreByPosToC21State(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//从C21中取出一个试管架放入试管架仓中,QUERY
ERROR_SUB IPC_ModRackMoveQueryLoadInRackFormC21ToRackStoreState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
ERROR_SUB IPC_ModRackMoveCommunicationTestWhileAck(void);
//试管架移动单元电机复位,ACK
ERROR_SUB IPC_ModRackMoveMotorResetWhileAck(STEP_MOTOR_RACK_MOVE motorNo);
//试管架移动单元电机走步数,ACK
ERROR_SUB IPC_ModRackMoveMotorRunStepWhileAck(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps);
//试管架移动单元电机状态读取,ACK
ERROR_SUB IPC_ModRackMoveMotorStateReadWhileAck(STEP_MOTOR_RACK_MOVE motorNo);
//试管架移动单元传感器信号单个读取,ACK
ERROR_SUB IPC_ModRackMoveSensorReadSingleWhileAck(RACK_MOVE_SENSOR sensorNo);
//试管架移动单元传感器信号批量读取,ACK
ERROR_SUB IPC_ModRackMoveSensorReadAllWhileAck(void);
//获取试管架移动单元当前位置,ACK
ERROR_SUB IPC_ModRackMoveGetCurrentPosWhileAck(void);
//试管架移动单元复位,ACK
ERROR_SUB IPC_ModRackMoveModuleResetWhileAck(void);
//试管架移动单元到空闲位置,ACK
ERROR_SUB IPC_ModRackMoveModuleToIdleWhileAck(void);
//横移电机移动到指定试管架仓位置,ACK
ERROR_SUB IPC_ModRackMoveLaterallyToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos);
//横移电机移动到C21接口位置,ACK
ERROR_SUB IPC_ModRackMoveLaterallyToC21WhileAck(void);
//装载机构移动到装载试管架仓位置,ACK
ERROR_SUB IPC_ModRackMoveRunToLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos);
//装载机构移动到卸载试管架仓位置,ACK
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackStorePosWhileAck(RACK_STORE_HOUSE rackHousePos);
//装载机构移动到装载C21位置,ACK
ERROR_SUB IPC_ModRackMoveRunToLoadRackC21PosWhileAck(void);
//装载机构移动到卸载C21位置,ACK
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackC21WhileAck(void);
//在当前位置执行试管架仓样本架装载,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreCurrentWhileAck(FunctionalState barScanSet);
//在当前位置执行试管架仓样本架放入,ACK
ERROR_SUB IPC_ModRackMovePushOutRackToStoreCurrentWhileAck(void);
//在当前位置执行C21样本架装载,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21CurrentWhileAck(void);
//在当前位置执行C21样本架放入,ACK
ERROR_SUB IPC_ModRackMovePushOutRackToC21CurrentWhileAck(void);
//集成指令,从试管架仓中装载一个试管架,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet);
//集成指令,往试管架仓中放入一个试管架,ACK
ERROR_SUB IPC_ModRackMovePushOutRackToStoreByPosWhileAck(RACK_STORE_HOUSE rackHousePos);
//集成指令,从C21接口装载一个试管架,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21WhileAck(void);
//集成指令,往C21接口放入一个试管架,ACK
ERROR_SUB IPC_ModRackMovePushOutRackToC21WhileAck(void);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToStoreByPosWhileAck(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,
                                                                                FunctionalState barScanSet);
//集成指令,从试管仓中取出一个试管架放入C21中,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToC21WhileAck(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet);
//从C21中取出一个试管架放入试管架仓中,ACK
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21ToRackStoreWhileAck(RACK_STORE_HOUSE rackTargetHousePos);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
ERROR_SUB IPC_ModRackMoveCommunicationTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//试管架移动单元电机复位,RETURN
ERROR_SUB IPC_ModRackMoveMotorResetWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机走步数,RETURN
ERROR_SUB IPC_ModRackMoveMotorRunStepWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,int32_t steps,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元电机状态读取,RETURN
ERROR_SUB IPC_ModRackMoveMotorStateReadWhileReturn(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//试管架移动单元传感器信号单个读取,RETURN
ERROR_SUB IPC_ModRackMoveSensorReadSingleWhileReturn(RACK_MOVE_SENSOR sensorNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);
//试管架移动单元传感器信号批量读取,RETURN
ERROR_SUB IPC_ModRackMoveSensorReadAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//获取试管架移动单元当前位置,RETURN
ERROR_SUB IPC_ModRackMoveGetCurrentPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//试管架移动单元复位,RETURN
ERROR_SUB IPC_ModRackMoveModuleResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//试管架移动单元到空闲位置,RETURN
ERROR_SUB IPC_ModRackMoveModuleToIdleWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//横移电机移动到指定试管架仓位置,RETURN
ERROR_SUB IPC_ModRackMoveLaterallyToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//横移电机移动到C21接口位置,RETURN
ERROR_SUB IPC_ModRackMoveLaterallyToC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载试管架仓位置,RETURN
ERROR_SUB IPC_ModRackMoveRunToLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载试管架仓位置,RETURN
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackStorePosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到装载C21位置,RETURN
ERROR_SUB IPC_ModRackMoveRunToLoadRackC21PosWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//装载机构移动到卸载C21位置,RETURN
ERROR_SUB IPC_ModRackMoveRunToUnLoadRackC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_POS* rackMovePosPtr);
//在当前位置执行试管架仓样本架装载,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreCurrentWhileReturn(FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,
                                                                    RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行试管架仓样本架放入,RETURN
ERROR_SUB IPC_ModRackMovePushOutRackToStoreCurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架装载,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21CurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//在当前位置执行C21样本架放入,RETURN
ERROR_SUB IPC_ModRackMovePushOutRackToC21CurrentWhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管架仓中装载一个试管架,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,
                                                                RACK_BAR_MSG* rackBarMsgPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往试管架仓中放入一个试管架,RETURN
ERROR_SUB IPC_ModRackMovePushOutRackToStoreByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从C21接口装载一个试管架,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,往C21接口放入一个试管架,RETURN
ERROR_SUB IPC_ModRackMovePushOutRackToC21WhileReturn(SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入另一个试管仓中,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToStoreByPosWhileReturn(RACK_STORE_HOUSE rackSrcHousePos,RACK_STORE_HOUSE rackTargetHousePos,FunctionalState barScanSet,
                                                                            SYS_RESULT_PACK* resultPackPtr,RACK_BAR_MSG* rackBarMsgPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//集成指令,从试管仓中取出一个试管架放入C21中,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormStoreByPosToC21WhileReturn(RACK_STORE_HOUSE rackSrcHousePos,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,
                                                                    RACK_BAR_MSG* rackBarMsgPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);
//从C21中取出一个试管架放入试管架仓中,RETURN
ERROR_SUB IPC_ModRackMoveLoadInRackFormC21ToRackStoreWhileReturn(RACK_STORE_HOUSE rackTargetHousePos,SYS_RESULT_PACK* resultPackPtr,RACK_MOVE_SENSOR_SET* sensorSetPtr);



#endif














