/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:28:40
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseStepMotor14.h"


//电机复位            
void SrvImplBaseStepMotor14Reset(StepMotorResetCmd* resetCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //开始复位
    errorSub = StepMotorReset(resetCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[9] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[10] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[11] = (uint8_t)PIN_STATE_INVALID;
}

//电机走步数            
void SrvImplBaseStepMotor14RunStep(StepMotorRunStepsCmd* runStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    MOTOR_SENSOR_FLAGS motorSensorFlags;
    //执行走步数
    errorSub = StepMotorRunSteps(runStepCmdPtr,&motorSensorFlags);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[9] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[10] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[11] = (uint8_t)PIN_STATE_INVALID;
}

//电机走坐标            
void SrvImplBaseStepMotor14RunCoordinate(StepMotorRun2CoordationCmd* runCoordCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    MOTOR_SENSOR_FLAGS motorSensorFlags;
    //执行走坐标
    errorSub = StepMotorRunCoordinate(runCoordCmdPtr,&motorSensorFlags);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[9] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[10] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[11] = (uint8_t)PIN_STATE_INVALID;
}

//电机回零            
void SrvImplBaseStepMotor14ReturnZero(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //执行回零
    errorSub = StepMotorReturnZero(returnZeroCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[9] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[10] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[11] = (uint8_t)PIN_STATE_INVALID;
    
}

//电机持续运转            
void SrvImplBaseStepMotor14RunAlways(StepMotorRunAlwaysCmd* runAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //执行持续运转
    errorSub = StepMotorRunAlwaysAcc(runAlwayCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    
}

//电机立即停止            
void SrvImplBaseStepMotor14StopNow(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //立即停止
    errorSub = StepMotorRunAlwaysStopImmediate(STEP_MOTOR_NO14);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
} 

//电机减速停止            
void SrvImplBaseStepMotor14StopSlowDown(uint8_t slowDownMode,uint32_t slowDownSteps,uint32_t timeOutMs,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //执行减速停止
    if(slowDownMode == 0)
    {
        //慢速
        errorSub = StepMotorRunAlwaysDecSlow(STEP_MOTOR_NO14,timeOutMs);
    }
    else
    {
        //快速
        errorSub = StepMotorRunAlwaysDecFast(STEP_MOTOR_NO14,slowDownSteps,timeOutMs);
    }
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[9] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[10] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[11] = (uint8_t)PIN_STATE_INVALID;
}

//获取电机当前坐标            
void SrvImplBaseStepMotor14GetCoordinate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 8;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = 0;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    //读取当前坐标
    StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO14,&motorCurrentCoordinate,&motorCurrentEncoder);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    
}

//获取当前原点传感器状态            
void SrvImplBaseStepMotor14ReadOriginSensor(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 4;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取电机传感器状态
    MOTOR_SENSOR_SET motorSensorStates;
    StepMotorReadSensorState(STEP_MOTOR_NO14,&motorSensorStates);
    resultBufferPtr[0] = (uint8_t)motorSensorStates.resetSensorState;
    resultBufferPtr[1] = (uint8_t)motorSensorStates.emStopSensorState;
    resultBufferPtr[2] = (uint8_t)motorSensorStates.utilStopSensorState;
    resultBufferPtr[3] = (uint8_t)PIN_STATE_INVALID;
    
}

//电机失能            
void SrvImplBaseStepMotor14Disable(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //立即停止
    errorSub = StepMotorMainDisable(STEP_MOTOR_NO14);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
}

//电机进入教学模式            
void SrvImplBaseStepMotor14EnterTeachMode(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //立即停止
    errorSub = StepMotorMainEnterTeachingMode(STEP_MOTOR_NO14);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
}


//电机强制进入默认状态
void SrvImplBaseStepMotor14ForceToDefault(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //立即停止
    errorSub = StepMotorMainForceToDefaultState(STEP_MOTOR_NO14);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
}









