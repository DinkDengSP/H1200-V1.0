/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 14:58:19 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"

/**************************************电机持续运行函数申明****************************************/
//持续运行的加速段处理
static void StepMotorProcRunAlwaysSubAccelerate(STEP_MOTOR_NO motorNo);
//持续运行的持续段处理
static void StepMotorProcRunAlwaysSubHolding(STEP_MOTOR_NO motorNo);
//持续运行的减速段处理
static void StepMotorProcRunAlwaysSubDecelerate(STEP_MOTOR_NO motorNo);
/***********************************************************************************************/


//持续运行
void CpldStepMotorRunAlways(STEP_MOTOR_NO motorNo)
{
    switch(stepMotorVar[motorNo]->var.motorSubState)
    {
        case STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE:
            //步进电机持续运转的加速过程
            StepMotorProcRunAlwaysSubAccelerate(motorNo);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING:
            //已经运转到最高速,开始保持运转
            StepMotorProcRunAlwaysSubHolding(motorNo);
            break;
        case STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE:
            //切换到减速运转
            StepMotorProcRunAlwaysSubDecelerate(motorNo);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


/*****************************************************系统持续运转相关函数申明******************************************************/
//持续运行的加速段处理
static void StepMotorProcRunAlwaysSubAccelerate(STEP_MOTOR_NO motorNo)
{
    uint32_t currentDataIndex = stepMotorVar[motorNo]->var.currentTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    //只有一段说明是匀速过程
    if(stepMotorVar[motorNo]->var.currentTableMaxLength == 1)
    {
        //匀速运动,直接切换到匀速段
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
        stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
        //发送一次单次最大脉冲数
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,//电机序号
                                            BOARD_CPLD_MOTOR_EN_ENABLE,//使能
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,//方向
                                            BOARD_CPLD_MOTOR_STOP_OFF,//不急停
                                            stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],//速度
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax//脉冲数
                                            );
        //记录上次发送脉冲
        stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
    }
    else
    {
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
            //当定位表进入最大行程的一半的时候,说明已经到了最大速度位置,此时,进入保持段
            if(stepMotorVar[motorNo]->var.currentTableIndex == ((stepMotorVar[motorNo]->var.currentTableMaxLength)/2))
            {
                stepMotorVar[motorNo]->var.currentTableIndex -= 1;
                //设置走位状态
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_HOLDING;
            }
        }
    }
}

//持续运行的持续段处理
static void StepMotorProcRunAlwaysSubHolding(STEP_MOTOR_NO motorNo)
{
    uint32_t currentDataIndex = stepMotorVar[motorNo]->var.currentTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    //只有一段说明是匀速过程
    if(stepMotorVar[motorNo]->var.currentTableMaxLength == 1)
    {
        //发送一次单次最大脉冲数
        BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,//电机序号
                                        BOARD_CPLD_MOTOR_EN_ENABLE,//使能
                                        (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,//方向
                                        BOARD_CPLD_MOTOR_STOP_OFF,//不急停
                                        stepMotorVar[motorNo]->var.stepMotorSpeedTableDirect[0],//速度
                                        (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                        stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax//脉冲数
                                        );
        //记录上次发送脉冲
        stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
    }
    else
    {
        //保持运转,每次发最大脉冲数,且速度都是最大速度
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
    }
}

//持续运行的减速段处理
static void StepMotorProcRunAlwaysSubDecelerate(STEP_MOTOR_NO motorNo)
{
    //减速运转
    uint32_t currentDataIndex = stepMotorVar[motorNo]->var.currentTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    //只有一段说明是匀速过程
    if(stepMotorVar[motorNo]->var.currentTableMaxLength == 1)
    {
        //直接停止
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    }
    else
    {
        //检查当前序号,必须要切换到转折点之后
        if(stepMotorVar[motorNo]->var.currentTableIndex < (stepMotorVar[motorNo]->var.currentTableMaxLength/2))
        {
            stepMotorVar[motorNo]->var.currentTableIndex = 
                stepMotorVar[motorNo]->var.currentTableMaxLength - stepMotorVar[motorNo]->var.currentTableIndex -1;
        }
        else
        {
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
    }
}

/*********************************************************************************************************************************/

//指定步进电机持续运转
ERROR_SUB StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    STEP_MOTOR_NO motorNo = runAlwaysConfigPtr->motorNo;
    //检查步进电机状态是否是指令急停,指令急停状态只能复位不能运动
    errorSubCode = StepMotorUtilCheckEmergencyCmdState(motorNo,stepMotorVar[motorNo]);
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
    StepMotorStateInitForRunAlways(&(stepMotorVar[motorNo]->var));
    //开始计算步进电机运行曲线
    StepMotorTableCalcWithRunAlways(motorNo,runAlwaysConfigPtr->selectConfig,
                                    runAlwaysConfigPtr->speedRatioMode,runAlwaysConfigPtr->speedRatio);
    //记录下启动运行时间,后续会通过这个时间进行电机运行时间的检测
    stepMotorVar[motorNo]->var.stepMotorStartRunningTimeMs = tx_time_get();
    //设置走位状态
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_RUN_ALWAYS;
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_ACCELERATE;
    //持续运转的方向
    stepMotorVar[motorNo]->var.currentDir = runAlwaysConfigPtr->dir;
    //发出正式的走位脉冲
    StepMotorUtilSendStartPluse(motorNo,stepMotorVar[motorNo]);
    return ERROR_SUB_OK;
}



