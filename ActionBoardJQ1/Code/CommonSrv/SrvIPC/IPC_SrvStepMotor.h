/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 20:11:08 +0800
************************************************************************************************/ 
#ifndef __IPC_SRV_STEP_MOTOR_H_
#define __IPC_SRV_STEP_MOTOR_H_
#include "IPC_CommonSrvBase.h"
#if(SYSTEM_STEP_MOTOR_COUNT > 0)
#include "IPC_SrvStepMotor1.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 1)
#include "IPC_SrvStepMotor2.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 2)
#include "IPC_SrvStepMotor3.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 3)
#include "IPC_SrvStepMotor4.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 4)
#include "IPC_SrvStepMotor5.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 5)
#include "IPC_SrvStepMotor6.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 6)
#include "IPC_SrvStepMotor7.h"
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 7)
#include "IPC_SrvStepMotor8.h"
#endif


/*****************************************************通道控制************************************************************/
//清空IPC信道
ERROR_SUB IPC_SrvStepMotorClearChannelByCmd(STEP_MOTOR_NO motorNo,uint32_t cmdCode);
//清空全部信道
ERROR_SUB IPC_SrvStepMotorClearChannel(STEP_MOTOR_NO motorNo);

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_SrvStepMotorWaitCmdReturn(STEP_MOTOR_NO motorNo,uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);
//等待上一次发送的指令结果返回
void IPC_SrvStepMotorWaitLastCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr);

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvStepMotorQueryCmdState(STEP_MOTOR_NO motorNo,uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_RETURN_DATA* returnDataPtr);
//查询上一次发送的指令的状态
void IPC_SrvStepMotorQueryLastCmdState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_RETURN_DATA* returnDataPtr);

/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvStepMotorCommTestWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机复位,WAIT
void IPC_SrvStepMotorResetWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,WAIT
void IPC_SrvStepMotorRunStepWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,WAIT
void IPC_SrvStepMotorRunCoordinateWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,WAIT
void IPC_SrvStepMotorReturnZeroWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,WAIT
void IPC_SrvStepMotorRunAlwaysWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,WAIT
void IPC_SrvStepMotorStopNowWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,WAIT
void IPC_SrvStepMotorStopSlowDownWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,WAIT
void IPC_SrvStepMotorGetCoordinateWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,WAIT
void IPC_SrvStepMotorReadOriginSensorWaitCmdReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvStepMotorQueryCommTestState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机复位,QUERY
void IPC_SrvStepMotorQueryResetState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,QUERY
void IPC_SrvStepMotorQueryRunStepState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,QUERY
void IPC_SrvStepMotorQueryRunCoordinateState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                            IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,QUERY
void IPC_SrvStepMotorQueryReturnZeroState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,QUERY
void IPC_SrvStepMotorQueryRunAlwaysState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,QUERY
void IPC_SrvStepMotorQueryStopNowState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,QUERY
void IPC_SrvStepMotorQueryStopSlowDownState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,QUERY
void IPC_SrvStepMotorQueryGetCoordinateState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,QUERY
void IPC_SrvStepMotorQueryReadOriginSensorState(STEP_MOTOR_NO motorNo,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                            IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************发送指令等待ACK************************************************************/
//通讯测试,ACK
void IPC_SrvStepMotorCommTestWhileAck(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机复位,ACK
void IPC_SrvStepMotorResetWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走步数,ACK
void IPC_SrvStepMotorRunStepWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机走坐标,ACK
void IPC_SrvStepMotorRunCoordinateWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机回零,ACK
void IPC_SrvStepMotorReturnZeroWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机持续运转,ACK
void IPC_SrvStepMotorRunAlwaysWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,ACK
void IPC_SrvStepMotorStopNowWhileAck(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,ACK
void IPC_SrvStepMotorStopSlowDownWhileAck(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//获取电机当前坐标,ACK
void IPC_SrvStepMotorGetCoordinateWhileAck(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//获取当前原点传感器状态,ACK
void IPC_SrvStepMotorReadOriginSensorWhileAck(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_SrvStepMotorCommTestWhileReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机复位,RETURN
void IPC_SrvStepMotorResetWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走步数,RETURN
void IPC_SrvStepMotorRunStepWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机走坐标,RETURN
void IPC_SrvStepMotorRunCoordinateWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机回零,RETURN
void IPC_SrvStepMotorReturnZeroWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr);
//电机持续运转,RETURN
void IPC_SrvStepMotorRunAlwaysWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//电机立即停止,RETURN
void IPC_SrvStepMotorStopNowWhileReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//电机减速停止,RETURN
void IPC_SrvStepMotorStopSlowDownWhileReturn(STEP_MOTOR_NO motorNo,IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr);
//获取电机当前坐标,RETURN
void IPC_SrvStepMotorGetCoordinateWhileReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr);
//获取当前原点传感器状态,RETURN
void IPC_SrvStepMotorReadOriginSensorWhileReturn(STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr);

/*****************************************************独立API************************************************************/
//设置电机状态
void IPC_SrvStepMotorSetState(STEP_MOTOR_NO motorNo,STEP_MOTOR_MAIN_STATE mainState,STEP_MOTOR_SUB_STATE subState);
//修改电机坐标
void IPC_SrvStepMotorModifyMotorCordinate(STEP_MOTOR_NO motorNo,int32_t motorCurrentCoordinate);
//检查电机是否指令急停
FlagStatus IPC_SrvStepMotorCheckEmergencyStopCmd(STEP_MOTOR_NO motorNo);
//检查电机是否信号急停
FlagStatus IPC_SrvStepMotorCheckEmergencyStopSignal(STEP_MOTOR_NO motorNo);

#endif













