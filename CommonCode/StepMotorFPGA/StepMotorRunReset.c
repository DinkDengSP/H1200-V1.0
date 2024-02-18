/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:17:04 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-19 17:23:17 +0800
************************************************************************************************/ 
#include "StepMotorMain.h"

//复位不支持的状态的处理
static void StepMotorResetStateIntProcessSubUnSupport(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubUnSupport BoardFPGA_StepMotorCmdStop Error!!\r\n");
        return;
    }
    else
    {
        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubUnSupport STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE Error!!\r\n");
        return;
    }
}

//复位触发急停减速
static void StepMotorResetStateIntProcessSubDecEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessDecEmStop STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//正常运转切换到急停减速过程
static void StepMotorResetStateIntProcessToEmStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    // 急停有效导致的停止,检查是否设置了急停检测,设置了急停那么进入减速,否则进入异常信号停止
    if (motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet == ENABLE)
    {
        // 查看接下来应该是急停还是减速
        if (motorDataSetPtr->attriPtr->motorRunAttri.decStepsEmStopTrig == 0)
        {
            //直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //急停信号触发停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 电机切入指令停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
            }
            return;
        }
        else
        {
            //查看当前状态,如果是快速回零或者快速离开远点,那么切换减速
            if((motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN)&&(motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor == STEP_MOTOR_RESET_MODE_FIX_FAST))
            {
                //定频快速找远点
                //查看停止标记,必须是定频遇到急停,其他情况是不识别的异常
                if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID))
                {
                    //切换减速,从高速找原点切换到低速停止
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 曲线减速
                    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                    // 方向不变
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                    // 细分设定
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                    // 停止条件设定
                    // 打开脉冲发送完成停止
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                    //关闭其他的中断
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                    //关闭停止信号传感器检测
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                    //从快速定频到慢速定频的减速曲线计算
                    StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(motorDataSetPtr->stateSet.utilCurveTabDataPtr,motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast,
                                                                    motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow,
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
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                        // 直接停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        // 正常停止
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                        return;
                    }
                    // 启动曲线运转,使用预写入
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        // 错误打印
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                        // 状态切入停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                        return;
                    }
                    //电机切入emstop减速
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP;
                    return;
                }
                else
                {
                    //不正确的状态,直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
                    // 加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 电机切入指令停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE Error!!\r\n");
                    }
                    return;
                }
            }
            else if((motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN)&&(motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor == STEP_MOTOR_RESET_MODE_CURVE))
            {
                //曲线快速找原点
                //查看停止标记,必须是曲线遇到急停,其他情况是不识别的异常
                if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
                {
                    //切换减速
                    //切换减速,从复位曲线到慢速速度
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 曲线减速
                    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                    // 方向不变
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                    // 细分设定
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                    // 停止条件设定
                    // 打开脉冲发送完成停止
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                    //关闭其他的中断
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                    //关闭停止信号传感器检测
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                    //从曲线运转到快速停止
                    StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
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
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                        // 直接停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        // 正常停止
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                        return;
                    }
                    // 启动曲线运转,使用预写入
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        // 错误打印
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                        // 状态切入停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                        return;
                    }
                    //电机切入emstop减速
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP;
                    return;
                }
                else
                {
                    //不正确的状态,直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
                    // 加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 电机切入指令停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE Error!!\r\n");
                    }
                    return;
                }
            }
            else if((motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN)&&(motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor == STEP_MOTOR_RESET_MODE_FIX_FAST))
            {
                //定频快速离开原点
                //查看停止标记,必须是定频遇到急停,其他情况是不识别的异常
                if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID))
                {
                    //切换减速
                    //切换减速,从高速找原点切换到低速停止
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 曲线减速
                    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                    // 方向不变
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                    // 细分设定
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                    // 停止条件设定
                    // 打开脉冲发送完成停止
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                    //关闭其他的中断
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                    //关闭停止信号传感器检测
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                    //从快速定频到慢速定频的减速曲线计算
                    StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(motorDataSetPtr->stateSet.utilCurveTabDataPtr,motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast,
                                                                    motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow,
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
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                        // 直接停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        // 正常停止
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                        return;
                    }
                    // 启动曲线运转,使用预写入
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        // 错误打印
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                        // 状态切入停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                        return;
                    }
                    //电机切入emstop减速
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP;
                    return;
                }
                else
                {
                    //不正确的状态,直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
                    // 加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 电机切入指令停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE Error!!\r\n");
                    }
                    return;
                }
            }
            else if((motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN)&&(motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor == STEP_MOTOR_RESET_MODE_CURVE))
            {
                //曲线快速离开原点
                //查看停止标记,必须是曲线遇到急停,其他情况是不识别的异常
                if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
                {
                    //切换减速
                    //切换减速,从复位曲线到慢速速度
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 曲线减速
                    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
                    // 方向不变
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
                    // 细分设定
                    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
                    // 停止条件设定
                    // 打开脉冲发送完成停止
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
                    //关闭其他的中断
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
                    //关闭停止信号传感器检测
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = DISABLE;
                    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
                    //从曲线运转到快速停止
                    StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
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
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                        // 直接停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        // 正常停止
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
                        return;
                    }
                    // 启动曲线运转,使用预写入
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        // 错误打印
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
                        // 状态切入停止
                        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
                        return;
                    }
                    //电机切入emstop减速
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP;
                    return;
                }
                else
                {
                    //不正确的状态,直接停止
                    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE;
                    // 加载电机只读寄存器状态
                    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
                    // 电机切入指令停止
                    motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &motorStateReg);
                    if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE Error!!\r\n");
                    }
                    return;
                }
            }
            else
            {
                //其余低速运动,不需要减速
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP;
                //如果是匀速运转的时候遇到的急停,那么直接停下来并报错
                motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
                if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
                {
                    MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
                }
                else
                {
                    MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
                }
                return;
            }
        }
    }
    else
    {
        //没配置急停信号
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP;
        //如果是匀速运转的时候遇到的急停,那么直接停下来并报错
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessToEmStop STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP Error!!\r\n");
        }
        return;
    }
}

//遇上复位传感器有效,切换减速过程或者直接停止
static void StepMotorResetStateIntProcessSubToOriginResetSensorValid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检查传感器信号是否伪触发
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_INVALID)
    {
        //传感器信号伪触发
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        //复位传感器伪触发
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG;
        //电机停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            return;
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
            return;
        }
    }
    //查看之前的运行模式,快速定频回零模式和曲线回零模式下需要减速,其余模式直接停止
    if((motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor == STEP_MOTOR_RESET_MODE_CURVE)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID)))
    {
        //切换减速,从复位曲线到慢速速度
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        // 方向不变
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 细分设定
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        // 停止条件设定
        // 打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //关闭复位传感器有效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        //打开复位传感器无效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
        //根据配置打开急停检测,关闭两个辅助急停检测
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //从曲线运转到快速停止
        StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        // 启动曲线运转,使用预写入
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
            return;
        }
        //电机复位减速
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_DEC;
        return;
    }
    else if((motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor == STEP_MOTOR_RESET_MODE_FIX_FAST)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)))
    {
        // 定频减速
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        // 方向不变
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 细分设定
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        // 停止条件设定
        // 打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //关闭复位传感器有效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
        //打开复位传感器无效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
        //根据配置打开急停检测,关闭两个辅助急停检测
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;

        //从快速定频到慢速定频的减速曲线计算
        StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(motorDataSetPtr->stateSet.utilCurveTabDataPtr,motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast,
                                                        motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow,
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        // 启动曲线运转,使用预写入
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
            return;
        }
        //电机复位减速
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_DEC;
        return;
    }
    else if((motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor == STEP_MOTOR_RESET_MODE_FIX_SLOW)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)))
    {
        //找原点完成,方向为CCW
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        //正常停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_NONE;
        //方向切换为反向
        motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
        return;
    }
    else
    {
        //不预期的异常,直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_ERR;
        //如果是匀速运转的时候遇到的急停,那么直接停下来并报错
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid STEP_MOTOR_STATE_SUB_STOP_STATE_ERR Error!!\r\n");
        }
        return;
    }
}

//首次回原点
static void StepMotorResetStateIntProcessSubToOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
            //未知状态下收到了电机中断
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //遇上复位传感器有效,切换减速过程或者直接停止
            StepMotorResetStateIntProcessSubToOriginResetSensorValid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOrigin STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//快速回原点撞到原点之后的减速
static void StepMotorResetStateIntProcessSubToOriginDec(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,切换方向,定频脱离原点
            motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
            //慢速脱离原点
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
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
            //根据配置打开急停检测,关闭两个辅助急停检测
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
            //切换方向
            motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
            motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 启动定频运转
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                return;
            }
            //电机切换找到原点之后的脱离原点
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_BACK;
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //减速过程中检测到传感器无效,报错
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            //未知状态下收到了电机中断
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//原点减速之后回到刚离开原点的位置
static void StepMotorResetStateIntProcessSubToOriginBack(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发完也没脱离传感器,报错异常
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN;
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //脱离了原点传感器,检测传感器信号是否误触发,然后切换回到触发传感器位置
            motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
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
            //根据配置打开急停检测,关闭两个辅助急停检测
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
            //切换方向
            motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
            motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            // 电机停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
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
            // 启动定频运转
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                return;
            }
            //电机切换回到原点的位置
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_RETURN;
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//回到刚触发原点的位置
static void StepMotorResetStateIntProcessSubToOriginReturn(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发完也没检测到传感器,报错异常
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginReturn STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //找到了传感器,电机可以停止并且回零完成
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginReturn STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}


//遇上复位传感器无效,切换减速过程或者回原点过程
static void StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检查传感器信号是否伪消失
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_VALID)
    {
        //传感器信号伪消失
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP;
        //电机停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            return;
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP Error!!\r\n");
            return;
        }
    }
    //查看之前的运行模式,快速定频回零模式和曲线回零模式下需要减速,其余模式直接停止
    if((motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor == STEP_MOTOR_RESET_MODE_CURVE)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID)))
    {
        //切换减速,从复位曲线到慢速速度
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        // 方向不变
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 细分设定
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        // 停止条件设定
        // 打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //打开复位传感器有效停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
        //关闭复位传感器无效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        //根据配置打开急停检测,关闭两个辅助急停检测
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //从曲线运转到快速停止
        StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(motorDataSetPtr->stateSet.mainCurveTabDataPtr,motorDataSetPtr->stateSet.utilCurveTabDataPtr,
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        // 启动曲线运转,使用预写入
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
            return;
        }
        //电机复位减速
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_DEC;
        return;
    }
    else if((motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor == STEP_MOTOR_RESET_MODE_FIX_FAST)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)))
    {
        // 定频减速
        motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = DISABLE;
        // 方向不变
        motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 细分设定
        motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
        // 停止条件设定
        // 打开脉冲发送完成停止
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
        //打开复位传感器有效停止
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
        //关闭复位传感器无效中断
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
        //根据配置打开急停检测,关闭两个辅助急停检测
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //从快速定频到慢速定频的减速曲线计算
        StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(motorDataSetPtr->stateSet.utilCurveTabDataPtr,motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast,
                                                        motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow,
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        // 启动曲线运转,使用预写入
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorTabSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdTabSpd), ENABLE);
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid BoardFPGA_StepMotorTabSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT;
            return;
        }
        //电机复位减速
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_DEC;
        return;
    }
    else if((motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor == STEP_MOTOR_RESET_MODE_FIX_SLOW)&&
        ((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)))
    {
        //切换慢速回到原点
        motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
        motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
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
        //根据配置打开急停检测,关闭两个辅助急停检测
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
        motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
        //切换方向
        motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
        motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
        // 电机停止
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack BoardFPGA_StepMotorCmdStop Error!!\r\n");
            // 直接停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            // 正常停止
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT;
            return;
        }
        // 启动定频运转
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            // 错误打印
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginBack BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
            // 状态切入停止
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
            return;
        }
        //电机切换回到原点的位置
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_RETURN;
        return;
    }
    else
    {
        //不预期的异常,直接停止
        motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
        motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_STATE_ERR;
        //如果是匀速运转的时候遇到的急停,那么直接停下来并报错
        motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
        if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginResetSensorValid STEP_MOTOR_STATE_SUB_STOP_STATE_ERR Error!!\r\n");
        }
        return;
    }
}

//首次离开原点
static void StepMotorResetStateIntProcessSubLeaveOrigin(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
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
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_INVALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_INVALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_INVALID))
        {
            //遇上复位传感器无效,切换减速过程或者回原点过程
            StepMotorResetStateIntProcessSubLeaveOriginResetSensorInvalid(motorNo,motorDataSetPtr,motorState,stopReason);
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOrigin STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//高速离开原点,脱离原点之后的减速
static void StepMotorResetStateIntProcessSubLeaveOriginDec(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发送完成,切换方向,再次到触发原点位置
            motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = DISABLE;
            motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
            //步数设置为最大找原点步数加上复位换向补偿步数
            motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetDecelerationSteps + 
                                                                motorDataSetPtr->attriPtr->motorRunAttri.resetDirSwitchStepsCompensate;
            motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
            //停止条件设定
            //打开脉冲发送完成停止
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
            //打开复位传感器有效停止,当检测到复位传感器的时候停下来
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
            //关闭复位传感器无效停止
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
            //根据配置打开急停检测,关闭两个辅助急停检测
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
            motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
            //切换方向
            motorDataSetPtr->stateSet.currentDir = (STEP_MOTOR_DIR)(!motorDataSetPtr->stateSet.currentDir);
            motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
            // 加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            // 启动定频运转
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorFixSpdRunStart(motorNo, &(motorDataSetPtr->stateSet.cmdFixSpd));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                // 错误打印
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginDec BoardFPGA_StepMotorFixSpdRunStart Error!!\r\n");
                // 状态切入停止
                motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
                motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT;
                return;
            }
            //电机脱离原点减速停止之后再次找原点
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_RETURN;
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //减速过程中检测到传感器有效,报错
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG;
            //加载电机只读寄存器状态
            BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginDec STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubToOriginDec STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}

//脱离原点减速完成后回到刚碰到原点的位置
static void StepMotorResetStateIntProcessSubLeaveOriginReturn(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg;
    //正常运转阶段,停止条件需要靠参数识别
    if(motorState == STEP_MOTOR_STATE_STOP_CONDITION)
    {
        if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_PLUSE_OVER)||
            (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_PLUSE_OVER)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_PLUSE_OVER))
        {
            //脉冲发完也没检测到传感器,报错异常
            motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
            motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN;
            //电机切入停止
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo,&motorStateReg);
            if(motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN Error!!\r\n");
            }
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_EM_STOP_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_EM_STOP_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_EM_STOP_SENSOR_VALID))
        {
            //正常运行切减速急停
            StepMotorResetStateIntProcessToEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
            return;
        }
        else if((stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_FIX_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_FIX_RESET_SENSOR_VALID)||
                (stopReason == STEP_MOTOR_STOP_REASON_CW_SPD_TAB_RESET_SENSOR_VALID)||(stopReason == STEP_MOTOR_STOP_REASON_CCW_SPD_TAB_RESET_SENSOR_VALID))
        {
            //找到了传感器,电机可以停止并且回零完成
            motorDataSetPtr->stateSet.errorSubCodeInt = BoardFPGA_StepMotorCmdStop(motorNo, &(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.motorStateAndStopReason));
            if (motorDataSetPtr->stateSet.errorSubCodeInt != ERROR_SUB_OK)
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG Error!!\r\n");
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
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
            }
            else 
            {
                MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
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
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn BoardFPGA_StepMotorCmdStop Error!!\r\n");
        }
        else 
        {
            MCU_RTT_SendStringInt("StepMotorResetStateIntProcessSubLeaveOriginReturn STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP Error!!\r\n");
        }
        return;
    }
}



//复位状态下的中断处理
void StepMotorResetStateIntProcess(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,STEP_MOTOR_STATE motorState,STEP_MOTOR_STOP_REASON stopReason)
{
    switch(motorDataSetPtr->stateSet.subState)
    {
        case STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN:
            //首次回原点
            StepMotorResetStateIntProcessSubToOrigin(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_DEC:
            //快速回原点撞到原点之后的减速
            StepMotorResetStateIntProcessSubToOriginDec(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_BACK:
            //原点减速之后回到刚离开原点的位置
            StepMotorResetStateIntProcessSubToOriginBack(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN_RETURN:
            //回到刚触发原点的位置
            StepMotorResetStateIntProcessSubToOriginReturn(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN:
            //首次离开远点
            StepMotorResetStateIntProcessSubLeaveOrigin(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_DEC:
            //高速离开原点,脱离原点之后的减速
            StepMotorResetStateIntProcessSubLeaveOriginDec(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN_RETURN:
            //脱离原点减速完成后回到刚碰到原点的位置
            StepMotorResetStateIntProcessSubLeaveOriginReturn(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        case STEP_MOTOR_STATE_SUB_RESET_DEC_EM_STOP:
            //运行过程触发急停减速停止之后的处理
            StepMotorResetStateIntProcessSubDecEmStop(motorNo,motorDataSetPtr,motorState,stopReason);
        break;
        default:
            //复位不支持的状态的处理
            StepMotorResetStateIntProcessSubUnSupport(motorNo,motorDataSetPtr,motorState,stopReason);
            break;
    }
}

//参数检查
static ERROR_SUB StepMotorResetBaseParamCheck(StepMotorResetCmd* paramPtr)
{
    if(paramPtr == NULL)
    {
        //错误打印,参数为空
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_RESET_PARAM_NULL);
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_RESET_PARAM_NULL;
    }
    if(paramPtr->motorNo >= STEP_MOTOR_NUM_COUNT)
    {
        //错误打印,电机需要大于系统支持最大序号
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT);
        //电机端口不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_NO_UNSUPPORT;
    }
    if((uint8_t)(paramPtr->selectCruveCorrection) >= SM_CURVE_SET_MAX)
    {
        //错误打印,电机曲线大于系统支持最大曲线
        MCU_LogShowErrorSubCode(ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT);
        //电机曲线不支持
        return ERROR_SUB_FPGA_SM_LIB_MOTOR_CURVE_UNSUPPORT;
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
static ERROR_SUB StepMotorResetBaseBeforeStateCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    //首先检查电机是否使能
    if(motorDataSetPtr->attriPtr->motorRunAttri.motorExist == DISABLE)
    {
        //电机未使能
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_MOTOR_NOT_EXIST + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    if((motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_IDLE)&&
        (motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_STOP))
    {
        //电机不是空闲状态,不能运转
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_MOTOR_NOT_IDLE_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检查急停引脚
    if((motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet == ENABLE)&&
        (motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.emStopSensorState == PIN_STATE_VALID))
    {
        //启动前检测到急停有效
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_EM_STOP_VALID_BEFORE_START + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
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
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps != 0)&&
        (paramPtr->correctionCoordinate > motorDataSetPtr->attriPtr->motorRunAttri.positiveDirMaxSteps))
    {
        //电机超正向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_POSITIVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //计算目标是否超行程限制
    if((motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps != 0)&&
        ((paramPtr->correctionCoordinate) < motorDataSetPtr->attriPtr->motorRunAttri.reverseDirMaxSteps))
    {
        //电机超反向行程
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_RESERVE_COORDINATE_LIMIT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //没装复位传感器却设置了复位修正
    if((paramPtr->correctionCoordinate != 0)&&(motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorSet == DISABLE))
    {
        //没装复位传感器却设置了复位修正
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_RESET_SENSOR_NO_WITH_CORRECT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//运行前状态准备
static ERROR_SUB StepMotorResetBaseParpare(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除电机中断保存参数
    motorDataSetPtr->stateSet.errorSubCodeInt = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.motorStateFpgaInt = STEP_MOTOR_STATE_IDLE;
    motorDataSetPtr->stateSet.motorStopReasonFpgaInt = STEP_MOTOR_STOP_REASON_NONE;
    //拷贝电机运行指令
    UserMemCopy((uint8_t*)&(motorDataSetPtr->stateSet.paramMotorReset),(uint8_t*)paramPtr,DIM_STRUCT_LENGTH_BYTE(StepMotorResetCmd));
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //电机使能
    errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,ENABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
    if(errorSub != ERROR_SUB_OK)
    {
        //电机使能失败
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_ENABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//定频快速找原点
static ERROR_SUB StepMotorResetBaseStartSubToOriginFixFast(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = ENABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast;
    //启动运转,步数直接给最高
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
    //方向为反向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位信号检测到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位信号接收到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //进入快速找原点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//定频慢速找原点
static ERROR_SUB StepMotorResetBaseStartSubToOriginFixSlow(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = ENABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
    //启动运转,步数直接给最高
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor;
    //方向为反向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位信号检测到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位信号接收到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //进入快速找原点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//曲线找原点
static ERROR_SUB StepMotorResetBaseStartSubToOriginCurve(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SM_CURVE_SELECT toOriginCurveSelect = motorDataSetPtr->attriPtr->motorRunAttri.resetCurveSelect;
    //曲线运转
    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = ENABLE;
    //方向为反向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CCW;
    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //打开复位信号检测到停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = ENABLE;
    //关闭复位信号接收到停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = DISABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //曲线计算
    StepMotorCurveCalcMainTableWithStep(motorDataSetPtr->stateSet.mainCurveTabDataPtr,&motorDataSetPtr->attriPtr->motorCurveAttriSet[toOriginCurveSelect],
                                                SPEED_RATIO_MODE_GLOBAL,1.00f,
                                                motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsToOriginSensor);
    //曲线设置
    motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->spdTableArray;
    motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->stpTableArray;
    motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.mainCurveTabDataPtr->tabLengthAfterCalc;
    motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.mainCurveTabDataPtr->totalRunningSteps;
    //进入快速找原点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_TO_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//定频快速离开原点
static ERROR_SUB StepMotorResetBaseStartSubLeaveOriginFixFast(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = ENABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmFast;
    //启动运转,步数直接给最高
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
    //方向为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位信号检测到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位信号接收到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //进入快速离开远点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//定频慢速离开原点
static ERROR_SUB StepMotorResetBaseStartSubLeaveOriginFixSlow(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    motorDataSetPtr->stateSet.cmdFixSpd.clearEncoderPosSet = ENABLE;
    motorDataSetPtr->stateSet.cmdFixSpd.rpmSet = motorDataSetPtr->attriPtr->motorRunAttri.resetSpdRpmSlow;
    //启动运转,步数直接给最高
    motorDataSetPtr->stateSet.cmdFixSpd.stepCount = motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin;
    //方向为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdFixSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //打开脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位信号检测到停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位信号脱离停止
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdFixSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //进入离开远点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//曲线离开原点
static ERROR_SUB StepMotorResetBaseStartSubLeaveOriginCurve(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    SM_CURVE_SELECT toOriginCurveSelect = motorDataSetPtr->attriPtr->motorRunAttri.resetCurveSelect;
    //曲线运转
    motorDataSetPtr->stateSet.cmdTabSpd.clearEncoderPosSet = ENABLE;
    //方向为正向
    motorDataSetPtr->stateSet.currentDir = STEP_MOTOR_DIR_CW;
    motorDataSetPtr->stateSet.cmdTabSpd.motorDir = motorDataSetPtr->stateSet.currentDir;
    motorDataSetPtr->stateSet.cmdTabSpd.motorDiv = motorDataSetPtr->attriPtr->motorRunAttri.motorDriverDiv;
    //停止条件设定
    //脉冲发送完成停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.pluseSendOverStop = ENABLE;
    //关闭复位信号检测到停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorValidStop = DISABLE;
    //打开复位信号离开停止
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.resetSensorInvalidStop = ENABLE;
    //根据配置打开急停检测,关闭两个辅助急停检测
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.emStopSensorValidStop = motorDataSetPtr->attriPtr->motorSensorAttri.emStopSensorSet;
    motorDataSetPtr->stateSet.cmdTabSpd.stopCondition.valBits.utilStopSensorValidStop = DISABLE;
    //曲线计算
    StepMotorCurveCalcMainTableWithStep(motorDataSetPtr->stateSet.mainCurveTabDataPtr,&motorDataSetPtr->attriPtr->motorCurveAttriSet[toOriginCurveSelect],
                                                SPEED_RATIO_MODE_GLOBAL,1.00f,
                                                motorDataSetPtr->attriPtr->motorRunAttri.resetMaxStepsLeaveOrigin);
    //曲线设置
    motorDataSetPtr->stateSet.cmdTabSpd.rpmSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->spdTableArray;
    motorDataSetPtr->stateSet.cmdTabSpd.stepSetArrayPtr = motorDataSetPtr->stateSet.mainCurveTabDataPtr->stpTableArray;
    motorDataSetPtr->stateSet.cmdTabSpd.tableLength = motorDataSetPtr->stateSet.mainCurveTabDataPtr->tabLengthAfterCalc;
    motorDataSetPtr->stateSet.cmdTabSpd.stepCountTotal = motorDataSetPtr->stateSet.mainCurveTabDataPtr->totalRunningSteps;
    //进入离开远点模式
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_RESET;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_RESET_LEAVE_ORIGIN;
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//运行启动
static ERROR_SUB StepMotorResetBaseStart(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    //加载电机只读寄存器状态
    BoardFPGA_StepMotorGetReadOnlyReg(motorNo,&(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs));
    //检测原点的情况决定运行模式
    if(motorDataSetPtr->stateSet.fpgaRegSetPtr->readOnlyRegs.sensorStateAndTabIndexCurrent.valBits.resetSensorState == PIN_STATE_INVALID)
    {
        //原点传感器无效,要找原点
        if(motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor  == STEP_MOTOR_RESET_MODE_FIX_FAST)
        {
            //定频快速找原点
            return StepMotorResetBaseStartSubToOriginFixFast(motorNo,motorDataSetPtr,paramPtr);
        }
        else if(motorDataSetPtr->attriPtr->motorRunAttri.firstToOriginSensor  == STEP_MOTOR_RESET_MODE_FIX_SLOW)
        {
            //定频慢速找原点
            return StepMotorResetBaseStartSubToOriginFixSlow(motorNo,motorDataSetPtr,paramPtr);
        }
        else
        {
            //曲线找原点
            return StepMotorResetBaseStartSubToOriginCurve(motorNo,motorDataSetPtr,paramPtr);
        }
    }
    else 
    {
        //此时原点传感器有效,要脱离传感器
        if(motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor  == STEP_MOTOR_RESET_MODE_FIX_FAST)
        {
            //定频快速离开
            return StepMotorResetBaseStartSubLeaveOriginFixFast(motorNo,motorDataSetPtr,paramPtr);
        }
        else if(motorDataSetPtr->attriPtr->motorRunAttri.firstLeaveOriginSensor  == STEP_MOTOR_RESET_MODE_FIX_SLOW)
        {
            //定频慢速离开
            return StepMotorResetBaseStartSubLeaveOriginFixSlow(motorNo,motorDataSetPtr,paramPtr);
        }
        else
        {
            //曲线离开
            return StepMotorResetBaseStartSubLeaveOriginCurve(motorNo,motorDataSetPtr,paramPtr);
        }
    }
}


//等待运行完成
static ERROR_SUB StepMotorResetBaseWaitOver(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
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
        if(motorDataSetPtr->stateSet.runningTimeMsCount > motorDataSetPtr->stateSet.paramMotorReset.timeOutMsSet)
        {
            //超时跳出
            break;
        }
        //检查电机状态变化
        if(motorDataSetPtr->stateSet.mainState != STEP_MOTOR_STATE_MAIN_RESET)
        {
            //检查是不是被外部停止了
            if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
            {
                //外部指令急停了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_EM_STOP_CMD + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_NOT_SUPPORT_STATE))
            {
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_NOT_SUPPORT_STATE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_SINGAL_UNEXCEPT_EM_STOP))
            {
                //急停减速完成,失败
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_UNEXCEPT_SINGAL_EM_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_UNEXCEPT_STOP))
            {
                //不期望的异常导致的急停
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_UNEXCEPT_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_CMD_STOP_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB_INT))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_TAB_INT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_STATE_FPGA_STATE))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_TRIG))
            {
                //复位传感器伪触发,出发后再去检测信号消失,传感器不稳定
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_RESET_SENSOR_DUMMY_TRIG + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NOT_FIND_ORIGIN))
            {
                //脉冲发送完成没找到原点
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_PLUSE_OVER_NOT_FIND_ORIGIN + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_LOSS))
            {
                //找原点减速过程中原点信号丢失
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_ORIGIN_DEC_LOSS + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_PLUSE_OVER_NO_LEAVE_ORIGIN))
            {
                //离开原点时脉冲发送完成也没能脱离原点,检查最大行程或者传感器状态,可能电机卡住了
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_PLUSE_OVER_NO_LEAVE_ORIGIN + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_RESET_SENSOR_DUMMY_DISAPP))
            {
                //检测到传感器信号无效并停止后再检测传感器信号变成有效,传感器不稳定
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_RESET_SENSOR_DUMMY_DISAPP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_ORIGIN_DEC_TRIG))
            {
                //脱离原点减速的过程中原点传感器有变得有效了,检查传感器或电机,可能电机缺相或传感器故障
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_ORIGIN_DEC_TRIG + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_CMD_STOP))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_FIX))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_FIX + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
            else if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&(motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_FPFA_CONTROL_START_TAB))
            {
                //运行时FPGA控制异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_START_TAB + (ERROR_SUB)(((uint32_t)motorNo)<<8));
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
                        MCU_LogSendString("StepMotorResetBaseWaitOver BoardFPGA_StepMotorCmdStop Error!!\r\n");
                    }
                    else
                    {
                        MCU_LogSendString("StepMotorResetBaseWaitOver STEP_MOTOR_STATE_SUB_STOP_STATE_ERR Error!!\r\n");
                    }
                }
                //状态机混乱导致异常
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_STATE_ERR + (ERROR_SUB)(((uint32_t)motorNo)<<8));
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_CMD_STOP + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    MCU_LogSendString("StepMotorResetBaseWaitOver STEP_MOTOR_STATE_SUB_STOP_TIME_OUT Error!!\r\n");
    //报错运行超时
    motorDataSetPtr->stateSet.mainState = STEP_MOTOR_STATE_MAIN_STOP;
    motorDataSetPtr->stateSet.subState = STEP_MOTOR_STATE_SUB_STOP_TIME_OUT;
    //电机运行超时
    return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_TIME_OUT + (ERROR_SUB)(((uint32_t)motorNo)<<8));
}

//运行完成的后置收尾
static ERROR_SUB StepMotorResetBaseWindUp(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr)
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
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FPGA_CONTROL_TO_IDLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return ERROR_SUB_OK;
}

//修正步数运转
static ERROR_SUB StepMotorResetRunCorrect(STEP_MOTOR_NO motorNo,STEP_MOTOR_DATA_SET* motorDataSetPtr,StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //不需要修正
    if(paramPtr->correctionCoordinate == 0)
    {
        //没有修正坐标,如果处于教学模式,释放电机
        if(motorDataSetPtr->stateSet.teachMode == STEP_MOTOR_MODE_TEACH_ENABLE)
        {
            errorSub = BoardFPGA_StepMotorAndEncoderState(motorNo,DISABLE,motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet);
            if(errorSub != ERROR_SUB_OK)
            {
                //电机使能失败
                return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_MOTOR_DISABLE + (ERROR_SUB)(((uint32_t)motorNo)<<8));
            }
        }
        return ERROR_SUB_OK;
    }
    MOTOR_SENSOR_FLAGS sensorFlags;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.motorNo = motorNo;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.emStopSensorSet = ENABLE;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.utilStopSensorSet = DISABLE;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.targetCoordinate = paramPtr->correctionCoordinate;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.selectCurveConfig = paramPtr->selectCruveCorrection;

    motorDataSetPtr->stateSet.paramMotorRunCoordinate.dirSwitchCorrectSteps = 0;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.speedRatio = 1.0f;
    motorDataSetPtr->stateSet.paramMotorRunCoordinate.speedRatioMode = SPEED_RATIO_MODE_GLOBAL;
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
    errorSub = StepMotorRunCoordinate(&(motorDataSetPtr->stateSet.paramMotorRunCoordinate),&sensorFlags);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查传感器信号
    if(sensorFlags.sensorEmStopFlag == SET)
    {
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_CORRECT_EM_STOP_SINGAL + (ERROR_SUB)(((uint32_t)motorNo)<<8));
    }
    return errorSub;
}

//电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* paramPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    STEP_MOTOR_DATA_SET* motorDataSetPtr = NULL;
    //首先检查指令参数是否正确
    errorSub = StepMotorResetBaseParamCheck(paramPtr);
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
    errorSub = StepMotorResetBaseBeforeStateCheck(paramPtr->motorNo,motorDataSetPtr,paramPtr);
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
    errorSub = StepMotorResetBaseParpare(paramPtr->motorNo,motorDataSetPtr,paramPtr);
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
    //检查是否存在原点传感器,如果不存在,那么直接将电机当前坐标设置为0,并且返回
    if(motorDataSetPtr->attriPtr->motorSensorAttri.resetSensorSet == DISABLE)
    {
        //强制进入初始化状态
        errorSub = StepMotorMainForceToDefaultState(paramPtr->motorNo);
        if(errorSub != ERROR_SUB_OK)
        {
            //退出教学模式
            motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
            //释放互斥信号量
            StepMotorBaseReleaseLock(paramPtr->motorNo);
            //错误打印
            MCU_LogShowErrorSubCode(errorSub);
            //错误返回
            return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_FORCE_TO_DEFAULT + (ERROR_SUB)(paramPtr->motorNo));
        }
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //返回
        return ERROR_SUB_OK;
    }
    //记录启动时间
    motorDataSetPtr->stateSet.startTimeTick = tx_time_get();
    //启动运转
    errorSub = StepMotorResetBaseStart(paramPtr->motorNo,motorDataSetPtr,paramPtr);
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
    errorSub = StepMotorResetBaseWaitOver(paramPtr->motorNo,motorDataSetPtr);
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
    errorSub = StepMotorResetBaseWindUp(paramPtr->motorNo,motorDataSetPtr);
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
    //走到指定修正坐标
    errorSub = StepMotorResetRunCorrect(paramPtr->motorNo,motorDataSetPtr,paramPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //退出教学模式
        motorDataSetPtr->stateSet.teachMode = STEP_MOTOR_MODE_TEACH_DISABLE;
        //释放互斥信号量
        StepMotorBaseReleaseLock(paramPtr->motorNo);
        //错误打印
        MCU_LogShowErrorSubCode(errorSub);
        //错误返回
        return (ERROR_SUB)(ERROR_SUB_FPGA_SM1_RESET_RUN_CORRECT + (ERROR_SUB)(paramPtr->motorNo));;
    }
    //释放互斥信号量
    StepMotorBaseReleaseLock(paramPtr->motorNo);
    //返回
    return ERROR_SUB_OK;
}