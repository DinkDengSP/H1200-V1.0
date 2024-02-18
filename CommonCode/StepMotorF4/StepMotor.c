/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:15:22
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-04 10:20:27
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorInt.h"

//电机的默认配置
#if(SYSTEM_STEP_MOTOR_COUNT > 0)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex1;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 1)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex2;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 2)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex3;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 3)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex4;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 4)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex5;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 5)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex6;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 6)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex7;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 7)
extern const STEP_MOTOR_ATTRIBUTE StepMotorDefaultAttriIndex8;
#endif

//步进电机参数数组
STEP_MOTOR_VAR stepMotorVarArray[SYSTEM_STEP_MOTOR_COUNT] = {0};

//获取默认参数的指针
static STEP_MOTOR_ATTRIBUTE* StepMotorGetAttriByIndex(uint8_t index)
{
    STEP_MOTOR_ATTRIBUTE* attriPtr = NULL;
    switch(index)
    {
#if(SYSTEM_STEP_MOTOR_COUNT > 0)
        case 0:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex1);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 1)
        case 1:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex2);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 2)
        case 2:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex3);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 3)
        case 3:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex4);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 4)
        case 4:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex5);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 5)
        case 5:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex6);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 6)
        case 6:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex7);
            break;
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 7)
        case 7:
            attriPtr = (STEP_MOTOR_ATTRIBUTE*)(&StepMotorDefaultAttriIndex8);
            break;   
#endif 
        default:
            attriPtr = NULL;
            break;
    }
    return attriPtr;
}

//初始化步进电机状态
void StepMotorVarInit(void)
{
    uint16_t indexUtil = 0;
    //首先做好固定参数指针的映射
    for(indexUtil = 0; indexUtil < SYSTEM_STEP_MOTOR_COUNT; indexUtil++)
    {
        //电机属性映射
        stepMotorVarArray[indexUtil].attriPtr = StepMotorGetAttriByIndex(indexUtil);
        uint16_t indexUtilSub = 0;
        //电机状态初始化
        stepMotorVarArray[indexUtil].var.motorMainState = STEP_MOTOR_MAIN_STOP;
        stepMotorVarArray[indexUtil].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        stepMotorVarArray[indexUtil].var.currentDir = STEP_MOTOR_DIR_CW;//当前方向
        stepMotorVarArray[indexUtil].var.currentCoordinate = 0;//当前坐标
        stepMotorVarArray[indexUtil].var.stepMotorStartRunningTimeMs = 0;//本次运动启动时间
        stepMotorVarArray[indexUtil].var.stepMotorRunningTimeMsCount = 0;//本次运动已用时间
        //三个信号的状态
        stepMotorVarArray[indexUtil].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        stepMotorVarArray[indexUtil].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        stepMotorVarArray[indexUtil].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        stepMotorVarArray[indexUtil].var.currentTableIndex = 0;//当前步数表序号
        stepMotorVarArray[indexUtil].var.resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
        stepMotorVarArray[indexUtil].var.currentTableMaxLength = 0;
        //实际速度表和步数表
        for(indexUtilSub = 0; indexUtilSub < SM_SPEED_TABLE_LENGTH_MAX; indexUtilSub++)
        {
            //速度表归零
            stepMotorVarArray[indexUtil].var.stepMotorSpeedTableDirect[indexUtilSub] = 0.0;
            //步数表归零
            stepMotorVarArray[indexUtil].var.stepMotorStepTableDirect[indexUtilSub] = 0;
        }
        //当前减速步数表序号
        stepMotorVarArray[indexUtil].var.currentTableIndex = 0;
        stepMotorVarArray[indexUtil].var.currentTableMaxLength = 0;
        //减速速度和步数表
        for(indexUtilSub = 0; indexUtilSub < SM_SPEED_TABLE_LENGTH_MAX/2; indexUtilSub++)
        {
            //速度表归零
            stepMotorVarArray[indexUtil].var.stepMotorDaccSpeedTableDirect[indexUtilSub] = 0.0;
            //步数表归零
            stepMotorVarArray[indexUtil].var.stepMotorDaccStepTableDirect[indexUtilSub] = 0;
        }
    }
}

//读取电机传感器状态
void StepMotorReadSensorState(STEP_MOTOR_NO motorNo,PIN_STATE* originSensorPtr,PIN_STATE* emStopSensorPtr,
                                PIN_STATE* utilStop1SensorPtr,PIN_STATE* utilStop2SensorPtr)
{
    GPIO_PinState pinStateReal;
    //复位
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex == BOARD_PORT_IN_NULL)
    {
        *originSensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex);
        if(pinStateReal == stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinValidLevel)
        {
            *originSensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *originSensorPtr = PIN_STATE_INVALID;
        }
    }
    //急停
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.emergencyStopPinIndex == BOARD_PORT_IN_NULL)
    {
        *emStopSensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.emergencyStopPinIndex);
        if(pinStateReal == stepMotorVarArray[motorNo].attriPtr->baseConfig.emergencyStopPinValidLevel)
        {
            *emStopSensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *emStopSensorPtr = PIN_STATE_INVALID;
        }
    }
    //辅助急停1
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin1Index == BOARD_PORT_IN_NULL)
    {
        *utilStop1SensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin1Index);
        if(pinStateReal == stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin1ValidLevel)
        {
            *utilStop1SensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *utilStop1SensorPtr = PIN_STATE_INVALID;
        }
    }
    //辅助急停2
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin2Index == BOARD_PORT_IN_NULL)
    {
        *utilStop2SensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin2Index);
        if(pinStateReal == stepMotorVarArray[motorNo].attriPtr->baseConfig.utilStopPin2ValidLevel)
        {
            *utilStop2SensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *utilStop2SensorPtr = PIN_STATE_INVALID;
        }
    }
}

//步进电机初始化
void StepMotorInit(void)
{
    uint16_t indexUtil = 0;
    //步进电机初始化
    for(indexUtil = 0; indexUtil < SYSTEM_STEP_MOTOR_COUNT; indexUtil++)
    {
        //电机初始化
        stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.stepMotorInit((void*)stepMotorIntProcessFuncPtrArray[indexUtil]);
        //电机停止
        stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.stepMotorEnable();
        //电机停止
        stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.stepMotorStop();
        //设置电机方向顺时针
        stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.stepMotorSetDirCW();
    }
    //编码器初始化
    for(indexUtil = 0; indexUtil < SYSTEM_STEP_MOTOR_COUNT; indexUtil++)
    {
        //编码器初始化
        if(stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.encoderInit != NULL)
        {
            stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.encoderInit();
        }
        //编码器复位为默认值
        if(stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.encoderCountReset != NULL)
        {
            stepMotorVarArray[indexUtil].attriPtr->callBackFuncConfig.encoderCountReset();
        }
    }
}

//指定步进电机急停
ERROR_SUB StepMotorStopEmergency(STEP_MOTOR_NO motorNo)
{
    //步进电机状态切换为急停
    stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_CMD;
    //电机停止
    stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.stepMotorStop();
    return ERROR_SUB_OK;
}

//获取步进电机编码器当前计数值
ERROR_SUB StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO motorNo,int32_t* countResult)
{
    *countResult = 0;
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet == NULL)
    {
        //未配置编码器相关获取函数
        return StepMotorEncoderReadFuncNullError(motorNo);
    }
    else
    {
        //配置了编码器讯息,读取编码器讯息
        int32_t encoderCount = stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet();
        //对数据需要进行处理,使编码器数值和步进电机步数匹配
        encoderCount = (int32_t)(encoderCount/(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.ratioEncoderPosStepMotor));
        //获取是否需要反向
        if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderDir == ENCODER_DIR_POSITIVE)
        {
            //无需反向,直接返回
            *countResult = encoderCount;
        }
        else
        {
            //需要反向编码器数据
            *countResult = (0 - encoderCount);
        }
        return ERROR_SUB_OK;
    }
}

//复位步进电机编码器当前计数值
ERROR_SUB StepMotorResetEncoderCount(STEP_MOTOR_NO motorNo)
{
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountReset == NULL)
    {
        //未配置编码器复位相关获取函数
        return StepMotorEncoderResetFuncNullError(motorNo);
    }
    else
    {
        //执行编码器计数复位
        stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountReset();
        return ERROR_SUB_OK;
    }
}
