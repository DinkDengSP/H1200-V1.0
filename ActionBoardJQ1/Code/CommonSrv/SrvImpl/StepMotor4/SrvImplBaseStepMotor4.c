/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 18:38:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-26 16:24:34 +0800
************************************************************************************************/ 
#include "SrvImplStepMotor4.h"
#include "SrvImplBaseStepMotor4.h"
#include "SrvTaskHeader.h"


//电机复位            
void SrvImplBaseStepMotor4Reset(StepMotorResetCmd* resetCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
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
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    if(ERROR_SUB_OK != StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder))
    {
        motorCurrentEncoder = 0;
    }
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)originSensorState;
    resultBufferPtr[9] = (uint8_t)emStopSensorState;
    resultBufferPtr[10] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[11] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 

//电机走步数            
void SrvImplBaseStepMotor4RunStep(StepMotorRunStepsCmd* runStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //执行走步数
    errorSub = StepMotorRunSteps(runStepCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    if(ERROR_SUB_OK != StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder))
    {
        motorCurrentEncoder = 0;
    }
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)originSensorState;
    resultBufferPtr[9] = (uint8_t)emStopSensorState;
    resultBufferPtr[10] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[11] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//电机走坐标            
void SrvImplBaseStepMotor4RunCoordinate(StepMotorRun2CoordationCmd* runCoordCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //执行走坐标
    errorSub = StepMotorRun2SpecialCoordinates(runCoordCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    if(ERROR_SUB_OK != StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder))
    {
        motorCurrentEncoder = 0;
    }
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)originSensorState;
    resultBufferPtr[9] = (uint8_t)emStopSensorState;
    resultBufferPtr[10] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[11] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//电机回零            
void SrvImplBaseStepMotor4ReturnZero(StepMotorReturnZeroCmd* returnZeroCmdPtr,uint8_t needReset,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //执行回零
    if(needReset == 0)
    {
        //不复位
        errorSub = StepMotorReturnZero(returnZeroCmdPtr);
    }
    else
    {
        //复位
        errorSub = StepMotorReturnZeroReset(returnZeroCmdPtr);
    }
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    if(ERROR_SUB_OK != StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder))
    {
        motorCurrentEncoder = 0;
    }
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)originSensorState;
    resultBufferPtr[9] = (uint8_t)emStopSensorState;
    resultBufferPtr[10] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[11] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//电机持续运转            
void SrvImplBaseStepMotor4RunAlways(StepMotorRunAlwaysCmd* runAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //执行持续运转
    errorSub = StepMotorRunAlways(runAlwayCmdPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//电机立即停止            
void SrvImplBaseStepMotor4StopNow(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //立即停止
    errorSub = StepMotorStopImmediately(STEP_MOTOR_NO4);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//电机减速停止            
void SrvImplBaseStepMotor4StopSlowDown(uint8_t slowDownMode,uint32_t slowDownSteps,uint32_t timeOutMs,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 12;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //执行减速停止
    if(slowDownMode == 0)
    {
        //慢速
        errorSub = StepMotorStopDeceleration(STEP_MOTOR_NO4);
    }
    else
    {
        //快速
        errorSub = StepMotorStopDeceleration1(STEP_MOTOR_NO4);
    }
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_ERROR,
                                    errorSub);
        return;
    }
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    if(ERROR_SUB_OK != StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder))
    {
        motorCurrentEncoder = 0;
    }
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    resultBufferPtr[8] = (uint8_t)originSensorState;
    resultBufferPtr[9] = (uint8_t)emStopSensorState;
    resultBufferPtr[10] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[11] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//获取电机当前坐标            
void SrvImplBaseStepMotor4GetCoordinate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //返回内存长度设定
    *dataBufLen = 8;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取当前电机坐标
    int32_t motorCurrentCoordinate = stepMotorVarArray[STEP_MOTOR_NO4].var.currentCoordinate;
    //读取电机当前编码器坐标
    int32_t motorCurrentEncoder = 0;
    errorSub = StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO4,&motorCurrentEncoder);
    if(ERROR_SUB_OK != errorSub)
    {
        motorCurrentEncoder = 0;
    }
    //数据写入返回
    ProtUtilWriteInt32ToBuffer(motorCurrentCoordinate,resultBufferPtr);
    ProtUtilWriteInt32ToBuffer(motorCurrentEncoder,resultBufferPtr+4);
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 

//获取当前原点传感器状态            
void SrvImplBaseStepMotor4ReadOriginSensor(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 4;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,*dataBufLen);
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //读取电机传感器状态
    PIN_STATE originSensorState,emStopSensorState,utilStop1SensorState,utilStop2SensorState;
    StepMotorReadSensorState(STEP_MOTOR_NO4,&originSensorState,&emStopSensorState,&utilStop1SensorState,
                                &utilStop2SensorState);
    resultBufferPtr[0] = (uint8_t)originSensorState;
    resultBufferPtr[1] = (uint8_t)emStopSensorState;
    resultBufferPtr[2] = (uint8_t)utilStop1SensorState;
    resultBufferPtr[3] = (uint8_t)utilStop2SensorState;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
} 
 
















