/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 16:24:44 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"

//步进电机变量以及配置文件
STEP_MOTOR_VAR* stepMotorVar[SYSTEM_STEP_MOTOR_COUNT] = {NULL};

//步进电机中断回调函数
void StepMotorCPLD_CallBackProcessWithStateMachine(BOARD_CPLD_MOTOR motorNo)
{
    STEP_MOTOR_NO motorNoLocal = (STEP_MOTOR_NO)motorNo;
    //选择步进电机状态进行处理,状态机处理机制
    switch(stepMotorVar[motorNoLocal]->var.motorMainState)
    {
        case STEP_MOTOR_MAIN_STATE_STOP:
            break;
        case STEP_MOTOR_MAIN_STATE_RESET:
            //复位状态
            CpldStepMotorResetProc(motorNoLocal);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_STEPS:
            //运行指定步数状态
            CpldStepMotorRunStepsProc(motorNoLocal);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_COORDINATE:
            //运行到指定坐标状态
            CpldStepMotorRunCoordinate(motorNoLocal);
            break;
        case STEP_MOTOR_MAIN_STATE_RUN_ALWAYS:
            //持续运转状态
            CpldStepMotorRunAlways(motorNoLocal);
            break;
        case STEP_MOTOR_MAIN_STATE_DACC:
            //减速运行状态
            CpldStepMotorRunDacc(motorNoLocal);
            break;
        default:
            //不支持的状态
            stepMotorVar[motorNoLocal]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            //未知原因
            stepMotorVar[motorNoLocal]->var.motorSubState = STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}


//步进电机参数变量的初始化
void StepMotorVarInit(void)
{
    uint8_t indexUtil = 0;
    //内存申请
    for(indexUtil = 0; indexUtil < SYSTEM_STEP_MOTOR_COUNT; indexUtil++)
    {
        //申请内存
        stepMotorVar[indexUtil] = UserMemMallocWhileSuccess(MEM_SRAM_EXT,sizeof(STEP_MOTOR_VAR)/sizeof(uint8_t));
    }
    //加载电机配置,12个分别加载
    StepMotorParamLoad(STEP_MOTOR_PARAM1_ADDR,&(stepMotorVar[STEP_MOTOR_NO1]->attri),0);
    StepMotorParamLoad(STEP_MOTOR_PARAM2_ADDR,&(stepMotorVar[STEP_MOTOR_NO2]->attri),1);
    StepMotorParamLoad(STEP_MOTOR_PARAM3_ADDR,&(stepMotorVar[STEP_MOTOR_NO3]->attri),2);
    StepMotorParamLoad(STEP_MOTOR_PARAM4_ADDR,&(stepMotorVar[STEP_MOTOR_NO4]->attri),3);
    StepMotorParamLoad(STEP_MOTOR_PARAM5_ADDR,&(stepMotorVar[STEP_MOTOR_NO5]->attri),4);
    StepMotorParamLoad(STEP_MOTOR_PARAM6_ADDR,&(stepMotorVar[STEP_MOTOR_NO6]->attri),5);
    StepMotorParamLoad(STEP_MOTOR_PARAM7_ADDR,&(stepMotorVar[STEP_MOTOR_NO7]->attri),6);
    StepMotorParamLoad(STEP_MOTOR_PARAM8_ADDR,&(stepMotorVar[STEP_MOTOR_NO8]->attri),7);
    StepMotorParamLoad(STEP_MOTOR_PARAM9_ADDR,&(stepMotorVar[STEP_MOTOR_NO9]->attri),8);
    StepMotorParamLoad(STEP_MOTOR_PARAM10_ADDR,&(stepMotorVar[STEP_MOTOR_NO10]->attri),9);
    StepMotorParamLoad(STEP_MOTOR_PARAM11_ADDR,&(stepMotorVar[STEP_MOTOR_NO11]->attri),10);
    StepMotorParamLoad(STEP_MOTOR_PARAM12_ADDR,&(stepMotorVar[STEP_MOTOR_NO12]->attri),11);
    //初始化12个电机状态
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO1]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO2]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO3]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO4]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO5]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO6]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO7]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO8]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO9]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO10]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO11]->var));
    StepMotorStateInit(&(stepMotorVar[STEP_MOTOR_NO12]->var));
    return;
}

//将步进电机错误代码转换为系统错误代码
ERROR_SUB StepMotorConvertErrorSub(STEP_MOTOR_NO motorNo,STEP_MOTOR_SUB_STATE state)
{
    switch(state)
    {
        case STEP_MOTOR_STOP_SUB_NORMAL:
            return ERROR_SUB_OK;
        case STEP_MOTOR_STOP_SUB_EMERGENCY_CMD:
            return StepMotorErrorSubEmStopCmd(motorNo);
        case STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL:
            return StepMotorErrorSubEmStopSingal(motorNo);
        case STEP_MOTOR_STOP_SUB_DACC1:
            return StepMotorErrorSubStopUtil1(motorNo);
        case STEP_MOTOR_STOP_SUB_DACC2:
            return StepMotorErrorSubStopUtil2(motorNo);
        case STEP_MOTOR_STOP_SUB_TIMEOUT:
            return StepMotorErrorSubStopTimeOut(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_STEP_LIMIT:
            return StepMotorErrorSubStopResetStepLimit(motorNo);
        case STEP_MOTOR_STOP_SUB_SLAVE_READY_FAULT:
            return StepMotorErrorSubStopSlaveReadyFault(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_LEAVE_ZERO_LIMIT:
            return StepMotorErrorSubStopLeaveZeroLimit(motorNo);
        case STEP_MOTOR_STOP_SUB_RESET_DACC_LOSS:
            return StepMotorErrorSubStopResetDaccLoss(motorNo);
        case STEP_MOTOR_STOP_SUB_UNKNOW:
            return StepMotorErrorSubStopUnknow(motorNo);
        default:
            return ERROR_SUB_OK;
    }
}

//指定步进电机立即停止
ERROR_SUB StepMotorStopImmediately(STEP_MOTOR_NO motorNo)
{
    //步进电机状态切换为急停
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
    //上次的步数加上
    StepMotorUtilReflushCoordinate(motorNo);
    return ERROR_SUB_OK;
}



//指定步进电机减速停止,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration(STEP_MOTOR_NO motorNo)
{
    //只有在持续运行状态,且没到减速状态可以调用这个减速停止
    if(stepMotorVar[motorNo]->var.motorMainState == STEP_MOTOR_MAIN_STATE_RUN_ALWAYS)
    {
        if(stepMotorVar[motorNo]->var.motorSubState != STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE)
        {
            if(stepMotorVar[motorNo]->var.currentTableMaxLength == 1)
            {
                //匀速运动,步进电机状态切换为急停
                stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
                //子状态急停
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
                //步进电机写入急停
                BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
                //上次的步数加上
                StepMotorUtilReflushCoordinate(motorNo);
            }
            else
            {
                //步数表索引切入对称点
                stepMotorVar[motorNo]->var.currentTableIndex = 
                    stepMotorVar[motorNo]->var.currentTableMaxLength - stepMotorVar[motorNo]->var.currentTableIndex - 1;
                //状态切入减速段
                stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_RUN_ALWAYS_SUB_DECELERATE;
                //运行到下一步
                BoardCPLD_WriteStepMotorRunNextStep((BOARD_CPLD_MOTOR)motorNo);
                //更新当前坐标
                StepMotorUtilReflushCoordinate(motorNo);
            }
            
        }
    }
    else
    {
        //步进电机状态切换为急停
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //子状态急停
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_NORMAL;
        //步进电机写入急停
        BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
        //上次的步数加上
        StepMotorUtilReflushCoordinate(motorNo);
    }
    //等待运行完成
    while(stepMotorVar[motorNo]->var.motorMainState != STEP_MOTOR_MAIN_STATE_STOP)
    {
        //等待步进电机状态机停止,进行延时
        CoreDelayMinTick();
    }
    return ERROR_SUB_OK;
}



//指定步进电机急停
ERROR_SUB StepMotorStopEmergency(STEP_MOTOR_NO motorNo)
{
    //步进电机状态切换为急停
    stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
    //子状态急停
    stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_CMD;
    //步进电机写入急停
    BoardCPLD_StepMotorEmergencyStop((BOARD_CPLD_MOTOR)motorNo);
    //上次的步数加上
    StepMotorUtilReflushCoordinate(motorNo);
    return ERROR_SUB_OK;
}



//读取电机传感器状态
void StepMotorReadSensorState(STEP_MOTOR_NO motorNo,PIN_STATE* originSensorPtr,PIN_STATE* emStopSensorPtr,
                                PIN_STATE* utilStop1SensorPtr,PIN_STATE* utilStop2SensorPtr)
{
    GPIO_PinState pinStateReal;
    //复位
    if(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex == BOARD_PORT_IN_NULL)
    {
        *originSensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.resetPinIndex);
        if(pinStateReal == stepMotorVar[motorNo]->attri.baseConfig.resetPinValidLevel)
        {
            *originSensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *originSensorPtr = PIN_STATE_INVALID;
        }
    }
    //急停
    if(stepMotorVar[motorNo]->attri.baseConfig.emStopPinIndex == BOARD_PORT_IN_NULL)
    {
        *emStopSensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.emStopPinIndex);
        if(pinStateReal == stepMotorVar[motorNo]->attri.baseConfig.emStopPinValidLevel)
        {
            *emStopSensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *emStopSensorPtr = PIN_STATE_INVALID;
        }
    }
    //辅助急停1
    if(stepMotorVar[motorNo]->attri.baseConfig.utilStopPin1Index == BOARD_PORT_IN_NULL)
    {
        *utilStop1SensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.utilStopPin1Index);
        if(pinStateReal == stepMotorVar[motorNo]->attri.baseConfig.utilStopPin1ValidLevel)
        {
            *utilStop1SensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *utilStop1SensorPtr = PIN_STATE_INVALID;
        }
    }
    //辅助急停2
    if(stepMotorVar[motorNo]->attri.baseConfig.utilStopPin2Index == BOARD_PORT_IN_NULL)
    {
        *utilStop2SensorPtr = PIN_STATE_INVALID;
    }
    else
    {
        pinStateReal = BoardPortInReadState(stepMotorVar[motorNo]->attri.baseConfig.utilStopPin2Index);
        if(pinStateReal == stepMotorVar[motorNo]->attri.baseConfig.utilStopPin2ValidLevel)
        {
            *utilStop2SensorPtr = PIN_STATE_VALID;
        }
        else
        {
            *utilStop2SensorPtr = PIN_STATE_INVALID;
        }
    }
}
















