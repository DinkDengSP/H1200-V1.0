/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:14:54
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-07 13:17:05
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorInt.h"
#include "StepMotor.h"

//中断中更新当前电机坐标,以及编码器当前坐标
static void StepMotorIntReflushCoordinate(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //当前坐标增加,判定方向与正方向相同,坐标+1,否则坐标-1,这一步是先更新坐标再发送脉冲
    if(stepMotorVarPtr->var.currentDir == stepMotorVarPtr->attriPtr->baseConfig.dir)
    {
        stepMotorVarPtr->var.currentCoordinate += 1;
    }
    else
    {
        stepMotorVarPtr->var.currentCoordinate -= 1;
    }
    //更新当前编码器坐标，编码器坐标是当前的坐标，所以电机坐标会实时的比当前坐标多1
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        stepMotorVarPtr->var.currentEncoderCoordinate = stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet();
        //对数据需要进行处理,使编码器数值和步进电机步数匹配
        stepMotorVarPtr->var.currentEncoderCoordinate = (int32_t)((stepMotorVarPtr->var.currentEncoderCoordinate)/
            (stepMotorVarPtr->attriPtr->callBackFuncConfig.ratioEncoderPosStepMotor));
        //获取是否需要反向
        if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderDir == ENCODER_DIR_NEGATIVE)
        {
            stepMotorVarPtr->var.currentEncoderCoordinate = 0 - stepMotorVarPtr->var.currentEncoderCoordinate;
        }
    }    
}


//公用的步进电机错误回调函数
static void StepMotorCommonIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //更新坐标
    StepMotorIntReflushCoordinate(stepMotorVarPtr);
    //选择步进电机状态进行处理,状态机处理机制
    switch(stepMotorVarPtr->var.motorMainState)
    {
        case STEP_MOTOR_MAIN_STOP:
            break;
        case STEP_MOTOR_MAIN_RESET:
            //复位状态
            StepMotorResetIntProcess(stepMotorVarPtr);
            break;
        case STEP_MOTOR_MAIN_RUN_STEPS:
            //运行指定步数状态
            StepMotorRunStepsIntProcess(stepMotorVarPtr);
            break;
        case STEP_MOTOR_MAIN_RUN_COORDINATE:
            //运行到指定坐标状态
            StepMotorRunCoordinateIntProcess(stepMotorVarPtr);
            break;
        case STEP_MOTOR_MAIN_RUN_ALWAYS:
            //持续运转状态
            StepMotorRunAlwaysIntProcess(stepMotorVarPtr);
            break;
        case STEP_MOTOR_MAIN_DACC:
            //减速运行状态
            StepMotorRunDaccIntProcess(stepMotorVarPtr);
            break;
        default:
            //不支持的状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //未知原因
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
    }
}

#if(SYSTEM_STEP_MOTOR_COUNT > 0)
//步进电机1中断处理函数
static void StepMotor1IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[0]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 1)
//步进电机2中断处理函数
static void StepMotor2IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[1]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 2)
//步进电机3中断处理函数
static void StepMotor3IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[2]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 3)
//步进电机4中断处理函数
static void StepMotor4IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[3]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 4)
//步进电机5中断处理函数
static void StepMotor5IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[4]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 5)
//步进电机6中断处理函数
static void StepMotor6IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[5]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 6)
//步进电机7中断处理函数
static void StepMotor7IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[6]));
}
#endif

#if(SYSTEM_STEP_MOTOR_COUNT > 7)
//步进电机8中断处理函数
static void StepMotor8IntProcess(void)
{
    StepMotorCommonIntProcess(&(stepMotorVarArray[7]));
}
#endif


//步进电机回调函数表
const StepMotorIntProcessFuncPtr stepMotorIntProcessFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT] = {
#if(SYSTEM_STEP_MOTOR_COUNT > 0)
    StepMotor1IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 1)
    StepMotor2IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 2)
    StepMotor3IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 3)
    StepMotor4IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 4)
    StepMotor5IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 5)
    StepMotor6IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 6)
    StepMotor7IntProcess,
#endif
#if(SYSTEM_STEP_MOTOR_COUNT > 7)
    StepMotor8IntProcess,
#endif
};


