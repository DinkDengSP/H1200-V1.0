/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 14:50:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 14:53:33 +0800
************************************************************************************************/ 
#include "CanMasterStepMotor.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//复位指令初始化
void CanMasterStepMotorSetResetCmdDefault(CAN_MASTER_SM_CMD_RESET* resetCmdPtr)
{
    resetCmdPtr->resetCorrectPos = 0;
    resetCmdPtr->resetCorrectCurve = SM_CURVE_0;
    resetCmdPtr->timeOutMs = 60000;
}

//走步数指令初始化
void CanMasterStepMotorSetRunStepCmdDefault(CAN_MASTER_SM_CMD_RUN_STEP* runStepCmdPtr)
{
    runStepCmdPtr->totalSteps = 0;
    runStepCmdPtr->motorRunCurve = SM_CURVE_0;
    runStepCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runStepCmdPtr->speedRatioUint16 = 100;
    runStepCmdPtr->dirSwitchCorrect = 0;
    runStepCmdPtr->emStopEnable = DISABLE;
    runStepCmdPtr->utilStop1Enable = DISABLE;
    runStepCmdPtr->utilStop2Enable = DISABLE;
    runStepCmdPtr->timeOutMs = 60000;
}

//走坐标指令初始化
void CanMasterStepMotorSetRunCoordinateCmdDefault(CAN_MASTER_SM_CMD_RUN_COORDINATE* runCoordinateCmdPtr)
{
    runCoordinateCmdPtr->targetCoordinate = 0;
    runCoordinateCmdPtr->motorRunCurve = SM_CURVE_0;
    runCoordinateCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runCoordinateCmdPtr->speedRatioUint16 = 100;
    runCoordinateCmdPtr->dirSwitchCorrect = 0;
    runCoordinateCmdPtr->emStopEnable = DISABLE;
    runCoordinateCmdPtr->utilStop1Enable = DISABLE;
    runCoordinateCmdPtr->utilStop2Enable = DISABLE;
    runCoordinateCmdPtr->timeOutMs = 60000;
}

//回零指令初始化
void CanMasterStepMotorSetReturnZeroCmdDefault(CAN_MASTER_SM_CMD_RETURN_ZERO* returnZeroCmdPtr)
{
    returnZeroCmdPtr->returnZeroCorrectPos = 0;
    returnZeroCmdPtr->motorRunCurve = SM_CURVE_0;
    returnZeroCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    returnZeroCmdPtr->speedRatioUint16 = 100;
    returnZeroCmdPtr->dirSwitchCorrect = 0;
    returnZeroCmdPtr->emStopEnable = DISABLE;
    returnZeroCmdPtr->utilStop1Enable = DISABLE;
    returnZeroCmdPtr->utilStop2Enable = DISABLE;
    returnZeroCmdPtr->needReset = ENABLE;
    returnZeroCmdPtr->timeOutMs = 60000;
}

//持续运转指令初始化
void CanMasterStepMotorSetRunAlwaysCmdDefault(CAN_MASTER_SM_CMD_RUN_ALWAY* runAlwayCmdPtr)
{
    runAlwayCmdPtr->runAlwaysDir = STEP_MOTOR_DIR_CW;
    runAlwayCmdPtr->motorRunCurve = SM_CURVE_0;
    runAlwayCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runAlwayCmdPtr->speedRatioUint16 = 100;
}

//减速停止指令初始化
void CanMasterStepMotorSetStopSlowCmdDefault(CAN_MASTER_SM_CMD_STOP_SLOW* stopSlowCmdPtr)
{
    stopSlowCmdPtr->slowDownMode = 0;
    stopSlowCmdPtr->slowDownSteps = 0;
    stopSlowCmdPtr->timeOutMs = 60000;
}

//状态初始化
void CanMasterStepMotorSetStateDefault(CAN_MASTER_SM_STATE* motorStatePtr)
{
    motorStatePtr->motorPosCurrent = 0;
    motorStatePtr->encoderPosCurrent = 0;
    motorStatePtr->originSensorState = PIN_STATE_INVALID;
    motorStatePtr->emStopSensorState = PIN_STATE_INVALID;
    motorStatePtr->utilStopSensorState = PIN_STATE_INVALID;
    motorStatePtr->utilStop2SensorState = PIN_STATE_INVALID;
}

//坐标初始化
void CanMasterStepMotorSetPosDefault(CAN_MASTER_SM_POS* motorPosPtr)
{
    motorPosPtr->motorPosCurrent = 0;
    motorPosPtr->encoderPosCurrent = 0;
}

//传感器初始化
void CanMasterStepMotorSetSensorDefault(CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    motorSensorPtr->originSensorState = PIN_STATE_INVALID;
    motorSensorPtr->emStopSensorState = PIN_STATE_INVALID;
    motorSensorPtr->utilStopSensorState = PIN_STATE_INVALID;
    motorSensorPtr->utilStop2SensorState = PIN_STATE_INVALID;
}

//从结果中获取数据
void CanMasterStepMotorGetResultMotorState(uint8_t* resultBuffer,CAN_MASTER_SM_STATE* motorStatePtr)
{
    motorStatePtr->motorPosCurrent = ProtUtilReadInt32Buffer(resultBuffer);
    motorStatePtr->encoderPosCurrent = ProtUtilReadInt32Buffer(resultBuffer+4);
    motorStatePtr->originSensorState = (PIN_STATE)(resultBuffer[8]);
    motorStatePtr->emStopSensorState = (PIN_STATE)(resultBuffer[9]);
    motorStatePtr->utilStopSensorState = (PIN_STATE)(resultBuffer[10]);
    motorStatePtr->utilStop2SensorState = (PIN_STATE)(resultBuffer[11]);
}

//从结果中获取数据
void CanMasterStepMotorGetResultMotorPos(uint8_t* resultBuffer,CAN_MASTER_SM_POS* motorPosPtr)
{
    motorPosPtr->motorPosCurrent = ProtUtilReadInt32Buffer(resultBuffer);
    motorPosPtr->encoderPosCurrent = ProtUtilReadInt32Buffer(resultBuffer+4);
}

//从结果中获取数据
void CanMasterStepMotorGetResultMotorSensor(uint8_t* resultBuffer,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    motorSensorPtr->originSensorState = (PIN_STATE)(resultBuffer[0]);
    motorSensorPtr->emStopSensorState = (PIN_STATE)(resultBuffer[1]);
    motorSensorPtr->utilStopSensorState = (PIN_STATE)(resultBuffer[2]);
    motorSensorPtr->utilStop2SensorState = (PIN_STATE)(resultBuffer[3]);
}

//从电机序号获取电机通道
CAN_MASTER_CH CanMasterStepMotorGetChByNo(STEP_MOTOR_NO motorNo)
{
    CAN_MASTER_CH canMasterCh = CAN_MASTER_CH_ACT_SM1;
    canMasterCh += (motorNo);
    if(canMasterCh >= CAN_MASTER_CH_ACT_SM16)
    {
        return CAN_MASTER_CH_ACT_SM16;
    }
    return canMasterCh;
}




