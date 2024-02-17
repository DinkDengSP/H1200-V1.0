/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 15:04:04 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_STEP_MOTOR_H_
#define __CAN_MASTER_STEP_MOTOR_H_
#include "CanMasterUtil.h"

/*********************************************************基础参数初始化指令********************************************************************/
//复位指令初始化
void CanMasterStepMotorSetResetCmdDefault(CAN_MASTER_SM_CMD_RESET* resetCmdPtr);
//走步数指令初始化
void CanMasterStepMotorSetRunStepCmdDefault(CAN_MASTER_SM_CMD_RUN_STEP* runStepCmdPtr);
//走坐标指令初始化
void CanMasterStepMotorSetRunCoordinateCmdDefault(CAN_MASTER_SM_CMD_RUN_COORDINATE* runCoordinateCmdPtr);
//回零指令初始化
void CanMasterStepMotorSetReturnZeroCmdDefault(CAN_MASTER_SM_CMD_RETURN_ZERO* returnZeroCmdPtr);
//持续运转指令初始化
void CanMasterStepMotorSetRunAlwaysCmdDefault(CAN_MASTER_SM_CMD_RUN_ALWAY* runAlwayCmdPtr);
//减速停止指令初始化
void CanMasterStepMotorSetStopSlowCmdDefault(CAN_MASTER_SM_CMD_STOP_SLOW* stopSlowCmdPtr);
//状态初始化
void CanMasterStepMotorSetStateDefault(CAN_MASTER_SM_STATE* motorStatePtr);
//坐标初始化
void CanMasterStepMotorSetPosDefault(CAN_MASTER_SM_POS* motorPosPtr);
//传感器初始化
void CanMasterStepMotorSetSensorDefault(CAN_MASTER_SM_SENSOR* motorSensorPtr);
//从结果中获取数据
void CanMasterStepMotorGetResultMotorState(uint8_t* resultBuffer,CAN_MASTER_SM_STATE* motorStatePtr);
//从结果中获取数据
void CanMasterStepMotorGetResultMotorPos(uint8_t* resultBuffer,CAN_MASTER_SM_POS* motorPosPtr);
//从结果中获取数据
void CanMasterStepMotorGetResultMotorSensor(uint8_t* resultBuffer,CAN_MASTER_SM_SENSOR* motorSensorPtr);
//从电机序号获取电机通道
CAN_MASTER_CH CanMasterStepMotorGetChByNo(STEP_MOTOR_NO motorNo);

/*********************************************************发送指令等待ACK********************************************************************/
//步进电机复位
void CanMasterStepMotorResetWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制走步数
void CanMasterStepMotorRunStepWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制走坐标
void CanMasterStepMotorRunCoordinateWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制回零
void CanMasterStepMotorReturnZeroWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制持续运转
void CanMasterStepMotorRunAlwaysWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void CanMasterStepMotorStopNowWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void CanMasterStepMotorStopSlowWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制获取当前坐标
void CanMasterStepMotorGetCoordinateWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr);
//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorReadOriginSensorWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                        SYS_RESULT_PACK* resultPackPtr);

/*********************************************************等待指定指令完成返回********************************************************************/
//步进电机复位
void CanMasterStepMotorWaitResetResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走步数
void CanMasterStepMotorWaitRunStepResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走坐标
void CanMasterStepMotorWaitRunCoordinateResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制回零
void CanMasterStepMotorWaitReturnZeroResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制持续运转
void CanMasterStepMotorWaitRunAlwaysResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void CanMasterStepMotorWaitStopNowResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void CanMasterStepMotorWaitStopSlowResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制获取当前坐标
void CanMasterStepMotorWaitGetCoordinateResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr);
//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorWaitReadOriginSensorResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr);

/*********************************************************发送指令等待完成返回********************************************************************/
//步进电机复位
void CanMasterStepMotorResetWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走步数
void CanMasterStepMotorRunStepWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走坐标
void CanMasterStepMotorRunCoordinateWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制回零
void CanMasterStepMotorReturnZeroWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制持续运转
void CanMasterStepMotorRunAlwaysWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void CanMasterStepMotorStopNowWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void CanMasterStepMotorStopSlowWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo, CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制获取当前坐标
void CanMasterStepMotorGetCoordinateWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr);
//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorReadOriginSensorWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr);




#endif













