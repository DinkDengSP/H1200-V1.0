/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-01 08:58:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 13:01:17 +0800
************************************************************************************************/ 
#include "StepMotorParam.h"
//步进电机7:  试剂针注射泵(描述电机名称)
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotor7DefaultAttri = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_PORT_IN_ARM_SEN7_PE4,//BOARD_IN_PIN resetPinIndex;复位引脚
        GPIO_PIN_SET,//GPIO_PinState resetPinValidLevel;复位有效电平
        STEP_MOTOR_DIR_CCW,//STEP_MOTOR_DIR dir;运行正方向
        STEP_MOTOR_DIV_8,//STEP_MOTOR_DIV div;电机细分
        BOARD_PORT_IN_NULL,//BOARD_IN_PIN emStopPinIndex;急停IO
        GPIO_PIN_RESET,//GPIO_PinState emStopPinValidLevel;急停有效电平
        BOARD_PORT_IN_NULL,//BOARD_IN_PIN utilStopPin1Index;辅助急停IO
        GPIO_PIN_RESET,//GPIO_PinState utilStopPin1ValidLevel;辅助急停有效电平
        BOARD_PORT_IN_NULL,//BOARD_IN_PIN utilStopPin2Index;辅助急停IO2
        GPIO_PIN_RESET,//GPIO_PinState utilStopPin2ValidLevel;辅助急停2有效电平
        BOARD_PORT_IN_NULL,//BOARD_PORT_IN limitPointPinIndex;限位点引脚,适用于复位原点在中间,当检测到限位点后,切换方向为复位运行的反方向
        GPIO_PIN_RESET,//PIO_PinState limitPointPinValidLevel;限位点有效电平
        BOARD_PORT_IN_NULL,//BOARD_PORT_IN readyOkPinIndex;电机准备OK,用于故障检测
        GPIO_PIN_RESET,//GPIO_PinState readyOkPinValidLevel;电机准备有效电平
        17000,//uint32_t resetMaxSteps;电机复位最大行程
        0,//uint32_t resetDaccSteps,到零点后减速停止的步数,不能挡片遮挡原点传感器的间隙步数
        0,//uint32_t resetOutSensorSteps 复位时脱离零点运行的步数
        -500,//int32_t reverseMaxSteps;反向最大坐标
        16000,//int32_t positiveMaxSteps;正向最大坐标
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        100.0f,//float resetSpeedFast;复位速度,快速,主要用在找原点
        50.0f,//float resetSpeedSlow;复位速度,慢速,主要用在脱离原点和二次回到原点
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,调试曲线
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            25.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            1.875f,//maxAcc 最大加速度
            106.25f,//maxSpeed 最大速度
        },
        //速度曲线1,试剂针注射泵灌注时吸液速度  
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            162.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            650.0f,//maxSpeed 最大速度
        },
        //速度曲线2,试剂针注射泵灌注时排液速度 
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            162.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            650.0f,//maxSpeed 最大速度
        },
        //速度曲线3,试剂针注射泵清洗完成后吸空气速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            212.5f,//maxSpeed 最大速度
        },
        //速度曲线4,试剂针注射泵吸液完成后吸空气速度
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            212.5f,//maxSpeed 最大速度
        },
        //速度曲线5,试剂针注射泵吸液速度 1-40
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            15.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            1.5f,//maxAcc 最大加速度
            62.5f,//maxSpeed 最大速度
        },
        //速度曲线6,试剂针注射泵吸液速度 41-70
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            31.25f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.25f,//maxAcc 最大加速度
            125.0f,//maxSpeed 最大速度
        },
        //速度曲线7,试剂针注射泵吸液速度 71-90
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            43.75f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            162.5f,//maxSpeed 最大速度
        },
        //速度曲线8,试剂针注射泵吸液速度 91-115
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            200.0f,//maxSpeed 最大速度
        },
        //速度曲线9,试剂针注射泵吸液速度 116-140
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            62.5f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线10,试剂针注射泵吸液速度 141-180
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            75.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            3.125f,//maxAcc 最大加速度
            300.0f,//maxSpeed 最大速度
        },
        //速度曲线11,试剂针注射泵吸液速度 181-225
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            87.5f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            3.125f,//maxAcc 最大加速度
            375.0f,//maxSpeed 最大速度
        },
        //速度曲线12,试剂针注射泵吸液速度 226-280
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            125.0f,//startSpeed 启动速度设定
            0.0f,//startAcc 启动加速度
            0.125f,//accOfAcc 加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            462.5f,//maxSpeed 最大速度
        },
        //速度曲线13,试剂针注射泵吸液速度 281-400
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            162.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            650.0f,//maxSpeed 最大速度
        },
        //速度曲线14,试剂针注射泵吸液速度 401-600
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            162.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            650.0f,//maxSpeed 最大速度
        },
        //速度曲线15,试剂针注射泵注液速度 0-70
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            43.75f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            1.75f,//maxAcc 最大加速度
            200.0f,//maxSpeed 最大速度
        },
        //速度曲线16,试剂针注射泵注液速度 71-260
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            125.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.5,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            325.0f,//maxSpeed 最大速度
        },
        //速度曲线17,试剂针注射泵注液速度 261-1000
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            162.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            3.75f,//maxAcc 最大加速度
            650.0f,//maxSpeed 最大速度
        },
        //速度曲线18
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            12.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            1.25f,//maxAcc 最大加速度
            50.0f,//maxSpeed 最大速度
        },
        //速度曲线19
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            12.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            1.25f,//maxAcc 最大加速度
            50.0f,//maxSpeed 最大速度
        },
        //速度曲线20
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            1,//uint32_t startStep;启动步数设定
            12.5f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            1.25f,//maxAcc 最大加速度
            50.0f,//maxSpeed 最大速度
        },
    }
};
