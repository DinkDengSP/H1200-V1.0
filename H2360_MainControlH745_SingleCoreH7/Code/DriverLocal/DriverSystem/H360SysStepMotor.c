/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 18:00:52 +0800
************************************************************************************************/ 
#include "H360SysStepMotor.h"

//获取板号和模块号
static ERROR_SUB H360SysStepMotorGetModuleBoard(H360_SYS_SM motorNo,uint8_t* moduleNo,uint8_t* boardID,
                                            STEP_MOTOR_NO* convertMotorNo)
{
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
    return ERROR_SUB_OK;
}

/*********************************************************发送指令等待ACK********************************************************************/
//步进电机复位
void H360SysStepMotorResetWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorResetWhileAck(moduleNo,boardID,stepMotorNoLocal,motorResetCmdPtr,resultPackPtr);
    return;
}

//步进电机控制走步数
void H360SysStepMotorRunStepWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorRunStepWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunStepCmdPtr,resultPackPtr);
    return;
}

//步进电机控制走坐标
void H360SysStepMotorRunCoordinateWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorRunCoordinateWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunCoordinateCmdPtr,resultPackPtr);
    return;
}

//步进电机控制回零
void H360SysStepMotorReturnZeroWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorReturnZeroWhileAck(moduleNo,boardID,stepMotorNoLocal,motorReturnZeroCmdPtr,resultPackPtr);
    return;
}

//步进电机控制持续运转
void H360SysStepMotorRunAlwaysWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                            SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorRunAlwaysWhileAck(moduleNo,boardID,stepMotorNoLocal,motorRunAlwaysCmdPtr,resultPackPtr);
    return;
}

//步进电机控制立即停止
void H360SysStepMotorStopNowWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorStopNowWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制减速运转
void H360SysStepMotorStopSlowWhileAck(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorStopSlowWhileAck(moduleNo,boardID,stepMotorNoLocal,motorStopSlowCmdPtr,resultPackPtr);
    return;
}

//步进电机控制获取当前坐标
void H360SysStepMotorGetCoordinateWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorGetCoordinateWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制获取当前原点传感器状态
void H360SysStepMotorReadOriginSensorWhileAck(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorReadOriginSensorWhileAck(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}


/*********************************************************等待指定指令完成返回********************************************************************/
//步进电机复位
void H360SysStepMotorWaitResetResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitResetResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制走步数
void H360SysStepMotorWaitRunStepResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitRunStepResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制走坐标
void H360SysStepMotorWaitRunCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitRunCoordinateResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制回零
void H360SysStepMotorWaitReturnZeroResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitReturnZeroResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制持续运转
void H360SysStepMotorWaitRunAlwaysResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitRunAlwaysResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制立即停止
void H360SysStepMotorWaitStopNowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitStopNowResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr);
    return;
}

//步进电机控制减速运转
void H360SysStepMotorWaitStopSlowResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitStopSlowResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorStatePtr);
    return;
}

//步进电机控制获取当前坐标
void H360SysStepMotorWaitGetCoordinateResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitGetCoordinateResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorPosPtr);
    return;
}

//步进电机控制获取当前原点传感器状态
void H360SysStepMotorWaitReadOriginSensorResult(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t moduleNo = 0;
    uint8_t boardID = 0;
    STEP_MOTOR_NO stepMotorNoLocal = STEP_MOTOR_NO1;
    
    //识别端口号
    errorSub = H360SysStepMotorGetModuleBoard(motorNo,&moduleNo,&boardID,&stepMotorNoLocal);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_STEP_MOTOR_SYS_CONTROL,
                                        ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    CanMasterStepMotorWaitReadOriginSensorResult(moduleNo,boardID,stepMotorNoLocal,resultPackPtr,motorSensorPtr);
    return;
}


/*********************************************************发送指令等待完成返回********************************************************************/
//步进电机复位
void H360SysStepMotorResetWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysStepMotorResetWhileAck(motorNo,motorResetCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitResetResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走步数
void H360SysStepMotorRunStepWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysStepMotorRunStepWhileAck(motorNo,motorRunStepCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitRunStepResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走坐标
void H360SysStepMotorRunCoordinateWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysStepMotorRunCoordinateWhileAck(motorNo,motorRunCoordinateCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitRunCoordinateResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制回零
void H360SysStepMotorReturnZeroWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysStepMotorReturnZeroWhileAck(motorNo,motorReturnZeroCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitReturnZeroResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制持续运转
void H360SysStepMotorRunAlwaysWhileReturn(H360_SYS_SM motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    H360SysStepMotorRunAlwaysWhileAck(motorNo,motorRunAlwaysCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitRunAlwaysResult(motorNo,resultPackPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制立即停止
void H360SysStepMotorStopNowWhileReturn(H360_SYS_SM motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    H360SysStepMotorStopNowWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitStopNowResult(motorNo,resultPackPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制减速运转
void H360SysStepMotorStopSlowWhileReturn(H360_SYS_SM motorNo, CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    H360SysStepMotorStopSlowWhileAck(motorNo,motorStopSlowCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitStopSlowResult(motorNo,resultPackPtr,motorStatePtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前坐标
void H360SysStepMotorGetCoordinateWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    //发送指令等待ACK
    H360SysStepMotorGetCoordinateWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitGetCoordinateResult(motorNo,resultPackPtr,motorPosPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}   

//步进电机控制获取当前原点传感器状态
void H360SysStepMotorReadOriginSensorWhileReturn(H360_SYS_SM motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    //发送指令等待ACK
    H360SysStepMotorReadOriginSensorWhileAck(motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        return;
    }
    //等待结果
    H360SysStepMotorWaitReadOriginSensorResult(motorNo,resultPackPtr,motorSensorPtr);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}











