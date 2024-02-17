/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-12 16:52:58 +0800
************************************************************************************************/ 
#include "ModImplBase.h"

//IO急停操作
static void SrvImplBaseUtilEmStopWithOutput(void)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < BOARD_PORT_OUT_COUNT; indexUtil++)
    {
        BoardPortOutInitSingle((BOARD_PORT_OUT)indexUtil);
    }
}

//系统急停实现
void SrvImplBaseUtilEmStop(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //12个电机全部急停
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO1);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO2);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO3);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO4);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO5);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO6);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO7);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO8);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO9);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO10);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO11);
    IPC_SrvStepMotorEmergencyStop(STEP_MOTOR_NO12);
    //IO急停
    SrvImplBaseUtilEmStopWithOutput();
}
























