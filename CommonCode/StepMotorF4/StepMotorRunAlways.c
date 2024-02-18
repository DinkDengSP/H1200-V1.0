/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:16:16
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-09 17:12:01
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorUtil.h"
#include "StepMotorTable.h"

/**************************************电机持续运行函数申明****************************************/
//持续运行的加速段处理
static void StepMotorRunAlwaysIntProSubAccelerate(STEP_MOTOR_VAR* stepMotorVarPtr);
//持续运行的持续段处理
static void StepMotorProcessRunAlwaysSubHolding(STEP_MOTOR_VAR* stepMotorVarPtr);
//持续运行的减速段处理
static void StepMotorProcessRunAlwaysSubDecelerate(STEP_MOTOR_VAR* stepMotorVarPtr);
/***********************************************************************************************/
//电机持续运转编码器信号检测
void StepMotorRunAlwaysIntEncoderProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //检查是否配置了电机准备OK点
    if(stepMotorVarPtr->attriPtr->baseConfig.readyOkPinIndex < BOARD_PORT_IN_COUNT)
    {
        //读取电机准备OK状态
        GPIO_PinState readyOkSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.readyOkPinIndex);
        //伺服电机没有准备好
        if(readyOkSensorValue != stepMotorVarPtr->attriPtr->baseConfig.readyOkPinValidLevel)
        {
            //发生故障
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT;  
        }
    }
    //最大的电机位置和编码器位置差异
    int32_t encoderLossLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderLossLimit);
    //编码器信号识别判定
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        int32_t coordinateDifference = stepMotorVarPtr->var.currentCoordinate - stepMotorVarPtr->var.currentEncoderCoordinate;
        //计算绝对值
        if(coordinateDifference < 0)
        {
            coordinateDifference = 0 - coordinateDifference;
        }
        //检测电机是否堵住
        if(coordinateDifference > encoderLossLimit)
        {
            //编码器的信号丢失数量超过了电机配置的最大丢步限制
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RUN_STEP_BLOCK;
        }
    }
}

//持续运行
void StepMotorRunAlwaysIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //编码器状态机设定
    StepMotorRunAlwaysIntEncoderProcess(stepMotorVarPtr);
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
        return;
    }
    switch(stepMotorVarPtr->var.motorSubState)
    {
        case STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE:
            //步进电机持续运转的加速过程
            StepMotorRunAlwaysIntProSubAccelerate(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING:
            //已经运转到最高速,开始保持运转
            StepMotorProcessRunAlwaysSubHolding(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE:
            //切换到减速运转
            StepMotorProcessRunAlwaysSubDecelerate(stepMotorVarPtr);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
    }
}


/*****************************************************系统持续运转相关函数申明******************************************************/
//持续运行的加速段处理
static void StepMotorRunAlwaysIntProSubAccelerate(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //匀速运动,直接切换到匀速段
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RUN_ALWAYS;
        stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
        //发送脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[0]);
    }
    else
    {
        //脉冲发出一步
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
        //步数减小
        stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] -= 1;
        //本步数执行完成,索引增加
        if(stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] == 0)
        {
            stepMotorVarPtr->var.currentTableIndex += 1;
        }
        //当定位表进入最大行程的一半的时候,说明已经到了最大速度位置,此时,进入保持段
        if(stepMotorVarPtr->var.currentTableIndex == ((stepMotorVarPtr->var.currentTableMaxLength)/2))
        {
            stepMotorVarPtr->var.currentTableIndex -= 1;
            //设置走位状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RUN_ALWAYS;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
        }
    }
}

//持续运行的持续段处理
static void StepMotorProcessRunAlwaysSubHolding(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //发送脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[0]);
    }
    else
    {
        //发送脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
    }
}

//持续运行的减速段处理
static void StepMotorProcessRunAlwaysSubDecelerate(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //减速运转
    uint32_t currentDataIndex = stepMotorVarPtr->var.currentTableIndex;
    //只有一段说明是匀速过程
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //直接停止
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //检查当前序号,必须要切换到转折点之后
        if(stepMotorVarPtr->var.currentTableIndex < (stepMotorVarPtr->var.currentTableMaxLength/2))
        {
            stepMotorVarPtr->var.currentTableIndex = stepMotorVarPtr->var.currentTableMaxLength - stepMotorVarPtr->var.currentTableIndex -1;
        }
        else
        {
            if(currentDataIndex >= stepMotorVarPtr->var.currentTableMaxLength)
            {
                //当前坐标已经到达最大位置,全部的数据都发送完成了
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_NORMAL;
            }
            else
            {
                //发送脉冲
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorSpeedTableDirect[currentDataIndex]);
                //步数表-1
                stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] -= 1;
                if(stepMotorVarPtr->var.stepMotorStepTableDirect[currentDataIndex] == 0)
                {
                    //定位往前加1
                    stepMotorVarPtr->var.currentTableIndex += 1;
                }
            }
        }
    }
}

/*********************************************************************************************************************************/

//指定步进电机持续运转
ERROR_SUB StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = runAlwaysConfigPtr->motorNo;
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarArray[motorNo].var.currentEncoderCoordinate));
        stepMotorVarArray[motorNo].var.currentCoordinate = stepMotorVarArray[motorNo].var.currentEncoderCoordinate;
    }
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(runAlwaysConfigPtr->motorNo,&(stepMotorVarArray[motorNo]));
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //相关状态变量初始化到准备走位
    StepMotorStateInitForRunAlways(&(stepMotorVarArray[motorNo].var));
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithRunAlways(motorNo,runAlwaysConfigPtr->selectConfig,runAlwaysConfigPtr->speedRatioMode,runAlwaysConfigPtr->speedRatio);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVarArray[motorNo].var.stepMotorStartRunningTimeMs = OS_TICK_TO_MS(tx_time_get());
    //设置走位状态
    stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_RUN_ALWAYS;
    stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE;
    //持续运转的方向是指定方向
    stepMotorVarArray[motorNo].var.currentDir = runAlwaysConfigPtr->dir;
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(&(stepMotorVarArray[motorNo]));
    return ERROR_SUB_OK;
}

//指定步进电机立即停止
ERROR_SUB StepMotorStopImmediately(STEP_MOTOR_NO motorNo)
{
    //步进电机状态切换为急停
    stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    //电机停止
    stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.stepMotorStop();
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarArray[motorNo].var.currentEncoderCoordinate));
        stepMotorVarArray[motorNo].var.currentCoordinate = stepMotorVarArray[motorNo].var.currentEncoderCoordinate;
    }
    return ERROR_SUB_OK;
}

//指定步进电机减速停止,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration(STEP_MOTOR_NO motorNo)
{
    //只有在持续运行状态,且没到减速状态可以调用这个减速停止
    if(stepMotorVarArray[motorNo].var.motorMainState == STEP_MOTOR_MAIN_RUN_ALWAYS)
    {
        if(stepMotorVarArray[motorNo].var.motorSubState != STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE)
        {
            if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
            {
                //匀速运动,步进电机状态切换为急停
                stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
                //子状态急停
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            }
            else
            {
                //步数表索引切入对称点
                stepMotorVarArray[motorNo].var.currentTableIndex = 
                    stepMotorVarArray[motorNo].var.currentTableMaxLength - stepMotorVarArray[motorNo].var.currentTableIndex - 1;
                //状态切入减速段
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
            }
            
        }
    }
    else
    {
        //步进电机状态切换为急停
        stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //子状态急停
        stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    //等待运行完成
    while(stepMotorVarArray[motorNo].var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
    }
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarArray[motorNo].var.currentEncoderCoordinate));
        stepMotorVarArray[motorNo].var.currentCoordinate = stepMotorVarArray[motorNo].var.currentEncoderCoordinate;
    }
    return ERROR_SUB_OK;
}
//指定步进电机减速停止运行指定步数,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration1(STEP_MOTOR_NO motorNo)
{
    //只有在持续运行状态,且没到减速状态可以调用这个减速停止
    if(stepMotorVarArray[motorNo].var.motorMainState == STEP_MOTOR_MAIN_RUN_ALWAYS)
    {
        if(stepMotorVarArray[motorNo].var.motorSubState != STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE)
        {
            // if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
            // {
            //     //匀速运动,步进电机状态切换为急停
            //     stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //     //子状态急停
            //     stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            // }
            // else
            // {
            //     //步数表索引切入对称点
            //     stepMotorVarArray[motorNo].var.currentTableIndex = 
            //         stepMotorVarArray[motorNo].var.currentTableMaxLength - stepMotorVarArray[motorNo].var.currentTableIndex - 1;
            //     //状态切入减速段
            //     stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
            // }
            if(stepMotorVarArray[motorNo].attriPtr->baseConfig.dacc2StopSteps == 0)
            {
                //减速步数为0,切换到停止状态
                stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
                //指示减速停止
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            }
            else
            {
                //计算步进电机减速表
                StepMotorTableCalcSlowDown1Table(&stepMotorVarArray[motorNo]);
                //发出减速表第一步
                StepMotorUtilSendDaccStartPluse(&stepMotorVarArray[motorNo]);
                //指示减速停止
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
            }
            
        }
    }
    else
    {
        //步进电机状态切换为急停
        stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //子状态急停
        stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    //等待运行完成
    while(stepMotorVarArray[motorNo].var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
    }
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarArray[motorNo].var.currentEncoderCoordinate));
        stepMotorVarArray[motorNo].var.currentCoordinate = stepMotorVarArray[motorNo].var.currentEncoderCoordinate;
    }
    return ERROR_SUB_OK;
}
//指定步进电机减速停止,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration2(STEP_MOTOR_NO motorNo)
{
    //只有在持续运行状态,且没到减速状态可以调用这个减速停止
    if(stepMotorVarArray[motorNo].var.motorMainState == STEP_MOTOR_MAIN_RUN_ALWAYS)
    {
        if(stepMotorVarArray[motorNo].var.motorSubState != STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE)
        {
            // if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
            // {
            //     //匀速运动,步进电机状态切换为急停
            //     stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //     //子状态急停
            //     stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            // }
            // else
            // {
            //     //步数表索引切入对称点
            //     stepMotorVarArray[motorNo].var.currentTableIndex = 
            //         stepMotorVarArray[motorNo].var.currentTableMaxLength - stepMotorVarArray[motorNo].var.currentTableIndex - 1;
            //     //状态切入减速段
            //     stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
            // }
            if(stepMotorVarArray[motorNo].attriPtr->baseConfig.dacc2StopSteps == 0)
            {
                //减速步数为0,切换到停止状态
                stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
                //指示减速停止
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            }
            else
            {
                //计算步进电机减速表
                StepMotorTableCalcSlowDown2Table(&stepMotorVarArray[motorNo]);
                //发出减速表第一步
                StepMotorUtilSendDaccStartPluse(&stepMotorVarArray[motorNo]);
                //指示减速停止
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
            }
            
        }
    }
    else
    {
        //步进电机状态切换为急停
        stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //子状态急停
        stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    //等待运行完成
    while(stepMotorVarArray[motorNo].var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
    }
    //如果存在编码器.通过编码器更新电机坐标
    if(stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        StepMotorGetCurrentEncoderPos(motorNo,&(stepMotorVarArray[motorNo].var.currentEncoderCoordinate));
        stepMotorVarArray[motorNo].var.currentCoordinate = stepMotorVarArray[motorNo].var.currentEncoderCoordinate;
    }
    return ERROR_SUB_OK;
}

