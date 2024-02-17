/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32f4xx_hal.h"

//延时初始化
void CoreDelayInit(void);

//微秒延时
void CoreDelayUs(uint32_t nus);

//毫秒延时
void CoreDelayMs(uint32_t nms);

//最小系统时基定时
void CoreDelayMinTick(void);

//高敏感延时函数
void CoreDelayMsSensitivity(uint16_t nms);

#endif










