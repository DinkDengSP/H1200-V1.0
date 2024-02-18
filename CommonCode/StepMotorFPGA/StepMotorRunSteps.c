/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:16:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-18 18:39:43 +0800
************************************************************************************************/ 
#include "StepMotorMain.h"

//电机坐标更新
static void StepMotorRunStepsReflushMotorCoordinate(STEP_MOTOR_DATA_SET* motorDataSetPtr)
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

//正常运转切换到脉冲发送完成停止
static void StepMotorRunStepsStateIntProcessNormalToPluseStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //脉冲发送完成
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入指令停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
}

//正常运转切换到急停减速过程
static void StepMotorRunStepsStateIntProcessNormalToEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    // 急停有效导致的停止,检查是否设置了急停检测,设置了急停那么进入减速,否则进入异常信号停止
    if (motorDataSetPtr->stateSet.paramMotorRunSteps.emStopSensorSet == ENABLE)
    {
        // 查看接下来应该是急停还是减速
        if (motorDataSetPtr->attriPtr->motorRunAttri.decStepsEmStopTrig == 0)
        {
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            return;
        }
        else
        {
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 减速停止,计算减速曲线,分为匀减速和曲线减速
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID))
            {
                // 匀减速
                motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunSteps.selectCurveConfig].startRpm;
                // 计算速度比率
                if (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio != 1.0f)
                {
                    if (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
                    {
                        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio;
                    }
                    else
                    {
                        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio));
                    }
                }
                // 只需要一步就可以了
                motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.decStepsEmStopTrig;
                motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                // 停止条件设定
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                // 打开脉冲发送完成停止
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
            }
            else
            {
                // 曲线减速
                motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                // 停止条件设定
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                // 打开脉冲发送完成停止
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                // 减速曲线计算
                StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr, motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                                   motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent,
                                                                   motorDataSetPtr->attriPtr->motorRunAttri.decStepsEmStopTrig);
                // 曲线设置
                motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
                motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
                motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
                motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
            }
            // 电机停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            // 更新电机当前坐标
            StepMotorRunStepsReflushMotorCoordinate(motorDataSetPtr);
            // 定频运转减速
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID))
            {
                // 启动定频运转
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToEmStop BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                    return;
                }
            }
            else
            {
                // 启动曲线运转,使用预写入
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                    return;
                }
            }
            // 电机切入emstop减速
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_STEPS;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_EM_STOP;
            return;
        }
    }
    else
    {
        // 程序设置没打开急停
        // 电机切入异常停止状态
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 未知状态下收到了电机中断
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP;
        // 加载电机只读寄存器状态
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        // 电机切入停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
        }
        return;
    }
}

//正常运转切换到辅助急停减速过程
static void StepMotorRunStepsStateIntProcessNormalToUtilStop1(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    // 辅助急停有效导致的停止,检查是否设置了辅助急停检测,设置了辅助急停那么进入减速,否则进入异常信号停止
    if (motorDataSetPtr->stateSet.paramMotorRunSteps.utilStopSensorSet == ENABLE)
    {
        // 查看接下来应该是辅助急停还是减速
        if (motorDataSetPtr->attriPtr->motorRunAttri.decStepsUtilStopTrig == 0)
        {
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            return;
        }
        else
        {
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 减速停止,计算减速曲线,分为匀减速和曲线减速
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID))
            {
                // 匀减速
                motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorRunSteps.selectCurveConfig].startRpm;
                // 计算速度比率
                if (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio != 1.0f)
                {
                    if (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
                    {
                        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio;
                    }
                    else
                    {
                        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorRunSteps.speedRatio));
                    }
                }
                // 只需要一步就可以了
                motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.decStepsUtilStopTrig;
                motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                // 停止条件设定
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                // 打开脉冲发送完成停止
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
            }
            else
            {
                // 曲线减速
                motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                // 停止条件设定
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                // 打开脉冲发送完成停止
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                // 减速曲线计算
                StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr, motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                                   motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent,
                                                                   motorDataSetPtr->attriPtr->motorRunAttri.decStepsUtilStopTrig);
                // 曲线设置
                motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
                motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
                motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
                motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
            }
            // 电机停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            // 更新电机当前坐标
            StepMotorRunStepsReflushMotorCoordinate(motorDataSetPtr);
            // 定频运转减速
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID))
            {
                // 启动定频运转
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                    return;
                }
            }
            else
            {
                // 启动曲线运转,使用预写入
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                    return;
                }
            }
            // 电机切入emstop减速
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_STEPS;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_UTIL_STOP;
            return;
        }
    }
    else
    {
        // 程序设置没打开辅助急停
        // 电机切入异常停止状态
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 未知状态下收到了电机中断
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP;
        // 加载电机只读寄存器状态
        BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        // 电机切入停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormalToUtilStop1 STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP Error!!\r\n");
        }
        return;
    }
}

//正常运转中断
static void StepMotorRunStepsStateIntProcessNormal(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //正常运行切换脉冲发送完成停止
            StepMotorRunStepsStateIntProcessNormalToPluseStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorRunStepsStateIntProcessNormalToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorRunStepsStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else
        {
            //不期望的中断,电机切入异常停止状态
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //未知状态下收到了电机中断
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
            }
            return;
        }
    }
    else
    {
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
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNormal STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//减速急停中断
static void StepMotorRunStepsStateIntProcessDecEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //减速完成停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            return;
        }
        else
        {
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
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
            }
            return;
        }
    }
    else
    {
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
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//辅助急停中断
static void StepMotorRunStepsStateIntProcessDecUtilStop1(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //减速完成停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            return;
        }
        else
        {
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
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
            }
            return;
        }
    }
    else
    {
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
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessDecUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//不支持的状态中断
static void StepMotorRunStepsStateIntProcessNotSupport(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //电机切入异常停止状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    //未知状态下收到了电机中断
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机切入停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
    if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNotSupport BoardFPGA_StepMotorCmdStop Error!!\r\n");
    }
    else
    {
        MCU_RTT_SendStringInt("StepMotorRunStepsStateIntProcessNotSupport STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE Error!!\r\n");
    }
    return;
}


//走步数状态下的中断处理
void StepMotorRunStepsStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
    switch(motorDataSetPtr->stateSet.subState)
    {
        case STEP_MOTOR_STATE_SUB_RUN_STEPS_NORMAL:
            //正常运转中断
            StepMotorRunStepsStateIntProcessNormal(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_EM_STOP:
            //减速急停中断
            StepMotorRunStepsStateIntProcessDecEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RUN_STEPS_DEC_UTIL_STOP:
            //辅助急停中断
            StepMotorRunStepsStateIntProcessDecUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        default:
            //不支持的状态中断
            StepMotorRunStepsStateIntProcessNotSupport(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
    }
}

/******************************************************************************************************************************************************/
//参数检查
static ERROR_SUB StepMotorRunStepsBaseParamCheck(StepMotorRunStepsCmd* paramPtr)
{
    if(paramPtr == NULL)
    {
        //错误打印,参数为空
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_STEPS_PARAM_NULL);
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_STEPS_PARAM_NULL;
    }
    if(paramPtr->motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印,电机需要大于系统支持最大序号
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->selectCurveConfig) >= SM_CURVE_SET_MAX)
    {
        //错误打印,电机曲线大于系统支持最大曲线
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT);
        //电机曲线不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->speedRatioMode) > (uint8_t)SPEED_RATIO_MODE_UP_START)
    {
        //错误打印,速度配比模式不是系统支持的配比模式
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_MODE_UNSUPPORT);
        //电机速度比例模式不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_MODE_UNSUPPORT;
    }
    if(paramPtr->speedRatio == 0.0f)
    {
        //错误打印,速度比例为0
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_UNSUPPORT);
        //电机速度比例不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_SPD_RATIO_UNSUPPORT;
    }
    //检查运行时间是否超过限制
    if(paramPtr->timeOutMsSet > STEP_MOTOR_TIME_OUT_MAX_MS)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX);
        //超时时间超限制
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_TIME_OUT_MAX;
    }
    //检查运行步数是否为0,为0报错
    if(paramPtr->totalSteps == 0)
    {
        //错误打印
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_STEPS_STEP_ZERO);
        //超时时间超限制
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RUN_STEPS_STEP_ZERO;
    }
    return ERROR_SUB_OK;
}

//运行前检查电机状态是否正确
static ERROR_SUB StepMotorRunStepsBaseBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunStepsCmd* paramPtr,
                                                            int32_t* dirSwitchStepsRepair)
{
    *dirSwitchStepsRepair = 0;
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)
    {
        //电机不是空闲状态,不能运转
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_MOTOR_NOT_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //检查是否设置了急停,设置急停的话就需要配置急停引脚
    if((paramPtr->emStopSensorSet == ENABLE)&&(motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet == DISABLE))
    {
        //电机设置了急停但是急停引脚未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_EM_STOP_ENABLE_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //检查是否设置了辅助急停,设置辅助急停的话就需要配置辅助急停引脚
    if((paramPtr->utilStopSensorSet == ENABLE)&&(motorDataSetPtr->attriPtr->motorSensorAttri.utilStopSensorSet == DISABLE))
    {
        //电机设置了辅助急停但是辅助急停引脚未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_UTIL_STOP_ENABLE_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //带编码器的情况下不能带编码器修正系数
    if((motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)&&(paramPtr->dirSwitchCorrectSteps != 0))
    {
        //带编码器的情况下不能带正反向修正系数
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_DIR_SWITCH_FIX_ENCODER_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //存在编码器就更新编码器坐标
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)
    {
        //有编码器的情况下更新坐标,设置电机当前坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.encoderCurrentPos;
        //基础坐标等于当前坐标
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else
    {
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    //计算目标位置
    motorDataSetPtr->stateSet.targetCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate + paramPtr->totalSteps;
    //这个地方可以加入正反向修正
    if(paramPtr->dirSwitchCorrectSteps != 0)
    {
        //查看是否需要修正
        if(motorDataSetPtr->attriPtr->motorRunAttri.dirSwitchFixMode == STEP_MOTOR_DIR_SWITCH_FIX_MODE_NONE)
        {
            //电机未设置修正模式但是给了修正系数
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_DIR_SWITCH_FIX_SET_NONE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
        else if(motorDataSetPtr->attriPtr->motorRunAttri.dirSwitchFixMode == STEP_MOTOR_DIR_SWITCH_FIX_MODE_CW_TO_CCW)
        {
            //正向切换反向的时候进行修正
            if((motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)&&(paramPtr->totalSteps < 0))
            {
                //需要修正,在原有的负坐标的情况下再多走一段
                motorDataSetPtr->stateSet.targetCoordinate -= paramPtr->dirSwitchCorrectSteps;
                *dirSwitchStepsRepair = paramPtr->dirSwitchCorrectSteps;
            }
        }
        else if(motorDataSetPtr->attriPtr->motorRunAttri.dirSwitchFixMode == STEP_MOTOR_DIR_SWITCH_FIX_MODE_CCW_TO_CW)
        {
            //反向切换正向的时候需要修正
            if((motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CCW)&&(paramPtr->totalSteps > 0))
            {
                //需要修正,在原有的正坐标的情况下再多走一段
                motorDataSetPtr->stateSet.targetCoordinate += paramPtr->dirSwitchCorrectSteps;
                *dirSwitchStepsRepair = 0 - paramPtr->dirSwitchCorrectSteps;
            }
        }
        else if(motorDataSetPtr->attriPtr->motorRunAttri.dirSwitchFixMode == STEP_MOTOR_DIR_SWITCH_FIX_MODE_ALL)
        {
            //只要当前方向和目标方向不一致就需要修正
            if((motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CW)&&(paramPtr->totalSteps < 0))
            {
                //需要修正,在原有的负坐标的情况下再多走一段
                motorDataSetPtr->stateSet.targetCoordinate -= paramPtr->dirSwitchCorrectSteps;
                *dirSwitchStepsRepair = paramPtr->dirSwitchCorrectSteps;
            }
            else if((motorDataSetPtr->stateSet.currentDir == STEP_MOTOR_DIR_CCW)&&(paramPtr->totalSteps > 0))
            {
                //需要修正,在原有的正坐标的情况下再多走一段
                motorDataSetPtr->stateSet.targetCoordinate += paramPtr->dirSwitchCorrectSteps;
                *dirSwitchStepsRepair = 0 - paramPtr->dirSwitchCorrectSteps;
            }
        }
    }
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps != 0)&&
        ((motorDataSetPtr->stateSet.targetCoordinate) > motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps))
    {
        //电机超正向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_POSITIVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps != 0)&&
        ((motorDataSetPtr->stateSet.targetCoordinate) < motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps))
    {
        //电机超正向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_RESERVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //启动之间检测急停信号,启动前就有效的直接报错
    if((paramPtr->emStopSensorSet == ENABLE)&&
        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.emStopSensorState == PIN_STATE_VALID))
    {
        //启动前检测到急停有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_EM_STOP_VALID_BEFORE_START + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //启动之间检测辅助急停信号,启动前就有效的直接报错
    if((paramPtr->utilStopSensorSet == ENABLE)&&
        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.utilStopSensorState == PIN_STATE_VALID))
    {
        //启动前检测到辅助急停有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_UTIL_STOP_VALID_BEFORE_START + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}


//运行前状态准备
static ERROR_SUB StepMotorRunStepsBaseParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunStepsCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //拷贝电机运行指令
    UserMemCopy((uint8_t*)&(motorDataSetPtr->stateSet.paramMotorRunSteps),(uint8_t*)paramPtr,DIM_STRUCT_LENGTH_BYTE(StepMotorRunStepsCmd));
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机使能
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //电机使能失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //设置电机运行状态
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RUN_STEPS;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RUN_STEPS_NORMAL;
    return ERROR_SUB_OK;
}


//运行启动
static ERROR_SUB StepMotorRunStepsBaseStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorRunStepsCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t steps = 0;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if(motorDataSetPtr->stateSet.targetCoordinate >= motorDataSetPtr->stateSet.currentMotorCoordinate)
    {
        //加载电机方向
        motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
        steps = motorDataSetPtr->stateSet.targetCoordinate - motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    else
    {
        //加载电机方向
        motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
        steps = motorDataSetPtr->stateSet.currentMotorCoordinate - motorDataSetPtr->stateSet.targetCoordinate;
    }
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
        motorDataSetPtr->stateSet.cmdFixSpd.stepCount = steps;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        //使用曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        //曲线计算
        StepMotorCurveCalcMainTableWithStep(motorDataSetPtr->stateSet.mainCurveTabDataPtr,&motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig],
                                                    paramPtr->speedRatioMode,paramPtr->speedRatio,steps);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.mainCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.mainCurveTabDataPtr->totalRunningSteps;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    return ERROR_SUB_OK;
}

//等待运行完成
static ERROR_SUB StepMotorRunStepsBaseWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
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
        if(motorDataSetPtr->stateSet.runningTimeMsCount > motorDataSetPtr->stateSet.paramMotorRunSteps.timeOutMsSet)
        {
            //超时跳出
            break;
        }
        //检查电机状态变化
        if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RUN_STEPS)
        {
            //检查是不是被外部停止了
            if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
            {
                //外部指令急停了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
            {
                //不期望的异常导致的急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
            {
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP))
            {
                //不期望的停止信号导致中断,急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_UNEXCEPT_SINGAL_EM_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP))
            {
                //不期望的停止信号导致中断,辅助急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_UNEXCEPT_SINGAL_UTIL_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_CMD_STOP_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_START_FIX_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_START_TAB_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_STOP))
            {
                //脉冲发送完成,正常停止
                return ERROR_SUB_OK;
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE))
            {
                //急停减速完成,正常停止
                return ERROR_SUB_OK;
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE))
            {
                //辅助急停减速完成,正常停止
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
                        MCU_RTT_SendStringInt("StepMotorRunStepsBaseWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                }
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
        }
        //这里可以插入编码器检测

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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //报错运行超时
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_TIME_OUT;
    //电机运行超时
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_TIME_OUT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunStepsBaseWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,MOTOR_SENSOR_FLAGS* sensorFlagsPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunStepsReflushMotorCoordinate(motorDataSetPtr);
    //检查电机是否触发信号
    sensorFlagsPtr->sensorEmStopFlag = RESET;
    sensorFlagsPtr->sensorUtilStopFlag = RESET;
    if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE))
    {
        //急停减速完成,正常停止
        sensorFlagsPtr->sensorEmStopFlag = SET;
    }
    else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE))
    {
        //辅助急停减速完成,正常停止
        sensorFlagsPtr->sensorUtilStopFlag = SET;
    }
    //运动完成,电机状态修改为空闲,便于下一次操作
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//编码器失步检测系数
static ERROR_SUB StepMotorRunStepsEncoderCorrect(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,uint16_t* encoderRetryCountPtr,uint8_t* breakFlagPtr)
{
    *breakFlagPtr = 0;
    //这里进行编码器检测修正
    if(motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == DISABLE)
    {
        //没有编码器,结束
        *breakFlagPtr = 1;
        return ERROR_SUB_OK;
    }
    else
    {
        //有编码器,检测当前坐标和目标坐标是否相等
        if(motorDataSetPtr->stateSet.targetCoordinate >= motorDataSetPtr->stateSet.currentMotorCoordinate)
        {
            if(motorDataSetPtr->stateSet.targetCoordinate - motorDataSetPtr->stateSet.currentMotorCoordinate >= 
                motorDataSetPtr->attriPtr->motorEncoderAttri.lossLimitRunning)
            {
                //丢步超过限制
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_ENCODER_LOSS_STEP_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if(motorDataSetPtr->stateSet.targetCoordinate - motorDataSetPtr->stateSet.currentMotorCoordinate <= 
                    motorDataSetPtr->attriPtr->motorEncoderAttri.passLimit)
            {
                //丢步在系统范围内,退出循环
                *breakFlagPtr = 1;
                return ERROR_SUB_OK;
            }
            else
            {
                //需要修正
                if((*encoderRetryCountPtr) >= motorDataSetPtr->attriPtr->motorEncoderAttri.encoderCorrectRetryMax)
                {
                    //超过修正次数,溢出
                    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_ENCODER_CORRECT_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
                }
                else
                {
                    //修正
                    (*encoderRetryCountPtr)  += 1;
                    //继续循环
                    *breakFlagPtr = 0;
                    return ERROR_SUB_OK;
                }
            }
        }
        else 
        {
            if(motorDataSetPtr->stateSet.currentMotorCoordinate - motorDataSetPtr->stateSet.targetCoordinate >= 
                motorDataSetPtr->attriPtr->motorEncoderAttri.lossLimitRunning)
            {
                //丢步超过限制
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_ENCODER_LOSS_STEP_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if(motorDataSetPtr->stateSet.currentMotorCoordinate - motorDataSetPtr->stateSet.targetCoordinate <= 
                    motorDataSetPtr->attriPtr->motorEncoderAttri.passLimit)
            {
                //丢步在系统范围内,退出循环
                *breakFlagPtr = 1;
                return ERROR_SUB_OK;
            }
            else
            {
                //需要修正
                if((*encoderRetryCountPtr)  >= motorDataSetPtr->attriPtr->motorEncoderAttri.encoderCorrectRetryMax)
                {
                    //超过修正次数,溢出
                    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_ENCODER_CORRECT_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
                }
                else
                {
                    //修正
                    (*encoderRetryCountPtr) += 1;
                    //继续循环
                    *breakFlagPtr = 0;
                    return ERROR_SUB_OK;
                }
            }
        }
    }
}

//运行完成的后置收尾
static ERROR_SUB StepMotorRunStepsMainWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,int32_t dirSwitchStepsRepair,MOTOR_SENSOR_FLAGS* sensorFlagsPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //更新电机当前坐标
    StepMotorRunStepsReflushMotorCoordinate(motorDataSetPtr);
    //运动完成,电机状态修改为空闲,便于下一次操作
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    //正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //任意急停信号触发,不处理坐标
    if((sensorFlagsPtr->sensorEmStopFlag == SET)||(sensorFlagsPtr->sensorUtilStopFlag == SET))
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        return ERROR_SUB_OK;
    }
    //换向修正步数
    if((motorDataSetPtr->stateSet.paramMotorRunSteps.dirSwitchCorrectSteps != 0)&&(dirSwitchStepsRepair != 0))
    {
        motorDataSetPtr->stateSet.currentMotorCoordinate += dirSwitchStepsRepair;
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorDataSetPtr->stateSet.currentMotorCoordinate;
    }
    //教学模式
    if(motorDataSetPtr->stateSet.teachMode == STEP_MOTOR_MODE_TEACH_ENABLE)
    {
        //电机失能
        errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
        if(errorSub != ERROR_SUB_OK)
        {
            //状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_DISABLE;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //电机使能失败
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RUN_STEPS_DISABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }   
    return ERROR_SUB_OK;
}

//电机走步数
ERROR_SUB StepMotorRunSteps(StepMotorRunStepsCmd* paramPtr,MOTOR_SENSOR_FLAGS* sensorFlagsPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    uint16_t encoderRetryCount = 0;
    uint8_t encoderCorrectBreakFlag = 0;
    //换向修正的恢复步数,换向修正增加了或者减少了一些步数,结束运行的时候靠这个修正回来
    int32_t dirSwitchStepsRepairStep = 0;
    //首先检查指令参数是否正确
    errorSub = StepMotorRunStepsBaseParamCheck(paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        return errorSub;
    }
    //获取状态指针
    motorDataSetPtr = StepMotorBaseGetDataSetPtr(paramPtr->motorNo);
    //申请互斥量
    errorSub = StepMotorBaseGetLock(paramPtr->motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //运行前状态检查
    errorSub = StepMotorRunStepsBaseBeforeStateCheck(paramPtr->motorNo,motorDataSetPtr,paramPtr,&dirSwitchStepsRepairStep);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //启动前各种准备
    errorSub = StepMotorRunStepsBaseParpare(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    //开始运转
    while(1)
    {
        //启动运转
        errorSub = StepMotorRunStepsBaseStart(paramPtr->motorNo,motorDataSetPtr,paramPtr);
        if(errorSub != ERROR_SUB_OK)
        {
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
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
        errorSub = StepMotorRunStepsBaseWaitOver(paramPtr->motorNo,motorDataSetPtr);
        if(errorSub != ERROR_SUB_OK)
        {
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
            //读取FPGA当前状态查看是否状态
            BoardFPGA_StepMotorGetReadOnlyReg(paramPtr->motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //更新坐标
            StepMotorRunStepsReflushMotorCoordinate(motorDataSetPtr);        
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
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //错误返回
            return errorSub;
        }
        //运行完成之后收尾
        errorSub = StepMotorRunStepsBaseWindUp(paramPtr->motorNo,motorDataSetPtr,sensorFlagsPtr);
        if(errorSub != ERROR_SUB_OK)
        {
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
            //释放互斥信号量
            StepMotorBaseReleaseLock(paramPtr->motorNo);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //错误返回
            return errorSub;
        }
        //任意急停信号触发,退出循环
        if((sensorFlagsPtr->sensorEmStopFlag == SET)||(sensorFlagsPtr->sensorUtilStopFlag == SET))
        {
            break;
        }
        //编码器修正检测
        errorSub = StepMotorRunStepsEncoderCorrect(paramPtr->motorNo,motorDataSetPtr,&encoderRetryCount,&encoderCorrectBreakFlag);
        if(errorSub != ERROR_SUB_OK)
        {
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
            //释放互斥信号量
            StepMotorBaseReleaseLock(paramPtr->motorNo);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //错误返回
            return errorSub;
        }
        if(encoderCorrectBreakFlag == 0)
        {
            continue;
        }
        //修正结束跳出循环
        break;
    }
    //换向修正步数处理以及教学模式处理
    errorSub = StepMotorRunStepsMainWindUp(paramPtr->motorNo,motorDataSetPtr,dirSwitchStepsRepairStep,sensorFlagsPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
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