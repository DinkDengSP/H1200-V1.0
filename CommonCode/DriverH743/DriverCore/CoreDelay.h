/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __CORE_DELAY_H_
#define __CORE_DELAY_H_
#include "stm32h7xx.h"

//延时初始化
void CoreDelayInit(void);

//微秒延时
void CoreDelayUs(uint32_t nus);

//毫秒延时
void CoreDelayMs(uint32_t nms);

//最小系统时基定时
void CoreDelayMinTick(void);


#endif










