/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 14:11:00 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"
#include "StepMotorAlarm.h"

/**************************************电机复位函数申明*****************************************/
//复位第一步找零点
static void StepMotorProcResetSubFindZeroFirst(STEP_MOTOR_NO motorNo);
//复位第二步离开零点
static void StepMotorProcResetSubLeaveZero(STEP_MOTOR_NO motorNo);
//复位第三步,再次回原点
static void StepMotorProcResetSubFindZeroSecond(STEP_MOTOR_NO motorNo);
//复位第四步 复位修正
static void StepMotorProcResetSubCorrect(STEP_MOTOR_NO motorNo);
//从限位位置找原点
static void StepMotorProcResetSubFindZeroLocation(STEP_MOTOR_NO motorNo);
/**********************************************************************************************/

//复位
void CpldStepMotorResetProc(STEP_MOTOR_NO motorNo)
{
    switch(stepMotorVar[motorNo]->var.motorSubState)
    {
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST:
            //第一步找零点
            StepMotorProcResetSubFindZeroFirst(motorNo);
            break;
        case STEP_MOTOR_RESET_SUB_LEAVE_ZERO:
            //第二步离开零点
            StepMotorProcResetSubLeaveZero(motorNo);
            break;
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND:
            //第三步找零点
            StepMotorProcResetSubFindZeroSecond(motorNo);
            break;
        case STEP_MOTOR_RESET_SUB_CORRECTION:
            //零位修正
            StepMotorProcResetSubCorrect(motorNo);
            break;
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION:
            //从限位位置找原点
            StepMotorProcResetSubFindZeroLocation(motorNo);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/********************************************************系统复位相关状态机处理函数**************************************************/
//复位第一步找零点
static void StepMotorProcResetSubFindZeroFirst(STEP_MOTOR_NO motorNo)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则找原点
    if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
    {
        //设置了复位见到光耦再走几步的情况下
        if(stepMotorVar[motorNo]->attri.baseConfig.resetDaccSteps != 0)
        {
            if(stepMotorVar[motorNo]->var.daccStepPluseOutCount == stepMotorVar[motorNo]->attri.baseConfig.resetDaccSteps)
            {
                //减速停止到位了,切换离开原点
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
                //离开原点
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
                //设置方向和发出步数
                stepMotorVar[motorNo]->var.currentDir = (stepMotorVar[motorNo]->attri.baseConfig.dir);
                stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
                //计数值清空为0
                stepMotorVar[motorNo]->var.daccStepPluseOutCount = 0;  
                //当前坐标归零
                stepMotorVar[motorNo]->var.currentCoordinate = 0;
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return;
            }
            else
            {
                //继续减速到停下来
                stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
                //减速步数增加
                stepMotorVar[motorNo]->var.daccStepPluseOutCount += 1;
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return;
            }
        }
        else
        {
            //找到了,切换离开原点
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
            //离开原点
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
            //设置方向和发出步数
            stepMotorVar[motorNo]->var.currentDir = (stepMotorVar[motorNo]->attri.baseConfig.dir);
            stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
            //计数值清空为0
            stepMotorVar[motorNo]->var.daccStepPluseOutCount = 0;  
            //当前坐标归零
            stepMotorVar[motorNo]->var.currentCoordinate = 0;
            //发出一步数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            1);
            return;
        }
    }
    else
    {
        //没遇到原点,检测减速步数,如果减速步数不为0,代表减速过程中原点消失
        if((stepMotorVar[motorNo]->attri.baseConfig.resetDaccSteps != 0)&&
            (stepMotorVar[motorNo]->var.daccStepPluseOutCount != 0))
        {
            //切换到停止状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //减速原点消失
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_DACC_LOSS;
            return;
        }
        //检查是否配置了限位点,触发限位点,反向找原点
        if(stepMotorVar[motorNo]->attri.baseConfig.limitPointPinIndex < BOARD_PORT_IN_COUNT)
        {
            //读取限位点当前状态
            GPIO_PinState limitPointSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.limitPointPinIndex);    
            if(limitPointSensorValue == stepMotorVar[motorNo]->attri.baseConfig.limitPointPinValidLevel)
            {
                //找原点,设置状态是从限位点找原点
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION;
                //限位标记清零
                stepMotorVar[motorNo]->var.resetLimitSensorOverTrigResetSensorFlag = 0;
                //电机当前方向应该是运行时正方向
                stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVar[motorNo]->attri.baseConfig.dir));
                stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return;
            }
        }
        //继续找
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
        return;
    }
}


//复位第二步离开零点
static void StepMotorProcResetSubLeaveZero(STEP_MOTOR_NO motorNo)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则,进入下一步回原点
    if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
    {
        if(stepMotorVar[motorNo]->attri.baseConfig.resetOutSensorSteps == 0)
        {
            //未限制离开原点步数,继续脱离原点
            stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
            //发出一步数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            1);
            return;
        }
        else
        {
            //限制了脱离原点步数
            if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse < stepMotorVar[motorNo]->attri.baseConfig.resetOutSensorSteps)
            {
                //还没到达脱离原点最大步数,发出一步数据       
                stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return; 
            }
            else
            {
                //到达了脱离原点最大步数
                //切换到停止状态
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                //步数超限制状态
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT;
                return;
            }
        }
    }
    else
    {
        //已经离开,现在再次找原点
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        //切换第三步继续找零点
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND;
        //切换方向
        stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVar[motorNo]->attri.baseConfig.dir));
        //步数计数
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
        //当前坐标归零
        stepMotorVar[motorNo]->var.currentCoordinate = 0;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
        return;
    }
}

//从限位位置找原点
static void StepMotorProcResetSubFindZeroLocation(STEP_MOTOR_NO motorNo)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
    if(stepMotorVar[motorNo]->var.resetLimitSensorOverTrigResetSensorFlag == 0)
    {
        //还没触发原点
        if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
        {
            //触发了原点
            stepMotorVar[motorNo]->var.resetLimitSensorOverTrigResetSensorFlag = 1;
        }
        //发出一步数据,继续向前走到离开原点
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
        return;
    }
    else
    {
        //已经触发了原点,要等到原点不触发
        if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
        {
            //发出一步数据,继续向前走到离开原点
            stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
            //发出一步数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            1);
            return;
        }
        else
        {
            //原点已经不触发了
            //重新找原点
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
            //当前坐标和编码器坐标归零
            stepMotorVar[motorNo]->var.currentCoordinate = 0;
            stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;  
            //清空计数
            stepMotorVar[motorNo]->var.daccStepPluseOutCount = 0;
            //限位标记清零
            stepMotorVar[motorNo]->var.resetLimitSensorOverTrigResetSensorFlag = 0;
            //切换为正方向的反方向
            stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVar[motorNo]->attri.baseConfig.dir));
            //发出一步数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            1);
            return;
        }
    }
}

//复位第三步,再次回原点
static void StepMotorProcResetSubFindZeroSecond(STEP_MOTOR_NO motorNo)
{
    //设置了最大复位步数的情况下
    if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //步数超限制状态
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么复位结束
    if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
    {
        //复位完成,切换停止
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //正常运行完成
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //当前坐标归零
        stepMotorVar[motorNo]->var.currentCoordinate = 0;
        return;
    }
    else
    {
        //继续找
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse += 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
    }
}

//复位第四步 复位修正
static void StepMotorProcResetSubCorrect(STEP_MOTOR_NO motorNo)
{
    if(stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] == 0)
    {
        //运行完成,切换停止
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //正常运行完成
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        return;
    }
    else
    {
        //继续运转
        if(stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] > stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax)
        {
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax);
            //减小发出的步数
            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] -= stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0]);
            //发完剩余步数为0
            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] = 0;
        }
        //继续修正
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_CORRECTION;
        return;
    }
}
/*********************************************************************************************************************************/

/**************************************************************************************************/
//检测步进电机的复位的起始运行条件
static ERROR_SUB StepMotorResetCheckStartCondition(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_NO motorNo)
{
    //检测电机行程限制,如果行程限制为0,代表不限制行程,否则,补偿有行程限制
    if(stepMotorVar[motorNo]->attri.baseConfig.positiveMaxSteps != 0)
    {
        //检查复位修正是否在运行行程里面
        if(resetCmdPtr->correctionCoordinate > (stepMotorVar[motorNo]->attri.baseConfig.positiveMaxSteps))
        {
            return StepMotorErrorSubStopResetCorrectPositive(motorNo);
        }
    }
    if(stepMotorVar[motorNo]->attri.baseConfig.reverseMaxSteps != 0)
    {
        //限制行程
        if(resetCmdPtr->correctionCoordinate < (stepMotorVar[motorNo]->attri.baseConfig.reverseMaxSteps))
        {
            return StepMotorErrorSubStopResetCorrectNegative(motorNo);
        }
    }
    if(resetCmdPtr->timeOutSet == 0)
    {
        return StepMotorErrorSubStopTimeOutZero(motorNo);
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
    return ERROR_SUB_OK;
}



//步进电机发送复位开始
static void StepMotorResetSendFindZeroStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_NO motorNo)
{
    //配置了原点
    //第一步,找原点,先读取当前原点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则找原点
    if(orginSensorValue == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
    {
        //离开原点
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
        //电机当前方向应该是运行时正方向
        stepMotorVar[motorNo]->var.currentDir = (stepMotorVar[motorNo]->attri.baseConfig.dir);
        //增加1步的复位脉冲数
        stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
        //发出一步数据
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->attri.baseConfig.resetSpeedSlow,
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        1);
        return;
    }
    else
    {
        //检查是否配置了限位点
        if(stepMotorVar[motorNo]->attri.baseConfig.limitPointPinIndex < BOARD_PORT_IN_COUNT)
        {
            //配置了限位点
            //读取限位点当前状态
            GPIO_PinState limitPointSensorValue = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.limitPointPinIndex);    
            if(limitPointSensorValue == stepMotorVar[motorNo]->attri.baseConfig.limitPointPinValidLevel)
            {
                //找原点,设置状态是从限位点找原点
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION;
                //电机当前方向应该是运行时正方向
                stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVar[motorNo]->attri.baseConfig.dir));
                //增加1步的复位脉冲数
                stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return;
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
                //发出一步数据
                BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                                BOARD_CPLD_MOTOR_STOP_OFF,
                                                stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                                (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                                1);
                return;    
            }
        }
        else
        {
            //没配置限位点,开始找原点
            //找原点,设置状态是找原点
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
            //电机当前方向应该是运行时反方向
            stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVar[motorNo]->attri.baseConfig.dir));
            //增加1步的复位脉冲数
            stepMotorVar[motorNo]->var.resetCurrentSetOutPluse = 1;
            //发出一步数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast,
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            1);
            return;
        }
    }
}

//等待步进电机找零点结束
static ERROR_SUB StepMotorResetWaitFindZeroOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //等待电机运行停下来
    while(stepMotorVar[motorNo]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount = tx_time_get() - stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
        //设置了最大复位步数的情况下
        if(stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps != 0)
        {
            //检测步数超了,自动停止
            if(stepMotorVar[motorNo]->var.resetCurrentSetOutPluse > stepMotorVar[motorNo]->attri.baseConfig.resetMaxSteps)
            {
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            }
        }
    }
    //将数据转换为结果代码之后发送出去
    errorSubCode = StepMotorConvertErrorSub(motorNo,stepMotorVar[motorNo]->var.motorSubState);
    return errorSubCode;
}

//发送步进电机复位修正
static void StepMotorResetSendCorrectStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_NO motorNo)
{
    if(resetCmdPtr->correctionCoordinate > 0)
    {
        //正向修正
        //电机当前方向应该是运行时正方向
        stepMotorVar[motorNo]->var.currentDir = (stepMotorVar[motorNo]->attri.baseConfig.dir);
        //步数符号转换
        stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] = (uint32_t)(resetCmdPtr->correctionCoordinate);
    }
    else
    {
        //反向修正
        //电机当前方向应该是运行时反方向
        stepMotorVar[motorNo]->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVar[motorNo]->attri.baseConfig.dir));
        //步数符号转换
        stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] = (uint32_t)(0 - resetCmdPtr->correctionCoordinate);
    }
    //设定速度,复位修正的速度是复位速度
    stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0] = stepMotorVar[motorNo]->attri.baseConfig.resetSpeedFast;
    //修正模式
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RESET;
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RESET_SUB_CORRECTION;
    //开始运转
    if(stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] > stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax)
    {
        //大于单次发送最大步数,一次发满
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax);
        stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] -= stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
        return;
    }
    else
    {
        //小于255,一次发完
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                        BOARD_CPLD_MOTOR_STOP_OFF,
                                        stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0]);
        stepMotorVar[motorNo]->var.stepMotorStepTableDirect[0] = 0;
        return;
    }
}


//等待步进电机复位修正结束
static ERROR_SUB StepMotorResetWaitCorrectOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_NO motorNo)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //等待完成
    while(stepMotorVar[motorNo]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount = tx_time_get() - stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVar[motorNo]->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
    }
    //将数据转换为结果代码之后发送出去
    errorSubCode = StepMotorConvertErrorSub(motorNo,stepMotorVar[motorNo]->var.motorSubState);
    return errorSubCode;
}

//指定步进电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* resetCmdPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = resetCmdPtr->motorNo;
    //复位运行起始条件检测
    errorSubCode = StepMotorResetCheckStartCondition(resetCmdPtr,motorNo);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //相关状态变量初始化到准备复位状态
    StepMotorStateInitForReset(&(stepMotorVar[motorNo]->var));
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs = tx_time_get();
    //首先查看是否配置零位传感器,配置了原点,就进入找原点流程
    if(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        //发送一个找原点讯息
        StepMotorResetSendFindZeroStart(resetCmdPtr,motorNo);
        //等待找原点结束
        errorSubCode = StepMotorResetWaitFindZeroOver(resetCmdPtr,motorNo);
        //结果未报错,开始运行修正
        if(errorSubCode != ERROR_SUB_OK)
        {
            return errorSubCode;
        }
    }
    //无修正,运行完成了
    if(resetCmdPtr->correctionCoordinate == 0)
    {
        return ERROR_SUB_OK; 
    }
    //发送修正信息
    StepMotorResetSendCorrectStart(resetCmdPtr,motorNo);
    //等待修正结束
    errorSubCode = StepMotorResetWaitCorrectOver(resetCmdPtr,motorNo);
    //结果未报错,修正完成
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    else
    {
        //记录当前坐标
        stepMotorVar[motorNo]->var.currentCoordinate = resetCmdPtr->correctionCoordinate;
        return ERROR_SUB_OK;
    }
}
























