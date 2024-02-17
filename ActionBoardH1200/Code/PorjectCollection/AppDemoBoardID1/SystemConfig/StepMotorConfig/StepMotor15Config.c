/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-05 20:19:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-05 20:23:43 +0800
************************************************************************************************/ 
#include "StepMotorType.h"

//步进电机15属性集合----(请在括号中描述电机具体功能)
const STEP_MOTOR_ATTRI stepMotor15Attri = {
    //电机传感器属性
    {
        //复位传感器是否安装
        DISABLE,
        //复位传感器有效电平
        GPIO_PIN_RESET,
        //急停传感器是否安装
        DISABLE,
        //急停传感器有效电平
        GPIO_PIN_RESET,
        //辅助急停传感器是否安装
        DISABLE,
        //辅助急停传感器有效电平
        GPIO_PIN_RESET,
        //电机使能有效信号
        GPIO_PIN_RESET,
        //电机正方向有效信号
        GPIO_PIN_RESET,
        //电机脉冲信号空闲电平
        GPIO_PIN_SET,
    },
    //电机编码器属性
    {
        //编码器是否使能
        DISABLE,
        //编码器A相有效信号电平
        GPIO_PIN_SET,
        //编码器B相有效信号电平
        GPIO_PIN_SET,
        //编码器坐标与电机坐标之间的比例关系
        1.00,
        //编码器最大允许的丢步范围,超过该范围无法修正,运行过程
        5000,
        //编码器许可的误差范围,在该范围内无需修正
        20,
        //编码器最大允许的丢步范围,超过该范围无法修正,复位过程
        100,
        //实时修正修正阈值,当运行时编码器差值阈值超过多少就要进行实时的修正,没超过就等到运行完成再修正
        100,
        //编码器尝试修正的重试次数
        10,
        //编码器位置修正模式
        STEP_MOTOR_ENCODER_FIXES_STOP,
    },
    //电机运行时属性
    {
        //电机是否存在
        ENABLE,
        //电机细分数
        STEP_MOTOR_DIV_64,
        //电机正方向最大行程
        10000000,
        //电机负方向最大行程
        -10000000,
        //复位第一次靠近原点的模式,复位的时候原点无效,找原点的时候的模式
        STEP_MOTOR_RESET_MODE_FIX_FAST,
        //复位第一次脱离原点的模式,复位的时候原点有效,脱离原点的模式
        STEP_MOTOR_RESET_MODE_FIX_SLOW,
        //电机复位找原点最大行程
        10000,
        //电机复位脱离原点最大行程
        10000,
        //电机复位速度,快速
        800,
        //电机复位速度,慢速
        100,
        //电机复位减速步数
        50,
        //复位换向补偿步数
        100,
        //复位使用曲线复位时,使用的复位曲线
        SM_CURVE_1,
        //方向切换的修正模式
        STEP_MOTOR_DIR_SWITCH_FIX_MODE_NONE,
        //急停被触发后的减速停止步数
        0,
        //辅助急停被触发后的减速停止步数
        25,
    },
    //电机加减速曲线属性集合
    {
        //电机加减速曲线.STEP_MOTOR_CURVE1
        {
            //启动速度
            200,
            //最大速度
            800,
            //启动时加速度
            0.0,
            //最大加速度
            4.0,
            //加速度增长梯度,也就是加加速
            0.03,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE2
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE3
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE4
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE5
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE6
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE7
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE8
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE9
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE10
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE11
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE12
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE13
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE14
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE15
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE16
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE17
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE18
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE19
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE20
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
        //电机加减速曲线.STEP_MOTOR_CURVE21
        {
            //启动速度
            100,
            //最大速度
            500,
            //启动时加速度
            0,
            //最大加速度
            8,
            //加速度增长梯度,也就是加加速
            0.2,
            //启动步数
            3,
            //步数配比模式
            STEP_ALLOC_MODE_SAME_TIME_FRAME,
        },
    },
};

