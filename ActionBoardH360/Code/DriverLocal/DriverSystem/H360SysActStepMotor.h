/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 10:24:10 +0800
************************************************************************************************/ 
#ifndef __H360_SYS_ACT_STEP_MOTOR_H_
#define __H360_SYS_ACT_STEP_MOTOR_H_
#include "DriverHeaderBoard.h"
#include "SystemBase.h"
#include "UserMemManage.h"
#include "CanMasterHeader.h"

typedef enum H360_SYS_SM
{
    //板卡1
    H360_SYS_BOARD1_SM1     = 0,
    H360_SYS_BOARD1_SM2     = 1,
    H360_SYS_BOARD1_SM3     = 2,
    H360_SYS_BOARD1_SM4     = 3,
    H360_SYS_BOARD1_SM5     = 4,
    H360_SYS_BOARD1_SM6     = 5,
    H360_SYS_BOARD1_SM7     = 6,
    H360_SYS_BOARD1_SM8     = 7,
    H360_SYS_BOARD1_SM9     = 8,
    H360_SYS_BOARD1_SM10    = 9,
    H360_SYS_BOARD1_SM11    = 10,
    H360_SYS_BOARD1_SM12    = 11,
    //板卡2
    H360_SYS_BOARD2_SM1     = 12,
    H360_SYS_BOARD2_SM2     = 13,
    H360_SYS_BOARD2_SM3     = 14,
    H360_SYS_BOARD2_SM4     = 15,
    H360_SYS_BOARD2_SM5     = 16,
    H360_SYS_BOARD2_SM6     = 17,
    H360_SYS_BOARD2_SM7     = 18,
    H360_SYS_BOARD2_SM8     = 19,
    H360_SYS_BOARD2_SM9     = 20,
    H360_SYS_BOARD2_SM10    = 21,
    H360_SYS_BOARD2_SM11    = 22,
    H360_SYS_BOARD2_SM12    = 23,
    //板卡3
    H360_SYS_BOARD3_SM1     = 24,
    H360_SYS_BOARD3_SM2     = 25,
    H360_SYS_BOARD3_SM3     = 26,
    H360_SYS_BOARD3_SM4     = 27,
    H360_SYS_BOARD3_SM5     = 28,
    H360_SYS_BOARD3_SM6     = 29,
    H360_SYS_BOARD3_SM7     = 30,
    H360_SYS_BOARD3_SM8     = 31,
    H360_SYS_BOARD3_SM9     = 32,
    H360_SYS_BOARD3_SM10    = 33,
    H360_SYS_BOARD3_SM11    = 34,
    H360_SYS_BOARD3_SM12    = 35,
    //板卡4
    H360_SYS_BOARD4_SM1     = 36,
    H360_SYS_BOARD4_SM2     = 37,
    H360_SYS_BOARD4_SM3     = 38,
    H360_SYS_BOARD4_SM4     = 39,
    H360_SYS_BOARD4_SM5     = 40,
    H360_SYS_BOARD4_SM6     = 41,    
    H360_SYS_BOARD4_SM7     = 42,
    H360_SYS_BOARD4_SM8     = 43,
    H360_SYS_BOARD4_SM9     = 44,
    H360_SYS_BOARD4_SM10    = 45,
    H360_SYS_BOARD4_SM11    = 46,
    H360_SYS_BOARD4_SM12    = 47,
    //板卡5
    H360_SYS_BOARD5_SM1     = 48,
    H360_SYS_BOARD5_SM2     = 49,
    H360_SYS_BOARD5_SM3     = 50,
    H360_SYS_BOARD5_SM4     = 51,
    H360_SYS_BOARD5_SM5     = 52,
    H360_SYS_BOARD5_SM6     = 53,
    H360_SYS_BOARD5_SM7     = 54,
    H360_SYS_BOARD5_SM8     = 55,
    H360_SYS_BOARD5_SM9     = 56,
    H360_SYS_BOARD5_SM10    = 57,
    H360_SYS_BOARD5_SM11    = 58,
    H360_SYS_BOARD5_SM12    = 59,
}H360_SYS_SM;

/*********************************************************发送指令等待ACK********************************************************************/
//步进电机复位
void H360SysActStepMotorResetWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr);
//步进电机控制走步数
void H360SysActStepMotorRunStepWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr);
//步进电机控制走坐标
void H360SysActStepMotorRunCoordinateWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr);
//步进电机控制回零
void H360SysActStepMotorReturnZeroWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr);
//步进电机控制持续运转
void H360SysActStepMotorRunAlwaysWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void H360SysActStepMotorStopNowWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void H360SysActStepMotorStopSlowWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr);
//步进电机控制获取当前坐标
void H360SysActStepMotorGetCoordinateWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorReadOriginSensorWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);

/*********************************************************等待指定指令完成返回********************************************************************/
//步进电机复位
void H360SysActStepMotorWaitResetResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走步数
void H360SysActStepMotorWaitRunStepResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走坐标
void H360SysActStepMotorWaitRunCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制回零
void H360SysActStepMotorWaitReturnZeroResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制持续运转
void H360SysActStepMotorWaitRunAlwaysResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void H360SysActStepMotorWaitStopNowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void H360SysActStepMotorWaitStopSlowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制获取当前坐标
void H360SysActStepMotorWaitGetCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr);
//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorWaitReadOriginSensorResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr);

/*********************************************************发送指令等待完成返回********************************************************************/
//步进电机复位
void H360SysActStepMotorResetWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走步数
void H360SysActStepMotorRunStepWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制走坐标
void H360SysActStepMotorRunCoordinateWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制回零
void H360SysActStepMotorReturnZeroWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制持续运转
void H360SysActStepMotorRunAlwaysWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr);
//步进电机控制立即停止
void H360SysActStepMotorStopNowWhileReturn(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr);
//步进电机控制减速运转
void H360SysActStepMotorStopSlowWhileReturn(H360_SYS_SM motorNo, CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr);
//步进电机控制获取当前坐标
void H360SysActStepMotorGetCoordinateWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr);
//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorReadOriginSensorWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr);





#endif


