/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 12:55:18 +0800
************************************************************************************************/ 
#include "CanMasterStepMotor.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterStepMotorCmdConfigArray[] = {
    {CAN_MASTER_CMD_SM_RESET               ,        ENABLE,     60000,      12,      9   },//步进电机控制复位
    {CAN_MASTER_CMD_SM_RUN_STEPS           ,        ENABLE,     60000,      12,      19  },//步进电机控制走步数
    {CAN_MASTER_CMD_SM_RUN_COORDINATE      ,        ENABLE,     60000,      12,      19  },//步进电机控制走坐标
    {CAN_MASTER_CMD_SM_RETURN_ZERO         ,        ENABLE,     60000,      12,      20  },//步进电机控制回零
    {CAN_MASTER_CMD_SM_RUN_ALWAYS          ,        ENABLE,     60000,      0 ,      5   },//步进电机控制持续运转
    {CAN_MASTER_CMD_SM_STOP_NOW            ,        ENABLE,     60000,      0 ,      0   },//步进电机控制立即停止
    {CAN_MASTER_CMD_SM_STOP_SLOW_DOWN      ,        ENABLE,     60000,      12,      9   },//步进电机控制减速运转
    {CAN_MASTER_CMD_SM_GET_COORDINATE      ,        ENABLE,     60000,      8 ,      0   },//步进电机控制获取当前坐标
    {CAN_MASTER_CMD_SM_READ_ORIGIN_SENSOR  ,        ENABLE,     60000,      4 ,      0   },//步进电机控制获取当前原点传感器状态
};


/*********************************************************发送指令等待ACK********************************************************************/
//步进电机复位
void CanMasterStepMotorResetWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteInt32ToBuffer(motorResetCmdPtr->resetCorrectPos,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)(motorResetCmdPtr->resetCorrectCurve);
    ProtUtilWriteUint32ToBuffer(motorResetCmdPtr->timeOutMs,canCmdSendPtr->cmdBufferPtr+5);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制走步数
void CanMasterStepMotorRunStepWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteInt32ToBuffer(motorRunStepCmdPtr->totalSteps,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)(motorRunStepCmdPtr->motorRunCurve);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)(motorRunStepCmdPtr->dirSwitchCorrect);
    ProtUtilWriteUint16ToBuffer(motorRunStepCmdPtr->speedRatioUint16,canCmdSendPtr->cmdBufferPtr+6);
    ProtUtilWriteUint32ToBuffer(motorRunStepCmdPtr->dirSwitchCorrect,canCmdSendPtr->cmdBufferPtr+8);
    canCmdSendPtr->cmdBufferPtr[12] = (uint8_t)(motorRunStepCmdPtr->emStopEnable);
    canCmdSendPtr->cmdBufferPtr[13] = (uint8_t)(motorRunStepCmdPtr->utilStop1Enable);
    canCmdSendPtr->cmdBufferPtr[14] = (uint8_t)(motorRunStepCmdPtr->utilStop2Enable);
    ProtUtilWriteUint32ToBuffer(motorRunStepCmdPtr->timeOutMs,canCmdSendPtr->cmdBufferPtr+15);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制走坐标
void CanMasterStepMotorRunCoordinateWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_COORDINATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteInt32ToBuffer(motorRunCoordinateCmdPtr->targetCoordinate,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)(motorRunCoordinateCmdPtr->motorRunCurve);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)(motorRunCoordinateCmdPtr->dirSwitchCorrect);
    ProtUtilWriteUint16ToBuffer(motorRunCoordinateCmdPtr->speedRatioUint16,canCmdSendPtr->cmdBufferPtr+6);
    ProtUtilWriteUint32ToBuffer(motorRunCoordinateCmdPtr->dirSwitchCorrect,canCmdSendPtr->cmdBufferPtr+8);
    canCmdSendPtr->cmdBufferPtr[12] = (uint8_t)(motorRunCoordinateCmdPtr->emStopEnable);
    canCmdSendPtr->cmdBufferPtr[13] = (uint8_t)(motorRunCoordinateCmdPtr->utilStop1Enable);
    canCmdSendPtr->cmdBufferPtr[14] = (uint8_t)(motorRunCoordinateCmdPtr->utilStop2Enable);
    ProtUtilWriteUint32ToBuffer(motorRunCoordinateCmdPtr->timeOutMs,canCmdSendPtr->cmdBufferPtr+15);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制回零
void CanMasterStepMotorReturnZeroWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RETURN_ZERO;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteInt32ToBuffer(motorReturnZeroCmdPtr->returnZeroCorrectPos,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)(motorReturnZeroCmdPtr->motorRunCurve);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)(motorReturnZeroCmdPtr->dirSwitchCorrect);
    ProtUtilWriteUint16ToBuffer(motorReturnZeroCmdPtr->speedRatioUint16,canCmdSendPtr->cmdBufferPtr+6);
    canCmdSendPtr->cmdBufferPtr[8] = (uint8_t)(motorReturnZeroCmdPtr->needReset);
    ProtUtilWriteUint32ToBuffer(motorReturnZeroCmdPtr->dirSwitchCorrect,canCmdSendPtr->cmdBufferPtr+9);
    canCmdSendPtr->cmdBufferPtr[13] = (uint8_t)(motorReturnZeroCmdPtr->emStopEnable);
    canCmdSendPtr->cmdBufferPtr[14] = (uint8_t)(motorReturnZeroCmdPtr->utilStop1Enable);
    canCmdSendPtr->cmdBufferPtr[15] = (uint8_t)(motorReturnZeroCmdPtr->utilStop2Enable);
    ProtUtilWriteUint32ToBuffer(motorReturnZeroCmdPtr->timeOutMs,canCmdSendPtr->cmdBufferPtr+16);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制持续运转
void CanMasterStepMotorRunAlwaysWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_ALWAYS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)(motorRunAlwaysCmdPtr->runAlwaysDir);
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)(motorRunAlwaysCmdPtr->motorRunCurve);
    canCmdSendPtr->cmdBufferPtr[2] = (uint8_t)(motorRunAlwaysCmdPtr->speedRatioMode);
    ProtUtilWriteUint16ToBuffer(motorRunAlwaysCmdPtr->speedRatioUint16,canCmdSendPtr->cmdBufferPtr+3);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制立即停止
void CanMasterStepMotorStopNowWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_STOP_NOW;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制减速运转
void CanMasterStepMotorStopSlowWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_STOP_SLOW_DOWN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)(motorStopSlowCmdPtr->slowDownMode);
    ProtUtilWriteUint32ToBuffer(motorStopSlowCmdPtr->slowDownSteps,canCmdSendPtr->cmdBufferPtr+1);
    ProtUtilWriteUint32ToBuffer(motorStopSlowCmdPtr->timeOutMs,canCmdSendPtr->cmdBufferPtr+5);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制获取当前坐标
void CanMasterStepMotorGetCoordinateWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_GET_COORDINATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorReadOriginSensorWhileAck(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_READ_ORIGIN_SENSOR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterStepMotorCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterStepMotorCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*********************************************************等待指定指令完成返回********************************************************************/
//步进电机复位
void CanMasterStepMotorWaitResetResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorStatePtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走步数
void CanMasterStepMotorWaitRunStepResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorStatePtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走坐标
void CanMasterStepMotorWaitRunCoordinateResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_COORDINATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorStatePtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制回零
void CanMasterStepMotorWaitReturnZeroResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RETURN_ZERO;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    
    if(motorStatePtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制持续运转
void CanMasterStepMotorWaitRunAlwaysResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_RUN_ALWAYS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制立即停止
void CanMasterStepMotorWaitStopNowResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_STOP_NOW;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制减速运转
void CanMasterStepMotorWaitStopSlowResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_STOP_SLOW_DOWN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorStatePtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetStateDefault(motorStatePtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorState(returnDataBufferPtr,motorStatePtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前坐标
void CanMasterStepMotorWaitGetCoordinateResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_GET_COORDINATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorPosPtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetPosDefault(motorPosPtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorPos(returnDataBufferPtr,motorPosPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorWaitReadOriginSensorResult(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    
    uint8_t singalChannel = (uint8_t)CanMasterStepMotorGetChByNo(motorNo);
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_SM_GET_COORDINATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(motorSensorPtr != NULL)
    {
        //结果解析
        CanMasterStepMotorSetSensorDefault(motorSensorPtr);
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            CanMasterStepMotorGetResultMotorSensor(returnDataBufferPtr,motorSensorPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*********************************************************发送指令等待完成返回********************************************************************/
//步进电机复位
void CanMasterStepMotorResetWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RESET* motorResetCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterStepMotorResetWhileAck(targetModule,targetBoardID,motorNo,motorResetCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitResetResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走步数
void CanMasterStepMotorRunStepWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_STEP* motorRunStepCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterStepMotorRunStepWhileAck(targetModule,targetBoardID,motorNo,motorRunStepCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorRunStepWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitRunStepResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitRunStepResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制走坐标
void CanMasterStepMotorRunCoordinateWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterStepMotorRunCoordinateWhileAck(targetModule,targetBoardID,motorNo,motorRunCoordinateCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorRunCoordinateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitRunCoordinateResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitRunCoordinateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制回零
void CanMasterStepMotorReturnZeroWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterStepMotorReturnZeroWhileAck(targetModule,targetBoardID,motorNo,motorReturnZeroCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorReturnZeroWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitReturnZeroResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitReturnZeroResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制持续运转
void CanMasterStepMotorRunAlwaysWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,CAN_MASTER_SM_CMD_RUN_ALWAY* motorRunAlwaysCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterStepMotorRunAlwaysWhileAck(targetModule,targetBoardID,motorNo,motorRunAlwaysCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorRunAlwaysWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitRunAlwaysResult(targetModule,targetBoardID,motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitRunAlwaysResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制立即停止
void CanMasterStepMotorStopNowWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterStepMotorStopNowWhileAck(targetModule,targetBoardID,motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorStopNowWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitStopNowResult(targetModule,targetBoardID,motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitStopNowResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制减速运转
void CanMasterStepMotorStopSlowWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo, CAN_MASTER_SM_CMD_STOP_SLOW* motorStopSlowCmdPtr,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_STATE* motorStatePtr)
{
    //发送指令等待ACK
    CanMasterStepMotorStopSlowWhileAck(targetModule,targetBoardID,motorNo,motorStopSlowCmdPtr,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorStopSlowWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitStopSlowResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitStopSlowResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前坐标
void CanMasterStepMotorGetCoordinateWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_POS* motorPosPtr)
{
    //发送指令等待ACK
    CanMasterStepMotorGetCoordinateWhileAck(targetModule,targetBoardID,motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorGetCoordinateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitGetCoordinateResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitGetCoordinateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//步进电机控制获取当前原点传感器状态
void CanMasterStepMotorReadOriginSensorWhileReturn(uint8_t targetModule,uint8_t targetBoardID,STEP_MOTOR_NO motorNo,
                                                    SYS_RESULT_PACK* resultPackPtr,CAN_MASTER_SM_SENSOR* motorSensorPtr)
{
    //发送指令等待ACK
    CanMasterStepMotorReadOriginSensorWhileAck(targetModule,targetBoardID,motorNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorReadOriginSensorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterStepMotorWaitReadOriginSensorResult(targetModule,targetBoardID,motorNo,resultPackPtr,motorSensorPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterStepMotorWaitReadOriginSensorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}























