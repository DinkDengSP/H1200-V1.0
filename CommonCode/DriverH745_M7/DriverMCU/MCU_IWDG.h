/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-15 09:57:20 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-15 10:49:58 +0800
************************************************************************************************/ 
#ifndef __MCU_IWDG_H_
#define __MCU_IWDG_H_
#include "DriverHeaderCore.h"

//初始化IWDG，并使能IWDG,参数为喂狗周期
void MCU_IWDG_Init(uint16_t periodMs);

//独立看门狗喂狗
void MCU_IWDG_Feed(void);



#endif











