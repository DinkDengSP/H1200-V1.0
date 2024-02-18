/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:14:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-11 10:17:06 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_MAIN_H_
#define __STEP_MOTOR_MAIN_H_
#include "StepMotorBase.h"

//电机库数据初始化
void StepMotorMainLibDataInit(void);

//电机库基本配置,并且禁用全部的电机
ERROR_SUB StepMotorMainAllMotorConfigAndDisable(void);

//根据电机配置使能电机并且开启电机中断
ERROR_SUB StepMotorMainEnableByAttri(void);

//电机失能
ERROR_SUB StepMotorMainDisable(STEP_MOTOR_NO motorNo);

//电机指令急停
ERROR_SUB StepMotorMainEmStopCmd(STEP_MOTOR_NO motorNo);

//电机强制进入初始化状态
ERROR_SUB StepMotorMainForceToDefaultState(STEP_MOTOR_NO motorNo);

//电机进入编码器学习模式
ERROR_SUB StepMotorMainEnterTeachingMode(STEP_MOTOR_NO motorNo);

//读取当前电机编码器值
ERROR_SUB StepMotorReadEncoderPos(STEP_MOTOR_NO motorNo,int32_t* encoderPosPtr);

//读取当前电机坐标和编码器坐标
ERROR_SUB StepMotorReadMotorPosAndEncoderPos(STEP_MOTOR_NO motorNo,int32_t* motorPos,int32_t* encoderPosPtr);

//电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* paramPtr);

//电机走步数
ERROR_SUB StepMotorRunSteps(StepMotorRunStepsCmd* paramPtr,MOTOR_SENSOR_FLAGS* sensorFlagsPtr);

//电机走坐标
ERROR_SUB StepMotorRunCoordinate(StepMotorRun2CoordationCmd* paramPtr,MOTOR_SENSOR_FLAGS* sensorFlagsPtr);

//电机持续运转,加速
ERROR_SUB StepMotorRunAlwaysAcc(StepMotorRunAlwaysCmd* paramPtr);

//电机持续运转,减速到停止
ERROR_SUB StepMotorRunAlwaysDecSlow(STEP_MOTOR_NO motorNo,uint32_t timeOutMs);

//电机持续运转,快速停止
ERROR_SUB StepMotorRunAlwaysDecFast(STEP_MOTOR_NO motorNo,uint32_t steps,uint32_t timeOutMs);

//电机持续运转,立刻停止
ERROR_SUB StepMotorRunAlwaysStopImmediate(STEP_MOTOR_NO motorNo);

//电机回零
ERROR_SUB StepMotorReturnZero(StepMotorReturnZeroCmd* paramPtr);

//读取电机传感器状态
ERROR_SUB StepMotorReadSensorState(STEP_MOTOR_NO motorNo,MOTOR_SENSOR_SET* sensorStatePtr);

//运行时实时获取当前电机坐标
ERROR_SUB StepMotorReadMotorRealTimePosAndEncoderPos(STEP_MOTOR_NO motorNo,int32_t* motorPos,int32_t* encoderPosPtr);


#endif


