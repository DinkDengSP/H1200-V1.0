/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:17:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-18 19:33:19 +0800
************************************************************************************************/ 
#include "StepMotorMain.h"

//电机坐标更新
static void StepMotorRunAlwaysReflushMotorCoordinate(STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //电机坐标更新
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)
    {
        //有编码器的情况下更新坐标,设置电机当前坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else
    {
        //没有编码器的情况下更新坐标
        if(motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)
        {
            //正方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate + motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        else if(motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CCW)
        {
            //负方向
            motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.baseMotorCoordinate - motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorPluseSendOutAll;
        }
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
}

//不支持的中断停止
static void StepMotorRunAlwaysStateIntProcessUnExceptStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //未知状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorRunAlwaysStateIntProcessUnExceptStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    else
    {
        MCU_RTT_SendStringInt("StepMotorRunAlwaysStateIntProcessUnExceptStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
    }
    return;
}

//持续运转状态下的中断处理,加速
static void StepMotorRunAlwaysStateIntProcessAcc(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //加速阶段,任意停止讯息都都会导致不期望的中断停止
    StepMotorRunAlwaysStateIntProcessUnExceptStop(motorNo,motorDataSetPtr,motorState,stopReason);
    return;
}

//持续运转状态下的中断处理,减速
static void StepMotorRunAlwaysStateIntProcessDec(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //减速阶段,只有收到脉冲发送完成才能算正常停止,其余的都是不期望的状态导致停止
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //电机切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunAlwaysStateIntProcessDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            return;
        }
        else
        {
            //其余状态,不期望的中断导致的停止
            StepMotorRunAlwaysStateIntProcessUnExceptStop(motorNo,motorDataSetPtr,motorState,stopReason);
        }
    }
    else
    {
        //其余状态,不期望的中断导致的停止
        StepMotorRunAlwaysStateIntProcessUnExceptStop(motorNo,motorDataSetPtr,motorState,stopReason);
    }
    return;
}

//持续运转状态下的中断处理,不支持的子状态
static void StepMotorRunAlwaysStateIntProcessNotSupport(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //未知状态引发停止
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //未知状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入指令停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorRunAlwaysStateIntProcessUnExceptStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    else
    {
        MCU_RTT_SendStringInt("StepMotorRunAlwaysStateIntProcessUnExceptStop STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE Error!!\r\n");
    }
    return;
}

//持续运转状态下的中断处理
void StepMotorRunAlwaysStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
    switch(motorDataSetPtr->stateSet.subState)
    {
        case STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC:
            StepMotorRunAlwaysStateIntProcessAcc(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC:
            StepMotorRunAlwaysStateIntProcessDec(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        default:
            StepMotorRunAlwaysStateIntProcessNotSupport(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
    }
}
/******************************************************************************************************************************************************/
//参数检查
static ERROR_SUB StepMotorRunAlwaysAccParamCheck(StepMotorRunAlwaysCmd* paramPtr)
{
    if(paramPtr == NULL)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_ALWAYS_PARAM_NULL);
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_ALWAYS_PARAM_NULL;
    }
    if(paramPtr->motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->motorDir) > (uint8_t)STEP_MOTOR_DIR_CCW)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_DIR_UNSUPPORT);
        //电机方向不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_DIR_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->selectCurveConfig) >= SM_CURVE_SET_MAX)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT);
        //电机曲线不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->speedRatioMode) > (uint8_t)SPEED_RATIO_MODE_UP_START)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_MODE_UNSUPPORT);
        //电机速度比例模式不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_MODE_UNSUPPORT;
    }
    if(paramPtr->speedRatio == 0.0f)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_UNSUPPORT);
        //电机速度比例不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_UNSUPPORT;
    }
    return ERROR_SUB_OK;
}

//运行前检查电机状态是否正确
static ERROR_SUB StepMotorRunAlwaysAccBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunAlwaysCmd* paramPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)
    {
        //电机不是空闲状态,不能运转
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_NOT_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //关闭学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorRunAlwaysAccParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunAlwaysCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //拷贝电机运行指令
    UserMemCopy((uint8_t*)&(motorDataSetPtr->stateSet.paramMotorRunAlways),(uint8_t*)paramPtr,DIM_STRUCT_LENGTH_BYTE(StepMotorRunAlwaysCmd));
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)
    {
        //有编码器的情况下更新坐标,设置电机当前坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else
    {
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    //电机使能
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //电机使能失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //设置电机运行状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_ALWAYS;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC;
    return ERROR_SUB_OK;
}

//运行启动
static ERROR_SUB StepMotorRunAlwaysAccStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunAlwaysCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //电机切入空闲状态
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_TO_IDLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_FPFA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机方向
    motorDataSetPtr->stateSet.currentDir = paramPtr->motorDir;
    //根据曲线配置决定使用定频运转还是曲线运转
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].maxRpm)
    {
        //启动速度和最高速度对比,如果启动速度大于等于最大速度,表示使用定频运转曲线
        motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm;
        //计算速度比率
        if(paramPtr->speedRatio != 1.0f)
        {
            if(paramPtr->speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= paramPtr->speedRatio;
            }
            else
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (paramPtr->speedRatio)); 
            }
        }
        //持续运转,步数直接给最高
        motorDataSetPtr->stateSet.cmdFixSpd.stepCount = 0XFFFFFFFF;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //启动定频运转
        errorSub = BoardFPGA_StepMotorFixSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdFixSpd));
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_FPFA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        //使用曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //曲线计算
        StepMotorCurveCalcUtilTableWithRunAlwaysAcc(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                    &motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig],
                                                    paramPtr->speedRatioMode,paramPtr->speedRatio);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
        //启动曲线运转,不使用预写入
        errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdTabSpd),DISABLE);
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_FPFA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    return ERROR_SUB_OK;
}

//等待运行完成
static ERROR_SUB StepMotorRunAlwaysAccWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunAlwaysCmd* paramPtr)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //延时
    CoreDelayMs(STEP_MOTOR_RUN_STATE_QUERY_INTERVAL_MS);
    //记录当前运行时间
    motorDataSetPtr->stateSet.runningTimeMsCount = OS_TICK_TO_MS(tx_time_get() - motorDataSetPtr->stateSet.startTimeTick);
    //检查状态是否正确
    if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||(motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC))
    {
        //检查是不是被外部停止了
        if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
        {
            //外部指令急停了
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
        else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
        {
            //不期望的异常导致的急停
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
        else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
        {
            //状态机混乱导致异常
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
        else
        {
            if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_STOP)
            {
                //不是停止状态,强行停止,状态异常
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //状态机混乱导致异常
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_ERR;
                //电机切入停止
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
                if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    MCU_RTT_SendStringInt("StepMotorRunAlwaysAccWaitOver BoardFPGA_StepMotorConvertToIdle Error!!\r\n");
                }
            }
            //状态机混乱导致异常
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    //读取FPGA当前状态查看是否状态正确
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检查FPGA状态是否正确
    if(motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)
    {
        if(motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm >= 
                motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].maxRpm)
        {
            //定频运转
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_FIX_SPD)
            {
                return ERROR_SUB_OK;
            }
        }
        else
        {
            //曲线运转
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_TAB_SPD)
            {
                return ERROR_SUB_OK;
            }
        }
    }
    else
    {
        if(motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm >= 
                motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].maxRpm)
        {
            //定频运转
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_FIX_SPD)
            {
                return ERROR_SUB_OK;
            }
        }
        else
        {
            //曲线运转
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_TAB_SPD)
            {
                return ERROR_SUB_OK;
            }
        }
    }
    //不是停止状态,强行停止,状态异常
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //FPGA状态不匹配导致异常
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
    //电机切入停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorRunAlwaysAccWaitOver BoardFPGA_StepMotorConvertToIdle Error!!\r\n");
    }
    //状态机混乱导致异常
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_ACC_FPFA_STATE_NOT_MATCH + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunAlwaysAccWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunAlwaysCmd* paramPtr)
{
    //读取FPGA当前状态查看是否状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr); 
    return ERROR_SUB_OK;
}

//电机持续运转,加速
ERROR_SUB StepMotorRunAlwaysAcc(StepMotorRunAlwaysCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //首先检查指令参数是否正确
    errorSub = StepMotorRunAlwaysAccParamCheck(paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //获取状态指针
    motorDataSetPtr = StepMotorBaseGetDataSetPtr(paramPtr->motorNo);
    //申请互斥量
    errorSub = StepMotorBaseGetLock(paramPtr->motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行前状态检查
    errorSub = StepMotorRunAlwaysAccBeforeStateCheck(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动前各种准备
    errorSub = StepMotorRunAlwaysAccParpare(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动运转
    errorSub = StepMotorRunAlwaysAccStart(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放信号量
    StepMotorBaseReleaseLock(paramPtr->motorNo);
    //等待运行完成
    errorSub = StepMotorRunAlwaysAccWaitOver(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //读取FPGA当前状态查看是否状态
        BoardFPGA_StepMotorGetReadOnlyReg(paramPtr->motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //更新坐标
        StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);        
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(paramPtr->motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行完成之后收尾
    errorSub = StepMotorRunAlwaysAccWindUp(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(paramPtr->motorNo);
    //返回
    return ERROR_SUB_OK;
}

/******************************************************************************************************************************************************/
//参数检查
static ERROR_SUB StepMotorRunAlwaysDecSlowParamCheck(STEP_MOTOR_NO motorNo,uint32_t timeOutMs)
{
    if(motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    if(timeOutMs > STEP_MOTOR_TIME_OUT_MAX_MS)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX);
        //电机速度比例不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX;
    }
    return ERROR_SUB_OK;
}

//运行前检查电机状态是否正确
static ERROR_SUB StepMotorRunAlwaysDecSlowBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||
        (motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC))
    {
        //电机不是持续运转加速状态,只有加速状态才能且减速状态
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_NOT_RUN_ALWAYS_ACC + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //关闭学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorRunAlwaysDecSlowParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //设置电机运行状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_ALWAYS;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC;
    return ERROR_SUB_OK;
}

//运行启动
static ERROR_SUB StepMotorRunAlwaysDecSlowStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //根据曲线配置决定使用定频运转还是曲线运转
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
    {
        //启动速度和最高速度对比,如果启动速度大于等于最大速度,表示使用定频运转曲线
        motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm;
        //计算速度比率
        if(motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio != 1.0f)
        {
            if(motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio;
            }
            else
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio)); 
            }
        }
        //只需要一步就可以了
        motorDataSetPtr->stateSet.cmdFixSpd.stepCount = 1;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    }
    else
    {
        //使用曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //减速曲线计算
        StepMotorCurveCalcUtilTableWithRunAlwaysDec(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                    motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
    }
    //电机停止
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
    {
        //启动定频运转
        errorSub = BoardFPGA_StepMotorFixSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdFixSpd));
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        //启动曲线运转,使用预写入
        errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdTabSpd),ENABLE);
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    return ERROR_SUB_OK;
}

//等待运行完成
static ERROR_SUB StepMotorRunAlwaysDecSlowWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    do
    {
        //延时等待
        CoreDelayMs(STEP_MOTOR_RUN_STATE_QUERY_INTERVAL_MS);
        //记录当前运行时间
        motorDataSetPtr->stateSet.runningTimeMsCount = OS_TICK_TO_MS(tx_time_get() - motorDataSetPtr->stateSet.startTimeTick);
        //检查超时
        if(motorDataSetPtr->stateSet.runningTimeMsCount > timeOutMs)
        {
            //超时跳出
            break;
        }
        //检查电机状态变化
        if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||(motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC))
        {
            //检查是不是被外部停止了
            if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
            {
                //外部指令急停了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
            {
                //不期望的异常导致的急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
            {
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP))
            {
                //脉冲发送完成,正常停止
                return ERROR_SUB_OK;
            }
            else
            {
                if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_STOP)
                {
                    //不是停止状态,强行停止,状态异常
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    //状态机混乱导致异常
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_ERR;
                    //电机切入停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
                    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorRunAlwaysDecWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    //加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    //电机坐标更新
                    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
                }
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
        }
        //读取FPGA当前状态查看是否状态正确
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //检查FPGA状态是否正确
        if(motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)
        {
            if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
                    motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
            {
                //定频运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_FIX_SPD)
                {
                    continue;
                }
                //定频运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER))
                {
                    continue;
                }
            }
            else
            {
                //曲线运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_TAB_SPD)
                {
                    continue;
                }
                //曲线运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER))
                {
                    continue;
                }
            }
        }
        else
        {
            if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
                    motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
            {
                //定频运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_FIX_SPD)
                {
                    continue;
                }
                //定频运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER))
                {
                    continue;
                }
            }
            else
            {
                //曲线运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_TAB_SPD)
                {
                    continue;
                }
                //曲线运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
                {
                    continue;
                }
            }
        }
        //不是FPGA此时的正常状态
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //FPGA状态不匹配导致异常
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
        //电机切入停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
        if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorRunAlwaysDecWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        //加载电机只读寄存器状态
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //电机坐标更新
        StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
        //状态机混乱导致异常
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_STATE_NOT_MATCH + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }while(1);
    //读取FPGA当前状态
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //到这里肯定是因为超时,电机停止
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    //电机切入空闲状态
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_TO_IDLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //报错运行超时
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_TIME_OUT;
    //电机运行超时
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_TIME_OUT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunAlwaysDecSlowWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //运动完成,电机状态修改为空闲,便于下一次操作
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    return ERROR_SUB_OK;
}

//电机持续运转,减速到停止
ERROR_SUB StepMotorRunAlwaysDecSlow(STEP_MOTOR_NO motorNo,uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //获取状态指针
    motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //首先检查指令参数是否正确
    errorSub = StepMotorRunAlwaysDecSlowParamCheck(motorNo,timeOutMs);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行前状态检查
    errorSub = StepMotorRunAlwaysDecSlowBeforeStateCheck(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动前各种准备
    errorSub = StepMotorRunAlwaysDecSlowParpare(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动运转
    errorSub = StepMotorRunAlwaysDecSlowStart(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放信号量
    StepMotorBaseReleaseLock(motorNo);
    //等待运行完成
    errorSub = StepMotorRunAlwaysDecSlowWaitOver(motorNo,motorDataSetPtr,timeOutMs);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行完成之后收尾
    errorSub = StepMotorRunAlwaysDecSlowWindUp(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    //返回
    return ERROR_SUB_OK;
}

/******************************************************************************************************************************************************/
//参数检查
static ERROR_SUB StepMotorRunAlwaysDecFastParamCheck(STEP_MOTOR_NO motorNo,uint32_t steps,uint32_t timeOutMs)
{
    if(motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    if(timeOutMs > STEP_MOTOR_TIME_OUT_MAX_MS)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX);
        //电机速度比例不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX;
    }
    if(steps == 0)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_ALWAYS_DEC_STEP_ZERO);
        //电机速度比例不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_ALWAYS_DEC_STEP_ZERO;
    }
    return ERROR_SUB_OK;
}

//运行前检查电机状态是否正确
static ERROR_SUB StepMotorRunAlwaysDecFastBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||
        (motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC))
    {
        //电机不是持续运转加速状态,只有加速状态才能且减速状态
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_NOT_RUN_ALWAYS_ACC + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //关闭学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorRunAlwaysDecFastParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //设置电机运行状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_ALWAYS;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC;
    return ERROR_SUB_OK;
}


//运行启动
static ERROR_SUB StepMotorRunAlwaysDecFastStart(STEP_MOTOR_NO motorNo,uint32_t steps,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //根据曲线配置决定使用定频运转还是曲线运转
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
    {
        //启动速度和最高速度对比,如果启动速度大于等于最大速度,表示使用定频运转曲线
        motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm;
        //计算速度比率
        if(motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio != 1.0f)
        {
            if(motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio;
            }
            else
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorRunAlways.speedRatio)); 
            }
        }
        //只需要一步就可以了
        motorDataSetPtr->stateSet.cmdFixSpd.stepCount = steps;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    }
    else
    {
        //使用曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //减速曲线计算
        StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                            motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent,
                                                            steps);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
    }
    //电机停止
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
    {
        //启动定频运转
        errorSub = BoardFPGA_StepMotorFixSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdFixSpd));
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        //启动曲线运转,使用预写入
        errorSub = BoardFPGA_StepMotorTabSpdRunStart(motorNo,&(motorDataSetPtr->stateSet.cmdTabSpd),ENABLE);
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机控制失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    return ERROR_SUB_OK;
}


//等待运行完成
static ERROR_SUB StepMotorRunAlwaysDecFastWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    do
    {
        //延时等待
        CoreDelayMs(STEP_MOTOR_RUN_STATE_QUERY_INTERVAL_MS);
        //记录当前运行时间
        motorDataSetPtr->stateSet.runningTimeMsCount = OS_TICK_TO_MS(tx_time_get() - motorDataSetPtr->stateSet.startTimeTick);
        //检查超时
        if(motorDataSetPtr->stateSet.runningTimeMsCount > timeOutMs)
        {
            //超时跳出
            break;
        }
        //检查电机状态变化
        if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||(motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC))
        {
            //检查是不是被外部停止了
            if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
            {
                //外部指令急停了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
            {
                //不期望的异常导致的急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
            {
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP))
            {
                //脉冲发送完成,正常停止
                return ERROR_SUB_OK;
            }
            else
            {
                if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_STOP)
                {
                    //不是停止状态,强行停止,状态异常
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    //状态机混乱导致异常
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_ERR;
                    //电机切入停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
                    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorRunAlwaysDecWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    //加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    //电机坐标更新
                    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
                }
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
        }
        //读取FPGA当前状态查看是否状态正确
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //检查FPGA状态是否正确
        if(motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)
        {
            if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
                    motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
            {
                //定频运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_FIX_SPD)
                {
                    continue;
                }
                //定频运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER))
                {
                    continue;
                }
            }
            else
            {
                //曲线运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CW_TAB_SPD)
                {
                    continue;
                }
                //曲线运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER))
                {
                    continue;
                }
            }
        }
        else
        {
            if(motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].startRpm >= 
                    motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunAlways.selectCurveConfig].maxRpm)
            {
                //定频运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_FIX_SPD)
                {
                    continue;
                }
                //定频运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER))
                {
                    continue;
                }
            }
            else
            {
                //曲线运转
                if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_CCW_TAB_SPD)
                {
                    continue;
                }
                //曲线运转停止
                else if((motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.motorState == STEP_MOTOR_STATE_STOP_CONDITION)&&
                        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason.valBits.stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
                {
                    continue;
                }
            }
        }
        //不是FPGA此时的正常状态
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //FPGA状态不匹配导致异常
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
        //电机切入停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
        if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorRunAlwaysDecWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        //加载电机只读寄存器状态
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //电机坐标更新
        StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
        //状态机混乱导致异常
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_STATE_NOT_MATCH + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }while(1);
    //读取FPGA当前状态
    BoardFPGA_StepMotorGetAllReg(motorNo,motorDataSetPtr->stateSet.fpgaRegSetPtr);
    //到这里肯定是因为超时,电机停止
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    //电机切入空闲状态
    errorSub = BoardFPGA_StepMotorConvertToIdle(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_TO_IDLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //报错运行超时
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_TIME_OUT;
    //电机运行超时
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_TIME_OUT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunAlwaysDecFastWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //运动完成,电机状态修改为空闲,便于下一次操作
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    return ERROR_SUB_OK;
}

//电机持续运转,快速停止
ERROR_SUB StepMotorRunAlwaysDecFast(STEP_MOTOR_NO motorNo,uint32_t steps,uint32_t timeOutMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //获取状态指针
    motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //首先检查指令参数是否正确
    errorSub = StepMotorRunAlwaysDecFastParamCheck(motorNo,steps,timeOutMs);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行前状态检查
    errorSub = StepMotorRunAlwaysDecFastBeforeStateCheck(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动前各种准备
    errorSub = StepMotorRunAlwaysDecFastParpare(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动运转
    errorSub = StepMotorRunAlwaysDecFastStart(motorNo,steps,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放信号量
    StepMotorBaseReleaseLock(motorNo);
    //等待运行完成
    errorSub = StepMotorRunAlwaysDecFastWaitOver(motorNo,motorDataSetPtr,timeOutMs);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行完成之后收尾
    errorSub = StepMotorRunAlwaysDecFastWindUp(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    //返回
    return ERROR_SUB_OK;
}

/******************************************************************************************************************************************************/
//参数检查
static ERROR_SUB StepMotorRunAlwaysStopImmediateParamCheck(STEP_MOTOR_NO motorNo)
{
    if(motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    return ERROR_SUB_OK;
}


//运行前检查电机状态是否正确
static ERROR_SUB StepMotorRunAlwaysStopImmediateBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_ALWAYS)||
        (motorDataSetPtr->stateSet.subState != STEP_MOTOR_STATE_SUB_RUN_ALWAYS_ACC))
    {
        //电机不是持续运转加速状态,只有加速状态才能且减速状态
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_NOT_RUN_ALWAYS_ACC + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //关闭学习模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorRunAlwaysStopImmediateParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //设置电机运行状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_ALWAYS;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_ALWAYS_DEC;
    return ERROR_SUB_OK;
}


//运行启动
static ERROR_SUB StepMotorRunAlwaysStopImmediateStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //电机停止
    errorSub = BoardFPGA_StepMotorCmdStop(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if(errorSub != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_ALWAYS_DEC_FPFA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    return ERROR_SUB_OK;
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunAlwaysStopImmediateWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    //运动完成,电机状态修改为空闲,便于下一次操作
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunAlwaysReflushMotorCoordinate(motorDataSetPtr);
    return ERROR_SUB_OK;
}

//电机持续运转,立刻停止
ERROR_SUB StepMotorRunAlwaysStopImmediate(STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //获取状态指针
    motorDataSetPtr = StepMotorBaseGetDataSetPtr(motorNo);
    //首先检查指令参数是否正确
    errorSub = StepMotorRunAlwaysStopImmediateParamCheck(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //申请互斥量
    errorSub = StepMotorBaseGetLock(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行前状态检查
    errorSub = StepMotorRunAlwaysStopImmediateBeforeStateCheck(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动前各种准备
    errorSub = StepMotorRunAlwaysStopImmediateParpare(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动运转
    errorSub = StepMotorRunAlwaysStopImmediateStart(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行完成之后收尾
    errorSub = StepMotorRunAlwaysStopImmediateWindUp(motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(motorNo);
    //返回
    return ERROR_SUB_OK;
}