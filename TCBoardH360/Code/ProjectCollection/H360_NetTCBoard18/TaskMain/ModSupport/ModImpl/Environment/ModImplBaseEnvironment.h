/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_ENVIRONMENT_H_
#define __MOD_IMPL_BASE_ENVIRONMENT_H_
#include "ModImplBase.h"

//初始化环境温度控制状态
void ModImplBaseEnvironmentReloadTempControlParam(void);

//系统状态机
void ModImplBaseEnvironmentControlMachine(void);

//板卡温度上报
void ModImplBaseBoardTempControlReport(void);

//环境温度上报
void ModImplBaseEnvironmentTempControlReport(void);


#endif












