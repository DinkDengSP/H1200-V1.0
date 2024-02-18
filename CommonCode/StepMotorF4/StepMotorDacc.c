/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:21:12
**LastEditors: DengXiaoJun
**LastEditTime: 2021-04-02 16:33:55
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"
#include "StepMotorUtil.h"
#include "StepMotorTable.h"

/**********************************************电机辅助急停减速函数声明********************************/
//辅助急停1减速起始计算减速曲线
static void StepMotorDaccIntProcessSubStart1(STEP_MOTOR_VAR* stepMotorVarPtr);
//辅助急停2减速起始计算减速曲线
static void StepMotorDaccIntProcessSubStart2(STEP_MOTOR_VAR* stepMotorVarPtr);
//辅助急停1正常减速曲线发送
static void StepMotorDaccIntProcessSubNormal1(STEP_MOTOR_VAR* stepMotorVarPtr);
//辅助急停2正常减速曲线发送
static void StepMotorDaccIntProcessSubNormal2(STEP_MOTOR_VAR* stepMotorVarPtr);
/***********************************************************************************************/

//减速运行
void StepMotorRunDaccIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    //子状态处理
    switch(stepMotorVarPtr->var.motorSubState)
    {
        case STEP_MOTOR_DACC1_SUB_START:
            //刚发生辅助急停1的中断,现在开始计算减速曲线
            StepMotorDaccIntProcessSubStart1(stepMotorVarPtr);
            break;
        case STEP_MOTOR_DACC2_SUB_START:
            //刚发生辅助急停2的中断,现在开始计算减速曲线
            StepMotorDaccIntProcessSubStart2(stepMotorVarPtr);
            break;
        case STEP_MOTOR_DACC1_SUB_NORMAL:
            //减速曲线1运行中
            StepMotorDaccIntProcessSubNormal1(stepMotorVarPtr);
            break;
        case STEP_MOTOR_DACC2_SUB_NORMAL:
            //减速曲线2运行中
            StepMotorDaccIntProcessSubNormal2(stepMotorVarPtr);
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



/*****************************************************系统减速急停相关状态机处理函数*************************************************/
//发生辅助急停,现在要在这里计算减速曲线
static void StepMotorDaccIntProcessSubStart1(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    if(stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //指示减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(stepMotorVarPtr);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(stepMotorVarPtr);
        //切换到减速表发送过程
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_DACC;
        //指示减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_DACC1_SUB_NORMAL;
    }
}

//辅助急停2发生,现在在此处计算减速曲线
static void StepMotorDaccIntProcessSubStart2(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    if(stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps == 0)
    {
        //减速步数为0,切换到停止状态
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //指示减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        //计算步进电机减速表
        StepMotorTableCalcDaccTable(stepMotorVarPtr);
        //发出减速表第一步
        StepMotorUtilSendDaccStartPluse(stepMotorVarPtr);
        //切换到减速表发送过程
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_DACC;
        //指示减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_DACC2_SUB_NORMAL;
    }
}

//步进电机减速步数发送
static void StepMotorDaccIntProcessSubNormal1(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t tabDaccIndex = stepMotorVarPtr->var.currentDaccTableIndex;
    if(tabDaccIndex >= stepMotorVarPtr->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC1;
    }
    else
    {
        //发出下一个脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[tabDaccIndex]);
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= 1;
        if(stepMotorVarPtr->var.stepMotorDaccStepTableDirect[tabDaccIndex] == 0)
        {
            //步数表往前加一步
            stepMotorVarPtr->var.currentDaccTableIndex += 1;
        }
    }
}

static void StepMotorDaccIntProcessSubNormal2(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t tabDaccIndex = stepMotorVarPtr->var.currentDaccTableIndex;
    if(tabDaccIndex >= stepMotorVarPtr->var.currentDaccTableMaxLength)
    {
        //脉冲发送完成,切换到停止状态
        stepMotorVarPtr->var.motorMainState = STEP_MOTOR_MAIN_STOP;
        //减速停止
        stepMotorVarPtr->var.motorSubState = STEP_MOTOR_STOP_SUB_DACC2;
    }
    else
    {
        //发出下一个脉冲
        stepMotorVarPtr->attriPtr->callBackFuncConfig.stepMotorStart(stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[tabDaccIndex]);
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[tabDaccIndex] -= 1;
        if(stepMotorVarPtr->var.stepMotorDaccStepTableDirect[tabDaccIndex] == 0)
        {
            //步数表往前加一步
            stepMotorVarPtr->var.currentDaccTableIndex += 1;
        }
    }
}
