/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:18:24
**LastEditors: DengXiaoJun
**LastEditTime: 2021-04-02 18:44:48
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorUtil.h"
#include "StepMotorTable.h"

//启动时走到坐标零位
ERROR_SUB StepMotorReturnZeroWithStart2CoordinateZero(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //此时原点未触发,要计算步数,走向原点
    StepMotorTableCalcWithReturnZero(motorNo,returnZeroConfigPtr->selectConfig,returnZeroConfigPtr->speedRatioMode,returnZeroConfigPtr->speedRatio);
    //设置状态,回零走到坐标零位
    stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_RUN_COORDINATE;
    stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL;
    //计算方向
    StepMotorUtilCalcDirWithTargetCoordinate(&(stepMotorVarArray[motorNo]),0);
    //发出第一步
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVarArray[motorNo].var.stepMotorStartRunningTimeMs = OS_TICK_TO_MS(tx_time_get());
    //以计算速度发送一次脉冲
    StepMotorUtilSendStartPluse(&(stepMotorVarArray[motorNo]));
    //等待运动完成
    while(stepMotorVarArray[motorNo].var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //检测原点状态,原点触发时,立即切换减速
        if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex))
        {
            //设置状态
            stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //电机停止
            stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.stepMotorStop();
        }
        //检测运行时间
        stepMotorVarArray[motorNo].var.stepMotorRunningTimeMsCount = OS_TICK_TO_MS(tx_time_get()) - stepMotorVarArray[motorNo].var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVarArray[motorNo].var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
        {
            stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
    }
    if(stepMotorVarArray[motorNo].var.motorSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
    {
        //运行超时
        return StepMotorRunningTimeOutError(returnZeroConfigPtr->motorNo);
    }
    else
    {
        //运行过程中遇到零点立刻停止,正常脉冲发完也立刻停止
        return ERROR_SUB_OK;
    }
}

//回零的复位过程
ERROR_SUB StepMotorReturnZeroStageReset(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    StepMotorResetCmd resetCmd;
    resetCmd.correctionCoordinate = 0;
    resetCmd.motorNo = returnZeroConfigPtr->motorNo;
    resetCmd.timeOutSet = returnZeroConfigPtr->timeOutSet;
    return StepMotorReset(&resetCmd);
}

//步进电机找零点
ERROR_SUB StepMotorReturnZeroStageFindZeroSensor(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //已经找到原点
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex))
    {
        return ERROR_SUB_OK;
    }
    else
    {
        //找原点,设置状态是找原点
        stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_RESET;
        stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
        //电机当前方向应该是运行时反方向
        stepMotorVarArray[motorNo].var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarArray[motorNo].attriPtr->baseConfig.dir));
        //增加1步的复位脉冲数
        stepMotorVarArray[motorNo].var.resetCurrentSetOutPluse = 1;
        //记录启动时间
        stepMotorVarArray[motorNo].var.stepMotorStartRunningTimeMs = OS_TICK_TO_MS(tx_time_get());
        //设置电机运行方向
        StepMotorSetRunningDir(&(stepMotorVarArray[motorNo]));
        //发出一步数据
        stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetSpeed);
        //等待找到原点或者报错切换状态
        while(stepMotorVarArray[motorNo].var.motorMainState == STEP_MOTOR_MAIN_RESET)
        {
            CoreDelayMinTick();
            //检测运行时间
            stepMotorVarArray[motorNo].var.stepMotorRunningTimeMsCount = OS_TICK_TO_MS(tx_time_get()) - stepMotorVarArray[motorNo].var.stepMotorStartRunningTimeMs;
            //检测超时,自动停止
            if(stepMotorVarArray[motorNo].var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
            {
                stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
                stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            }
        }
        //记录状态(从复位状态流程中不能返回与运行超时、复位运行超步数限制相同的值,例如复位第一次找原点下一个流程是复位离开原点,而复位离开原点的值是4与运行超时的值相同,从而报运行超时的警报)
        STEP_MOTOR_SUB_STATE localSubState = stepMotorVarArray[motorNo].var.motorSubState;
        //设置状态,只要找到原点,电机立即停止不进行脱离原点和修正
        stepMotorVarArray[motorNo].var.motorMainState = STEP_MOTOR_MAIN_STOP;
        stepMotorVarArray[motorNo].var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //电机停止
        stepMotorVarArray[motorNo].attriPtr->callBackFuncConfig.stepMotorStop();
        //赶紧停下来
        if(localSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
        {
            //行程超限制
            return StepMotorRunningTimeOutError(returnZeroConfigPtr->motorNo);
        }
        else if(localSubState == STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT)
        {
            return StepMotorResetStepLimitError(returnZeroConfigPtr->motorNo);
        }
        return ERROR_SUB_OK;
    }
}

//走坐标
ERROR_SUB StepMotorReturnZeroCorrection(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    StepMotorRun2CoordationCmd run2CoordinateCmdPtr;
    run2CoordinateCmdPtr.motorNo = returnZeroConfigPtr->motorNo;
    run2CoordinateCmdPtr.targetCoordinate = returnZeroConfigPtr->correctionCoordinate;
    run2CoordinateCmdPtr.selectConfig = returnZeroConfigPtr->selectConfig;
    run2CoordinateCmdPtr.speedRatioMode = returnZeroConfigPtr->speedRatioMode;
    run2CoordinateCmdPtr.speedRatio = returnZeroConfigPtr->speedRatio;
    run2CoordinateCmdPtr.utilStop1Enable = returnZeroConfigPtr->utilStop1Enable;
    run2CoordinateCmdPtr.utilStop2Enable = returnZeroConfigPtr->utilStop2Enable;
    run2CoordinateCmdPtr.timeOutSet = returnZeroConfigPtr->timeOutSet;
    return StepMotorRun2SpecialCoordinates(&run2CoordinateCmdPtr);
}


//指定步进电机回零
ERROR_SUB StepMotorReturnZeroReset(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(returnZeroConfigPtr->motorNo,&(stepMotorVarArray[motorNo]));
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //检查步进电目标坐标是否在限制坐标范围内
    errorSubCode = StepMotorUtilCheckTargetCoordinateLimit(returnZeroConfigPtr->motorNo,&(stepMotorVarArray[motorNo]),
                                                            returnZeroConfigPtr->correctionCoordinate);
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVarArray[motorNo].var));
    //是否有原点,没有原点,直接开始修正过程
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex))
        {
            //调用复位指令
            errorSubCode = StepMotorReturnZeroStageReset(returnZeroConfigPtr);
            //结果未报错,可以开始修正
            if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
        }
        else
        {
            //运行到零点
            errorSubCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
            CoreDelayMinTick();
            //复位过程
            errorSubCode = StepMotorReturnZeroStageReset(returnZeroConfigPtr);
            //结果未报错,可以开始修正
            if(errorSubCode != ERROR_SUB_OK)
            {
                return errorSubCode;
            }
        }
    }
    //设置当前坐标为0
    stepMotorVarArray[motorNo].var.currentCoordinate = 0;
    //修正
    return StepMotorReturnZeroCorrection(returnZeroConfigPtr);
}


//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程,防止产生真空负压
ERROR_SUB StepMotorReturnZero(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(returnZeroConfigPtr->motorNo,&(stepMotorVarArray[motorNo]));
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //检查步进电目标坐标是否在限制坐标范围内
    errorSubCode = StepMotorUtilCheckTargetCoordinateLimit(returnZeroConfigPtr->motorNo,&(stepMotorVarArray[motorNo]),
                                                           returnZeroConfigPtr->correctionCoordinate);
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVarArray[motorNo].var));
    //是否有原点,没有原点,直接开始修正过程
    if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        if(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVarArray[motorNo].attriPtr->baseConfig.resetPinIndex))
        {
            //已经在零点了就不用再动了
            asm("nop");
        }
        else
        {
            //不在零点,运行到零点
            errorSubCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
            //此时不一定回到零点了,有可能回到了坐标零点但是还是没找到传感器,所以需要找传感器
            errorSubCode = StepMotorReturnZeroStageFindZeroSensor(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
        }
    }
    //设置当前坐标为0
    stepMotorVarArray[motorNo].var.currentCoordinate = 0;
    //无修正
    return ERROR_SUB_OK;
}
