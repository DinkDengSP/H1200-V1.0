/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 17:20:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 09:54:01 +0800
************************************************************************************************/ 
#ifndef __MCU_RANDOM_H_
#define __MCU_RANDOM_H_
#include "DriverHeaderCore.h"

//随机数初始化
void MCU_RandomInit(void);

//得到随机数
uint32_t MCU_RandomGetNext(void);

//正确计算随机数的方法
int32_t MCU_RandomGetNextInteger(int32_t min, int32_t max);

//正确计算随机数的方法
uint32_t MCU_RandomGetNextUnsigned(uint32_t min, uint32_t max);



#endif







