/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 16:24:19 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_H_
#define __STEP_MOTOR_H_
#include "StepMotorTable.h"

extern STEP_MOTOR_VAR* stepMotorVar[SYSTEM_STEP_MOTOR_COUNT];

//步进电机协议参数读取
uint32_t StepMotorGetParamWithProtocol(uint8_t motorNo,uint8_t paramIndex,uint16_t utilParamIndex);

//步进电机协议参数写入
void StepMotorSetParamWithProtocol(uint8_t motorNo,uint8_t paramIndex,uint16_t utilParamIndex,uint32_t writeParam);

//保存步进电机参数
void StepMotorSaveParamWithProtocol(void);


//步进电机参数变量的初始化
void StepMotorVarInit(void);

/*****************************这一块不能被外部调用****************************/
//将步进电机错误代码转换为系统错误代码
ERROR_SUB StepMotorConvertErrorSub(STEP_MOTOR_NO motorNo,STEP_MOTOR_SUB_STATE state);
//步进电机CPLD中断回调函数状态机,被电机中断调用
//注意,该函数内部千万不要调用操作系统方法
void StepMotorCPLD_CallBackProcessWithStateMachine(BOARD_CPLD_MOTOR motorNo);
//辅助复位状态机
void CpldStepMotorResetProc(STEP_MOTOR_NO motorNo);
//走步数
void CpldStepMotorRunStepsProc(STEP_MOTOR_NO motorNo);
//走坐标
void CpldStepMotorRunCoordinate(STEP_MOTOR_NO motorNo);
//持续运行
void CpldStepMotorRunAlways(STEP_MOTOR_NO motorNo);
//减速运行
void CpldStepMotorRunDacc(STEP_MOTOR_NO motorNo);
/******************************************************************************/
//读取电机传感器状态
void StepMotorReadSensorState(STEP_MOTOR_NO motorNo,PIN_STATE* originSensorPtr,PIN_STATE* emStopSensorPtr,
                                PIN_STATE* utilStop1SensorPtr,PIN_STATE* utilStop2SensorPtr);
                                
//指定步进电机复位
ERROR_SUB StepMotorReset(StepMotorResetCmd* resetCmdPtr);

//指定步进电机运行指定步数
ERROR_SUB StepMotorRunSteps(StepMotorRunStepsCmd* runStepConfigPtr);

//指定步进电机运行到指定位置
ERROR_SUB StepMotorRun2Coordinate(StepMotorRun2CoordationCmd* runCoordinatesConfigPtr);

//指定步进电机回零
ERROR_SUB StepMotorReturnZeroWithSensorReset(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指令步进电机回零,但是不进行复位动作,该指令只要用于泵回零,先找到零点,然后关阀然后复位的过程
//防止产生真空负压
ERROR_SUB StepMotorReturnZeroWithSensorNotReset(StepMotorReturnZeroCmd* returnZeroConfigPtr);

//指定步进电机持续运转
ERROR_SUB StepMotorRunAlways(StepMotorRunAlwaysCmd* runAlwaysConfigPtr);

//指定步进电机立即停止
ERROR_SUB StepMotorStopImmediately(STEP_MOTOR_NO motorNo);

//指定步进电机减速停止
ERROR_SUB StepMotorStopDeceleration(STEP_MOTOR_NO motorNo);

//指定步进电机急停
ERROR_SUB StepMotorStopEmergency(STEP_MOTOR_NO motorNo);

#endif





