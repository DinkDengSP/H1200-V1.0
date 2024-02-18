/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:21:42
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-04-02 10:44:41
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_UTIL_H_
#define __STEP_MOTOR_UTIL_H_
#include "StepMotorDataType.h"
#include "StepMotorError.h"

//急停中断回调函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorEmergencyStopByIntSingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];
//步进电机辅助急停1中断函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt1SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];
//步进电机辅助急停2中断函数数组
extern const MCU_Exti_CallBackFuncPtr StepMotorDaccStopByUtilInt2SingalFuncPtrArray[SYSTEM_STEP_MOTOR_COUNT];

//将步进电机错误代码转换为系统错误代码
ERROR_SUB StepMotorConvert2ErrorSubCode(STEP_MOTOR_NO motorNo,STEP_MOTOR_SUB_STATE state);

//初始化电机状态
void StepMotorStateInit(StepMotorState* motorStatePtr);

//电机状态初始化,针对复位
void StepMotorStateInitForReset(StepMotorState* motorStatePtr);

//电机状态初始化,针对走位
void StepMotorStateInitForRunSteps(StepMotorState* motorStatePtr);

//电机状态初始化,针对走坐标
void StepMotorStateInitForRunCoordinate(StepMotorState* motorStatePtr);

//电机状态初始化,针对电机持续运转
void StepMotorStateInitForRunAlways(StepMotorState* motorStatePtr);

//电机状态初始化,针对电机回零
void StepMotorStateInitForReturnZero(StepMotorState* motorStatePtr);

//检查电机走指定步数的参数是否合格
ERROR_SUB StepMotorCheckRunStepsVar(StepMotorRunStepsCmd* runStepPtr);

//检查电机走到指定坐标的参数是否合格
ERROR_SUB StepMotorCheckRun2CoordationVar(StepMotorRun2CoordationCmd* runCoordationPtr);

//检查电机回零指令的参数是否合格
ERROR_SUB StepMotorCheckReturnZeroVar(StepMotorReturnZeroCmd* returnZeroPtr);

//检查电机持续运转的参数是否合格
ERROR_SUB StepMotorCheckRunAlways(StepMotorRunAlwaysCmd* runAlwaysPtr);

//检查步进电机急停状态,如果是指令急停,那么报错
ERROR_SUB StepMotorUtilCheckEmergencyCmdState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr);

//检查步进电机电机的目标坐标是否超过限制
ERROR_SUB StepMotorUtilCheckTargetCoordinateLimit(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate);

//检查和设置步进电机急停引脚状态
ERROR_SUB StepMotorUtilCheckAndSetEmergencyPinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr);

//检查和设置步进电机辅助急停1状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop1PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable);

//检查和设置步进电机辅助急停2状态
ERROR_SUB StepMotorUtilCheckAndSetUtilStop2PinState(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint8_t enable);

//根据本次电机目的坐标决定方向
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate);

//设置电机运行方向
void StepMotorSetRunningDir(STEP_MOTOR_VAR* motorVarPtr);

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_VAR* motorVarPtr);

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_VAR* motorVarPtr);

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_VAR* motorVarPtr);

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet);


#endif
