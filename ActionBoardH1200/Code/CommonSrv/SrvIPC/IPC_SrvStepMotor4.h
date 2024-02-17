/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:13:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-11 08:52:11 +0800
************************************************************************************************/ 



/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 20:11:08 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_STEP_MOTOR4_H_
#define __IPC_SRV_STEP_MOTOR4_H_
#include "IPC_CommonBase.h"

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvStepMotor4Init(void);
//清空IPC信道
ERROR_SUB IPC_SrvStepMotor4ClearChannelByCmd(uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvStepMotor4ClearChannel(void);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvStepMotor4WaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvStepMotor4WaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvStepMotor4QueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvStepMotor4QueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvStepMotor4CommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,WAIT
void IPC_SrvStepMotor4ResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,WAIT
void IPC_SrvStepMotor4RunStepWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,WAIT
void IPC_SrvStepMotor4RunCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,WAIT
void IPC_SrvStepMotor4ReturnZeroWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,WAIT
void IPC_SrvStepMotor4RunAlwaysWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,WAIT
void IPC_SrvStepMotor4StopNowWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,WAIT
void IPC_SrvStepMotor4StopSlowDownWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,WAIT
void IPC_SrvStepMotor4GetCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,WAIT
void IPC_SrvStepMotor4ReadOriginSensorWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能
void IPC_SrvStepMotor4DisableWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式
void IPC_SrvStepMotor4EnterTeachModeWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态
void IPC_SrvStepMotor4ForceEnterDefaultWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvStepMotor4QueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机复位,QUERY
void IPC_SrvStepMotor4QueryResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,QUERY
void IPC_SrvStepMotor4QueryRunStepState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,QUERY
void IPC_SrvStepMotor4QueryRunCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,QUERY
void IPC_SrvStepMotor4QueryReturnZeroState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,QUERY
void IPC_SrvStepMotor4QueryRunAlwaysState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,QUERY
void IPC_SrvStepMotor4QueryStopNowState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,QUERY
void IPC_SrvStepMotor4QueryStopSlowDownState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,QUERY
void IPC_SrvStepMotor4QueryGetCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,QUERY
void IPC_SrvStepMotor4QueryReadOriginSensorState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能,QUERY
void IPC_SrvStepMotor4QueryDisableState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,QUERY
void IPC_SrvStepMotor4QueryEnterTeachModeState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,QUERY
void IPC_SrvStepMotor4QueryForceEnterDefaultState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvStepMotor4CommTestWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机复位,ACK
void IPC_SrvStepMotor4ResetWhileAck(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走步数,ACK
void IPC_SrvStepMotor4RunStepWhileAck(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走坐标,ACK
void IPC_SrvStepMotor4RunCoordinateWhileAck(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机回零,ACK
void IPC_SrvStepMotor4ReturnZeroWhileAck(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机持续运转,ACK
void IPC_SrvStepMotor4RunAlwaysWhileAck(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,ACK
void IPC_SrvStepMotor4StopNowWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,ACK
void IPC_SrvStepMotor4StopSlowDownWhileAck(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//获取电机当前坐标,ACK
void IPC_SrvStepMotor4GetCoordinateWhileAck(SYS_RESULT_PACK* resultPackPtr);
//获取当前原点传感器状态,ACK
void IPC_SrvStepMotor4ReadOriginSensorWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机失能,ACK
void IPC_SrvStepMotor4DisableWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,ACK
void IPC_SrvStepMotor4EnterTeachModeWhileAck(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,ACK
void IPC_SrvStepMotor4ForceEnterDefaultWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvStepMotor4CommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机复位,RETURN
void IPC_SrvStepMotor4ResetWhileReturn(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,RETURN
void IPC_SrvStepMotor4RunStepWhileReturn(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,RETURN
void IPC_SrvStepMotor4RunCoordinateWhileReturn(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,RETURN
void IPC_SrvStepMotor4ReturnZeroWhileReturn(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,RETURN
void IPC_SrvStepMotor4RunAlwaysWhileReturn(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,RETURN
void IPC_SrvStepMotor4StopNowWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,RETURN
void IPC_SrvStepMotor4StopSlowDownWhileReturn(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,RETURN
void IPC_SrvStepMotor4GetCoordinateWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,RETURN
void IPC_SrvStepMotor4ReadOriginSensorWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);
//电机失能,RETURN
void IPC_SrvStepMotor4DisableWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机进入教学模式,RETURN
void IPC_SrvStepMotor4EnterTeachModeWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//电机强行进入默认状态,RETURN
void IPC_SrvStepMotor4ForceEnterDefaultWhileReturn(SYS_RESULT_PACK* resultPackPtr);

/*****************************************************独立API************************************************************/
//设置电机状态
void IPC_SrvStepMotor4SetState(STEP_MOTOR_STATE_MAIN mainState,STEP_MOTOR_STATE_SUB subState);
//电机急停
void IPC_SrvStepMotor4EmergencyStop(void);
//获取电机实时坐标
int32_t IPC_SrvStepMotor4GetRealTimeCoordinate(void);
//修改电机坐标
void IPC_SrvStepMotor4ModifyMotorCordinate(int32_t motorCurrentCoordinate);
//检查电机是否指令急停
FlagStatus IPC_SrvStepMotor4CheckEmergencyStopCmd(void);
//检查电机是否信号急停
FlagStatus IPC_SrvStepMotor4CheckEmergencyStopSignal(void);


#endif














