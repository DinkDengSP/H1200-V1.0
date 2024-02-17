/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 13:59:03 +0800
************************************************************************************************/ 
#include "H360SysActStepMotor.h"
#include "IPC_SrvStepMotor.h"

//获取板号和模块号
static ERROR_SUB H360SysActStepMotorGetModuleBoard(H360_SYS_SM motorNo,uint8_t* moduleNo,uint8_t* boardID,
                                            STEP_MOTOR_NO* convertMotorNo,FlagStatus* isSelfBoard)
{
    *isSelfBoard = RESET;
    if(motorNo <= H360_SYS_BOARD1_SM12)
    {
        //下位机板1
        *moduleNo = CAN_MODULE_NO_BOARD1;
        *boardID = CAN_BOARD_ID_BOARD1;
        *convertMotorNo = (STEP_MOTOR_NO)(((uint16_t)(motorNo)) - ((uint16_t)(H360_SYS_BOARD1_SM1)));
    }
    else if(motorNo <= H360_SYS_BOARD2_SM12)
    {
        //下位机板2
        *moduleNo = CAN_MODULE_NO_BOARD2;
        *boardID = CAN_BOARD_ID_BOARD2;
        *convertMotorNo = (STEP_MOTOR_NO)(((uint16_t)(motorNo)) - ((uint16_t)(H360_SYS_BOARD2_SM1)));
    }
    else if(motorNo <= H360_SYS_BOARD3_SM12)
    {
        //下位机板3
        *moduleNo = CAN_MODULE_NO_BOARD3;
        *boardID = CAN_BOARD_ID_BOARD3;
        *convertMotorNo = (STEP_MOTOR_NO)(((uint16_t)(motorNo)) - ((uint16_t)(H360_SYS_BOARD3_SM1)));
    }
    else if(motorNo <= H360_SYS_BOARD4_SM12)
    {
        //下位机板4
        *moduleNo = CAN_MODULE_NO_BOARD4;
        *boardID = CAN_BOARD_ID_BOARD4;
        *convertMotorNo = (STEP_MOTOR_NO)(((uint16_t)(motorNo)) - ((uint16_t)(H360_SYS_BOARD4_SM1)));
    }
    else if(motorNo <= H360_SYS_BOARD5_SM12)
    {
        //下位机板5
        *moduleNo = CAN_MODULE_NO_BOARD5;
        *boardID = CAN_BOARD_ID_BOARD5;
        *convertMotorNo = (STEP_MOTOR_NO)(((uint16_t)(motorNo)) - ((uint16_t)(H360_SYS_BOARD5_SM1)));
    }
    else
    {
        return ERROR_SUB_SYS_STEP_MOTOR_INDEX;
    }
    if((*moduleNo == SELF_MODULE_NO)&&(*boardID == SELF_BOARD_ID))
    {
        *isSelfBoard = SET;
    }
    return ERROR_SUB_OK;
}

/*********************************************************发送指令等待ACK********************************************************************/
//步进电机复位
void H360SysActStepMotorResetWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_RESET ipcMotorResetCmd;
        //数据拷贝
        ipcMotorResetCmd.resetCorrectPos = motorResetCmdPtr->resetCorrectPos;
        ipcMotorResetCmd.resetCorrectCurve = motorResetCmdPtr->resetCorrectCurve;
        ipcMotorResetCmd.timeOutMs = motorResetCmdPtr->timeOutMs;
        //调用板上IPC实现
        IPC_SrvStepMotorResetWhileAck(stepMotorNoLocal,&ipcMotorResetCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorResetWhileAck(moduleNo,boardID,stepMotorNoLocal,motorResetCmdPtr,resultPackPtr);
    return;
}

//步进电机控制走步数
void H360SysActStepMotorRunStepWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_RUN_STEP ipcMotorRunStepCmd;
        //数据拷贝
        ipcMotorRunStepCmd.totalSteps = motorRunStepCmdPtr->totalSteps;
        ipcMotorRunStepCmd.runStepCurve = motorRunStepCmdPtr->runStepCurve;
        ipcMotorRunStepCmd.speedRatioMode = motorRunStepCmdPtr->speedRatioMode;
        ipcMotorRunStepCmd.speedRatioUint16 = motorRunStepCmdPtr->speedRatioUint16;
        ipcMotorRunStepCmd.dirSwitchCorrect = motorRunStepCmdPtr->dirSwitchCorrect;
        ipcMotorRunStepCmd.emStopEnable = motorRunStepCmdPtr->emStopEnable;
        ipcMotorRunStepCmd.utilStop1Enable = motorRunStepCmdPtr->utilStop1Enable;
        ipcMotorRunStepCmd.utilStop2Enable = motorRunStepCmdPtr->utilStop2Enable;
        ipcMotorRunStepCmd.timeOutMs = motorRunStepCmdPtr->timeOutMs;
        //调用板上IPC实现
        IPC_SrvStepMotorRunStepWhileAck(stepMotorNoLocal,&ipcMotorRunStepCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorRunStepWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunStepCmdPtr,resultPackPtr);
    return;
}

//步进电机控制走坐标
void H360SysActStepMotorRunCoordinateWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_RUN_COORDINATE ipcMotorRunCoordinateCmd;
        //数据拷贝
        ipcMotorRunCoordinateCmd.targetCoordinate = motorRunCoordinateCmdPtr->targetCoordinate;
        ipcMotorRunCoordinateCmd.runStepCurve = motorRunCoordinateCmdPtr->runStepCurve;
        ipcMotorRunCoordinateCmd.speedRatioMode = motorRunCoordinateCmdPtr->speedRatioMode;
        ipcMotorRunCoordinateCmd.speedRatioUint16 = motorRunCoordinateCmdPtr->speedRatioUint16;
        ipcMotorRunCoordinateCmd.dirSwitchCorrect = motorRunCoordinateCmdPtr->dirSwitchCorrect;
        ipcMotorRunCoordinateCmd.emStopEnable = motorRunCoordinateCmdPtr->emStopEnable;
        ipcMotorRunCoordinateCmd.utilStop1Enable = motorRunCoordinateCmdPtr->utilStop1Enable;
        ipcMotorRunCoordinateCmd.utilStop2Enable = motorRunCoordinateCmdPtr->utilStop2Enable;
        ipcMotorRunCoordinateCmd.timeOutMs = motorRunCoordinateCmdPtr->timeOutMs;
        //调用板上IPC实现
        IPC_SrvStepMotorRunCoordinateWhileAck(stepMotorNoLocal,&ipcMotorRunCoordinateCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorRunCoordinateWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunCoordinateCmdPtr,resultPackPtr);
    return;
}

//步进电机控制回零
void H360SysActStepMotorReturnZeroWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_RETURN_ZERO ipcReturnZeroCmd;
        //数据拷贝
        ipcReturnZeroCmd.returnZeroCorrectPos = motorReturnZeroCmdPtr->returnZeroCorrectPos;
        ipcReturnZeroCmd.runStepCurve = motorReturnZeroCmdPtr->runStepCurve;
        ipcReturnZeroCmd.speedRatioMode = motorReturnZeroCmdPtr->speedRatioMode;
        ipcReturnZeroCmd.speedRatioUint16 = motorReturnZeroCmdPtr->speedRatioUint16;
        ipcReturnZeroCmd.dirSwitchCorrect = motorReturnZeroCmdPtr->dirSwitchCorrect;
        ipcReturnZeroCmd.emStopEnable = motorReturnZeroCmdPtr->emStopEnable;
        ipcReturnZeroCmd.utilStop1Enable = motorReturnZeroCmdPtr->utilStop1Enable;
        ipcReturnZeroCmd.utilStop2Enable = motorReturnZeroCmdPtr->utilStop2Enable;
        ipcReturnZeroCmd.needReset = motorReturnZeroCmdPtr->needReset;
        ipcReturnZeroCmd.timeOutMs = motorReturnZeroCmdPtr->timeOutMs;
        //调用板上IPC实现
        IPC_SrvStepMotorReturnZeroWhileAck(stepMotorNoLocal,&ipcReturnZeroCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorReturnZeroWhileAck(moduleNo,boardID,stepMotorNoLocal,motorReturnZeroCmdPtr,resultPackPtr);
    return;
}

//步进电机控制持续运转
void H360SysActStepMotorRunAlwaysWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_RUN_ALWAY ipcRunAlwaysCmd;
        //数据拷贝
        ipcRunAlwaysCmd.runAlwaysDir = motorRunAlwaysCmdPtr->runAlwaysDir;
        ipcRunAlwaysCmd.runStepCurve = motorRunAlwaysCmdPtr->runStepCurve;
        ipcRunAlwaysCmd.speedRatioMode = motorRunAlwaysCmdPtr->speedRatioMode;
        ipcRunAlwaysCmd.speedRatioUint16 = motorRunAlwaysCmdPtr->speedRatioUint16;
        //调用板上IPC实现
        IPC_SrvStepMotorRunAlwaysWhileAck(stepMotorNoLocal,&ipcRunAlwaysCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorRunAlwaysWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunAlwaysCmdPtr,resultPackPtr);
    return;
}

//步进电机控制立即停止
void H360SysActStepMotorStopNowWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板上IPC实现
        IPC_SrvStepMotorStopNowWhileAck(stepMotorNoLocal,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorStopNowWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制减速运转
void H360SysActStepMotorStopSlowWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_CMD_STOP_SLOW ipcMotorStopSlowCmd;
        //数据拷贝
        ipcMotorStopSlowCmd.slowDownMode = motorStopSlowCmdPtr->slowDownMode;
        ipcMotorStopSlowCmd.slowDownSteps = motorStopSlowCmdPtr->slowDownSteps;
        ipcMotorStopSlowCmd.timeOutMs = motorStopSlowCmdPtr->timeOutMs;
        //调用板上IPC实现
        IPC_SrvStepMotorStopSlowDownWhileAck(stepMotorNoLocal,&ipcMotorStopSlowCmd,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorStopSlowWhileAck(moduleNo,boardID,stepMotorNoLocal,motorStopSlowCmdPtr,resultPackPtr);
    return;
}

//步进电机控制获取当前坐标
void H360SysActStepMotorGetCoordinateWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板上IPC实现
        IPC_SrvStepMotorGetCoordinateWhileAck(stepMotorNoLocal,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorGetCoordinateWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorReadOriginSensorWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板上IPC实现
        IPC_SrvStepMotorReadOriginSensorWhileAck(stepMotorNoLocal,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorReadOriginSensorWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}


/*********************************************************等待指定指令完成返回********************************************************************/
//步进电机复位
void H360SysActStepMotorWaitResetResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_STATE ipcMotorState;
        //调用板上IPC实现
        IPC_SrvStepMotorResetWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorState);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorStatePtr->motorPosCurrent = ipcMotorState.motorPosCurrent;
        motorStatePtr->encoderPosCurrent = ipcMotorState.encoderPosCurrent;
        motorStatePtr->originSensorState = ipcMotorState.originSensorState;
        motorStatePtr->emStopSensorState = ipcMotorState.emStopSensorState;
        motorStatePtr->utilStop1SensorState = ipcMotorState.utilStop1SensorState;
        motorStatePtr->utilStop2SensorState = ipcMotorState.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitResetResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制走步数
void H360SysActStepMotorWaitRunStepResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_STATE ipcMotorState;
        //调用板上IPC实现
        IPC_SrvStepMotorRunStepWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorState);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorStatePtr->motorPosCurrent = ipcMotorState.motorPosCurrent;
        motorStatePtr->encoderPosCurrent = ipcMotorState.encoderPosCurrent;
        motorStatePtr->originSensorState = ipcMotorState.originSensorState;
        motorStatePtr->emStopSensorState = ipcMotorState.emStopSensorState;
        motorStatePtr->utilStop1SensorState = ipcMotorState.utilStop1SensorState;
        motorStatePtr->utilStop2SensorState = ipcMotorState.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitRunStepResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制走坐标
void H360SysActStepMotorWaitRunCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_STATE ipcMotorState;
        //调用板上IPC实现
        IPC_SrvStepMotorRunCoordinateWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorState);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorStatePtr->motorPosCurrent = ipcMotorState.motorPosCurrent;
        motorStatePtr->encoderPosCurrent = ipcMotorState.encoderPosCurrent;
        motorStatePtr->originSensorState = ipcMotorState.originSensorState;
        motorStatePtr->emStopSensorState = ipcMotorState.emStopSensorState;
        motorStatePtr->utilStop1SensorState = ipcMotorState.utilStop1SensorState;
        motorStatePtr->utilStop2SensorState = ipcMotorState.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitRunCoordinateResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制回零
void H360SysActStepMotorWaitReturnZeroResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_STATE ipcMotorState;
        //调用板上IPC实现
        IPC_SrvStepMotorReturnZeroWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorState);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorStatePtr->motorPosCurrent = ipcMotorState.motorPosCurrent;
        motorStatePtr->encoderPosCurrent = ipcMotorState.encoderPosCurrent;
        motorStatePtr->originSensorState = ipcMotorState.originSensorState;
        motorStatePtr->emStopSensorState = ipcMotorState.emStopSensorState;
        motorStatePtr->utilStop1SensorState = ipcMotorState.utilStop1SensorState;
        motorStatePtr->utilStop2SensorState = ipcMotorState.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitReturnZeroResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制持续运转
void H360SysActStepMotorWaitRunAlwaysResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板上IPC实现
        IPC_SrvStepMotorRunAlwaysWaitCmdReturn(stepMotorNoLocal,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorWaitRunAlwaysResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制立即停止
void H360SysActStepMotorWaitStopNowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        //调用板上IPC实现
        IPC_SrvStepMotorStopNowWaitCmdReturn(stepMotorNoLocal,resultPackPtr);
        return;
    }
    //不是自身
    CanMasterStepMotorWaitStopNowResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制减速运转
void H360SysActStepMotorWaitStopSlowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_STATE ipcMotorState;
        //调用板上IPC实现
        IPC_SrvStepMotorStopSlowDownWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorState);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorStatePtr->motorPosCurrent = ipcMotorState.motorPosCurrent;
        motorStatePtr->encoderPosCurrent = ipcMotorState.encoderPosCurrent;
        motorStatePtr->originSensorState = ipcMotorState.originSensorState;
        motorStatePtr->emStopSensorState = ipcMotorState.emStopSensorState;
        motorStatePtr->utilStop1SensorState = ipcMotorState.utilStop1SensorState;
        motorStatePtr->utilStop2SensorState = ipcMotorState.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitStopSlowResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制获取当前坐标
void H360SysActStepMotorWaitGetCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_POS ipcMotorPos;
        //调用板上IPC实现
        IPC_SrvStepMotorGetCoordinateWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorPos);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorPosPtr->motorPosCurrent = ipcMotorPos.motorPosCurrent;
        motorPosPtr->encoderPosCurrent = ipcMotorPos.encoderPosCurrent;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitGetCoordinateResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorPosPtr);
    return;
}

//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorWaitReadOriginSensorResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    FlagStatus isSelfBoard = RESET;
    //识别端口号
    errorSub = H360SysActStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal,&isSelfBoard);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //检查是否自身
    if(isSelfBoard == SET)
    {
        IPC_STEP_MOTOR_SENSOR ipcMotorSensor;
        //调用板上IPC实现
        IPC_SrvStepMotorReadOriginSensorWaitCmdReturn(stepMotorNoLocal,resultPackPtr,&ipcMotorSensor);
        if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
        {
            return;
        }
        //数据拷贝
        motorSensorPtr->originSensorState = ipcMotorSensor.originSensorState;
        motorSensorPtr->emStopSensorState = ipcMotorSensor.emStopSensorState;
        motorSensorPtr->utilStop1SensorState = ipcMotorSensor.utilStop1SensorState;
        motorSensorPtr->utilStop2SensorState = ipcMotorSensor.utilStop2SensorState;
        return;
    }
    //不是自身
    CanMasterStepMotorWaitReadOriginSensorResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorSensorPtr);
    return;
}


/*********************************************************发送指令等待完成返回********************************************************************/
//步进电机复位
void H360SysActStepMotorResetWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysActStepMotorResetWhileAck(motorNo,motorResetCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitResetResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走步数
void H360SysActStepMotorRunStepWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysActStepMotorRunStepWhileAck(motorNo,motorRunStepCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitRunStepResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走坐标
void H360SysActStepMotorRunCoordinateWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysActStepMotorRunCoordinateWhileAck(motorNo,motorRunCoordinateCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitRunCoordinateResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制回零
void H360SysActStepMotorReturnZeroWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysActStepMotorReturnZeroWhileAck(motorNo,motorReturnZeroCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitReturnZeroResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制持续运转
void H360SysActStepMotorRunAlwaysWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    H360SysActStepMotorRunAlwaysWhileAck(motorNo,motorRunAlwaysCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitRunAlwaysResult(motorNo,resultPackPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制立即停止
void H360SysActStepMotorStopNowWhileReturn(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    H360SysActStepMotorStopNowWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitStopNowResult(motorNo,resultPackPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制减速运转
void H360SysActStepMotorStopSlowWhileReturn(H360_SYS_SM motorNo, CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysActStepMotorStopSlowWhileAck(motorNo,motorStopSlowCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitStopSlowResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前坐标
void H360SysActStepMotorGetCoordinateWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    //发送指令等待ACK
    H360SysActStepMotorGetCoordinateWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitGetCoordinateResult(motorNo,resultPackPtr,motorPosPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}   

//步进电机控制获取当前原点传感器状态
void H360SysActStepMotorReadOriginSensorWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    //发送指令等待ACK
    H360SysActStepMotorReadOriginSensorWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysActStepMotorWaitReadOriginSensorResult(motorNo,resultPackPtr,motorSensorPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}











