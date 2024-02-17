/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 17:18:13 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_STEP_MOTOR1_H_
#define __SRV_IMPL_BASE_STEP_MOTOR1_H_
#include "CommonSrvImplBase.h"

//电机复位            
void SrvImplBaseStepMotor1Reset(StepMotorResetCmd* resetCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机走步数            
void SrvImplBaseStepMotor1RunStep(StepMotorRunStepsCmd* runStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机走坐标            
void SrvImplBaseStepMotor1RunCoordinate(StepMotorRun2CoordationCmd* runCoordCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机回零            
void SrvImplBaseStepMotor1ReturnZero(StepMotorReturnZeroCmd* returnZeroCmdPtr,uint8_t needReset,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//电机持续运转            
void SrvImplBaseStepMotor1RunAlways(StepMotorRunAlwaysCmd* runAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机立即停止            
void SrvImplBaseStepMotor1StopNow(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//电机减速停止            
void SrvImplBaseStepMotor1StopSlowDown(uint8_t slowDownMode,uint32_t slowDownSteps,uint32_t timeOutMs,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//获取电机当前坐标            
void SrvImplBaseStepMotor1GetCoordinate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//获取当前原点传感器状态            
void SrvImplBaseStepMotor1ReadOriginSensor(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif












