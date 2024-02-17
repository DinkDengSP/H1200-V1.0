/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-16 15:58:53 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_PLATE_REAGENT_H_
#define __MOD_IMPL_BASE_PLATE_REAGENT_H_
#include "ModImplBase.h"

//重新加载控温系数
void ModImplBasePlateReagentReloadTempControlParam(void);

//清除控温缓存
void ModImplBasePlateReagentClearTempControlCache(void);

//计算PID输出
void ModImplBasePlateReagentCalcPIDTempControlOutput(float currentSensorTemp,float* outputStrength);

//控温状态初始化
void ModImplBasePlateReagentTempControlStateInit(void);

//试剂盘控温状态机
void ModImplBasePlateReagentTempControlMachine(void);

//试剂盘盘温度上报
void ModImplBasePlateReagentTempControlReport(void);


#endif












