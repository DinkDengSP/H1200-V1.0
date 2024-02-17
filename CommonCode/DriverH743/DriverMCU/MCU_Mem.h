/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#ifndef __MCU_MEM_H_
#define __MCU_MEM_H_
#include "DriverHeaderCore.h"
#include "DriverConfigMCU.h"


/*----------------------------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
//AxiSram内存初始化
void MCU_MemInitAxiSram(void);
//申请内存
void* MCU_MemMallocAxiSram(uint32_t size);
//申请内存直到成功
void* MCU_MemMallocWhileSuccessAxiSram(uint32_t size);
//释放内存
void MCU_MemFreeAxiSram(void* ptr);
//获取当前动态内存使用率
uint8_t MCU_MemPerUsedAxiSram(void);

#endif

/*----------------------------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
//ItcmSram内存初始化
void MCU_MemInitItcmSram(void);
//申请内存
void* MCU_MemMallocItcmSram(uint32_t size);
//申请内存直到成功
void* MCU_MemMallocWhileSuccessItcmSram(uint32_t size);
//释放内存
void MCU_MemFreeItcmSram(void* ptr);
//获取当前动态内存使用率
uint8_t MCU_MemPerUsedItcmSram(void);

#endif


/*----------------------------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
//DtcmSram内存初始化
void MCU_MemInitDtcmSram(void);
//申请内存
void* MCU_MemMallocDtcmSram(uint32_t size);
//申请内存直到成功
void* MCU_MemMallocWhileSuccessDtcmSram(uint32_t size);
//释放内存
void MCU_MemFreeDtcmSram(void* ptr);
//获取当前动态内存使用率
uint8_t MCU_MemPerUsedDtcmSram(void);

#endif

/*----------------------------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
//D2Sram1内存初始化
void MCU_MemInitD2Sram1(void);
//申请内存
void* MCU_MemMallocD2Sram1(uint32_t size);
//申请内存直到成功
void* MCU_MemMallocWhileSuccessD2Sram1(uint32_t size);
//释放内存
void MCU_MemFreeD2Sram1(void* ptr);
//获取当前动态内存使用率
uint8_t MCU_MemPerUsedD2Sram1(void);

#endif

/*----------------------------------------------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
//D3Sram4内存初始化
void MCU_MemInitD3Sram4(void);
//申请内存
void* MCU_MemMallocD3Sram4(uint32_t size);
//申请内存直到成功
void* MCU_MemMallocWhileSuccessD3Sram4(uint32_t size);
//释放内存
void MCU_MemFreeD3Sram4(void* ptr);
//获取当前动态内存使用率
uint8_t MCU_MemPerUsedD3Sram4(void);

#endif



//一键初始化全部的SRAM
void MCU_MemInitAll(void);




#endif






