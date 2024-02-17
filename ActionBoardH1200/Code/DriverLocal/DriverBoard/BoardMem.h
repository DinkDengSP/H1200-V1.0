/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_MEM_H_
#define __BOARD_MEM_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

/*------------------------------------SDRAM----------------------------------------*/
#if(MEM_MANAGE_BYTE_SIZE_SDRAM != 0)
//SDRAM内存初始化
void BoardMemInitSDRAM(void);
//申请内存
void* BoardMemMallocSDRAM(uint32_t size);
//申请内存直到成功
void* BoardMemMallocWhileSuccessSDRAM(uint32_t size);
//释放内存
void BoardMemFreeSDRAM(void* ptr);
//获取当前动态内存使用率
uint8_t BoardMemPerUsedSDRAM(void);
#endif

#endif






