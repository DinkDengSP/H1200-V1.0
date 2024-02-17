/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 11:14:35 +0800
************************************************************************************************/ 
#ifndef __COMMON_SRV_IMPL_BASE_H_
#define __COMMON_SRV_IMPL_BASE_H_
#include "DriverHeaderSystem.h"
#include "SysState.h"
#include "SysParam.h"

//步进电机复位
ERROR_SUB CommonSrvImplBaseModuleStepMotorReset(uint8_t motorIdx,MODULE_STEP_MOTOR_CONFIG const* configArrayPtr,uint16_t arrayLength,
                                                    int32_t* motorCoordinate);

//步进电机走位
ERROR_SUB CommonSrvImplBaseModuleStepMotorRunSteps(uint8_t motorIdx,int32_t runSteps,MODULE_STEP_MOTOR_CONFIG const* configArrayPtr,
                                                    uint16_t arrayLength,int32_t* motorCoordinate);


#endif











