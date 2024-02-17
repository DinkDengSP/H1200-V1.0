/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_MEM_H_
#define __BOARD_MEM_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

/*------------------------------------SDRAM1----------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM1 != 0)
//SDRAM1内存初始化
void BoardMemInitSDRAM1(void);
//申请内存
void* BoardMemMallocSDRAM1(uint32_t size);
//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM1(uint32_t size);
//释放内存
void BoardMemFreeSDRAM1(void* ptr);
//获取当前动态内存使用率
uint8_t BoardMemPerUsedSDRAM1(void);
#endif


/*------------------------------------SDRAM2----------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM2 != 0)
//SDRAM2内存初始化
void BoardMemInitSDRAM2(void);
//申请内存
void* BoardMemMallocSDRAM2(uint32_t size);
//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM2(uint32_t size);
//释放内存
void BoardMemFreeSDRAM2(void* ptr);
//获取当前动态内存使用率
uint8_t BoardMemPerUsedSDRAM2(void);
#endif


#endif




