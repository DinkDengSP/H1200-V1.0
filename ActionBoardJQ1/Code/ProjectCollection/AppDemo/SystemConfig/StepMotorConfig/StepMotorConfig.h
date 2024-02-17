/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 19:38:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 19:39:15 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_CONFIG_H_
#define __STEP_MOTOR_CONFIG_H_
#include "stm32f4xx.h"
#include "DriverHeaderBoard.h"

//电机驱动的速度表的最大长度,也就是加速曲线最大长度
#define SM_SPEED_TABLE_LENGTH_MAX                           600
//步进电机编码器与电机方向配置相反的报警阈值,编码器计数值和电机计数值达到该参数两倍的时候会报警
#define SM_RESET_DIR_ENCODER_NOT_MATCH_THRESHOLD            50
//编码器与电机传动比阈值
#define SM_RESET_ENCODER_MORE_THRESHOLD                     100
//设定系统可配置加速曲线的数量(每个电机的曲线数量)
#define SM_SPEED_TABLE_COUNT_EACH_ONE                       21
//系统内部的步进电机数量
#define SYSTEM_STEP_MOTOR_COUNT                             5
//系统最大只能带八个电机
#if(SYSTEM_STEP_MOTOR_COUNT > 8)
#error "StepMotor Lib Max Support 8 Motor!!!"
#endif


#endif

