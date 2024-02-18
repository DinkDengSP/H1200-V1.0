/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 15:20:30 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"
#include "StepMotorAlarm.h"

//启动时走到坐标零位
ERROR_SUB StepMotorReturnZeroWithStart2CoordinateZero(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //此时原点未触发,要计算步数,走向原点
    StepMotorTableCalcWithReturnZero(motorNo,returnZeroConfigPtr->selectConfig,returnZeroConfigPtr->speedRatioMode,returnZeroConfigPtr->speedRatio);
    //设置状态,回零走到坐标零位
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_COORDINATE;
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RUN_COORDINATE_SUB_NORMAL;
    //修正电机坐标,防止速度过大影响运算
    if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
    {
        if(stepMotorVar[motorNo]->var.currentCoordinate > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
        {
            stepMotorVar[motorNo]->var.currentCoordinate = stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps;
        }
    }
    else if(stepMotorVar[motorNo]->attri.baseConfig.positiveMaxSteps != 0)
    {
        if(stepMotorVar[motorNo]->var.currentCoordinate > stepMotorVar[motorNo]->attri.baseConfig.positiveMaxSteps)
        {
            stepMotorVar[motorNo]->var.currentCoordinate = stepMotorVar[motorNo]->attri.baseConfig.positiveMaxSteps;
        }
    }
    if(stepMotorVar[motorNo]->attri.baseConfig.reverseMaxSteps != 0)
    {
        if(stepMotorVar[motorNo]->var.currentCoordinate < stepMotorVar[motorNo]->attri.baseConfig.reverseMaxSteps)
        {
            stepMotorVar[motorNo]->var.currentCoordinate = stepMotorVar[motorNo]->attri.baseConfig.reverseMaxSteps;
        }
    }
    //计算方向
    StepMotorUtilCalcDirWithTargetCoordinate(motorNo,stepMotorVar[motorNo],0);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs = tx_time_get();
    //以计算速度发送一次脉冲
    StepMotorUtilSendStartPluse(motorNo,stepMotorVar[motorNo]);
    //等待运动完成
    while(stepMotorVar[motorNo]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //检测原点状态,原点触发时,立即切换减速
        if(stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex))
        {
            //设置状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次切换下一步
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
        }
        //检测运行时间
        stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount = tx_time_get() - stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
        {
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
        }
    }
    if(stepMotorVar[motorNo]->var.motorSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
    {
        //运行超时
        return StepMotorErrorSubStopTimeOut(motorNo);
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
    if(stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex))
    {
        return ERROR_SUB_OK;
    }
    else
    {
        //找原点,设置状态是找原点
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
        //电机当前方向应该是运行时反方向
        stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVar[motorNo]->attri.baseConfig.dir));
        //增加1步的复位脉冲数
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
        //记录启动时间
        stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs = tx_time_get();
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
        //等待找到原点或者报错切换状态
        while(stepMotorVar[motorNo]->var.motorSubState == STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST)
        {
            CoreDelayMinTick();
            //检测运行时间
            stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount = tx_time_get() - stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs;
            //检测超时,自动停止
            if(stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount > returnZeroConfigPtr->timeOutSet)
            {
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            }
        }
        //记录状态
        STEP_MOTOR_SUB_STATE localSubState = stepMotorVar[motorNo]->var.motorSubState;
        //设置状态
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //停止
        BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
        //上次的步数加上
        StepMotorUtilReflushCoordinate(motorNo);
        //赶紧停下来
        if(localSubState == STEP_MOTOR_STOP_SUB_TIMEOUT)
        {
            //行程超限制
            return StepMotorErrorSubStopTimeOut(motorNo);;
        }
        else if(localSubState == STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT)
        {
            return StepMotorErrorSubStopResetStepLimit(motorNo);
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
    return StepMotorRun2Coordinate(&run2CoordinateCmdPtr);
}


//指定步进电机回零
ERROR_SUB StepMotorReturnZeroWithSensorReset(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(motorNo,stepMotorVar[motorNo]);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    errorSubCode = StepMotorUtilCheckTargetCoordinateLimit(motorNo,stepMotorVar[motorNo],returnZeroConfigPtr->correctionCoordinate);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检查ready信号是否有效,如果配置了ready信号且ready信号无效,则报错
    if(stepMotorVar[motorNo]->attri.baseConfig.readyOkPinIndex < BOARD_PORT_IN_COUNT)
    {
        //读取电机准备OK状态
        GPIO_PinState readyOkSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.readyOkPinIndex);
        //伺服电机没有准备好
        if(readyOkSensorValue != stepMotorVar[motorNo]->attri.baseConfig.readyOkPinValidLevel)
        {
            //发生故障
            return StepMotorErrorSubStopSlaveReadyFault(motorNo);  
        }
    }
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVar[motorNo]->var));
    //是否有原点,没有原点,直接开始修正过程
    if(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        if(stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex))
        {
            //调用复位指令
            errorSubCode = StepMotorReturnZeroStageReset(returnZeroConfigPtr);
            //结果未报错,可以开始修正
            if(errorSubCode != ERROR_SUB_OK)
            {
                return errorSubCode;
            }
        }
        else
        {
            //运行到零点
            errorSubCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)
            {
                return errorSubCode;
            }
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
    stepMotorVar[motorNo]->var.currentCoordinate = 0;
    //修正
    return StepMotorReturnZeroCorrection(returnZeroConfigPtr);
}


//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程
//防止产生真空负压
ERROR_SUB StepMotorReturnZeroWithSensorNotReset(StepMotorReturnZeroCmd* returnZeroConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = returnZeroConfigPtr->motorNo;
    //状态检测
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(motorNo,stepMotorVar[motorNo]);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    errorSubCode = StepMotorUtilCheckTargetCoordinateLimit(motorNo,stepMotorVar[motorNo],returnZeroConfigPtr->correctionCoordinate);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检查ready信号是否有效,如果配置了ready信号且ready信号无效,则报错
    if(stepMotorVar[motorNo]->attri.baseConfig.readyOkPinIndex < BOARD_PORT_IN_COUNT)
    {
        //读取电机准备OK状态
        GPIO_PinState readyOkSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.readyOkPinIndex);
        //伺服电机没有准备好
        if(readyOkSensorValue != stepMotorVar[motorNo]->attri.baseConfig.readyOkPinValidLevel)
        {
            //发生故障
            return StepMotorErrorSubStopSlaveReadyFault(motorNo);  
        }
    }
    //初始化电机状态,为回零
    StepMotorStateInitForReturnZero(&(stepMotorVar[motorNo]->var));
    //是否有原点,没有原点,直接完成
    if(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        if(stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel == BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex))
        {
            //当前已经在零点,直接完成
            asm("nop");
        }
        else
        {
            //不在零点,运行到零点
            errorSubCode = StepMotorReturnZeroWithStart2CoordinateZero(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)
            {
                return errorSubCode;
            }
            //此时不一定回到零点了,有可能回到了坐标零点但是还是没找到传感器,所以需要找传感器
            errorSubCode = StepMotorReturnZeroStageFindZeroSensor(returnZeroConfigPtr);
            if(errorSubCode != ERROR_SUB_OK)
            {
                return errorSubCode;
            }
        }
    }
    //设置当前坐标为0
    stepMotorVar[motorNo]->var.currentCoordinate = 0;
    //无修正
    return ERROR_SUB_OK;
}


