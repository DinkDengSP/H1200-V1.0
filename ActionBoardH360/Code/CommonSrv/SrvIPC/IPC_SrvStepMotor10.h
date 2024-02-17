/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 20:11:08 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_STEP_MOTOR10_H_
#define __IPC_SRV_STEP_MOTOR10_H_
#include "IPC_CommonSrvBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvStepMotor10Init(void);
//清空IPC信道
ERROR_SUB IPC_SrvStepMotor10ClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvStepMotor10ClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvStepMotor10WaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvStepMotor10WaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvStepMotor10QueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvStepMotor10QueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvStepMotor10CommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,WAIT
void IPC_SrvStepMotor10ResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,WAIT
void IPC_SrvStepMotor10RunStepWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,WAIT
void IPC_SrvStepMotor10RunCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,WAIT
void IPC_SrvStepMotor10ReturnZeroWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,WAIT
void IPC_SrvStepMotor10RunAlwaysWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,WAIT
void IPC_SrvStepMotor10StopNowWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,WAIT
void IPC_SrvStepMotor10StopSlowDownWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,WAIT
void IPC_SrvStepMotor10GetCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,WAIT
void IPC_SrvStepMotor10ReadOriginSensorWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvStepMotor10QueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机复位,QUERY
void IPC_SrvStepMotor10QueryResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,QUERY
void IPC_SrvStepMotor10QueryRunStepState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,QUERY
void IPC_SrvStepMotor10QueryRunCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,QUERY
void IPC_SrvStepMotor10QueryReturnZeroState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,QUERY
void IPC_SrvStepMotor10QueryRunAlwaysState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,QUERY
void IPC_SrvStepMotor10QueryStopNowState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,QUERY
void IPC_SrvStepMotor10QueryStopSlowDownState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,QUERY
void IPC_SrvStepMotor10QueryGetCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,QUERY
void IPC_SrvStepMotor10QueryReadOriginSensorState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvStepMotor10CommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机复位,ACK
void IPC_SrvStepMotor10ResetWhileAck(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走步数,ACK
void IPC_SrvStepMotor10RunStepWhileAck(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走坐标,ACK
void IPC_SrvStepMotor10RunCoordinateWhileAck(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机回零,ACK
void IPC_SrvStepMotor10ReturnZeroWhileAck(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机持续运转,ACK
void IPC_SrvStepMotor10RunAlwaysWhileAck(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,ACK
void IPC_SrvStepMotor10StopNowWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,ACK
void IPC_SrvStepMotor10StopSlowDownWhileAck(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//获取电机当前坐标,ACK
void IPC_SrvStepMotor10GetCoordinateWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取当前原点传感器状态,ACK
void IPC_SrvStepMotor10ReadOriginSensorWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvStepMotor10CommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,RETURN
void IPC_SrvStepMotor10ResetWhileReturn(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,RETURN
void IPC_SrvStepMotor10RunStepWhileReturn(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,RETURN
void IPC_SrvStepMotor10RunCoordinateWhileReturn(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,RETURN
void IPC_SrvStepMotor10ReturnZeroWhileReturn(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,RETURN
void IPC_SrvStepMotor10RunAlwaysWhileReturn(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,RETURN
void IPC_SrvStepMotor10StopNowWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,RETURN
void IPC_SrvStepMotor10StopSlowDownWhileReturn(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,RETURN
void IPC_SrvStepMotor10GetCoordinateWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,RETURN
void IPC_SrvStepMotor10ReadOriginSensorWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************独立API************************************************************/
//设置电机状态
void IPC_SrvStepMotor10SetState(STEP_MOTOR_MAIN_STATE mainState,STEP_MOTOR_SUB_STATE subState);
//电机急停
void IPC_SrvStepMotor10EmergencyStop(void);
//获取电机实时坐标
int32_t IPC_SrvStepMotor10GetRealTimeCoordinate(void);
//修改电机坐标
void IPC_SrvStepMotor10ModifyMotorCordinate(int32_t motorCurrentCoordinate);
//检查电机是否指令急停
FlagStatus IPC_SrvStepMotor10CheckEmergencyStopCmd(void);
//检查电机是否信号急停
FlagStatus IPC_SrvStepMotor10CheckEmergencyStopSignal(void);


#endif














