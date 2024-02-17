/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 20:12:46 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_UTIL_H_
#define __STEP_MOTOR_UTIL_H_
#include "StepMotorParam.h"
#include "StepMotorDataType.h"

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
void StepMotorUtilCalcDirWithTargetCoordinate(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,int32_t targetCoordinate);

//发送启动脉冲,启动运行
void StepMotorUtilSendStartPluse(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr);

//发送减速第一步
void StepMotorUtilSendDaccStartPluse(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr);

//步进电机解除全部的中断注册,运行完成后调用
void StepMotorUtilUnregisterIntPort(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr);

//等待步进电机停止运转
void StepMotorUtilWaitMotorStop(STEP_MOTOR_NO motorNo,STEP_MOTOR_VAR* motorVarPtr,uint32_t timeOutSet);

//更新坐标
void StepMotorUtilReflushCoordinate(STEP_MOTOR_NO motorNo);

//解析走步数错误码
ERROR_SUB StepMotorErrorSubRunStep(STEP_MOTOR_NO motorNo);

//解析走坐标错误码
ERROR_SUB StepMotorErrorSubRunCoordinate(STEP_MOTOR_NO motorNo);

//解析回零错误码
ERROR_SUB StepMotorErrorSubReturnZero(STEP_MOTOR_NO motorNo);

//解析持续运转错误码
ERROR_SUB StepMotorErrorSubRunAlways(STEP_MOTOR_NO motorNo);

//指令急停
ERROR_SUB StepMotorErrorSubEmStopCmd(STEP_MOTOR_NO motorNo);

//信号急停
ERROR_SUB StepMotorErrorSubEmStopSingal(STEP_MOTOR_NO motorNo);

//辅助急停1
ERROR_SUB StepMotorErrorSubStopUtil1(STEP_MOTOR_NO motorNo);

//辅助急停2
ERROR_SUB StepMotorErrorSubStopUtil2(STEP_MOTOR_NO motorNo);

//超时停止
ERROR_SUB StepMotorErrorSubStopTimeOut(STEP_MOTOR_NO motorNo);

//复位超行程
ERROR_SUB StepMotorErrorSubStopResetStepLimit(STEP_MOTOR_NO motorNo);

//正向超行程
ERROR_SUB StepMotorErrorSubStopCoordinatePositive(STEP_MOTOR_NO motorNo);

//反向超行程
ERROR_SUB StepMotorErrorSubStopCoordinateNegative(STEP_MOTOR_NO motorNo);

//复位修正超正向行程
ERROR_SUB StepMotorErrorSubStopResetCorrectPositive(STEP_MOTOR_NO motorNo);

//反向修正超最大行程
ERROR_SUB StepMotorErrorSubStopResetCorrectNegative(STEP_MOTOR_NO motorNo);

//超时时间设置为0
ERROR_SUB StepMotorErrorSubStopTimeOutZero(STEP_MOTOR_NO motorNo);

//READY信号无效
ERROR_SUB StepMotorErrorSubStopSlaveReadyFault(STEP_MOTOR_NO motorNo);

//离开原点超行程
ERROR_SUB StepMotorErrorSubStopLeaveZeroLimit(STEP_MOTOR_NO motorNo);

//找原点减速停止之后,原点信号丢失
ERROR_SUB StepMotorErrorSubStopResetDaccLoss(STEP_MOTOR_NO motorNo);

//未知异常
ERROR_SUB StepMotorErrorSubStopUnknow(STEP_MOTOR_NO motorNo);

#endif






