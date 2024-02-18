/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:16:26
**LastEditors: DengXiaoJun
**LastEditTime: 2021-06-09 15:02:38
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorUtil.h"
#include "StepMotorTable.h"

//步进电机复位切换方向的速度比例
#define STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO             (0.3f)
//减速停止使用
#define STEP_MOTOR_RESET_DIR_STOP_SPEED_RATIO               (0.1f)
/**************************************电机复位函数申明*****************************************/
//复位第一步找零点
static void StepMotorResetIntProcessSubFindZeroSensorFirst(STEP_MOTOR_VAR* stepMotorVarPtr);
//复位第二步离开零点
static void StepMotorResetIntProcessSubLeaveZeroSensor(STEP_MOTOR_VAR* stepMotorVarPtr);
//复位第三步,再次回原点
static void StepMotorResetIntProcessSubFindZeroSensorSecond(STEP_MOTOR_VAR* stepMotorVarPtr);
//复位第四步,从限位点处找原点
static void StepMotorResetIntProcessSubFindZeroSensorLimitPointLocation(STEP_MOTOR_VAR* stepMotorVarPtr);
//复位第四步 复位修正
static void StepMotorResetIntProcessSubCorrection(STEP_MOTOR_VAR* stepMotorVarPtr);
/**********************************************************************************************/
//复位编码器信号检测
void StepMotorResetIntEncoderProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
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
    int32_t encoderLossLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderLossLimit);
    int32_t coordinateDifference = 0;
    //编码器信号识别判定
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet != NULL)
    {
        if((stepMotorVarPtr->var.currentCoordinate > SM_RESET_DIR_ENCODER_NOT_MATCH_THRESHOLD)&&(stepMotorVarPtr->var.currentEncoderCoordinate < 0-SM_RESET_DIR_ENCODER_NOT_MATCH_THRESHOLD))
        {
            //编码器信号和电机实际信号方向不一致,且差异大于一定阈值
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RESET_ENCODER_DIR;
        }
        else if((stepMotorVarPtr->var.currentCoordinate < 0 - SM_RESET_DIR_ENCODER_NOT_MATCH_THRESHOLD)&&(stepMotorVarPtr->var.currentEncoderCoordinate > SM_RESET_DIR_ENCODER_NOT_MATCH_THRESHOLD))
        {
            //编码器信号和电机实际信号方向不一致,且差异大于一定阈值
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RESET_ENCODER_DIR;
        }
        else
        {
            //计算当前编码器位置和电机位置之间的差异
            coordinateDifference = stepMotorVarPtr->var.currentEncoderCoordinate - stepMotorVarPtr->var.currentCoordinate;
            if(coordinateDifference < 0)
            {
                coordinateDifference = 0 - coordinateDifference;
            }
            //超过最大限制
            if(coordinateDifference > encoderLossLimit)
            {
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RESET_ENCODER_BLOCK;
            }
            int32_t encoderMoreThreshold = SM_RESET_ENCODER_MORE_THRESHOLD;
            //未超过最大限制,检测计数
            if((stepMotorVarPtr->var.currentCoordinate >= 0)&&(stepMotorVarPtr->var.currentEncoderCoordinate >= 0))
            {
                if(stepMotorVarPtr->var.currentEncoderCoordinate - stepMotorVarPtr->var.currentCoordinate > encoderMoreThreshold)
                {
                    //编码器信号比电机信号多
                    stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                    stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RESET_ENCODER_MORE;
                }
            }
            else if((stepMotorVarPtr->var.currentCoordinate < 0)&&(stepMotorVarPtr->var.currentEncoderCoordinate < 0))
            {
                if(stepMotorVarPtr->var.currentEncoderCoordinate - stepMotorVarPtr->var.currentCoordinate < (0 - encoderMoreThreshold))
                {
                    //编码器信号比电机信号多
                    stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                    stepMotorVarPtr->var.motorSubState =  STEP_MOTOR_STOP_SUB_RESET_ENCODER_MORE;
                }
            }
        }
    }
}

//复位
void StepMotorResetIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //编码器状态机设定
    StepMotorResetIntEncoderProcess(stepMotorVarPtr);
    //查看状态,如果状态为停止,切换锁止电流并停止
    if(stepMotorVarPtr->var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //电机停止
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStop();
        return;
    }
    //实际运行时状态机
    switch(stepMotorVarPtr->var.motorSubState)
    {
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST:
            //第一步找零点
            StepMotorResetIntProcessSubFindZeroSensorFirst(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RESET_SUB_LEAVE_ZERO:
            //第二步离开零点
            StepMotorResetIntProcessSubLeaveZeroSensor(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND:
            //第三步找零点
            StepMotorResetIntProcessSubFindZeroSensorSecond(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION:
            //第四步找零点
            StepMotorResetIntProcessSubFindZeroSensorLimitPointLocation(stepMotorVarPtr);
            break;
        case STEP_MOTOR_RESET_SUB_CORRECTION:
            //零位修正
            StepMotorResetIntProcessSubCorrection(stepMotorVarPtr);
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


/********************************************************系统复位相关状态机处理函数**************************************************/
//复位第一步找零点
static void StepMotorResetIntProcessSubFindZeroSensorFirst(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //发出的步数增加
    stepMotorVarPtr->var.resetCurrentSetOutPluse += 1;
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex);
    //设置了最大复位步数的情况下
    if(stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVarPtr->var.resetCurrentSetOutPluse > stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //步数超限制状态
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)//触发原点
    {
        //设置了复位见到光耦再走几步的情况下
        if(stepMotorVarPtr->attriPtr->baseConfig.resetDaccSteps != 0)
        {
            if(stepMotorVarPtr->var.daccStepPluseOutCount == stepMotorVarPtr->attriPtr->baseConfig.resetDaccSteps)
            {
                //复位找原点完成,切换复位离开原点
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
                if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
                {
                    stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
                }
                //电机当前方向应该是运行正方向
                stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVarPtr->attriPtr->baseConfig.dir));
                //设置电机运行方向,运行方向为离开原点方向
                StepMotorSetRunningDir(stepMotorVarPtr);
                //发出一步数据
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
                //当前坐标和编码器坐标归零
                stepMotorVarPtr->var.currentCoordinate = 0;
                stepMotorVarPtr->var.currentEncoderCoordinate = 0; 
                //计数值清空为0
                stepMotorVarPtr->var.daccStepPluseOutCount = 0;  
                stepMotorVarPtr->var.resetCurrentSetOutPluse = 0;
            }
            else
            {
                //发出一步数据
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_STOP_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
                stepMotorVarPtr->var.daccStepPluseOutCount += 1;
            }
        }
        else
        {
            //复位找原点完成,切换复位离开原点
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
            if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
            {
                stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
            }
            //电机当前方向应该是运行正方向
            stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVarPtr->attriPtr->baseConfig.dir));
            //设置电机运行方向,运行方向为离开原点方向
            StepMotorSetRunningDir(stepMotorVarPtr);
            //发出一步数据
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
            //当前坐标和编码器坐标归零
            stepMotorVarPtr->var.currentCoordinate = 0;
            stepMotorVarPtr->var.currentEncoderCoordinate = 0; 
            //计数值清空为0
            stepMotorVarPtr->var.daccStepPluseOutCount = 0;  
            stepMotorVarPtr->var.resetCurrentSetOutPluse = 0;    
        }
    }
    else
    {
        //检查是否配置了限位点
        if(stepMotorVarPtr->attriPtr->baseConfig.limitPointPinIndex < BOARD_PORT_IN_COUNT)
        {
            //读取限位点当前状态
            GPIO_PinState limitPointSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.limitPointPinIndex);    
            if(limitPointSensorValue == stepMotorVarPtr->attriPtr->baseConfig.limitPointPinValidLevel)
            {
                //找原点,设置状态是从限位点找原点
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION;
                //限位标记清零
                stepMotorVarPtr->var.resetLimitSensorOverTrigResetSensorFlag = 0;
                //电机当前方向应该是运行时正方向
                stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVarPtr->attriPtr->baseConfig.dir));
                //设置电机运行方向,运行方向为离开原点方向
                StepMotorSetRunningDir(stepMotorVarPtr);
                //发出一步数据
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
            }
            else
            {
                //继续找
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
            }
        }
        else
        {
            //继续找
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
        }
    }
}

//复位第二步离开零点
static void StepMotorResetIntProcessSubLeaveZeroSensor(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //发出的步数增加
    stepMotorVarPtr->var.resetCurrentSetOutPluse += 1;
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex);
    //设置了最大复位步数的情况下
    if(stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVarPtr->var.resetCurrentSetOutPluse > stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //步数超限制状态
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //检测到离开零点切换到下一步,没离开零点继续离开
    if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)
    {
        if(stepMotorVarPtr->attriPtr->baseConfig.resetOutSensorSteps == 0)
        {
            //发出一步数据       
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed)); 
        }
        else
        {
            if(stepMotorVarPtr->var.resetCurrentSetOutPluse < stepMotorVarPtr->attriPtr->baseConfig.resetOutSensorSteps)
            {
                //发出一步数据       
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed)); 
            }
            else
            {
                //超过了脱离原点最大行程
                //切换到停止状态
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                //步数超限制状态
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT;
                return;
            }
        }
    }
    else
    {
        //已经离开,现在再次找原点
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
        //切换第三步继续找零点
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_SECOND;
        //复位编码器
        if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
        {
            stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
        }
        //切换为正方向的反方向
        stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarPtr->attriPtr->baseConfig.dir));
        //切换方向
        StepMotorSetRunningDir(stepMotorVarPtr);
        //发出一步数据
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
        //当前坐标和编码器坐标归零
        stepMotorVarPtr->var.currentCoordinate = 0;
        stepMotorVarPtr->var.currentEncoderCoordinate = 0;
        stepMotorVarPtr->var.resetCurrentSetOutPluse = 0;
    }
}


//复位第三步,再次回原点
static void StepMotorResetIntProcessSubFindZeroSensorSecond(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //发出的步数增加
    stepMotorVarPtr->var.resetCurrentSetOutPluse += 1;
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex);
    //设置了最大复位步数的情况下
    if(stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVarPtr->var.resetCurrentSetOutPluse > stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //步数超限制状态
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    //如果此时原点被触发,那么复位结束
    if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)
    {
        //复位完成,复位修正在流程中切换
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //正常运行完成
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
        {
            stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
        }
        //当前坐标和编码器坐标归零
        stepMotorVarPtr->var.currentCoordinate = 0;
        stepMotorVarPtr->var.currentEncoderCoordinate = 0;
    }
    else
    {
        //发出一步数据
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
    }
}

//复位第四步,复位从限位点处找原点
static void StepMotorResetIntProcessSubFindZeroSensorLimitPointLocation(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //发出的步数增加
    stepMotorVarPtr->var.resetCurrentSetOutPluse += 1;
    //读取零点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex);
    //设置了最大复位步数的情况下
    if(stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps != 0)
    {
        //先看数据是否超过限制,超过限制,切换报错
        if(stepMotorVarPtr->var.resetCurrentSetOutPluse > stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps)
        {
            //切换到停止状态
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            //步数超限制状态
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            return;
        }
    }
    if(stepMotorVarPtr->var.resetLimitSensorOverTrigResetSensorFlag == 0)
    {
        //还没触发原点
        if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)
        {
            //触发了原点
            stepMotorVarPtr->var.resetLimitSensorOverTrigResetSensorFlag = 1;
        }
        //发出一步数据
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
    }
    else
    {
        //已经触发了原点,要等到原点不触发
        if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)
        {
            //原点依然触发,继续走到原点不触发
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
        }
        else
        {
            //原点已经不触发了
            //重新找原点
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
            if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
            {
                stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
            }
            //当前坐标和编码器坐标归零
            stepMotorVarPtr->var.currentCoordinate = 0;
            stepMotorVarPtr->var.currentEncoderCoordinate = 0; 
            stepMotorVarPtr->var.resetCurrentSetOutPluse = 0;  
            //清空计数
            stepMotorVarPtr->var.daccStepPluseOutCount = 0;
            //限位标记清零
            stepMotorVarPtr->var.resetLimitSensorOverTrigResetSensorFlag = 0;
            //切换为正方向的反方向
            stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarPtr->attriPtr->baseConfig.dir));
            //切换方向
            StepMotorSetRunningDir(stepMotorVarPtr);
            //发出一步数据
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
        }
    }
}

//复位修正
static void StepMotorResetIntProcessSubCorrection(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    stepMotorVarPtr->var.stepMotorStepTableDirect[0] -= 1;
    if(stepMotorVarPtr->var.stepMotorStepTableDirect[0] == 0)
    {
        //运行完成,切换停止
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //正常运行完成
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //发送一步
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
    }
}
/*********************************************************************************************************************************/



/**************************************************************************************************/
//检测步进电机的复位的起始运行条件
static ERROR_SUB StepMotorResetCheckStartCondition(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //检测电机行程限制,如果行程限制为0,代表不限制行程,否则,补偿有行程限制
    if(stepMotorVarPtr->attriPtr->baseConfig.positiveMaxSteps != 0)
    {
        //检查复位修正是否在运行行程里面
        if(resetCmdPtr->correctionCoordinate > (stepMotorVarPtr->attriPtr->baseConfig.positiveMaxSteps))
        {
            return StepMotorResetCorrectMaxError(resetCmdPtr->motorNo);
        }
    }
    if(stepMotorVarPtr->attriPtr->baseConfig.reverseMaxSteps != 0)
    {
        //限制行程
        if(resetCmdPtr->correctionCoordinate < (stepMotorVarPtr->attriPtr->baseConfig.reverseMaxSteps))
        {
            return StepMotorResetCorrectMinError(resetCmdPtr->motorNo);
        }
    }
    if(resetCmdPtr->timeOutSet == 0)
    {
        return StepMotorRunTimeOutZeroError(resetCmdPtr->motorNo);
    }
    return ERROR_SUB_OK;
}



//步进电机发送复位开始
static void StepMotorResetSendFindZeroStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //配置了原点
    //第一步,找原点,先读取当前原点状态
    GPIO_PinState orginSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex);
    //如果此时原点被触发,那么离开原点,否则找原点
    if(orginSensorValue == stepMotorVarPtr->attriPtr->baseConfig.resetPinValidLevel)
    {
        //离开原点
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_LEAVE_ZERO;
        //电机当前方向应该是运行时正方向
        stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVarPtr->attriPtr->baseConfig.dir));
        //设置电机运行方向
        StepMotorSetRunningDir(stepMotorVarPtr);
        //发出一步数据
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(STEP_MOTOR_RESET_DIR_SWITCH_SPEED_RATIO*(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed));
    }
    else
    {
        //检查是否配置了限位点
        if(stepMotorVarPtr->attriPtr->baseConfig.limitPointPinIndex < BOARD_PORT_IN_COUNT)
        {
            //读取限位点当前状态
            GPIO_PinState limitPointSensorValue = BoardPortInReadState(stepMotorVarPtr->attriPtr->baseConfig.limitPointPinIndex);    
            if(limitPointSensorValue == stepMotorVarPtr->attriPtr->baseConfig.limitPointPinValidLevel)
            {
                //找原点,设置状态是从限位点找原点
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_LOCATION;
                //电机当前方向应该是运行时正方向
                stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)((stepMotorVarPtr->attriPtr->baseConfig.dir));
                //设置电机运行方向,运行方向为离开原点方向
                StepMotorSetRunningDir(stepMotorVarPtr);
                //发出一步数据
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
            }
            else
            {
                //找原点,设置状态是找原点
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
                //电机当前方向应该是运行时反方向
                stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarPtr->attriPtr->baseConfig.dir));
                //设置电机运行方向
                StepMotorSetRunningDir(stepMotorVarPtr);
                //发出一步数据
                stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);    
            }
        }
        else
        {
            //找原点,设置状态是找原点
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_FIND_ZERO_FIRST;
            //电机当前方向应该是运行时反方向
            stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarPtr->attriPtr->baseConfig.dir));
            //设置电机运行方向
            StepMotorSetRunningDir(stepMotorVarPtr);
            //发出一步数据
            stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
        }
    }
}

//等待步进电机找零点结束
static ERROR_SUB StepMotorResetWaitFindZeroOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //等待电机运行停下来
    while(stepMotorVarPtr->var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVarPtr->var.stepMotorRunningTimeMsCount = OS_TICK_TO_MS(tx_time_get()) - stepMotorVarPtr->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVarPtr->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
        //设置了最大复位步数的情况下
        if(stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps != 0)
        {
            //检测步数超了,自动停止
            if(stepMotorVarPtr->var.resetCurrentSetOutPluse > stepMotorVarPtr->attriPtr->baseConfig.resetMaxSteps)
            {
                stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
                stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT;
            }
        }
    }
    //将数据转换为结果代码之后发送出去
    errorSubCode = StepMotorConvert2ErrorSubCode(resetCmdPtr->motorNo,stepMotorVarPtr->var.motorSubState);
    return errorSubCode;
}


//发送步进电机复位修正
static void StepMotorResetSendCorrectStart(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    if(resetCmdPtr->correctionCoordinate > 0)
    {
        //正向修正
        //电机当前方向应该是运行时正方向
        stepMotorVarPtr->var.currentDir = (stepMotorVarPtr->attriPtr->baseConfig.dir);
        //步数符号转换
        stepMotorVarPtr->var.stepMotorStepTableDirect[0] = (uint32_t)(resetCmdPtr->correctionCoordinate);
    }
    else
    {
        //反向修正
        //电机当前方向应该是运行时反方向
        stepMotorVarPtr->var.currentDir = (STEP_MOTOR_DIR)(!(stepMotorVarPtr->attriPtr->baseConfig.dir));
        //步数符号转换
        stepMotorVarPtr->var.stepMotorStepTableDirect[0] = (uint32_t)(0 - resetCmdPtr->correctionCoordinate);
    }
    //修正模式
    stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_RESET;
    stepMotorVarPtr->var.motorSubState = STEP_MOTOR_RESET_SUB_CORRECTION;
    //设置电机运行方向
    StepMotorSetRunningDir(stepMotorVarPtr);
    //发出脉冲
    stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->attriPtr->baseConfig.resetSpeed);
}


//等待步进电机复位修正结束
static ERROR_SUB StepMotorResetWaitCorrectOver(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //等待完成
    while(stepMotorVarPtr->var.motorMainState != STEP_MOTOR_MAIN_STOP)
    {
        //等待步进电机状态机停止
        CoreDelayMinTick();
        //更新运行时间
        stepMotorVarPtr->var.stepMotorRunningTimeMsCount = OS_TICK_TO_MS(tx_time_get()) - stepMotorVarPtr->var.stepMotorStartRunningTimeMs;
        //检测超时,自动停止
        if(stepMotorVarPtr->var.stepMotorRunningTimeMsCount > resetCmdPtr->timeOutSet)
        {
            stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
            stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_TIMEOUT;
        }
    }
    //将数据转换为结果代码之后发送出去
    errorSubCode = StepMotorConvert2ErrorSubCode(resetCmdPtr->motorNo,stepMotorVarPtr->var.motorSubState);
    return errorSubCode;
}

//未设置修正补偿次数的情况下的电机运行完成编码器检测
ERROR_SUB StepMotorResetCorrectZeroCheck(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* stepMotorVarPtr,int32_t correctTarget)
{
    int32_t correctPassLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderPassLimit);
    //当前编码器位置与目标位置误差
    int32_t coordinateDifference = 0;
    coordinateDifference = correctTarget - stepMotorVarPtr->var.currentEncoderCoordinate;
    if(coordinateDifference < 0)
    {
        coordinateDifference = 0 - coordinateDifference;
    }
    //将当前坐标更新为编码器坐标
    stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    //根据误差进行识别后续结果
    if(coordinateDifference <= correctPassLimit)
    {
        //差值在允许范围内,正常结束
        return ERROR_SUB_OK;
    }
    else
    {
        //差值超过程序限制,上报失步异常
        return StepMotorRunEncoderCorrectZeroPassLimitError(motorNo);
    }
}

//执行第一次复位修正
ERROR_SUB StepMotorResetCorrectFirst(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    //发送修正信息
    StepMotorResetSendCorrectStart(resetCmdPtr,stepMotorVarPtr);
    //等待修正结束
    errorCode = StepMotorResetWaitCorrectOver(resetCmdPtr,stepMotorVarPtr);
    return errorCode;
}

//执行编码器复位修正
ERROR_SUB StepMotorResetCorrectWithEncoder(StepMotorResetCmd* resetCmdPtr,STEP_MOTOR_VAR* stepMotorVarPtr,int32_t correctTarget)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    //修正计数器
    uint32_t correctCount = 0;
    //当前编码器位置与目标位置误差
    int32_t coordinateDifference = 0;
    //可以允许的误差容限
    int32_t correctPassLimit = (int32_t)(stepMotorVarPtr->attriPtr->baseConfig.encoderPassLimit);
    while(correctCount < stepMotorVarPtr->attriPtr->baseConfig.encoderCorrectRetryMax)
    {
        //读取偏差
        coordinateDifference = correctTarget - stepMotorVarPtr->var.currentEncoderCoordinate;
        if(coordinateDifference < 0)
        {
            coordinateDifference = 0 - coordinateDifference;
        }
        //如果编码器误差和实际目标误差小于可以允许的误差,就不用修正
        if(coordinateDifference <= correctPassLimit)
        {
            //误差在允许范围内
            return ERROR_SUB_OK;
        }
        else
        {
            //误差不在允许范围内,需要进行修正,先把当前坐标设置为编码器坐标
            stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
            resetCmdPtr->correctionCoordinate = correctTarget - stepMotorVarPtr->var.currentCoordinate;
        }
        //运行到次数,说明需要修补,进行修补,发送修正信息
        StepMotorResetSendCorrectStart(resetCmdPtr,stepMotorVarPtr);
        //等待修正结束
        errorCode = StepMotorResetWaitCorrectOver(resetCmdPtr,stepMotorVarPtr);
        //结果报错,退出循环
        if(errorCode != ERROR_SUB_OK)
        {
            return errorCode;
        }
        //修正计数器增加
        correctCount++;
    }
    //修正完成,查看修正结果,更新编码器坐标
    stepMotorVarPtr->var.currentCoordinate = stepMotorVarPtr->var.currentEncoderCoordinate;
    //读取偏差
    coordinateDifference = correctTarget - stepMotorVarPtr->var.currentEncoderCoordinate;
    if(coordinateDifference < 0)
    {
        coordinateDifference = 0 - coordinateDifference;
    }
    if(coordinateDifference <= correctPassLimit)
    {
        //误差在允许范围内
        return ERROR_SUB_OK;
    }
    else
    {
        //误差不在允许范围内,修正失败
        return StepMotorEncoderCorrectMaxError(resetCmdPtr->motorNo);
    }
}


//指定步进电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* resetCmdPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = resetCmdPtr->motorNo;
    STEP_MOTOR_VAR* stepMotorVarPtr = &(stepMotorVarArray[motorNo]);
    int32_t correctTarget = resetCmdPtr->correctionCoordinate;
    //复位运行起始条件检测
    errorSubCode = StepMotorResetCheckStartCondition(resetCmdPtr,stepMotorVarPtr);
    if(errorSubCode != ERROR_SUB_OK)return errorSubCode;
    //相关状态变量初始化到准备复位状态
    StepMotorStateInitForReset(&(stepMotorVarPtr->var));
    //如果存在编码器,编码器状态复位
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
    {
        stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
    }
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVarPtr->var.stepMotorStartRunningTimeMs = OS_TICK_TO_MS(tx_time_get());
    //首先查看是否配置零位传感器,配置了原点,就进入找原点流程
    if(stepMotorVarPtr->attriPtr->baseConfig.resetPinIndex < BOARD_PORT_IN_COUNT)
    {
        //发送一个找原点讯息
        StepMotorResetSendFindZeroStart(resetCmdPtr,stepMotorVarPtr);
        //等待找原点结束
        errorSubCode = StepMotorResetWaitFindZeroOver(resetCmdPtr,stepMotorVarPtr);
        //结果未报错,开始运行修正,否则报错
        if(errorSubCode != ERROR_SUB_OK)
        {
            return errorSubCode;
        }
    }
    CoreDelayMs(1000);
    //如果存在编码器,编码器状态复位
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset != NULL)
    {
        stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountReset();
        //编码器计数值复位
        stepMotorVarPtr->var.currentEncoderCoordinate = 0;
    }
    //设置当前坐标为0坐标
    stepMotorVarPtr->var.currentCoordinate = 0;
    //无修正,运行完成了
    if(resetCmdPtr->correctionCoordinate == 0)
    {
        return ERROR_SUB_OK;
    }
    //执行一次复位修正
    errorSubCode = StepMotorResetCorrectFirst(resetCmdPtr,stepMotorVarPtr);
    //结果未报错,修正完成
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //编码器信号识别判定,不存在编码器的情况下,直接返回
    if(stepMotorVarPtr->attriPtr->callBackFuncConfig.encoderCountGet == NULL)
    {
        //没配置编码器,直接返回
        return ERROR_SUB_OK;
    }
    //开始运行修正,首先检测修正次数,修正次数为0的情况下之做检测不做判断
    if(stepMotorVarPtr->attriPtr->baseConfig.encoderCorrectRetryMax == 0)
    {
        //修正次数为0的编码器检测判断
        return StepMotorResetCorrectZeroCheck(resetCmdPtr->motorNo,stepMotorVarPtr,correctTarget);
    }
    //修正运行
    return StepMotorResetCorrectWithEncoder(resetCmdPtr,stepMotorVarPtr,correctTarget);
}









