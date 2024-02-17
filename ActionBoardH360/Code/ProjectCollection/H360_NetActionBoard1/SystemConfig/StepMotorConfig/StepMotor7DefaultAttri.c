/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-01 08:58:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 16:37:09 +0800
************************************************************************************************/ 
#include "StepMotorParam.h"
//步进电机7:  清洗摇匀升降(描述电机名称)
//步进电机默认配置
const STEP_MOTOR_ATTRIBUTE StepMotor7DefaultAttri = {
    SM_CONFIG_FLAG,
    //基础配置
    {
        BOARD_PORT_IN_ARM_SEN7_PE4,//BOARD_IN_PIN resetPinIndex;复位引脚
        GPIO_PIN_RESET,//GPIO_PinState resetPinValidLevel;复位有效电平
        STEP_MOTOR_DIR_CW,//STEP_MOTOR_DIR dir;运行正方向
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
        4000,//uint32_t resetMaxSteps;电机复位最大行程
        0,//uint32_t resetDaccSteps,到零点后减速停止的步数,不能挡片遮挡原点传感器的间隙步数
        0,//uint32_t resetOutSensorSteps 复位时脱离零点运行的步数
        -100,//int32_t reverseMaxSteps;反向最大坐标
        3600,//int32_t positiveMaxSteps;正向最大坐标
        100,//uint32_t dacc2StopSteps;减速停止的减速步数
        87.5f,//float resetSpeedFast;复位速度,快速,主要用在找原点
        50.0f,//float resetSpeedSlow;复位速度,慢速,主要用在脱离原点和二次回到原点
        255//uint8_t singlePluseMax;单次脉冲发送量
    },
    //速度表配置,每个电机带六个速度表
    {
        //速度曲线0,摇匀升降速度
        {   
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线1,摇匀升降升起来的速度,模式1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线2,摇匀升降降下去的速度,模式1
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线3,摇匀升降升起来的速度,模式2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线4,摇匀升降降下去的速度,模式2
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线5,摇匀升降升起来的速度,模式3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线6,摇匀升降降下去的速度,模式3
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线7,摇匀升降升起来的速度,模式4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线8,摇匀升降降下去的速度,模式4
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线9,摇匀升降升起来的速度,模式5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线10,摇匀升降降下去的速度,模式5
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线11,摇匀升降升起来的速度,模式6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线12,摇匀升降降下去的速度,模式6
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线13,摇匀升降升起来的速度,模式7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线14,摇匀升降降下去的速度,模式7
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线15,摇匀升降升起来的速度,模式8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线16,摇匀升降降下去的速度,模式8
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线17,摇匀升降升起来的速度,模式9
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线18,摇匀升降降下去的速度,模式9
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
        //速度曲线19,摇匀升降升起来的速度,模式10
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            50.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            250.0f,//maxSpeed 最大速度
        },
        //速度曲线20,,摇匀升降降下去的速度,模式10
        {
            SAME_TIME_FRAME,//STEP_MODE stepMode;步数适配模式,等步数或者等时间帧
            2,//uint32_t startStep;启动步数设定
            100.0f,//startSpeed 启动速度设定
            0.0f,//startAcc启动加速度
            0.125f,//accOfAcc加加速(加速度的加速度)
            2.5f,//maxAcc 最大加速度
            400.0f,//maxSpeed 最大速度
        },
    }
};
