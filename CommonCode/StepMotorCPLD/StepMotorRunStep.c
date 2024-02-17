/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 14:18:55 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"
#include "StepMotorAlarm.h"

/************************************电机走步数函数申明*******************************************/
static void StepMotorProcRunStepsSubNormal(STEP_MOTOR_NO motorNo);
/***********************************************************************************************/

//走步数
void CpldStepMotorRunStepsProc(STEP_MOTOR_NO motorNo)
{
    switch(stepMotorVar[motorNo]->var.motorSubState)
    {
        case STEP_MOTOR_RUN_STEPS_SUB_NORMAL:
            //步进电机正常走步数
            StepMotorProcRunStepsSubNormal(motorNo);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/********************************************************步进电机正常走步数的状态机实现*******************************************/
static void StepMotorProcRunStepsSubNormal(STEP_MOTOR_NO motorNo)
{
    uint32_t currentDataIndex = stepMotorVar[motorNo]->var.currentTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    //检查步数表中是否还有未发送的数据
    if(currentDataIndex >= stepMotorVar[motorNo]->var.currentTableMaxLength)
    {
        //当前坐标已经到达最大位置,全部的数据都发送完成了
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //数据没有发送完成,继续发送数据
        if(stepMotorVar[motorNo]->var.stepMotorStepTableDirect[currentDataIndex] > stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax)
        {
            //还在正常运行过程
            //等待发送的数据大于电机配置的最大单次发送脉冲数,发送数据
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,//电机序号
                                            BOARD_CPLD_MOTOR_EN_ENABLE,//使能
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,//方向
                                            BOARD_CPLD_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[currentDataIndex],//速度
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax//脉冲数
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
            //内部的步数表存储的步数减少
            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[currentDataIndex] -= stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //还在正常运行过程
            //等待发送的数据小于电机配置的最大单次发送脉冲数,那么可以一次发送完成
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,//电机序号
                                            BOARD_CPLD_MOTOR_EN_ENABLE,//使能
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,//方向
                                            BOARD_CPLD_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[currentDataIndex],//速度
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[currentDataIndex]//脉冲数
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->var.stepMotorStepTableDirect[currentDataIndex];
            //步数表减少
            stepMotorVar[motorNo]->var.stepMotorStepTableDirect[currentDataIndex] = 0;
            //定位往前加1
            stepMotorVar[motorNo]->var.currentTableIndex += 1;
        }
    }
}

/*********************************************************************************************************************************/
//指定步进电机运行指定步数
ERROR_SUB StepMotorRunSteps(StepMotorRunStepsCmd* runStepConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = runStepConfigPtr->motorNo;
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(motorNo,stepMotorVar[motorNo]);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //如果步数为0,不需要运行,直接返回
    if(runStepConfigPtr->specialSteps == 0)
    {
        //电机停下来,等待状态切换
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //写入一次急停
        BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
        return ERROR_SUB_OK;
    }
    //检查步进电目标坐标是否在限制坐标范围内
    errorSubCode = StepMotorUtilCheckTargetCoordinateLimit(motorNo,stepMotorVar[motorNo],stepMotorVar[motorNo]->var.currentCoordinate + runStepConfigPtr->specialSteps);
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
    //相关状态变量初始化到准备走位
    StepMotorStateInitForRunSteps(&(stepMotorVar[motorNo]->var));
    //检测急停引脚状态和初始化急停相关状态
    errorSubCode = StepMotorUtilCheckAndSetEmergencyPinState(motorNo,stepMotorVar[motorNo]);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检测辅助急停1引脚状态和初始化相关变量标志
    errorSubCode = StepMotorUtilCheckAndSetUtilStop1PinState(motorNo,stepMotorVar[motorNo],runStepConfigPtr->utilStop1Enable);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检测辅助急停2引脚状态和初始化相关变量标志
    errorSubCode = StepMotorUtilCheckAndSetUtilStop2PinState(motorNo,stepMotorVar[motorNo],runStepConfigPtr->utilStop2Enable);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithStep(runStepConfigPtr->specialSteps,motorNo,runStepConfigPtr->selectConfig,
                                runStepConfigPtr->speedRatioMode,runStepConfigPtr->speedRatio);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs = tx_time_get();
    //设置走位状态
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_STEPS;
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RUN_STEPS_SUB_NORMAL;
    //计算电机方向
    StepMotorUtilCalcDirWithTargetCoordinate(motorNo,stepMotorVar[motorNo],stepMotorVar[motorNo]->var.currentCoordinate + runStepConfigPtr->specialSteps);
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(motorNo,stepMotorVar[motorNo]);
    //等待电机停止运行
    StepMotorUtilWaitMotorStop(motorNo,stepMotorVar[motorNo],runStepConfigPtr->timeOutSet);
    //要根据是否打开了相关的中断和辅助中断来关闭响应的辅助中断和中断
    StepMotorUtilUnregisterIntPort(motorNo,stepMotorVar[motorNo]);
    //将数据转换为结果代码之后发送出去
    errorSubCode = StepMotorConvertErrorSub(motorNo,stepMotorVar[motorNo]->var.motorSubState);
    return errorSubCode;
}











