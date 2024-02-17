/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-27 10:53:57 +0800
************************************************************************************************/ 
#ifndef __BOARD_STEP_MOTOR_H_
#define __BOARD_STEP_MOTOR_H_
#include "DriverHeaderMCU.h"

/*******************************步进电机1相关API********************************/
//步进电机初始化
void BoardStepMotor1Init(void* callBackFunc);
//设置步进电机方向,反向
void BoardStepMotor1SetDirCCW(void);
//设置步进电机方向,正向
void BoardStepMotor1SetDirCW(void);
//步进电机启动
void BoardStepMotor1Start(float rpm);
//步进电机停止
void BoardStepMotor1Stop(void);
//步进电机使能
void BoardStepMotor1Enable(void);
//步进电机失能
void BoardStepMotor1Disable(void);

/*******************************步进电机2相关API********************************/
//步进电机初始化
void BoardStepMotor2Init(void* callBackFunc);
//设置步进电机方向,反向
void BoardStepMotor2SetDirCCW(void);
//设置步进电机方向,正向
void BoardStepMotor2SetDirCW(void);
//步进电机启动
void BoardStepMotor2Start(float rpm);
//步进电机停止
void BoardStepMotor2Stop(void);
//步进电机使能
void BoardStepMotor2Enable(void);
//步进电机失能
void BoardStepMotor2Disable(void);


#endif






