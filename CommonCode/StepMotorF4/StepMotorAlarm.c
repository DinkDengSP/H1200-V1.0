/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:21:26
**LastEditors: DengXiaoJun
**LastEditTime: 2021-04-01 17:37:09
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotor.h"

//步进电机1急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor1()
{
    //步进电机状态切换为急停
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机2急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor2()
{
    //步进电机状态切换为急停
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
/*
//步进电机3急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor3()
{
    //步进电机状态切换为急停
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机4急停端口事件的中断回调函数
void StepMotorEmergencyStopByIntSingalMotor4()
{
    //步进电机状态切换为急停
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState = STEP_MOTOR_MAIN_STOP;
    //子状态急停
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorSubState = STEP_MOTOR_STOP_SUB_EMERGENCY_SIGNAL;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.emergencyStopPinIndex);
    //解除辅助急停1 2 的中断
    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
*/
//急停中断回调函数数组
const MCU_Exti_CallBackFuncPtr StepMotorEmergencyStopByIntSingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT] = {
    StepMotorEmergencyStopByIntSingalMotor1,
    StepMotorEmergencyStopByIntSingalMotor2,
    //StepMotorEmergencyStopByIntSingalMotor3,
    //StepMotorEmergencyStopByIntSingalMotor4,
};


//步进电机1辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor1(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机2辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor2(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
/*
//步进电机3辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor3(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机4辅助急停中断回调函数
void StepMotorDaccStopByUtilInt1SingalMotor4(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin1Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorSubState = STEP_MOTOR_DACC1_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin1Index);
    //关闭急停和辅助急停2的中断
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停2
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin2Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
*/

//步进电机辅助急停1中断函数数组
const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt1SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT] = {
    StepMotorDaccStopByUtilInt1SingalMotor1,
    StepMotorDaccStopByUtilInt1SingalMotor2,
    //StepMotorDaccStopByUtilInt1SingalMotor3,
    //StepMotorDaccStopByUtilInt1SingalMotor4,
};

//步进电机1辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor1(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO1].var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO1].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO1].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机2辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor2(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO2].var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO2].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO2].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
/*
//步进电机3辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor3(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO3].var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO3].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO3].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}

//步进电机4辅助急停中断回调函数
void StepMotorDaccStopByUtilInt2SingalMotor4(void)
{
    //只有在系统不是停止状态才能切入DACC减速状态
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState == STEP_MOTOR_MAIN_STOP)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //如果已经是减速状态,就不能再次切入减速状态,防止重新计算减速曲线
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState == STEP_MOTOR_MAIN_DACC)
    {
        //监控状态变化
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
        //将自身中断注册解除,可以肯定运行到这里是注册了的
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin2Index);
        return;
    }
    //切换状态到减速模式的刚开始减速的状态
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorMainState = STEP_MOTOR_MAIN_DACC;
    stepMotorVarArray[STEP_MOTOR_NO4].var.motorSubState = STEP_MOTOR_DACC2_SUB_START;
    //监控状态变化
    stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop2 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    //将自身中断注册解除,可以肯定运行到这里是注册了的
    BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin2Index);
    //关闭急停与辅助急停1
    //关闭急停
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.emergencyStopPinIndex);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateEmergency = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }

    //关闭辅助急停1
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 == STEP_MOTOR_SINGAL_STATE_ENABLE_REGISTER)
    {
        //中断注册,解除注册
        BoardExtiUnRegister(stepMotorVarArray[STEP_MOTOR_NO4].attriPtr->baseConfig.utilStopPin1Index);
    }
    if(stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 != STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT)
    {
        //关闭中断使能
        stepMotorVarArray[STEP_MOTOR_NO4].var.stepMotorSingalStateUtilStop1 = STEP_MOTOR_SINGAL_STATE_DISABLE_UNSUPPORT;
    }
}
*/

//步进电机辅助急停2中断函数数组
const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt2SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT] = {
    StepMotorDaccStopByUtilInt2SingalMotor1,
    StepMotorDaccStopByUtilInt2SingalMotor2,
    //StepMotorDaccStopByUtilInt2SingalMotor3,
    //StepMotorDaccStopByUtilInt2SingalMotor4,
};
