/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_PLATE_REACTION_H_
#define __MOD_IMPL_BASE_PLATE_REACTION_H_
#include "ModImplBase.h"

//重新加载控温系数
void ModImplBasePlateReactionReloadTempControlParam(void);

//清除控温缓存
void ModImplBasePlateReactionClearTempControlCache(void);

//计算PID输出
void ModImplBasePlateReactionCalcPIDTempControlOutput(float currentSensorTemp,float* outputStrength);

//控温状态初始化
void ModImplBasePlateReactionTempControlStateInit(void);

//控温状态机运转
void ModImplBasePlateReactionTempControlStateRun(void);

//反应盘温度上报
void ModImplBasePlateReactionTempControlReport(void);

#endif












