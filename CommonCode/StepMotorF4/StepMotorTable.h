/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:22:25
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-04-02 16:34:17
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_TABLE_H_
#define __STEP_MOTOR_TABLE_H_
#include "StepMotorDataType.h"
#include "StepMotorError.h"

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
void StepMotorTableCalcDaccTable(STEP_MOTOR_VAR* stepMotorVarPtr);

//计算步进电机持续运行减速曲线1,加速过程的核心问题,怎么样计算减速步数表
void StepMotorTableCalcSlowDown1Table(STEP_MOTOR_VAR* stepMotorVarPtr);

//计算步进电机持续运行减速曲线1,加速过程的核心问题,怎么样计算减速步数表
void StepMotorTableCalcSlowDown2Table(STEP_MOTOR_VAR* stepMotorVarPtr);

#endif
