/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:15:02
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-04-02 15:47:05
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_INT_H_
#define __STEP_MOTOR_INT_H_
#include "StepMotorDataType.h"
#include "StepMotorError.h"

//复位中断处理
void StepMotorResetIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr);
//走坐标中断处理子状态机
void StepMotorRunCoordinateIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr);
//持续运行
void StepMotorRunAlwaysIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr);
//走步数
void StepMotorRunStepsIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr);
//减速运行
void StepMotorRunDaccIntProcess(STEP_MOTOR_VAR* stepMotorVarPtr);

//步进电机中断回调函数类型
typedef void (*StepMotorIntProcessFuncPtr)(void);

//步进电机中断处理函数指针
extern const StepMotorIntProcessFuncPtr stepMotorIntProcessFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];


#endif




