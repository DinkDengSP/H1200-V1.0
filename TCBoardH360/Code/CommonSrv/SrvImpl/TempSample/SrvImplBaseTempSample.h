/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 11:26:35 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_TEMP_SAMPLE_H_
#define __SRV_IMPL_BASE_TEMP_SAMPLE_H_
#include "CommonSrvImplBase.h"

//板上温度传感器状态机初始化
void SrvImplBaseTempSample_FSM_InitBoardSensor(void);
//板上温度传感器状态机周期运转
void SrvImplBaseTempSample_FSM_PeriodBoardSensor(void);

//环境温度传感器状态机初始化
void SrvImplBaseTempSample_FSM_InitEnvirSensor(void);
//环境温度传感器状态机周期运转
void SrvImplBaseTempSample_FSM_PeriodEnvirSensor(void);

//反应盘温度传感器状态机初始化
void SrvImplBaseTempSample_FSM_InitPlateReactionSensor(void);
//反应盘温度传感器状态机周期运转
void SrvImplBaseTempSample_FSM_PeriodPlateReactionSensor(void);

//清洗盘温度传感器状态机初始化
void SrvImplBaseTempSample_FSM_InitPlateWashSensor(void);
//清洗盘温度传感器状态机周期运转
void SrvImplBaseTempSample_FSM_PeriodPlateWashSensor(void);

//试剂盘温度传感器状态机初始化
void SrvImplBaseTempSample_FSM_InitPlateReagentSensor(void);
//试剂盘温度传感器状态机周期运转
void SrvImplBaseTempSample_FSM_PeriodPlateReagentSensor(void);

//获取板卡温度
ERROR_SUB SrvImplBaseTempSample_GetBoardTemp(float* tempVal);
//获取环境温度
ERROR_SUB SrvImplBaseTempSample_GetEnvirTemp(float* tempVal);
//获取反应盘温度
ERROR_SUB SrvImplBaseTempSample_GetPlateReactionTemp(float* tempVal);
//获取清洗盘温度
ERROR_SUB SrvImplBaseTempSample_GetPlateWashTemp(float* tempVal);
//获取试剂盘温度
ERROR_SUB SrvImplBaseTempSample_GetPlateReagentTemp(float* tempVal);


#endif







