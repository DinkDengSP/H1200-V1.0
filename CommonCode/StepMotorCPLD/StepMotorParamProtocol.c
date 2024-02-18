/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 15:26:32 +0800
************************************************************************************************/ 
#include "StepMotor.h"


//步进电机参数读写
//步进电机协议参数读取
uint32_t StepMotorGetParamWithProtocol(uint8_t motorNo,uint8_t paramIndex,uint16_t utilParamIndex)
{
    STEP_MOTOR_ATTRIBUTE* motorAttri = &(stepMotorVar[motorNo]->attri);
    int32_t paramUtil = 0;
    uint32_t paramResult = 0;
    //加速曲线的条目
    uint8_t subCurveIndex = 0;
    //加速曲线的偏移
    uint32_t subCurveUtil = 0;
    if(paramIndex == 0)
    {
        paramUtil = motorAttri->baseConfig.resetPinIndex;
    }
    else if(paramIndex == 1)
    {
        paramUtil = motorAttri->baseConfig.resetPinValidLevel;
    }
    else if(paramIndex == 2)
    {
        paramUtil = motorAttri->baseConfig.dir;
    }
    else if(paramIndex == 3)
    {
        paramUtil = motorAttri->baseConfig.emStopPinIndex;
    }
    else if(paramIndex == 4)
    {
        paramUtil = motorAttri->baseConfig.emStopPinValidLevel;
    }
    else if(paramIndex == 5)
    {
        paramUtil = motorAttri->baseConfig.utilStopPin1Index;
    }
    else if(paramIndex == 6)
    {
        paramUtil = motorAttri->baseConfig.utilStopPin1ValidLevel;
    }
    else if(paramIndex == 7)
    {
        paramUtil = motorAttri->baseConfig.utilStopPin2Index;
    }
    else if(paramIndex == 8)
    {
        paramUtil = motorAttri->baseConfig.utilStopPin2ValidLevel;
    }
    else if(paramIndex == 9)
    {
        paramUtil = motorAttri->baseConfig.resetMaxSteps;
    }
    else if(paramIndex == 10)
    {
        paramUtil = motorAttri->baseConfig.reverseMaxSteps;
    }
    else if(paramIndex == 11)
    {
        paramUtil = motorAttri->baseConfig.positiveMaxSteps;
    }
    else if(paramIndex == 12)
    {
        paramUtil = motorAttri->baseConfig.dacc2StopSteps;
    }
    else if(paramIndex == 13)
    {
        //浮点数放大1000 倍
        paramUtil = (int32_t)(1000*(motorAttri->baseConfig.resetSpeedFast));
    }
    else if(paramIndex == 14)
    {
        paramUtil = motorAttri->baseConfig.singlePluseMax;
    }
    else
    {
        subCurveIndex = (paramIndex-15)/10;
        subCurveUtil = (paramIndex-15)%10;
        if(subCurveUtil == 0)
        {
            paramUtil = (motorAttri->speedTableCollect[subCurveIndex]).stepMode;
        }
        else if(subCurveUtil == 1)
        {
            paramUtil = (motorAttri->speedTableCollect[subCurveIndex]).startStep;
        }
        else if(subCurveUtil == 2)
        {
            paramUtil = (int32_t)(1000*(motorAttri->speedTableCollect[subCurveIndex]).startSpeed);
        }
        else if(subCurveUtil == 3)
        {
            paramUtil = (int32_t)(1000*(motorAttri->speedTableCollect[subCurveIndex]).startAcc);
        }
        else if(subCurveUtil == 4)
        {
            paramUtil = (int32_t)(1000*(motorAttri->speedTableCollect[subCurveIndex]).accOfAcc);
        }
        else if(subCurveUtil == 5)
        {
            paramUtil = (int32_t)(1000*(motorAttri->speedTableCollect[subCurveIndex]).maxAcc);
        }
        else if(subCurveUtil == 6)
        {
            paramUtil = (int32_t)(1000*(motorAttri->speedTableCollect[subCurveIndex]).maxSpeed);
        }
    }

    paramResult = *((uint32_t*)(&paramUtil));
    return paramResult;
}

//步进电机协议参数写入
void StepMotorSetParamWithProtocol(uint8_t motorNo,uint8_t paramIndex,uint16_t utilParamIndex,uint32_t writeParam)
{
    STEP_MOTOR_ATTRIBUTE* motorAttri = &(stepMotorVar[motorNo]->attri);
    //加速曲线的条目
    uint8_t subCurveIndex = 0;
    //加速曲线的偏移
    uint32_t subCurveUtil = 0;
    if(paramIndex == 0)
    {
        motorAttri->baseConfig.resetPinIndex = (BOARD_PORT_IN)writeParam;
    }
    else if(paramIndex == 1)
    {
        motorAttri->baseConfig.resetPinValidLevel = (GPIO_PinState)writeParam;
    }
    else if(paramIndex == 2)
    {
        motorAttri->baseConfig.dir = (STEP_MOTOR_DIR)writeParam;
    }
    else if(paramIndex == 3)
    {
        motorAttri->baseConfig.emStopPinIndex = (BOARD_PORT_IN)writeParam;
    }
    else if(paramIndex == 4)
    {
        motorAttri->baseConfig.emStopPinValidLevel = (GPIO_PinState)writeParam;
    }
    else if(paramIndex == 5)
    {
        motorAttri->baseConfig.utilStopPin1Index = (BOARD_PORT_IN)writeParam;
    }
    else if(paramIndex == 6)
    {
        motorAttri->baseConfig.utilStopPin1ValidLevel = (GPIO_PinState)writeParam;
    }
    else if(paramIndex == 7)
    {
        motorAttri->baseConfig.utilStopPin2Index = (BOARD_PORT_IN)writeParam;
    }
    else if(paramIndex == 8)
    {
        motorAttri->baseConfig.utilStopPin2ValidLevel = (GPIO_PinState)writeParam;
    }
    else if(paramIndex == 9)
    {
        motorAttri->baseConfig.resetMaxSteps = writeParam;
    }
    else if(paramIndex == 10)
    {
        motorAttri->baseConfig.reverseMaxSteps  = (int32_t)writeParam;
    }
    else if(paramIndex == 11)
    {
        motorAttri->baseConfig.positiveMaxSteps  = (int32_t)writeParam;
    }
    else if(paramIndex == 12)
    {
        motorAttri->baseConfig.dacc2StopSteps  = writeParam;
    }
    else if(paramIndex == 13)
    {
        //浮点数放大1000 倍
        motorAttri->baseConfig.resetSpeedFast = (float)(writeParam)/(1000.0f);
    }
    else if(paramIndex == 14)
    {
        motorAttri->baseConfig.singlePluseMax = (uint8_t)writeParam;
    }
    else
    {
        subCurveIndex = (paramIndex-15)/10;
        subCurveUtil = (paramIndex-15)%10;
        if(subCurveUtil == 0)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).stepMode = (STEP_MODE)writeParam;
        }
        else if(subCurveUtil == 1)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).startStep = (uint32_t)writeParam;
        }
        else if(subCurveUtil == 2)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).startSpeed = (float)(writeParam)/(1000.0f);
        }
        else if(subCurveUtil == 3)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).startAcc = (float)(writeParam)/(1000.0f);
        }
        else if(subCurveUtil == 4)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).accOfAcc = (float)(writeParam)/(1000.0f);
        }
        else if(subCurveUtil == 5)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).maxAcc = (float)(writeParam)/(1000.0f);
        }
        else if(subCurveUtil == 6)
        {
            (motorAttri->speedTableCollect[subCurveIndex]).maxSpeed = (float)(writeParam)/(1000.0f);
        }
    }
}

//保存步进电机参数
void StepMotorSaveParamWithProtocol(void)
{
    StepMotorParamSet(STEP_MOTOR_PARAM1_ADDR,&(stepMotorVar[STEP_MOTOR_NO1]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM2_ADDR,&(stepMotorVar[STEP_MOTOR_NO2]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM3_ADDR,&(stepMotorVar[STEP_MOTOR_NO3]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM4_ADDR,&(stepMotorVar[STEP_MOTOR_NO4]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM5_ADDR,&(stepMotorVar[STEP_MOTOR_NO5]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM6_ADDR,&(stepMotorVar[STEP_MOTOR_NO6]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM7_ADDR,&(stepMotorVar[STEP_MOTOR_NO7]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM8_ADDR,&(stepMotorVar[STEP_MOTOR_NO8]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM9_ADDR,&(stepMotorVar[STEP_MOTOR_NO9]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM10_ADDR,&(stepMotorVar[STEP_MOTOR_NO10]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM11_ADDR,&(stepMotorVar[STEP_MOTOR_NO11]->attri));
    StepMotorParamSet(STEP_MOTOR_PARAM12_ADDR,&(stepMotorVar[STEP_MOTOR_NO12]->attri));

}
