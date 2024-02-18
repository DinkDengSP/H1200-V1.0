/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-11 10:59:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-19 11:58:43 +0800
************************************************************************************************/ 
#include "StepMotorMain.h"

//电机坐标更新
static void StepMotorReturnZeroReflushMotorCoordinate(STEP_MOTOR_DATA_SET* motorDataSetPtr)
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

//不支持的状态中断
static void StepMotorReturnZeroStateIntProcessNotSupport(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNotSupport BoardFPGA_StepMotorCmdStop Error!!\r\n");
        return;
    }
    else
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNotSupport STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE Error!!\r\n");
        return;
    }
}

//回零触发急停减速
static void StepMotorReturnZeroStateIntProcessDecEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP1 Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP2 Error!!\r\n");
        }
        return;
    }
}

//回零触发辅助急停减速
static void StepMotorReturnZeroStateIntProcessDecUtilStop1(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP 1 Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessDecUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP 2 Error!!\r\n");
        }
        return;
    }
}

//正常运转切换到急停减速过程
static void StepMotorReturnZeroStateIntProcessToEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    // 急停有效导致的停止,检查是否设置了急停检测,设置了急停那么进入减速,否则进入异常信号停止
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet == ENABLE)
    {
        // 查看接下来应该是急停还是减速
        if (motorDataSetPtr->attriPtr->motorRunAttri.decStepsEmStopTrig == 0)
        {
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //急停信号触发停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE 1 Error!!\r\n");
            }
            return;
        }
        else
        {
            // 减速停止,计算减速曲线,只有曲线才会减速,不是曲线直接停止
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID))
            {
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //急停信号触发停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE;
                //如果是匀速运转的时候遇到的急停,那么直接停下来并报错
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                }
                else
                {
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE 2 Error!!\r\n");
                }
                return;
            }
            else
            {
                // 加载电机只读寄存器状态
                BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                // 曲线减速
                motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                // 停止条件设定
                // 打开脉冲发送完成停止
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
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
                // 电机停止
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    // 直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    // 正常停止
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                    return;
                }
                // 更新电机当前坐标
                StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
                // 启动曲线运转,使用预写入
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                    return;
                }
                // 电机切入emstop减速
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
                //急停减速
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_EM_STOP;
                return;
            }
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
        }
        return;
    }
}

//正常运转切换到辅助急停减速过程
static void StepMotorReturnZeroStateIntProcessNormalToUtilStop1(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    // 辅助急停有效导致的停止,检查是否设置了辅助急停检测,设置了辅助急停那么进入减速,否则进入异常信号停止
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet == ENABLE)
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE Error!!\r\n");
            }
            return;
        }
        else
        {
            // 减速停止,计算减速曲线,只有曲线才会减速,不是曲线直接停止
            if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID))
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
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
                }
                else
                {
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE Error!!\r\n");
                }
                return;
            }
            else
            {
                // 加载电机只读寄存器状态
                BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
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
                // 电机停止
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    // 直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    // 正常停止
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                    return;
                }
                // 更新电机当前坐标
                StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
                // 启动曲线运转,使用预写入
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    // 错误打印
                    MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                    // 状态切入停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                    return;
                }
                // 电机切入emstop减速
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_UTIL_STOP;
                return;
            }
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToUtilStop1 STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP Error!!\r\n");
        }
        return;
    }
}

//第一次找原点,脉冲发送完成,没找到原点,切换继续慢速找原点
static void StepMotorReturnZeroStateIntProcessNormalToOriginFirstPluseOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //方向不变
    motorDataSetPtr->stateSet.currentDir = motorDataSetPtr->stateSet.currentDir;
    //匀速找原点
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    // 计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置为最大找原点步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
    //方向保持不变
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    // 停止条件设定
    // 打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //急停信号设置根据参数指定
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstPluseOver BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    // 电机切入第二次找原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    // 正常停止
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_SECOND;
    return;
}


//第一次找原点遇上复位传感器减速或者停止
static void StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    // 减速停止
    if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID))
    {
        //之前是定频运转,此时可以直接切换运转完成
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        //读取原点传感器状态,防止错误触发
        if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
        {
            //传感器有效,切换正常完成
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
            //方向切换为反向
            motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
            return;
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
            //传感器无效,传感器异常触发
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //复位传感器伪触发
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
            return;
        }
    }
    else
    {
        //之前不是定频运转而是曲线运转,读取原点传感器状态,防止错误触发
        if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_INVALID)
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
            //传感器无效,传感器异常触发
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //复位传感器伪触发
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
            return;
        }
        //这个时候判定是否设置了回零复位功能,没设置回零复位,直接停止,否则就要经历复杂的减速回来各项转换
        if(motorDataSetPtr->stateSet.paramMotorReturnZero.resetFlag == DISABLE)
        {
            //没有设置复位功能,直接停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            //传感器有效,切换正常完成
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
            //方向切换为正向
            motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
            return;
        }
        else//切换减速
        {
            //设置了复位功能且之前是曲线运转,现在可以减速了
            motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
            //方向保持不变
            motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
            // 停止条件设定
            // 打开脉冲发送完成停止
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
            //打开传感器无效停止,减速的时候如果遇上传感器无效,代表脱离了传感器有效区间,必须报错
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
            //关闭传感器有效停止
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
            //急停信号设置根据参数指定
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
            // 减速曲线计算,步数为复位减速步数
            StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr, motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                                motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent,
                                                                motorDataSetPtr->attriPtr->motorRunAttri.resetDecelerationSteps);
            // 曲线设置
            motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
            motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
            motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
            motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
            // 电机停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            // 更新电机当前坐标
            StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
            // 启动曲线运转,使用预写入
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                return;
            }
            // 电机切入减速
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_DEC;
            return;
        }
    }
}

//回零向原点运动,第一次快速运动
static void StepMotorReturnZeroStateIntProcessToOriginFirst(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,没找到原点,切换继续慢速找原点
            StepMotorReturnZeroStateIntProcessNormalToOriginFirstPluseOver(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //遇上复位传感器有效,切换减速过程
            StepMotorReturnZeroStateIntProcessNormalToOriginFirstResetSensorValid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginFirst BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginFirst STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginFirst BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginFirst STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//回零运动过程中没找到原点,慢速找原点
static void StepMotorReturnZeroStateIntProcessToOriginSecond(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,没找到原点,报错
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //脉冲发送完成没找到原点
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //检测到了复位传感器,加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            //再次检查原点防止原点误触发
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_INVALID)
            {
                //传感器无效,传感器异常触发
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //复位传感器伪触发
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
                return;
            }
            //找到了原点,停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
            //方向切换为正向
            motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginSecond STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//减速脉冲发送完成,电机停止之后反向脱离原点
static void StepMotorReturnZeroStateIntProcessNormalToOriginDecPluseOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //回到触发原点的位置
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    // 计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置为最大找原点步数加上复位换向补偿步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDecelerationSteps + 
                                                        motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位传感器无效停止,当脱离传感器的时候停下来
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //依靠参数设置运行时停止信号
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    //切换方向
    motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalToOriginDecPluseOver BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    // 电机切入慢速找原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_BACK;
    return;
}

//回零运动过程中撞击到原点,切换慢速停下来
static void StepMotorReturnZeroStateIntProcessToOriginDec(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //慢速停下来之后切换方向到脱离原点
            StepMotorReturnZeroStateIntProcessNormalToOriginDecPluseOver(motorNo,motorDataSetPtr,motorState,stopReason);
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //减速过程中原点失效,复位减速长度超过限制
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //未知状态下收到了电机中断
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS Error!!\r\n");
            }
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//退回到传感器无效位置,再次找原点
static void StepMotorReturnZeroStateIntProcessToOriginBackSensorInvalid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //回到触发原点的位置
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    // 计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置复位换向补偿步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //依靠参数设置运行时停止信号
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    //切换方向
    motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 电机停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBackSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
        // 直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 正常停止
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
        return;
    }
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //读取原点传感器状态,防止错误触发
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
    {
        //传感器有效,传感器伪消失
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //复位传感器伪触发
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP;
        return;
    }
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBackSensorInvalid BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    // 电机切入回原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_RETURN;
    return;
}


//回零运动过程中撞击到原点,停下来之后离开原点
static void StepMotorReturnZeroStateIntProcessToOriginBack(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //错误停止,脱离未检测到脱离信号
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN;
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //退回到传感器无效位置,再次找原点
            StepMotorReturnZeroStateIntProcessToOriginBackSensorInvalid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginBack STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//重新回到触发原点的地方
static void StepMotorReturnZeroStateIntProcessToOriginReturn(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //错误停止,没找到原点信号
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //找到了传感器,电机可以停止并且回零完成
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //读取原点传感器状态,防止错误触发
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
            {
                //传感器有效,切换正常完成
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
                //方向切换为正向
                motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
                return;
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
                //传感器无效,传感器异常触发
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //复位传感器伪触发
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
                return;
            }
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}


//脱离原点,脉冲发送完成,脱离原点,切换继续慢速慢速脱离原点
static void StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstPluseOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //匀速慢速脱离原点
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    // 计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置为最大慢速脱离原点步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    // 停止条件设定
    // 打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //根据参数打开相应的急停
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    //方向保持不变
    motorDataSetPtr->stateSet.currentDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstPluseOver BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    //电机尝试第二次脱离原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_SECOND;
    return;
}


//遇上复位传感器无效,此时已经脱离了远点,根据配置停止或者切换减速过程
static void StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
        // 直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 正常停止
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
        return;
    }
    //读取原点传感器状态,防止错误触发
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP Error!!\r\n");
        //传感器有效,传感器异常触发
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //复位传感器伪消失
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP;
        return;
    }
    // 减速停止
    if ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID) || (stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID))
    {
        //之前是定频运转,此时不需要减速,切换慢速返回原点
        motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
        // 计算速度比率
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
        {
            if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
            }
            else
            {
                motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
            }
        }
        //步数设置复位换向补偿步数
        motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //打开复位传感器有效停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
        //关闭复位传感器无效停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        //依靠参数设置运行时停止信号
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
        //切换方向
        motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 更新电机当前坐标
        StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
        // 启动定频运转
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
            return;
        }
        // 电机切入返回原点
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN;
        return;
    }
    else
    {
        //之前是曲线运转,检测是否设置复位,设置复位了,减速,否则直接切入return
        if(motorDataSetPtr->stateSet.paramMotorReturnZero.resetFlag == DISABLE)
        {
            //没设置复位就不需要减速,直接切换回到原点
            motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
            // 计算速度比率
            if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
            {
                if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
                {
                    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
                }
                else
                {
                    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
                }
            }
            //步数设置复位换向补偿步数
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
            motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
            //停止条件设定
            //打开脉冲发送完成停止
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
            //打开复位传感器有效停止
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
            //关闭复位传感器无效停止
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
            //依靠参数设置运行时停止信号
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
            //切换方向
            motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
            motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            // 更新电机当前坐标
            StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
            // 启动定频运转
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                return;
            }
            // 电机切入回原点
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN;
            return;
        }
        else
        {
            //切入减速
            motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
            //方向不变
            motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
            //停止条件设定
            //打开脉冲发送完成停止
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
            //关闭复位传感器检测
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
            //关闭传感器有效检测
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
            //依靠参数设置运行时停止信号
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
            // 减速曲线计算,步数为复位减速步数
            StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr, motorDataSetPtr->stateSet.utilCurveTabDataPtr,
                                                                motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.tabIndexCurrent,
                                                                motorDataSetPtr->attriPtr->motorRunAttri.resetDecelerationSteps);
            // 曲线设置
            motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->spdTableArray;
            motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.utilCurveTabDataPtr->stpTableArray;
            motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.utilCurveTabDataPtr->tabLengthAfterCalc;
            motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.utilCurveTabDataPtr->totalRunningSteps;
            // 更新电机当前坐标
            StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
            // 启动曲线运转,使用预写入
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                return;
            }
            // 电机切入减速
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
            //电机减速
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_DEC;
            return;
        }
    }
}

//回零离开原点运动,第一次快速离开远点
static void StepMotorReturnZeroStateIntProcessLeaveOriginFirst(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,没找到原点,切换继续慢速找原点
            StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstPluseOver(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //遇上复位传感器无效,切换减速过程
            StepMotorReturnZeroStateIntProcessNormalLeaveOriginFirstResetSensorInvalid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginFirst BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginFirst STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessToOriginFirst BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginFirst STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//第二次脱离原点的时候检测到原点无效,切换回到触发原点的位置
static void StepMotorReturnZeroStateIntProcessLeaveOriginSecondSensorInvalid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecondSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
        // 直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 正常停止
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
        return;
    }
    //读取原点传感器状态,防止错误触发
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecondSensorInvalid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP Error!!\r\n");
        //传感器有效,传感器异常触发
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //复位传感器伪消失
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP;
        return;
    }
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    //计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置复位换向补偿步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //依靠参数设置运行时停止信号
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    //切换方向
    motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecondSensorInvalid BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    // 电机切入回原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN;
    return;
}

//回零离开原点运动,回零过程中没有完全离开原点,再次尝试找原点
static void StepMotorReturnZeroStateIntProcessLeaveOriginSecond(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,没脱离原点
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //脉冲发送完成离开远点
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //第二次脱离原点的时候检测到原点无效,切换回到触发原点的位置
            StepMotorReturnZeroStateIntProcessLeaveOriginSecondSensorInvalid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginSecond STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//慢速停下来之后切换方向回到刚触发原点的地方
static void StepMotorReturnZeroStateIntProcessLeaveOriginDecPluseOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    // 加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机停止
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDecPluseOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
        // 直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        // 正常停止
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
        return;
    }
    //读取原点传感器状态,防止错误触发
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
    {
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDecPluseOver STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP Error!!\r\n");
        //传感器有效,传感器异常触发
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //复位传感器伪消失
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP;
        return;
    }
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[motorDataSetPtr->stateSet.paramMotorReturnZero.selectCurveConfig].startRpm;
    // 计算速度比率
    if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio != 1.0f)
    {
        if (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (motorDataSetPtr->stateSet.paramMotorReturnZero.speedRatio));
        }
    }
    //步数设置复位换向补偿步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDecelerationSteps + 
                                                        motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //依靠参数设置运行时停止信号
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = motorDataSetPtr->stateSet.paramMotorReturnZero.utilStopSensorSet;
    //切换方向
    motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 更新电机当前坐标
    StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        // 错误打印
        MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDecPluseOver BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
        // 状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
        return;
    }
    // 电机切入回原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN;
    return;
}

//减速完成,再次去撞原点
static void StepMotorReturnZeroStateIntProcessLeaveOriginDec(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //慢速停下来之后切换方向回到刚触发原点的地方
            StepMotorReturnZeroStateIntProcessLeaveOriginDecPluseOver(motorNo,motorDataSetPtr,motorState,stopReason);
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //减速过程中传感器又突然有效了
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //脱离原点减速的过程中传感器突然又有效了
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG Error!!\r\n");
            }
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//回零离开原点减速停止之后再次靠近原点
static void StepMotorReturnZeroStateIntProcessLeaveOriginReturn(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,没找到原点,报错
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //脉冲发送完成没找到原点
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorReturnZeroStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_UTIL_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_UTIL_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_UTIL_STOP_SENSOR_VALID))
        {
            //正常运行切辅助急停
            StepMotorReturnZeroStateIntProcessNormalToUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //找到了传感器,电机可以停止并且回零完成
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
                // 直接停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                // 正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                return;
            }
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //读取原点传感器状态,防止错误触发
            if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
            {
                //传感器有效,切换正常完成
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //正常停止
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
                //方向切换为正向
                motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
                return;
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
                //传感器无效,传感器异常触发
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                //复位传感器伪触发
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
                return;
            }
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
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorReturnZeroStateIntProcessLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//回零状态下的中断处理
void StepMotorReturnZeroIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //保存电机中断代码
    motorDataSetPtr->stateSet.motorStateFpgaInt = motorState;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = stopReason;
    switch(motorDataSetPtr->stateSet.subState)
    {
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_FIRST:
            //回零向原点运动,第一次快速运动
            StepMotorReturnZeroStateIntProcessToOriginFirst(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_SECOND:
            //回零运动过程中没找到原点,慢速找原点
            StepMotorReturnZeroStateIntProcessToOriginSecond(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_DEC:
            //回零运动过程中撞击到原点,切换慢速停下来
            StepMotorReturnZeroStateIntProcessToOriginDec(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_BACK:
            //回零运动过程中撞击到原点,停下来之后离开原点
            StepMotorReturnZeroStateIntProcessToOriginBack(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_RETURN:
            //回零运动过程中撞击到原点,停下来之后离开原点然后再次回到原点
            StepMotorReturnZeroStateIntProcessToOriginReturn(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_FIRST:
            //回零离开原点运动,第一次快速离开远点
            StepMotorReturnZeroStateIntProcessLeaveOriginFirst(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_SECOND:
            //回零离开原点运动,回零过程中没有完全离开原点,再次尝试找原点
            StepMotorReturnZeroStateIntProcessLeaveOriginSecond(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_DEC:
            //回零离开原点运动中撞击到原点,切换减速停止
            StepMotorReturnZeroStateIntProcessLeaveOriginDec(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_RETURN:
            //回零离开原点减速停止之后再次靠近原点
            StepMotorReturnZeroStateIntProcessLeaveOriginReturn(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_EM_STOP:
            //回零触发急停减速
            StepMotorReturnZeroStateIntProcessDecEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        case STEP_MOTOR_STATE_SUB_RETURN_ZERO_DEC_UTIL_STOP:
            //回零触发辅助急停减速
            StepMotorReturnZeroStateIntProcessDecUtilStop1(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
        default:
            //不支持的状态中断
            StepMotorReturnZeroStateIntProcessNotSupport(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
    }
}

//参数检查
static ERROR_SUB StepMotorReturnZeroBaseParamCheck(StepMotorReturnZeroCmd* paramPtr)
{
    if(paramPtr == NULL)
    {
        //错误打印,参数为空
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RETURN_ZERO_PARAM_NULL);
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RETURN_ZERO_PARAM_NULL;
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
    return ERROR_SUB_OK;
}


//运行前检查电机状态是否正确
static ERROR_SUB StepMotorReturnZeroBaseBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)
    {
        //电机不是空闲状态,不能运转
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_MOTOR_NOT_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //检查是否设置了急停,设置急停的话就需要配置急停引脚
    if((paramPtr->emStopSensorSet == ENABLE)&&(motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet == DISABLE))
    {
        //电机设置了急停但是急停引脚未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_EM_STOP_ENABLE_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //检查是否设置了辅助急停,设置辅助急停的话就需要配置辅助急停引脚
    if((paramPtr->utilStopSensorSet == ENABLE)&&(motorDataSetPtr->attriPtr->motorSensorAttri.utilStopSensorSet == DISABLE))
    {
        //电机设置了辅助急停但是辅助急停引脚未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_UTIL_STOP_ENABLE_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //带编码器的情况下不能带编码器修正系数
    if((motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == ENABLE)&&(paramPtr->dirSwitchCorrectSteps != 0))
    {
        //带编码器的情况下不能带正反向修正系数
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_DIR_SWITCH_FIX_ENCODER_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
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
    motorDataSetPtr->stateSet.targetCoordinate = paramPtr->correctionCoordinate;
    //这个地方可以加入正反向修正
    if(paramPtr->dirSwitchCorrectSteps != 0)
    {
        //查看是否需要修正
        if(motorDataSetPtr->attriPtr->motorRunAttri.dirSwitchFixMode == STEP_MOTOR_DIR_SWITCH_FIX_MODE_NONE)
        {
            //电机未设置修正模式但是给了修正系数
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_DIR_SWITCH_FIX_SET_NONE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps != 0)&&
        ((motorDataSetPtr->stateSet.targetCoordinate) > motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps))
    {
        //电机超正向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_POSITIVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps != 0)&&
        ((motorDataSetPtr->stateSet.targetCoordinate) < motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps))
    {
        //电机超正向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_RESERVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //启动之间检测急停信号,启动前就有效的直接报错
    if((paramPtr->emStopSensorSet == ENABLE)&&
        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.emStopSensorState == PIN_STATE_VALID))
    {
        //启动前检测到急停有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_EM_STOP_VALID_BEFORE_START + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //启动之间检测辅助急停信号,启动前就有效的直接报错
    if((paramPtr->utilStopSensorSet == ENABLE)&&
        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.utilStopSensorState == PIN_STATE_VALID))
    {
        //启动前检测到辅助急停有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_UTIL_STOP_VALID_BEFORE_START + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //没装复位传感器却设置了复位修正
    if((paramPtr->correctionCoordinate != 0)&&(motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorSet == DISABLE))
    {
        //启动前检测到辅助急停2有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_RESET_SENSOR_NO_WITH_CORRECT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorReturnZeroBaseParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //退出教学模式
    motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
    //拷贝电机运行指令
    UserMemCopy((uint8_t*)&(motorDataSetPtr->stateSet.paramMotorReturnZero),(uint8_t*)paramPtr,DIM_STRUCT_LENGTH_BYTE(StepMotorReturnZeroCmd));
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机使能
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //电机使能失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//进入第一段,快速找原点
static ERROR_SUB StepMotorReturnZeroBaseStartFirstToOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //方向为反向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].maxRpm)
    {
        //定频运转
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
        //启动运转,步数直接给最高
        if(motorDataSetPtr->stateSet.currentMotorCoordinate > motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor)
        {
            //当坐标大于找原点最大步数的时候,使用找原点最大步数
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->stateSet.currentMotorCoordinate;
        }
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //打开复位信号检测到停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
        //关闭复位信号接收到停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        //中断信号看参数配置
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        //进入快速找原点模式
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_FIRST;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        uint32_t utilSteps = 0;
        if(motorDataSetPtr->stateSet.currentMotorCoordinate > motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor)
        {
            //当坐标大于找原点最大步数的时候,使用找原点最大步数
            utilSteps = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
        }
        else
        {
            utilSteps = motorDataSetPtr->stateSet.currentMotorCoordinate;
        }
        //曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        //脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //打开复位信号检测到停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
        //关闭复位信号接收到停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        //中断信号看参数配置
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        //曲线计算
        StepMotorCurveCalcMainTableWithStep(motorDataSetPtr->stateSet.mainCurveTabDataPtr,&motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig],
                                                    paramPtr->speedRatioMode,paramPtr->speedRatio,utilSteps);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.mainCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.mainCurveTabDataPtr->totalRunningSteps;
        //进入快速找原点模式
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_FIRST;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    return ERROR_SUB_OK;
}

//进入第二段,慢速找原点
static ERROR_SUB StepMotorReturnZeroBaseStartSecondToOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //方向为反向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm;
    // 计算速度比率
    if (paramPtr->speedRatio != 1.0f)
    {
        if (paramPtr->speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= paramPtr->speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (paramPtr->speedRatio));
        }
    }
    //步数设置为最大找原点步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    // 停止条件设定
    // 打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //急停信号设置根据参数指定
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
    //方向保持不变
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 电机切入第二次找原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_TO_ORIGIN_SECOND;
    // 启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//进入第一段,快速离开远点
static ERROR_SUB StepMotorReturnZeroBaseStartFirstLeaveOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //方向为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    if(motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm >= 
        motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].maxRpm)
    {
        //定频运转
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
        //启动运转,步数直接给最高
        if((0 - motorDataSetPtr->stateSet.currentMotorCoordinate) > motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin)
        {
            //当坐标大于找原点最大步数的时候,使用找原点最大步数
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = (uint32_t)(0 - motorDataSetPtr->stateSet.currentMotorCoordinate);
        }
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        //打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //关闭复位信号检测到停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        //打开复位信号消失停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
        //中断信号看参数配置
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        //进入快速离开原点模式
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_FIRST;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    else
    {
        uint32_t utilSteps = 0;
        if((0 - motorDataSetPtr->stateSet.currentMotorCoordinate) > motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin)
        {
            //当坐标大于找原点最大步数的时候,使用找原点最大步数
            utilSteps = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
        }
        else
        {
            utilSteps = (0 - motorDataSetPtr->stateSet.currentMotorCoordinate);
        }
        //曲线运转
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        //停止条件设定
        //脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //关闭复位信号检测到停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        //打开复位信号消失停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
        //中断信号看参数配置
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
        //曲线计算
        StepMotorCurveCalcMainTableWithStep(motorDataSetPtr->stateSet.mainCurveTabDataPtr,&motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig],
                                                    paramPtr->speedRatioMode,paramPtr->speedRatio,utilSteps);
        //曲线设置
        motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->spdTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->stpTableArray;
        motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.mainCurveTabDataPtr->tabLengthAfterCalc;
        motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.mainCurveTabDataPtr->totalRunningSteps;
        //进入快速离开原点模式
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_FIRST;
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
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
        }
    }
    return ERROR_SUB_OK;
}

//进入第二段,慢速离开原点
static ERROR_SUB StepMotorReturnZeroBaseStartSecondLeaveOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //方向为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorCurveAttriSet[paramPtr->selectCurveConfig].startRpm;
    // 计算速度比率
    if (paramPtr->speedRatio != 1.0f)
    {
        if (paramPtr->speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet *= paramPtr->speedRatio;
        }
        else
        {
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet += ((motorDataSetPtr->stateSet.cmdFixSpd.rpmSet) * (paramPtr->speedRatio));
        }
    }
    //步数设置为最大找原点步数
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    // 停止条件设定
    // 打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位传感器有效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位传感器无效停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //急停信号设置根据参数指定
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = paramPtr->emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = paramPtr->utilStopSensorSet;
    //方向保持不变
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    // 电机切入第二次找原点
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RETURN_ZERO;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RETURN_ZERO_LEAVE_ORIGIN_SECOND;
    //启动定频运转
    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
    {
        //状态切入停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX;
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //电机控制失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}


//运行启动
static ERROR_SUB StepMotorReturnZeroBaseStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检测原点的情况决定运行模式
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_INVALID)
    {
        //此时原点传感器无效,要找传感器
        if(motorDataSetPtr->stateSet.currentMotorCoordinate > 0)
        {
            //进入第一段,快速找原点
            errorSub = StepMotorReturnZeroBaseStartFirstToOrigin(motorNo,motorDataSetPtr,paramPtr);
            if(errorSub != ERROR_SUB_OK)
            {
                //启动失败
                return errorSub;
            }
        }
        else
        {
            //进入第二段,慢速找原点
            errorSub = StepMotorReturnZeroBaseStartSecondToOrigin(motorNo,motorDataSetPtr,paramPtr);
            if(errorSub != ERROR_SUB_OK)
            {
                //启动失败
                return errorSub;
            }
        }
    }
    else 
    {
        //此时原点传感器有效,要脱离传感器
        if(motorDataSetPtr->stateSet.currentMotorCoordinate < 0)
        {
            //进入第一段,快速离开远点
            errorSub = StepMotorReturnZeroBaseStartFirstLeaveOrigin(motorNo,motorDataSetPtr,paramPtr);
            if(errorSub != ERROR_SUB_OK)
            {
                //启动失败
                return errorSub;
            }
        }
        else
        {
            //进入第二段,慢速离开原点
            errorSub = StepMotorReturnZeroBaseStartSecondLeaveOrigin(motorNo,motorDataSetPtr,paramPtr);
            if(errorSub != ERROR_SUB_OK)
            {
                //启动失败
                return errorSub;
            }
        }
    }
    return ERROR_SUB_OK;
}


//等待运行完成
static ERROR_SUB StepMotorReturnZeroBaseWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
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
        if(motorDataSetPtr->stateSet.runningTimeMsCount > motorDataSetPtr->stateSet.paramMotorReturnZero.timeOutMsSet)
        {
            //超时跳出
            break;
        }
        //检查电机状态变化
        if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RETURN_ZERO)
        {
            //检查是不是被外部停止了
            if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
            {
                //外部指令急停了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
            {
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
            {
                //不期望的异常导致的急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE))
            {
                //急停减速完成,失败
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_SINGAL_EM_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UTIL_STOP_SINGLE))
            {
                //辅助急停减速完成,失败
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_SINGAL_UTIL_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_CMD_STOP_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_TAB_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP))
            {
                //不期望的停止信号导致中断,急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_UNEXCEPT_SINGAL_EM_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_UTIL_STOP))
            {
                //不期望的停止信号导致中断,辅助急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_UNEXCEPT_SINGAL_UTIL_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG))
            {
                //复位传感器伪触发,出发后再去检测信号消失,传感器不稳定
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_RESET_SENSOR_DUMMY_TRIG + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN))
            {
                //脉冲发送完成没找到原点
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_PLUSE_OVER_NOT_FIND_ORIGIN + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS))
            {
                //脉冲发送完成没找到原点
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_ORIGIN_DEC_LOSS + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP))
            {
                //检测到传感器信号无效并停止后再检测传感器信号变成有效,传感器不稳定
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_RESET_SENSOR_DUMMY_DISAPP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN))
            {
                //离开原点时脉冲发送完成也没能脱离原点,检查最大行程或者传感器状态,可能电机卡住了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_PLUSE_OVER_NO_LEAVE_ORIGIN + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG))
            {
                //脱离原点减速的过程中原点传感器有变得有效了,检查传感器或电机,可能电机缺相或传感器故障
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_ORIGIN_DEC_TRIG + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_NONE))
            {
                //正常停止
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
                        MCU_LogSendString("StepMotorReturnZeroBaseWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_LogSendString("StepMotorReturnZeroBaseWaitOver STEP_MOTOR_STATE_SUB_STOP_STATE_ERR Error!!\r\n");
                    }
                }
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    MCU_LogSendString("StepMotorReturnZeroBaseWaitOver STEP_MOTOR_STATE_SUB_STOP_TIME_OUT Error!!\r\n");
    //报错运行超时
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_TIME_OUT;
    //电机运行超时
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_TIME_OUT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorReturnZeroBaseWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_IDLE;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
    //清空坐标
    motorDataSetPtr->stateSet.baseMotorCoordinate = 0;
    motorDataSetPtr->stateSet.currentMotorCoordinate = 0;
    //清空中断错误代码
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //不管编码器是否使能,清空编码器序号
    errorSub = BoardFPGA_StepMotorEncoderPosClear(motorNo);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(motorNo);
        MCU_LogShowErrorSubCode(errorSub);
        //编码器计数清零错误
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_ENCODER_POS_CLEAR_FAIL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载FPGA寄存器状态
    BoardFPGA_StepMotorGetAllReg(motorNo,(motorDataSetPtr->stateSet.fpgaRegSetPtr));
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//修正步数运转
static ERROR_SUB StepMotorReturnZeroRunCorrect(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorReturnZeroCmd* paramPtr)
{
    //不需要修正
    if(paramPtr->correctionCoordinate == 0)
    {
        return ERROR_SUB_OK;
    }
    MOTOR_SENSOR_FLAGS sensorFlags;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.motorNo = motorNo;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.emStopSensorSet = paramPtr->emStopSensorSet;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.utilStopSensorSet = paramPtr->utilStopSensorSet;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.targetCoordinate = paramPtr->correctionCoordinate;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.selectCurveConfig = paramPtr->selectCurveConfig;

    motorDataSetPtr->stateSet.paramMotorRunCoordinate.dirSwitchCorrectSteps = paramPtr->dirSwitchCorrectSteps;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.speedRatio = paramPtr->speedRatio;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.speedRatioMode = paramPtr->speedRatioMode;
    uint32_t timeUsed = tx_time_get() - motorDataSetPtr->stateSet.startTimeTick;
    //超时时间需要减去已经用的时间
    if(paramPtr->timeOutMsSet > timeUsed)
    {
        motorDataSetPtr->stateSet.paramMotorRunCoordinate.timeOutMsSet = paramPtr->timeOutMsSet - timeUsed;
    }
    else
    {
        //已经溢出了,但是还是给一个100MS
        motorDataSetPtr->stateSet.paramMotorRunCoordinate.timeOutMsSet = 100;
    }
    //走坐标
    return StepMotorRunCoordinate(&(motorDataSetPtr->stateSet.paramMotorRunCoordinate),&sensorFlags);
}


//电机回零
ERROR_SUB StepMotorReturnZero(StepMotorReturnZeroCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //首先检查指令参数是否正确
    errorSub = StepMotorReturnZeroBaseParamCheck(paramPtr);
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
    errorSub = StepMotorReturnZeroBaseBeforeStateCheck(paramPtr->motorNo,motorDataSetPtr,paramPtr);
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
    errorSub = StepMotorReturnZeroBaseParpare(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //检查是否存在原点传感器,如果不存在,那么直接将电机当前坐标设置为0,并且返回
    if(motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorSet == DISABLE)
    {
        //强制进入初始化状态
        errorSub = StepMotorMainForceToDefaultState(paramPtr->motorNo);
        if(errorSub != ERROR_SUB_OK)
        {
            //释放互斥信号量
            StepMotorBaseReleaseLock(paramPtr->motorNo);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //错误返回
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_FORCE_TO_DEFAULT + (ERROR_SUB)(paramPtr->motorNo));
        }
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //返回
        return ERROR_SUB_OK;
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    //启动运转
    errorSub = StepMotorReturnZeroBaseStart(paramPtr->motorNo,motorDataSetPtr,paramPtr);
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
    errorSub = StepMotorReturnZeroBaseWaitOver(paramPtr->motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //读取FPGA当前状态查看是否状态
        BoardFPGA_StepMotorGetReadOnlyReg(paramPtr->motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
        //更新坐标
        StepMotorReturnZeroReflushMotorCoordinate(motorDataSetPtr);        
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
    errorSub = StepMotorReturnZeroBaseWindUp(paramPtr->motorNo,motorDataSetPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return errorSub;
    }
    //走到指定修正坐标
    errorSub = StepMotorReturnZeroRunCorrect(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RETURN_ZERO_RUN_CORRECT + (ERROR_SUB)(paramPtr->motorNo));;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(paramPtr->motorNo);
    //返回
    return ERROR_SUB_OK;
}
