/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_MPU_H_
#define __MCU_MPU_H_
#include "DriverHeaderCore.h"

//初始化芯片MPU配置
void MCU_MPU_ConfigInit(void);

//更新更新Cache
void MCU_MPU_CacheReflush(CACHE_REFLUSH_CONFIG config);


#endif












