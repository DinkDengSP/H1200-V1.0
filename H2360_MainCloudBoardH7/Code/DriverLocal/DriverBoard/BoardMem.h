/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __BOARD_MEM_H_
#define __BOARD_MEM_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

/*------------------------------------SRAM----------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)

//SRAM内存初始化
void BoardMemInitSRAM(void);
//申请内存
void* BoardMemMallocSRAM(uint32_t size);
//申请内存直到成功
void* BoardMemMallocWhileSuccessSRAM(uint32_t size);
//释放内存
void BoardMemFreeSRAM(void* ptr);
//获取当前动态内存使用率
uint8_t BoardMemPerUsedSRAM(void);

#endif



#endif
















