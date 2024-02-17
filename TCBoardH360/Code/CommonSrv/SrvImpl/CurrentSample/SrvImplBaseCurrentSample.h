/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-20 16:26:17 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_CURRENT_SAMPLE_H_
#define __SRV_IMPL_BASE_CURRENT_SAMPLE_H_
#include "CommonSrvImplBase.h"

//电流采样状态机初始化
void SrvImplBaseCurrentSampleMachineInit(void);

//电流采样状态机周期执行
void SrvImplBaseCurrentSampleMachinePeriod(void);

//获取反应盘电流
void SrvImplBaseCurrentSampleGetPlateReaction(int32_t* currentMaPtr);

//获取清洗盘电流
void SrvImplBaseCurrentSampleGetPlateWash(int32_t* currentMaPtr);

//获取试剂盘1电流
void SrvImplBaseCurrentSampleGetPlateReagent1(int32_t* currentMaPtr);

//获取试剂盘2电流
void SrvImplBaseCurrentSampleGetPlateReagent2(int32_t* currentMaPtr);

//获取试剂盘3电流
void SrvImplBaseCurrentSampleGetPlateReagent3(int32_t* currentMaPtr);

//获取试剂盘4电流
void SrvImplBaseCurrentSampleGetPlateReagent4(int32_t* currentMaPtr);


#endif





