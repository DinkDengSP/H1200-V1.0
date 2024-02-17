/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 09:07:21 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_PARAM_H_
#define __STEP_MOTOR_PARAM_H_
#include "DriverHeaderBoard.h"
#include "StepMotorDataType.h"

//电机的默认配置
extern const STEP_MOTOR_ATTRIBUTE StepMotor1DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor2DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor3DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor4DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor5DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor6DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor7DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor8DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor9DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor10DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor11DefaultAttri;
extern const STEP_MOTOR_ATTRIBUTE StepMotor12DefaultAttri;

//系统参数检验
uint8_t StepMotorParamCheckValid(STEP_MOTOR_ATTRIBUTE* attr,uint8_t index);

//步进电机装载参数
void StepMotorParamLoad(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr,uint8_t index);

//步进电机写入参数
void StepMotorParamSet(uint32_t addr,STEP_MOTOR_ATTRIBUTE* attr);

#endif







