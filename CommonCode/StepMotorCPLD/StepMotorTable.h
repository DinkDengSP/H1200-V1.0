/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 15:26:32 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_TABLE_H_
#define __STEP_MOTOR_TABLE_H_
#include "StepMotorUtil.h"

//创建步进电机速度表格
//使用步数计算步进电机加速曲线
void StepMotorTableCalcWithStep(int32_t steps,STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//使用坐标计算步进电机加速曲线
void StepMotorTableCalcWithCoordinate(int32_t targetCoordinate,STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//回零计算电机加速曲线
void StepMotorTableCalcWithReturnZero(STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//步进电机计算持续运转转速
void StepMotorTableCalcWithRunAlways(STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio);

//计算步进电机减速曲线
void StepMotorTableCalcDaccTable(STEP_MOTOR_NO motorNo);

#endif





