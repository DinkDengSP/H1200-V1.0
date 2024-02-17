/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __USER_MEM_MANAGE_H_
#define __USER_MEM_MANAGE_H_
#include "DriverHeaderBoard.h"

//内存区域定义
typedef enum MEM_REGION
{
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    MEM_ITCM_SRAM,
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    MEM_DTCM_SRAM,
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    MEM_AXI_SRAM,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SRAM != 0)
    MEM_EXT_SRAM,
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

















