/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-31 14:03:33 +0800
************************************************************************************************/ 
#include "IPC_CommonBase.h"
#include "IPC_Port.h"
#include "ProtUtil.h"

//释放内存
void IPC_CommonFreeReturnData(IPC_RETURN_DATA* returnDataPtr)
{
    if(returnDataPtr == NULL)
    {
        return;
    }
    if((returnDataPtr->returnBufferPtr != NULL)&&(returnDataPtr->returnDataLength != 0))
    {
        IPC_PortMemFree(returnDataPtr->returnBufferPtr);
    }
    return;
}

//复位指令初始化
void IPC_StepMotorBaseSetResetCmdDefault(IPC_STEP_MOTOR_CMD_RESET* resetCmdPtr)
{
    resetCmdPtr->resetCorrectPos = 0;
    resetCmdPtr->resetCorrectCurve = SM_CURVE_1;
    resetCmdPtr->timeOutMs = STEP_MOTOR_TIME_OUT_MAX_MS;
}

//走步数指令初始化
void IPC_StepMotorBaseSetRunStepCmdDefault(IPC_STEP_MOTOR_CMD_RUN_STEP* runStepCmdPtr)
{
    runStepCmdPtr->totalSteps = 0;
    runStepCmdPtr->runStepCurve = SM_CURVE_1;
    runStepCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runStepCmdPtr->speedRatioUint16 = 100;
    runStepCmdPtr->dirSwitchCorrect = 0;
    runStepCmdPtr->emStopEnable = DISABLE;
    runStepCmdPtr->utilStopEnable = DISABLE;
    runStepCmdPtr->timeOutMs = STEP_MOTOR_TIME_OUT_MAX_MS;
}

//走坐标指令初始化
void IPC_StepMotorBaseSetRunCoordinateCmdDefault(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* runCoordinateCmdPtr)
{
    runCoordinateCmdPtr->targetCoordinate = 0;
    runCoordinateCmdPtr->runStepCurve = SM_CURVE_1;
    runCoordinateCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runCoordinateCmdPtr->speedRatioUint16 = 100;
    runCoordinateCmdPtr->dirSwitchCorrect = 0;
    runCoordinateCmdPtr->emStopEnable = DISABLE;
    runCoordinateCmdPtr->utilStopEnable = DISABLE;
    runCoordinateCmdPtr->timeOutMs = STEP_MOTOR_TIME_OUT_MAX_MS;
}

//回零指令初始化
void IPC_StepMotorBaseSetReturnZeroCmdDefault(IPC_STEP_MOTOR_CMD_RETURN_ZERO* returnZeroCmdPtr)
{
    returnZeroCmdPtr->returnZeroCorrectPos = 0;
    returnZeroCmdPtr->runStepCurve = SM_CURVE_1;
    returnZeroCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    returnZeroCmdPtr->speedRatioUint16 = 100;
    returnZeroCmdPtr->dirSwitchCorrect = 0;
    returnZeroCmdPtr->emStopEnable = DISABLE;
    returnZeroCmdPtr->utilStopEnable = DISABLE;
    returnZeroCmdPtr->needReset = ENABLE;
    returnZeroCmdPtr->timeOutMs = STEP_MOTOR_TIME_OUT_MAX_MS;
}

//持续运转指令初始化
void IPC_StepMotorBaseSetRunAlwaysCmdDefault(IPC_STEP_MOTOR_CMD_RUN_ALWAY* runAlwayCmdPtr)
{
    runAlwayCmdPtr->runAlwaysDir = STEP_MOTOR_DIR_CW;
    runAlwayCmdPtr->runStepCurve = SM_CURVE_1;
    runAlwayCmdPtr->speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
    runAlwayCmdPtr->speedRatioUint16 = 100;
}

//减速停止指令初始化
void IPC_StepMotorBaseSetStopSlowCmdDefault(IPC_STEP_MOTOR_CMD_STOP_SLOW* stopSlowCmdPtr)
{
    stopSlowCmdPtr->slowDownMode = 0;
    stopSlowCmdPtr->slowDownSteps = 0;
    stopSlowCmdPtr->timeOutMs = STEP_MOTOR_TIME_OUT_MAX_MS;
}

//状态初始化
void IPC_StepMotorBaseSetStateDefault(IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    motorStatePtr->motorPosCurrent = 0;
    motorStatePtr->encoderPosCurrent = 0;
    motorStatePtr->originSensorState = PIN_STATE_INVALID;
    motorStatePtr->emStopSensorState = PIN_STATE_INVALID;
    motorStatePtr->utilStop1SensorState = PIN_STATE_INVALID;
    motorStatePtr->utilStop2SensorState = PIN_STATE_INVALID;
}

//坐标初始化
void IPC_StepMotorBaseSetPosDefault(IPC_STEP_MOTOR_POS* motorPosPtr)
{
    motorPosPtr->motorPosCurrent = 0;
    motorPosPtr->encoderPosCurrent = 0;
}

//传感器初始化
void IPC_StepMotorBaseSetSensorDefault(IPC_STEP_MOTOR_SENSOR* motorSensorPtr)
{
    motorSensorPtr->originSensorState = PIN_STATE_INVALID;
    motorSensorPtr->emStopSensorState = PIN_STATE_INVALID;
    motorSensorPtr->utilStop1SensorState = PIN_STATE_INVALID;
    motorSensorPtr->utilStop2SensorState = PIN_STATE_INVALID;
}


//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorState(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    motorStatePtr->motorPosCurrent = ProtUtilReadInt32Buffer(ipcChannelStepMotorPtr->channelCache.dataBufferPtr);
    motorStatePtr->encoderPosCurrent = ProtUtilReadInt32Buffer(ipcChannelStepMotorPtr->channelCache.dataBufferPtr+4);
    motorStatePtr->originSensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[8]);
    motorStatePtr->emStopSensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[9]);
    motorStatePtr->utilStop1SensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[10]);
    motorStatePtr->utilStop2SensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[11]);
}

//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorPos(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_POS* motorPosPtr)
{
    motorPosPtr->motorPosCurrent = ProtUtilReadInt32Buffer(ipcChannelStepMotorPtr->channelCache.dataBufferPtr);
    motorPosPtr->encoderPosCurrent = ProtUtilReadInt32Buffer(ipcChannelStepMotorPtr->channelCache.dataBufferPtr+4);
}

//从结果中获取数据
void IPC_SrvStepMotorGetResultMotorSensor(IPC_CHANNEL_DATA* ipcChannelStepMotorPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr)
{
    motorSensorPtr->originSensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[0]);
    motorSensorPtr->emStopSensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[1]);
    motorSensorPtr->utilStop1SensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[2]);
    motorSensorPtr->utilStop2SensorState = (PIN_STATE)(ipcChannelStepMotorPtr->channelCache.dataBufferPtr[3]);
}

//设置电机复位参数
void IPC_SrvStepMotorSetResetCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RESET* resetCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_RESET;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = resetCmdPtr->resetCorrectPos;
    cmdBaseDataPtr->cmdParam2 = (uint32_t)(resetCmdPtr->resetCorrectCurve);
    cmdBaseDataPtr->cmdParam3 = resetCmdPtr->timeOutMs;
}

//设置电机走步数参数
void IPC_SrvStepMotorSetRunStepCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_STEP* runStepCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = *((uint32_t*)(&(runStepCmdPtr->totalSteps)));

    cmdBaseDataPtr->cmdParam2 = 0;
    cmdBaseDataPtr->cmdParam2 += (uint16_t)(runStepCmdPtr->speedRatioUint16);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runStepCmdPtr->speedRatioMode);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runStepCmdPtr->runStepCurve);

    cmdBaseDataPtr->cmdParam3 = runStepCmdPtr->dirSwitchCorrect;

    cmdBaseDataPtr->cmdParam4 = 0;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(runStepCmdPtr->utilStopEnable);
    cmdBaseDataPtr->cmdParam4 <<= 8;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(runStepCmdPtr->emStopEnable);

    cmdBaseDataPtr->cmdParam5 = runStepCmdPtr->timeOutMs;
}

//设置电机走坐标参数
void IPC_SrvStepMotorSetRunCoordinateCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_COORDINATE* runCoordinateCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = *((uint32_t*)(&(runCoordinateCmdPtr->targetCoordinate)));

    cmdBaseDataPtr->cmdParam2 = 0;
    cmdBaseDataPtr->cmdParam2 += (uint16_t)(runCoordinateCmdPtr->speedRatioUint16);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runCoordinateCmdPtr->speedRatioMode);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runCoordinateCmdPtr->runStepCurve);

    cmdBaseDataPtr->cmdParam3 = runCoordinateCmdPtr->dirSwitchCorrect;

    cmdBaseDataPtr->cmdParam4 = 0;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(runCoordinateCmdPtr->utilStopEnable);
    cmdBaseDataPtr->cmdParam4 <<= 8;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(runCoordinateCmdPtr->emStopEnable);
    
    cmdBaseDataPtr->cmdParam5 = runCoordinateCmdPtr->timeOutMs;
}

//设置电机回零参数
void IPC_SrvStepMotorSetReturnZeroCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RETURN_ZERO* returnZeroCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = *((uint32_t*)(&(returnZeroCmdPtr->returnZeroCorrectPos)));

    cmdBaseDataPtr->cmdParam2 = 0;
    cmdBaseDataPtr->cmdParam2 += (uint16_t)(returnZeroCmdPtr->speedRatioUint16);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(returnZeroCmdPtr->speedRatioMode);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(returnZeroCmdPtr->runStepCurve);

    cmdBaseDataPtr->cmdParam3 = returnZeroCmdPtr->dirSwitchCorrect;

    cmdBaseDataPtr->cmdParam4 = 0;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(returnZeroCmdPtr->needReset);
    cmdBaseDataPtr->cmdParam4 <<= 8;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(0);
    cmdBaseDataPtr->cmdParam4 <<= 8;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(returnZeroCmdPtr->utilStopEnable);
    cmdBaseDataPtr->cmdParam4 <<= 8;
    cmdBaseDataPtr->cmdParam4 += (uint8_t)(returnZeroCmdPtr->emStopEnable);
    
    cmdBaseDataPtr->cmdParam5 = returnZeroCmdPtr->timeOutMs;
}

//设置电机持续运转参数
void IPC_SrvStepMotorSetRunAlwayCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_RUN_ALWAY* runAlwayCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = (uint32_t)runAlwayCmdPtr->runAlwaysDir;

    cmdBaseDataPtr->cmdParam2 = 0;
    cmdBaseDataPtr->cmdParam2 += (uint16_t)(runAlwayCmdPtr->speedRatioUint16);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runAlwayCmdPtr->speedRatioMode);
    cmdBaseDataPtr->cmdParam2 <<= 8;
    cmdBaseDataPtr->cmdParam2 += (uint8_t)(runAlwayCmdPtr->runStepCurve);
}

//设置电机减速停止参数
void IPC_SrvStepMotorSetSlowStopCmdParam(IPC_CMD_BASE_DATA* cmdBaseDataPtr,IPC_STEP_MOTOR_CMD_STOP_SLOW* stopSlowCmdPtr)
{
    //指令
    cmdBaseDataPtr->commandCode = IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN;
    //命令参数
    cmdBaseDataPtr->cmdParam1 = (uint32_t)stopSlowCmdPtr->slowDownMode;
    cmdBaseDataPtr->cmdParam2 = (uint32_t)stopSlowCmdPtr->slowDownSteps;
    cmdBaseDataPtr->cmdParam3 = (uint32_t)stopSlowCmdPtr->timeOutMs;
}















