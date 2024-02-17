/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 15:43:34 +0800
************************************************************************************************/ 
#include "StepMotor.h"
#include "UserMemManage.h"
#include "StepMotorAlarm.h"

/**********************************************电机辅助急停减速函数声明********************************/
//辅助急停1减速起始计算减速曲线
static void StepMotorProcDaccSubStart1(STEP_MOTOR_NO motorNo);
//辅助急停2减速起始计算减速曲线
static void StepMotorProcDaccSubStart2(STEP_MOTOR_NO motorNo);
//辅助急停1正常减速曲线发送
static void StepMotorProcDaccSubNormal1(STEP_MOTOR_NO motorNo);
//辅助急停2正常减速曲线发送
static void StepMotorProcDaccSubNormal2(STEP_MOTOR_NO motorNo);
/***********************************************************************************************/

//减速运行
void CpldStepMotorRunDacc(STEP_MOTOR_NO motorNo)
{
    //子状态处理
    switch(stepMotorVar[motorNo]->var.motorSubState)
    {
        case STEP_MOTOR_DACC1_SUB_START:
            //刚发生辅助急停1的中断,现在开始计算减速曲线
            StepMotorProcDaccSubStart1(motorNo);
            break;
        case STEP_MOTOR_DACC2_SUB_START:
            //刚发生辅助急停2的中断,现在开始计算减速曲线
            StepMotorProcDaccSubStart2(motorNo);
            break;
        case STEP_MOTOR_DACC1_SUB_NORMAL:
            //减速曲线1运行中
            StepMotorProcDaccSubNormal1(motorNo);
            break;
        case STEP_MOTOR_DACC2_SUB_NORMAL:
            //减速曲线2运行中
            StepMotorProcDaccSubNormal2(motorNo);
            break;
        default:
            //不识别的状态,直接停掉
            stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
            stepMotorVar[motorNo]->var.motorSubState =  STEP_MOTOR_STOP_SUB_UNKNOW;
            break;
    }
}



/*****************************************************系统减速急停相关状态机处理函数*************************************************/
//发生辅助急停,现在要在这里计算减速曲线
static void StepMotorProcDaccSubStart1(STEP_MOTOR_NO motorNo)
{
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    if(stepMotorVar[motorNo]->attri.baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //指示减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(motorNo);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(motorNo,stepMotorVar[motorNo]);
        //切换到减速表发送过程
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
        //指示减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_DACC1_SUB_NORMAL;
    }
}

//辅助急停2发生,现在在此处计算减速曲线
static void StepMotorProcDaccSubStart2(STEP_MOTOR_NO motorNo)
{
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    if(stepMotorVar[motorNo]->attri.baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //指示减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(motorNo);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(motorNo,stepMotorVar[motorNo]);
        //切换到减速表发送过程
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_DACC;
        //指示减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_DACC2_SUB_NORMAL;
    }
}

//步进电机减速步数发送
static void StepMotorProcDaccSubNormal1(STEP_MOTOR_NO motorNo)
{
    uint32_t tabDaccIndex = stepMotorVar[motorNo]->var.currentDaccTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    if(tabDaccIndex >= stepMotorVar[motorNo]->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        if(stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] > stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax)
        {
            //大于单次发送最大步数,一次发满
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
            //步数表实时减小
            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //小于最大脉冲数,一次发完
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex]
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex];
            //一次发完,步数表归零
            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] = 0;
            //步数表往前加一步
            stepMotorVar[motorNo]->var.currentDaccTableIndex += 1;
        }
    }
}

static void StepMotorProcDaccSubNormal2(STEP_MOTOR_NO motorNo)
{
    uint32_t tabDaccIndex = stepMotorVar[motorNo]->var.currentDaccTableIndex;
    //更新当前坐标
    StepMotorUtilReflushCoordinate(motorNo);
    if(tabDaccIndex >= stepMotorVar[motorNo]->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVar[motorNo]->var.motorMainState = STEP_MOTOR_MAIN_STATE_STOP;
        //减速停止
        stepMotorVar[motorNo]->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        if(stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] > stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax)
        {
            //大于单次发送最大步数,一次发满
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
            //步数表实时减小
            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= stepMotorVar[motorNo]->attri.baseConfig.singlePluseMax;
        }
        else
        {
            //小于最大脉冲数,一次发完
            BoardCPLD_WriteStepMotorSingle((BOARD_CPLD_MOTOR)motorNo,
                                            BOARD_CPLD_MOTOR_EN_ENABLE,
                                            (BOARD_CPLD_MOTOR_DIR)stepMotorVar[motorNo]->var.currentDir,
                                            BOARD_CPLD_MOTOR_STOP_OFF,
                                            stepMotorVar[motorNo]->var.stepMotorDaccSpeedTableDirect[tabDaccIndex],
                                            (uint8_t)(stepMotorVar[motorNo]->attri.baseConfig.div),
                                            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex]
                                            );
            //记录上次发送脉冲
            stepMotorVar[motorNo]->var.lastSendOutPluse = stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex];
            //一次发完,步数表归零
            stepMotorVar[motorNo]->var.stepMotorDaccStepTableDirect[tabDaccIndex] = 0;
            //步数表往前加一步
            stepMotorVar[motorNo]->var.currentDaccTableIndex += 1;
        }
    }
}

/*********************************************************************************************************************************/




