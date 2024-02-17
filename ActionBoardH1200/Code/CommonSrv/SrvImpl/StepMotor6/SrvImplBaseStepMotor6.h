/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:28:40
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_STEP_MOTOR6_H_
#define __SRV_IMPL_BASE_STEP_MOTOR6_H_
#include "CommonSrvImplBase.h"


//电机复位            
void SrvImplBaseStepMotor6Reset(StepMotorResetCmd* resetCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机走步数            
void SrvImplBaseStepMotor6RunStep(StepMotorRunStepsCmd* runStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机走坐标            
void SrvImplBaseStepMotor6RunCoordinate(StepMotorRun2CoordationCmd* runCoordCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机回零            
void SrvImplBaseStepMotor6ReturnZero(StepMotorReturnZeroCmd* returnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//电机持续运转            
void SrvImplBaseStepMotor6RunAlways(StepMotorRunAlwaysCmd* runAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen); 

//电机立即停止            
void SrvImplBaseStepMotor6StopNow(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//电机减速停止            
void SrvImplBaseStepMotor6StopSlowDown(uint8_t slowDownMode,uint32_t slowDownSteps,uint32_t timeOutMs,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//获取电机当前坐标            
void SrvImplBaseStepMotor6GetCoordinate(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//获取当前原点传感器状态            
void SrvImplBaseStepMotor6ReadOriginSensor(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//电机失能            
void SrvImplBaseStepMotor6Disable(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//电机进入教学模式            
void SrvImplBaseStepMotor6EnterTeachMode(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//电机强制进入默认状态
void SrvImplBaseStepMotor6ForceToDefault(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif










