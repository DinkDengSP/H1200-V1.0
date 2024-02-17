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
#ifndef __BOARD_SDRAM_H_
#define __BOARD_SDRAM_H_
#include "DriverHeaderMCU.h"

//sdram初始化
void BoardSDRAM_Init(void);

//读取指定区域指定长度内存
void BoardSDRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

//写入指定区域指定长度数据
void BoardSDRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

//自检
ERROR_SUB BoardSDRAM_Check(void);


#endif





