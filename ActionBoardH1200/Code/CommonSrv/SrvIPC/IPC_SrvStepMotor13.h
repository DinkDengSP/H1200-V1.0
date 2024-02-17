/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 20:11:08 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_STEP_MOTOR13_H_
#define __IPC_SRV_STEP_MOTOR13_H_
#include "IPC_CommonBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvStepMotor13Init(void);
//清空IPC信道
ERROR_SUB IPC_SrvStepMotor13ClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvStepMotor13ClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvStepMotor13WaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvStepMotor13WaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvStepMotor13QueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvStepMotor13QueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvStepMotor13CommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,WAIT
void IPC_SrvStepMotor13ResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,WAIT
void IPC_SrvStepMotor13RunStepWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,WAIT
void IPC_SrvStepMotor13RunCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,WAIT
void IPC_SrvStepMotor13ReturnZeroWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,WAIT
void IPC_SrvStepMotor13RunAlwaysWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,WAIT
void IPC_SrvStepMotor13StopNowWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,WAIT
void IPC_SrvStepMotor13StopSlowDownWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,WAIT
void IPC_SrvStepMotor13GetCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,WAIT
void IPC_SrvStepMotor13ReadOriginSensorWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能
void IPC_SrvStepMotor13DisableWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式
void IPC_SrvStepMotor13EnterTeachModeWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态
void IPC_SrvStepMotor13ForceEnterDefaultWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvStepMotor13QueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机复位,QUERY
void IPC_SrvStepMotor13QueryResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,QUERY
void IPC_SrvStepMotor13QueryRunStepState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,QUERY
void IPC_SrvStepMotor13QueryRunCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,QUERY
void IPC_SrvStepMotor13QueryReturnZeroState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,QUERY
void IPC_SrvStepMotor13QueryRunAlwaysState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,QUERY
void IPC_SrvStepMotor13QueryStopNowState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,QUERY
void IPC_SrvStepMotor13QueryStopSlowDownState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,QUERY
void IPC_SrvStepMotor13QueryGetCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,QUERY
void IPC_SrvStepMotor13QueryReadOriginSensorState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能,QUERY
void IPC_SrvStepMotor13QueryDisableState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,QUERY
void IPC_SrvStepMotor13QueryEnterTeachModeState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,QUERY
void IPC_SrvStepMotor13QueryForceEnterDefaultState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvStepMotor13CommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机复位,ACK
void IPC_SrvStepMotor13ResetWhileAck(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走步数,ACK
void IPC_SrvStepMotor13RunStepWhileAck(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走坐标,ACK
void IPC_SrvStepMotor13RunCoordinateWhileAck(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机回零,ACK
void IPC_SrvStepMotor13ReturnZeroWhileAck(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机持续运转,ACK
void IPC_SrvStepMotor13RunAlwaysWhileAck(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,ACK
void IPC_SrvStepMotor13StopNowWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,ACK
void IPC_SrvStepMotor13StopSlowDownWhileAck(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//获取电机当前坐标,ACK
void IPC_SrvStepMotor13GetCoordinateWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取当前原点传感器状态,ACK
void IPC_SrvStepMotor13ReadOriginSensorWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机失能,ACK
void IPC_SrvStepMotor13DisableWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,ACK
void IPC_SrvStepMotor13EnterTeachModeWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,ACK
void IPC_SrvStepMotor13ForceEnterDefaultWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvStepMotor13CommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,RETURN
void IPC_SrvStepMotor13ResetWhileReturn(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,RETURN
void IPC_SrvStepMotor13RunStepWhileReturn(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,RETURN
void IPC_SrvStepMotor13RunCoordinateWhileReturn(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,RETURN
void IPC_SrvStepMotor13ReturnZeroWhileReturn(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,RETURN
void IPC_SrvStepMotor13RunAlwaysWhileReturn(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,RETURN
void IPC_SrvStepMotor13StopNowWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,RETURN
void IPC_SrvStepMotor13StopSlowDownWhileReturn(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,RETURN
void IPC_SrvStepMotor13GetCoordinateWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,RETURN
void IPC_SrvStepMotor13ReadOriginSensorWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能,RETURN
void IPC_SrvStepMotor13DisableWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,RETURN
void IPC_SrvStepMotor13EnterTeachModeWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,RETURN
void IPC_SrvStepMotor13ForceEnterDefaultWhileReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************独立API************************************************************/
//设置电机状态
void IPC_SrvStepMotor13SetState(STEP_MOTOR_STATE_MAIN mainState,STEP_MOTOR_STATE_SUB subState);
//电机急停
void IPC_SrvStepMotor13EmergencyStop(void);
//获取电机实时坐标
int32_t IPC_SrvStepMotor13GetRealTimeCoordinate(void);
//修改电机坐标
void IPC_SrvStepMotor13ModifyMotorCordinate(int32_t motorCurrentCoordinate);
//检查电机是否指令急停
FlagStatus IPC_SrvStepMotor13CheckEmergencyStopCmd(void);
//检查电机是否信号急停
FlagStatus IPC_SrvStepMotor13CheckEmergencyStopSignal(void);


#endif














