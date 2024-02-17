/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 18:47:28 +0800
************************************************************************************************/ 
#ifndef __USER_MEM_MANAGE_H_
#define __USER_MEM_MANAGE_H_
#include "DriverHeaderBoard.h"
#include "DriverConfigBoard.h"

//内存区域定义
typedef enum MEM_REGION
{
    MEM_SRAM_IN    = 0X00,//SRAM内存,DMA与MCU都能访问
#if(MEM_MANAGE_BYTE_SIZE_SRAM_EXT != 0)
    MEM_SRAM_EXT   = 0x01,//外部SRAM内存
#endif
}MEM_REGION;

//动态内存初始化
void UserMemInit(MEM_REGION locate);

//申请内存
void* UserMemMalloc(MEM_REGION locate,uint32_t size);

//释放内存
void UserMemFree(MEM_REGION locate,void* ptr);

//申请内存
void* UserMemMallocWhileSuccess(MEM_REGION locate,uint32_t size);

//获取当前动态内存使用率
uint8_t UserMemPerUsed(MEM_REGION locate);

#endif










