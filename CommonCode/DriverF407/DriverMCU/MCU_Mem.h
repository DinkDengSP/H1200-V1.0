/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __MCU_MEM_H_
#define __MCU_MEM_H_
#include "DriverHeaderCore.h"

//SramIn内存初始化
void MCU_MemInitSramIn(void);

//申请内存
void* MCU_MemMallocSramIn(uint32_t size);

//申请内存直到成功
void* MCU_MemMallocWhileSuccessSramIn(uint32_t size);

//释放内存
void MCU_MemFreeSramIn(void* ptr);

//获取当前动态内存使用率
uint8_t MCU_MemPerUsedSramIn(void);



#endif









