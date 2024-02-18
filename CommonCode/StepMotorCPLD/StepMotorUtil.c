/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 15:47:03 +0800
************************************************************************************************/ 
#include "StepMotorUtil.h"
#include "StepMotorAlarm.h"

extern STEP_MOTOR_VAR* stepMotorVar[SYSTEM_STEP_MOTOR_COUNT];

//初始化电机状态
void StepMotorStateInit(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //状态设定
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = STEP_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->currentDir = STEP_MOTOR_DIR_CW;//当前方向
    motorStatePtr->currentCoordinate = 0;//当前坐标
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    //上次发送步数归零
    motorStatePtr->lastSendOutPluse = 0;
}

//电机状态初始化,针对电机持续运转
void StepMotorStateInitForRunAlways(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    //上次发送步数归零
    motorStatePtr->lastSendOutPluse = 0;
}

void StepMotorStateInitForRunCoordinate(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    //上次发送步数归零
    motorStatePtr->lastSendOutPluse = 0;
}

void StepMotorStateInitForReturnZero(StepMotorState* motorStatePtr)
{
    uint16_t utilIndex = 0;
    //停止状态
    motorStatePtr->motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    motorStatePtr->motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    motorStatePtr->stepMotorStartRunningTimeMs = 0;//本次运动启动时间
    motorStatePtr->stepMotorRunningTimeMsCount = 0;//本次运动已用时间
    //三个信号的状态
    motorStatePtr->stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    motorStatePtr->stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
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
    //上次发送步数归零
    motorStatePtr->lastSendOutPluse = 0;
}



//检查电机走指定步数的参数是否合格
ERROR_SUB StepMotorCheckRunStepsVar(StepMotorRunStepsCmd* runStepPtr)
{
    if(runStepPtr->motorNo > STEP_MOTOR_NO12)
    {
        //电机序号不对
        return ERROR_SUB_SM_NO_LIMIT;
    }
    if(runStepPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);
    }
    if(runStepPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);;
    }
    if(runStepPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);;
    }
    if(runStepPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);;
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
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);;
    }
    if(runStepPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorErrorSubRunStep(runStepPtr->motorNo);;
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机走到指定坐标的参数是否合格
ERROR_SUB StepMotorCheckRun2CoordationVar(StepMotorRun2CoordationCmd* runCoordationPtr)
{
    if(runCoordationPtr->motorNo > STEP_MOTOR_NO12)
    {
        //电机序号不对
        return ERROR_SUB_SM_NO_LIMIT;
    }
    if(runCoordationPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
    }
    if(runCoordationPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
    }
    if(runCoordationPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
    }
    if(runCoordationPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
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
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
    }
    if(runCoordationPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorErrorSubRunCoordinate(runCoordationPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机回零指令的参数是否合格
ERROR_SUB StepMotorCheckReturnZeroVar(StepMotorReturnZeroCmd* returnZeroPtr)
{
    if(returnZeroPtr->motorNo > STEP_MOTOR_NO12)
    {
        //电机序号不对
        return ERROR_SUB_SM_NO_LIMIT;
    }
    if(returnZeroPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
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
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
    }
    if(returnZeroPtr->timeOutSet == 0)
    {
        //超时时间不能为0
        return StepMotorErrorSubReturnZero(returnZeroPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查电机持续运转的参数是否合格
ERROR_SUB StepMotorCheckRunAlways(StepMotorRunAlwaysCmd* runAlwaysPtr)
{
    if(runAlwaysPtr->motorNo > STEP_MOTOR_NO12)
    {
        //电机序号不对
        return ERROR_SUB_SM_NO_LIMIT;
    }
    if(runAlwaysPtr->selectConfig >= SM_SPEED_TABLE_COUNT_EACH_ONE)
    {
        //电机选择的加速曲线超出系统限制
        return StepMotorErrorSubRunAlways(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatioMode > SPEED_RATIO_MODE_UP_START)
    {
        //电机速度比例模式错误
        return StepMotorErrorSubRunAlways(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatio > 100)
    {
        //速度比例错误
        return StepMotorErrorSubRunAlways(runAlwaysPtr->motorNo);
    }
    if(runAlwaysPtr->speedRatio == 0)
    {
        //速度比例错误
        return StepMotorErrorSubRunAlways(runAlwaysPtr->motorNo);
    }
    //校验通过
    return ERROR_SUB_OK;
}

//检查步进电机急停状态,如果是指令急停,那么报错
ERROR_SUB StepMotorUtilCheckEmergencyCmdState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    //检测电机状态,如果电机状态为指令急停,那么报错,指令急停必须复位取消
    if((motorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STATE_STOP)&&
        (motorVarPtr->var.motorSubState == STEP_MOTOR_STOP_SUB_EMERGENCY_CMD))
    {
        return StepMotorErrorSubEmStopCmd(motorNo);
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
    if(motorVarPtr->attri.baseConfig.positiveMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate > (motorVarPtr->attri.baseConfig.positiveMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            return StepMotorErrorSubStopCoordinatePositive(motorNo);
        }
    }
    //负方向的行程限制
    if(motorVarPtr->attri.baseConfig.reverseMaxSteps != 0)
    {
        //检查目的坐标是否在运行行程里面
        if(targetCoordinate < (motorVarPtr->attri.baseConfig.reverseMaxSteps))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            return StepMotorErrorSubStopCoordinateNegative(motorNo);
        }
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机急停引脚状态
ERROR_SUB StepMotorUtilCheckAndSetEmergencyPinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //如果系统支持急停,那么查看是否有急停端口,并且尝试将急停设置为中断
    if((motorVarPtr->attri.baseConfig.emStopPinIndex)< BOARD_PORT_IN_COUNT)
    {
        //配置了系统急停,那么检测此时急停IO状态,触发状态,直接报错
        if(motorVarPtr->attri.baseConfig.emStopPinValidLevel ==
                     BoardPortInReadState(motorVarPtr->attri.baseConfig.emStopPinIndex))
        {
            //电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            return StepMotorErrorSubEmStopSingal(motorNo);
        }
        else
        {
            //系统急停没有触发,那么将尝试将急停设置为中断
            if(motorVarPtr->attri.baseConfig.emStopPinValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.emStopPinIndex,
                                                    MCU_EXTI_MODE_IT_FALLING,
                                                    StepMotorEmergencyStopByIntSingalFuncPtrArray[motorNo]
                                                );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.emStopPinIndex,
                                                    MCU_EXTI_MODE_IT_RISING,
                                                    StepMotorEmergencyStopByIntSingalFuncPtrArray[motorNo]
                                                    );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //急停信号没有使能
        motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机辅助急停1状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop1PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //如果系统支持辅助急停端口,那么查看是否设置有辅助急停端口,并尝试将急停设置为中断
    //辅助急停1被使能且辅助急停1配置为系统输入
    if((enable == 1)&&(motorVarPtr->attri.baseConfig.utilStopPin1Index) < BOARD_PORT_IN_COUNT)
    {
        //首先查看辅助急停状态
        if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel ==
                     BoardPortInReadState(motorVarPtr->attri.baseConfig.utilStopPin1Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            return StepMotorErrorSubStopUtil1(motorNo);
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index,
                                                    MCU_EXTI_MODE_IT_FALLING,
                                                    StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorNo]
                                                    );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index,
                                                    MCU_EXTI_MODE_IT_RISING,
                                                    StepMotorDaccStopByUtilInt1SingalFuncPtrArray[motorNo]
                                                    );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return ERROR_SUB_OK;
}

//检查和设置步进电机辅助急停2状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop2PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //辅助急停2被使能且辅助急停配置为系统输入
    if((enable == 1)&&(motorVarPtr->attri.baseConfig.utilStopPin2Index) < BOARD_PORT_IN_COUNT)
    {
        //先检测辅助急停是否已经触发,如果已经出发,函数直接返回
        if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel ==
                     BoardPortInReadState(motorVarPtr->attri.baseConfig.utilStopPin2Index))
        {
            //辅助急停此时已经被触发,电机停下来,等待状态切换
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            return StepMotorErrorSubStopUtil2(motorNo);
        }
        else
        {
            //辅助急停未触发,尝试将辅助急停注册到中断函数中
            if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel == GPIO_PIN_RESET)
            {
                //低电平有效,设置为下降沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index,
                                                    MCU_EXTI_MODE_IT_FALLING,
                                                    StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorNo]
                                                    );
            }
            else
            {
                //高电平有效,设置为上升沿中断
                errorSubCode = BoardExtiRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index,
                                                    MCU_EXTI_MODE_IT_RISING,
                                                    StepMotorDaccStopByUtilInt2SingalFuncPtrArray[motorNo]
                                                    );
            }
            //查看注册错误代码
            if(errorSubCode == ERROR_SUB_OK)
            {
                //如果错误代码为无错误,说明已经设置好了中断,运行时不需要循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_EnableRegister;
            }
            else
            {
                //注册失败,需要运行时循环检测
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_EnableUnregister;
            }
        }
    }
    else
    {
        //辅助急停信号没使能
        motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
    }
    return ERROR_SUB_OK;
}

//根据本次电机目的坐标决定方向
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate)
{
    //然后设置当前方向
    if(targetCoordinate > motorVarPtr->var.currentCoordinate)
    {
        motorVarPtr->var.currentDir = motorVarPtr->attri.baseConfig.dir;
    }
    else
    {
        if(motorVarPtr->attri.baseConfig.dir == STEP_MOTOR_DIR_CCW)
        {
            motorVarPtr->var.currentDir = STEP_MOTOR_DIR_CW;
        }
        else
        {
            motorVarPtr->var.currentDir = STEP_MOTOR_DIR_CCW;
        }
    }
}

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentTableIndex = 0;
    //发出第一步
    if(motorVarPtr->var.stepMotorStepTableDirect[0] > motorVarPtr->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)motorVarPtr->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        motorVarPtr->attri.baseConfig.singlePluseMax
                                        );
        //上次发送的步数计数
        motorVarPtr->var.lastSendOutPluse = motorVarPtr->attri.baseConfig.singlePluseMax;
        //步数表实时减小
        motorVarPtr->var.stepMotorStepTableDirect[0] -= motorVarPtr->attri.baseConfig.singlePluseMax;
        
    }
    else
    {
        //小于最大脉冲数,一次发完
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)motorVarPtr->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        motorVarPtr->var.stepMotorStepTableDirect[0]
                                        );
        //上次发送的步数计数
        motorVarPtr->var.lastSendOutPluse = motorVarPtr->var.stepMotorStepTableDirect[0];
        //一次发完,步数表归零
        motorVarPtr->var.stepMotorStepTableDirect[0] = 0;
        //步数表往前加一步
        motorVarPtr->var.currentTableIndex += 1;
    }
}

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    //首先将当前序号设置为0
    motorVarPtr->var.currentDaccTableIndex = 0;
    //发出第一步
    if(motorVarPtr->var.stepMotorDaccStepTableDirect[0] > motorVarPtr->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)motorVarPtr->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorDaccSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        motorVarPtr->attri.baseConfig.singlePluseMax
                                        );
        //上次发送的步数计数
        motorVarPtr->var.lastSendOutPluse = motorVarPtr->attri.baseConfig.singlePluseMax;
        //步数表实时减小
        motorVarPtr->var.stepMotorDaccStepTableDirect[0] -= motorVarPtr->attri.baseConfig.singlePluseMax;
    }
    else
    {
        //小于最大脉冲数,一次发完
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)motorVarPtr->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        motorVarPtr->var.stepMotorDaccSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        motorVarPtr->var.stepMotorDaccStepTableDirect[0]
                                        );
        //上次发送的步数计数
        motorVarPtr->var.lastSendOutPluse = motorVarPtr->var.stepMotorDaccStepTableDirect[0];
        //一次发完,步数表归零
        motorVarPtr->var.stepMotorDaccStepTableDirect[0] = 0;
        //步数表往前加一步
        motorVarPtr->var.currentDaccTableIndex += 1;
    }
}

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr)
{
    //急停
    if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emStopPinIndex);
    }
    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;

    //辅助急停1
    if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;

    //辅助急停2
    if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
    {
        BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
    }
    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
}

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet)
{
    //等待步进电机处于停止状态,在等待过程中,如果前面设置的两个中断不支持,那么就要实时监测IO口状态,并进行处理
    while(motorVarPtr->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //检测超时,自动停止
        if(motorVarPtr->var.stepMotorRunningTimeMsCount > timeOutSet)
        {
            //实时监测的时候应该关闭中断
            motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
            //写入一次急停
            BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
            //上次的步数加上
            StepMotorUtilReflushCoordinate(motorNo);
            //结束循环
            continue;
        }
        //如果急停处于使能未注册的状态,那么需要检测电平,并进行处理
        if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足急停
            if(motorVarPtr->attri.baseConfig.emStopPinValidLevel == BoardPortInReadState(motorVarPtr->attri.baseConfig.emStopPinIndex))
            {
                //电机停下来,等待状态切换,如果辅助急停和急停同时出发,那么急停的优先级更高,减速过程会终止
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                motorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
                //写入一次急停
                BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
                //上次的步数加上
                StepMotorUtilReflushCoordinate(motorNo);
                //同时将监控状态去掉
                motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //如果辅助急停1处于使能未注册状态,需要实时监测
        if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attri.baseConfig.utilStopPin1ValidLevel == BoardPortInReadState(motorVarPtr->attri.baseConfig.utilStopPin1Index))
            {
                //注意,因为两个辅助急停不能同时生效,所以基本不用考虑两个辅助急停同时触发的情况,否则,还要考虑
                //两个辅助急停同时触发,状态变量之间的互斥关系
                //切换到减速过程
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
                //发送立即进入下一步运动step
                BoardCPLD_WriteStepMotorRunNextStep((BOARD_CPLD_MOTOR)motorNo);
                //更新当前坐标
                StepMotorUtilReflushCoordinate(motorNo);
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱

                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停2
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin2Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                continue;
            }
        }
        //辅助急停2的状态的处理
        if(motorVarPtr->var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_EnableUnregister)
        {
            //读取的状态满足辅助急停
            if(motorVarPtr->attri.baseConfig.utilStopPin2ValidLevel == BoardPortInReadState(motorVarPtr->attri.baseConfig.utilStopPin2Index))
            {
                motorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
                //切换到减速起始状态,要计算减速曲线的那种
                motorVarPtr->var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
                //发送立即进入下一步运动step
                BoardCPLD_WriteStepMotorRunNextStep((BOARD_CPLD_MOTOR)motorNo);
                //更新当前坐标
                StepMotorUtilReflushCoordinate(motorNo);
                //去除监控状态
                motorVarPtr->var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                //关闭急停
                if(motorVarPtr->var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.emStopPinIndex);
                }
                if(motorVarPtr->var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    motorVarPtr->var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }

                //关闭辅助急停1
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_EnableRegister)
                {
                    //中断注册,解除注册
                    BoardExtiUnRegister(motorVarPtr->attri.baseConfig.utilStopPin1Index);
                }
                if(motorVarPtr->var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DisableUnsupport)
                {
                    //关闭中断使能
                    motorVarPtr->var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DisableUnsupport;
                }
                //处理完成,本次循环跳出,不进行下次的其余模块检查,防止其他急停触发导致系统状态混乱
                continue;
            }
        }
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
        //更新运行时间
        motorVarPtr->var.stepMotorRunningTimeMsCount = tx_time_get() - motorVarPtr->var.stepMotorStartRunningTimeMs;
    }
}

void StepMotorUtilReflushCoordinate(STEP_MOTOR_NO motorNo)
{
    //上次的步数加上
    if(stepMotorVar[motorNo]->var.lastSendOutPluse != 0)
    {
        if(stepMotorVar[motorNo]->var.currentDir == stepMotorVar[motorNo]->attri.baseConfig.dir)
        {
            //正方向
            stepMotorVar[motorNo]->var.currentCoordinate += stepMotorVar[motorNo]->var.lastSendOutPluse;
        }
        else
        {
            //反方向
            stepMotorVar[motorNo]->var.currentCoordinate -= stepMotorVar[motorNo]->var.lastSendOutPluse;
        }
        stepMotorVar[motorNo]->var.lastSendOutPluse = 0;
    }
}

//解析走步数错误码
ERROR_SUB StepMotorErrorSubRunStep(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_PARAM_STEPS;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_PARAM_STEPS;
    }
    else
    {
        return ERROR_SUB_SM_12_PARAM_STEPS;
    }
}

//解析走坐标错误码
ERROR_SUB StepMotorErrorSubRunCoordinate(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_PARAM_COORDATION;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_PARAM_COORDATION;
    }
    else
    {
        return ERROR_SUB_SM_12_PARAM_COORDATION;
    }
}


//解析回零错误码
ERROR_SUB StepMotorErrorSubReturnZero(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_PARAM_RETURN_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_PARAM_RETURN_ZERO;
    }
    else
    {
        return ERROR_SUB_SM_12_PARAM_RETURN_ZERO;
    }
}


//解析持续运转错误码
ERROR_SUB StepMotorErrorSubRunAlways(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_PARAM_RUN_ALWAYS;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_PARAM_RUN_ALWAYS;
    }
    else
    {
        return ERROR_SUB_SM_12_PARAM_RUN_ALWAYS;
    }
}


//指令急停
ERROR_SUB StepMotorErrorSubEmStopCmd(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_EMERGENCY_CMD;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_EMERGENCY_CMD;
    }
    else
    {
        return ERROR_SUB_SM_12_EMERGENCY_CMD;
    }
}

//信号急停
ERROR_SUB StepMotorErrorSubEmStopSingal(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_EMERGENCY_SIGNAL;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_EMERGENCY_SIGNAL;
    }
    else
    {
        return ERROR_SUB_SM_12_EMERGENCY_SIGNAL;
    }
}

//辅助急停1
ERROR_SUB StepMotorErrorSubStopUtil1(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_UTIL_STOP1;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_UTIL_STOP1;
    }
    else
    {
        return ERROR_SUB_SM_12_UTIL_STOP1;
    }
}

//辅助急停2
ERROR_SUB StepMotorErrorSubStopUtil2(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_UTIL_STOP2;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_UTIL_STOP2;
    }
    else
    {
        return ERROR_SUB_SM_12_UTIL_STOP2;
    }
}

//超时停止
ERROR_SUB StepMotorErrorSubStopTimeOut(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_TIMEOUT;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_TIMEOUT;
    }
    else
    {
        return ERROR_SUB_SM_12_TIMEOUT;
    }
}

//复位超行程
ERROR_SUB StepMotorErrorSubStopResetStepLimit(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_RESET_STEP_LIMIT;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_RESET_STEP_LIMIT;
    }
    else
    {
        return ERROR_SUB_SM_12_RESET_STEP_LIMIT;
    }
}

//正向超行程
ERROR_SUB StepMotorErrorSubStopCoordinatePositive(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_CORDINATE_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_CORDINATE_MAX;
    }
    else
    {
        return ERROR_SUB_SM_12_CORDINATE_MAX;
    }
}

//反向超行程
ERROR_SUB StepMotorErrorSubStopCoordinateNegative(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_CORDINATE_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_CORDINATE_MIN;
    }
    else
    {
        return ERROR_SUB_SM_12_CORDINATE_MIN;
    }
}

//复位修正超正向行程
ERROR_SUB StepMotorErrorSubStopResetCorrectPositive(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_RESET_CORRECT_MAX;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_RESET_CORRECT_MAX;
    }
    else
    {
        return ERROR_SUB_SM_12_RESET_CORRECT_MAX;
    }
}

//反向修正超最大行程
ERROR_SUB StepMotorErrorSubStopResetCorrectNegative(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_RESET_CORRECT_MIN;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_RESET_CORRECT_MIN;
    }
    else
    {
        return ERROR_SUB_SM_12_RESET_CORRECT_MIN;
    }
}

//超时时间设置为0
ERROR_SUB StepMotorErrorSubStopTimeOutZero(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_TIME_OUT_ZERO;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_TIME_OUT_ZERO;
    }
    else
    {
        return ERROR_SUB_SM_12_TIME_OUT_ZERO;
    }
}

//READY信号无效
ERROR_SUB StepMotorErrorSubStopSlaveReadyFault(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_SLAVE_READY_FAULT;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_SLAVE_READY_FAULT;
    }
    else
    {
        return ERROR_SUB_SM_12_SLAVE_READY_FAULT;
    }
}

//离开原点超行程
ERROR_SUB StepMotorErrorSubStopLeaveZeroLimit(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_RESET_LEAVE_ORIGIN_SENSOR;
    }
    else
    {
        return ERROR_SUB_SM_12_RESET_LEAVE_ORIGIN_SENSOR;
    }
}

//找原点减速停止之后,原点信号丢失
ERROR_SUB StepMotorErrorSubStopResetDaccLoss(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_RESET_DACC_SENSOR_LOSS;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_RESET_DACC_SENSOR_LOSS;
    }
    else
    {
        return ERROR_SUB_SM_12_RESET_DACC_SENSOR_LOSS;
    }
}

//未知异常
ERROR_SUB StepMotorErrorSubStopUnknow(STEP_MOTOR_NO motorNo)
{
    if(motorNo == STEP_MOTOR_NO1)
    {
        return ERROR_SUB_SM_1_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO2)
    {
        return ERROR_SUB_SM_2_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO3)
    {
        return ERROR_SUB_SM_3_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO4)
    {
        return ERROR_SUB_SM_4_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO5)
    {
        return ERROR_SUB_SM_5_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO6)
    {
        return ERROR_SUB_SM_6_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO7)
    {
        return ERROR_SUB_SM_7_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO8)
    {
        return ERROR_SUB_SM_8_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO9)
    {
        return ERROR_SUB_SM_9_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO10)
    {
        return ERROR_SUB_SM_10_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO11)
    {
        return ERROR_SUB_SM_11_STATE_LOGIC;
    }
    else if(motorNo == STEP_MOTOR_NO12)
    {
        return ERROR_SUB_SM_12_STATE_LOGIC;
    }
    else
    {
        return ERROR_SUB_SM_12_STATE_LOGIC;
    }
}