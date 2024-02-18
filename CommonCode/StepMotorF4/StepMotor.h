/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:15:35
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-06-04 10:14:46
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_H_
#define __STEP_MOTOR_H_
#include "StepMotorDataType.h"
#include "StepMotorUtil.h"

//步进电机配置状态变量集合
extern STEP_MOTOR_VAR stepMotorVarArray[SYSTEM_STEP_MOTOR_COUNT];

//初始化步进电机状态
void StepMotorVarInit(void);

//步进电机初始化
void StepMotorInit(void);

//获取步进电机编码器当前计数值
ERROR_SUB StepMotorGetCurrentEncoderPos(STEP_MOTOR_NO motorNo,int32_t* countResult);

//读取电机传感器状态
void StepMotorReadSensorState(STEP_MOTOR_NO motorNo,PIN_STATE* originSensorPtr,PIN_STATE* emStopSensorPtr,
                                PIN_STATE* utilStop1SensorPtr,PIN_STATE* utilStop2SensorPtr);

//复位步进电机编码器当前计数值
ERROR_SUB StepMotorResetEncoderCount(STEP_MOTOR_NO motorNo);

//指定步进电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* resetCmdPtr);

//指定步进电机运行到指定位置
ERROR_SUB StepMotorRun2SpecialCoordinates(StepMotorRun2CoordationCmd* runCoordinatesConfigPtr);

//指定步进电机持续运转
ERROR_SUB StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr);

//指定步进电机立即停止
ERROR_SUB StepMotorStopImmediately(STEP_MOTOR_NO motorNo);

//指定步进电机急停
ERROR_SUB StepMotorStopEmergency(STEP_MOTOR_NO motorNo);

//指定步进电机减速停止,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration(STEP_MOTOR_NO motorNo);

//指定步进电机运行指定步数
ERROR_SUB StepMotorRunSteps(StepMotorRunStepsCmd* runStepConfigPtr);

//指定步进电机回零
ERROR_SUB StepMotorReturnZeroReset(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程,防止产生真空负压
ERROR_SUB StepMotorReturnZero(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指定步进电机减速停止运行指定步数1,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration1(STEP_MOTOR_NO motorNo);

//指定步进电机减速停止指定步数2,这个指令只会响应RunAlways状态
ERROR_SUB StepMotorStopDeceleration2(STEP_MOTOR_NO motorNo);



#endif




