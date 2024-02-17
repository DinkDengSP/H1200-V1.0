/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:53:59
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
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
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    MEM_D2_SRAM1,
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    MEM_D3_SRAM4,
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
    MEM_SDRAM,
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




