/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:12:13 +0800
************************************************************************************************/ 
#ifndef __BOARD_MEM_H_
#define __BOARD_MEM_H_
#include "DriverHeaderMCU.h"

//SramExt内存初始化
void BoardMemInitSramExt(void);

//申请内存
void* BoardMemMallocSramExt(uint32_t size);

//申请内存直到成功
void* BoardMemMallocWhileSuccessSramExt(uint32_t size);

//释放内存
void BoardMemFreeSramExt(void* ptr);

//获取当前动态内存使用率
uint8_t BoardMemPerUsedSramExt(void);



#endif









