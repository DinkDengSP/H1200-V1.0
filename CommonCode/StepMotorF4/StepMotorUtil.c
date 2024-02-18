/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:21:34
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-08 16:50:56
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorUtil.h"

//将步进电机错误代码转换为系统错误代码
ERROR_SUB StepMotorConvert2ErrorSubCode(STEP_MOTOR_NO motorNo,STEP_MOTOR_SUB_STATE state)
{
    switch(state)
    {
        case STEP_MOTOR_STOP_SUB_NORMAL://正常停止
            return ERROR_SUB_OK;
        case STEP_MOTOR_STOP_SUB_EMERGENCY_CMD://指令急停
            return StepMotorEmergencyCmdError(motorNo);
        case STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL://信号急停,也是防撞急停
            return StepMotorEmergencySignalError(motorNo);
        case STEP_MOTOR_STOP_SUB_DACC1://减速急停1
            return StepMotorUtilStop1Error(motorNo);
        case STEP_MOTOR_STOP_SUB_DACC2://减速急停2
            return StepMotorUtilStop2Error(motorNo);
        case STEP_MOTOR_STOP_SUB_TIMEOUT://运行时间超时
            return StepMotorRunningTimeOutError(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT://超过复位最大行程
            return StepMotorResetStepLimitError(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_ENCODER_BLOCK://复位时编码器堵住
            return StepMotorResetEncoderBlockError(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_ENCODER_DIR://复位时编码器方向错误
            return StepMotorResetEncodeDirError(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_ENCODER_MORE://编码器信号多于电机信号,传动比异常
            return StepMotorResetEncodeMoreError(motorNo);
        case STEP_MOTOR_STOP_SUB_RUN_STEP_BLOCK://步进电机走步数时丢步超过最大丢步限制
            return StepMotorRunStepEncoderLossLimitError(motorNo);
        case STEP_MOTOR_STOP_SUB_RUN_COORDINATE_BLOCK://步进电机走坐标时丢步超过最大丢步限制
            return StepMotorRunCoordinateEncoderLossLimitError(motorNo);
        case STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT://电机发生故障
            return StepMotorSlaveReadyPinCheckError(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT://脱离原点超最大行程
            StepMotorResetLeaveOriginSensorError(motorNo);
        default:
            return ERROR_SUB_OK;
    }
}

//初始化电机状态
void StepMotorStateInit(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //状态设定
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = STEP_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 0;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}



//电机状态初始化,针对复位
void StepMotorStateInitForReset(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = STEP_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->currentEncoderCoordinate = 0;//当前编码器位置
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
    //传感器原点减速步数
    motorStatePtr->daccStepPluseOutCount = 0;
    //从限位点找原点触发原点标记
    motorStatePtr->resetLimitSensorOverTrigResetSensorFlag = 0;
}

//电机状态初始化,针对走位
void StepMotorStateInitForRunSteps(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

//电机状态初始化,针对电机持续运转
void StepMotorStateInitForRunAlways(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

void StepMotorStateInitForRunCoordinate(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}

void StepMotorStateInitForReturnZero(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    motorStatePtr->currentTableIndex = 0;//当前步数表序号
    motorStatePtr->resetCurrentSetOutPluse = 0;//当前复位发出脉冲总数
    motorStatePtr->currentTableMaxLength = 1;
    //实际速度表和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorStepTableDirect[utilIndex] = 0;
    }
    //当前减速步数表序号
    motorStatePtr->currentTableIndex = 0;
    motorStatePtr->currentTableMaxLength = 0;
    //减速速度和步数表
    for(utilIndex = 0; utilIndex < SM_SPEED_TABLE_LENGTH_MAX/2; utilIndex++)
    {
        //速度表归零
        motorStatePtr->stepMotorDaccSpeedTableDirect[utilIndex] = 0.0;
        //步数表归零
        motorStatePtr->stepMotorDaccStepTableDirect[utilIndex] = 0;
    }
}



//检查电机走指定步数的参数是否合格
ERROR_SUB StepMotorCheckRunStepsVar(StepMotorRunStepsCmd* runStepPtr)
{
    if(runStepPtr->motorNo > STEP_MOTOR_NO_MAX)
    {
        //电机序号不对
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        runStepPtr->utilStop1Enable = 1;
    }
    if(runStepPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        runStepPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((runStepPtr->utilStop1Enable != 0)&&(runStepPtr->utilStop2Enable != 0))
    {
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    if(runStepPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorRunStepParamError(runStepPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机走到指定坐标的参数是否合格
ERROR_SUB StepMotorCheckRun2CoordationVar(StepMotorRun2CoordationCmd* runCoordationPtr)
{
    if(runCoordationPtr->motorNo > STEP_MOTOR_NO_MAX)
    {
        //电机序号不对
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);
    }
    if(runCoordationPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    if(runCoordationPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    if(runCoordationPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    if(runCoordationPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    if(runCoordationPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        runCoordationPtr->utilStop1Enable = 1;
    }
    if(runCoordationPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        runCoordationPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((runCoordationPtr->utilStop1Enable != 0)&&(runCoordationPtr->utilStop2Enable != 0))
    {
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    if(runCoordationPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorRunCoordationParamError(runCoordationPtr->motorNo);;
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机回零指令的参数是否合格
ERROR_SUB StepMotorCheckReturnZeroVar(StepMotorReturnZeroCmd* returnZeroPtr)
{
    if(returnZeroPtr->motorNo > STEP_MOTOR_NO_MAX)
    {
        //电机序号不对
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->utilStop1Enable != 0)
    {
        //纠正一下可能出错的数据
        returnZeroPtr->utilStop1Enable = 1;
    }
    if(returnZeroPtr->utilStop2Enable != 0)
    {
        //纠正一下可能出错的数据
        returnZeroPtr->utilStop2Enable = 1;
    }
    //两个辅助急停不能同时被使能
    if((returnZeroPtr->utilStop1Enable != 0)&&(returnZeroPtr->utilStop2Enable != 0))
    {
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorReturnZeroParamError(returnZeroPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机持续运转的参数是否合格
ERROR_SUB StepMotorCheckRunAlways(StepMotorRunAlwaysCmd* runAlwaysPtr)
{
    if(runAlwaysPtr->motorNo > STEP_MOTOR_NO_MAX)
    {
        //电机序号不对
        return StepMotorRunAlwaysParamError(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorRunAlwaysParamError(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorRunAlwaysParamError(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorRunAlwaysParamError(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorRunAlwaysParamError(runAlwaysPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查步进电机急停状态,如果是指令急停,那么报错
ERROR_SUB StepMotorUtilCheckEmergencyCmdState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    //检测电机状态,如果电机状态为指令急停,那么报错,指令急停必须复位取消
    if((motorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)&&(motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_EMERGENCY_CMD))
    {
        return StepMotorEmergencyCmdError(motorNo);
    }
    else
    {
        return ERROR_SUB_OK;
    }
}

//检查步进电机电机的目标坐标是否超过限制
ERROR_SUB StepMotorUtilCheckTargetCoordinateLimit(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate)
{
    //检测电机行程限制,如果行程限制为0,代表不限制行程,否则,补偿有行程限制
    if(motorVarPtr->attriPtr->baseConfig.positiveMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate > (motorVarPtr->attriPtr->baseConfig.positiveMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            return StepMotorRunCordinateMaxError(motorNo);
        }
    }
    //负方向的行程限制
    if(motorVarPtr->attriPtr->baseConfig.reverseMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate < (motorVarPtr->attriPtr->baseConfig.reverseMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            return StepMotorRunCordinateMinError(motorNo);
        }
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机急停引脚状态
ERROR_SUB StepMotorUtilCheckAndSetEmergencyPinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //如果系统支持急停,那么查看是否有急停端口,并且尝试将急停设置为中断
    if((motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex) < BOARD_PORT_IN_COUNT)
    {
        //配置了系统急停,那么检测此时急停IO状态,触发状态,直接报错
        if(motorVarPtr->attriPtr->baseConfig.emergencyStopPinValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
            return StepMotorEmergencySignalError(motorNo);
        }
        else
        {
            //系统急停没有触发,那么将尝试将急停设置为中断
            if(motorVarPtr->attriPtr->baseConfig.emergencyStopPinValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex,MCU_EXTI_MODE_IT_FALLING,
                                                StepMotorEmergencyStopByIntSingalFuncPtrArray[motorNo]
                                                );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex,MCU_EXTI_MODE_IT_RISING,
                                                StepMotorEmergencyStopByIntSingalFuncPtrArray[motorNo]
                                                );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER;
            }
        }
    }
    else
    {
        //急停信号没有使能
        motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机辅助急停1状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop1PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //如果系统支持辅助急停端口,那么查看是否设置有辅助急停端口,并尝试将急停设置为中断
    //辅助急停1被使能且辅助急停1配置为系统输入
    if((enable == 1)&&(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index) < BOARD_PORT_IN_COUNT)
    {
        //首先查看辅助急停状态
        if(motorVarPtr->attriPtr->baseConfig.utilStopPin1ValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
            //不调用停止函数,上一个脉冲发完自然就会进行中断进行下一步减速计算
            return StepMotorUtilStop1Error(motorNo);
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attriPtr->baseConfig.utilStopPin1ValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index,MCU_EXTI_MODE_IT_FALLING,
                                                StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorNo]
                                                );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index,MCU_EXTI_MODE_IT_RISING,
                                                StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorNo]
                                                );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机辅助急停2状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop2PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //辅助急停2被使能且辅助急停配置为系统输入
    if((enable == 1)&&(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index) < BOARD_PORT_IN_COUNT)
    {
        //先检测辅助急停是否已经触发,如果已经出发,函数直接返回
        if(motorVarPtr->attriPtr->baseConfig.utilStopPin2ValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
            //不调用停止函数,上一个脉冲发完自然就会进入计算减速段
            return StepMotorUtilStop2Error(motorNo);
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attriPtr->baseConfig.utilStopPin2ValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index,MCU_EXTI_MODE_IT_FALLING,
                                                StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorNo]
                                                );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index,MCU_EXTI_MODE_IT_RISING,
                                                StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorNo]
                                                );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
    return ERROR_SUB_OK;
}

//根据本次电机目的坐标决定方向
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate)
{
    //然后设置当前方向
    if(targetCoordinate >= motorVarPtr->var.currentCoordinate)
    {
        motorVarPtr->var.currentDir = motorVarPtr->attriPtr->baseConfig.dir;
    }
    else
    {
        if(motorVarPtr->attriPtr->baseConfig.dir == STEP_MOTOR_DIR_CCW)
        {
            motorVarPtr->var.currentDir = STEP_MOTOR_DIR_CW;
        }
        else
        {
            motorVarPtr->var.currentDir = STEP_MOTOR_DIR_CCW;
        }
    }
}

//设置电机运行方向
void StepMotorSetRunningDir(STEP_MOTOR_VAR* motorVarPtr)
{
    if(motorVarPtr->var.currentDir == STEP_MOTOR_DIR_CCW)
    {
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCCW();
    }
    else
    {
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCW();
    }
}

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentTableIndex = 0;
    //设置方向
    if(motorVarPtr->var.currentDir == STEP_MOTOR_DIR_CCW)
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCCW();
    else 
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCW();
    //步数表实时减小
    motorVarPtr->var.stepMotorStepTableDirect[0] -= 1;
    //发出第一步
    if(motorVarPtr->var.stepMotorStepTableDirect[0] == 0)
    {
        //步数表往前加一步
        motorVarPtr->var.currentTableIndex += 1;
    }
    //发出脉冲
    motorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(motorVarPtr->var.stepMotorSpeedTableDirect[0]);
}

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentDaccTableIndex = 0;
    //设置方向
    if(motorVarPtr->var.currentDir == STEP_MOTOR_DIR_CCW)
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCCW();
    else 
        motorVarPtr->attriPtr->callBackFuncConfig.stepMotorSetDirCW();
    //步数表实时减小
    motorVarPtr->var.stepMotorDaccStepTableDirect[0] -= 1;
    //发出第一步
    if(motorVarPtr->var.stepMotorDaccStepTableDirect[0] == 0)
    {
        //步数表往前加一步
        motorVarPtr->var.currentDaccTableIndex += 1;
    }
    //发出脉冲
    motorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(motorVarPtr->var.stepMotorDaccSpeedTableDirect[0]);
}

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_VAR* motorVarPtr)
{
    //急停
    if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex);
    }
    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;

    //辅助急停1
    if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;

    //辅助急停2
    if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
}

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet)
{
    //等待步进电机处于停止状态,在等待过程中,如果前面设置的两个中断不支持,那么就要实时监测IO口状态,并进行处理
    while(motorVarPtr->var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //检测超时,自动停止
        if(motorVarPtr->var.stepMotorRunningTimeMsCount > timeOutSet)
        {
            //实时监测的时候应该关闭中断
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            //状态设置,自动停止
            //结束循环
            continue;
        }
        //如果急停处于使能未注册的状态,那么需要检测电平,并进行处理
        if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER)
        {
            //读取的状态满足急停
            if(motorVarPtr->attriPtr->baseConfig.emergencyStopPinValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex))
            {
                //电机停下来,等待状态切换,如果辅助急停和急停同时出发,那么急停的优先级更高,减速过程会终止
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
                //同时将监控状态去掉
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //如果辅助急停1处于使能未注册状态,需要实时监测
        if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attriPtr->baseConfig.utilStopPin1ValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index))
            {
                //注意,因为两个辅助急停不能同时生效,所以基本不用考虑两个辅助急停同时触发的情况,否则,还要考虑
                //两个辅助急停同时触发,状态变量之间的互斥关系
                //切换到减速过程
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱

                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }

                continue;
            }
        }
        //辅助急停2的状态的处理
        if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_UNREGISTER)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attriPtr->baseConfig.utilStopPin2ValidLevel == BoardPortInReadState(motorVarPtr->attriPtr->baseConfig.utilStopPin2Index))
            {
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.emergencyStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }

                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attriPtr->baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
                }
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
        CoreDelayMs(30);
        //更新运行时间
        motorVarPtr->var.stepMotorRunningTimeMsCount = OS_TICK_TO_MS(tx_time_get()) - motorVarPtr->var.stepMotorStartRunningTimeMs;
    }
}

