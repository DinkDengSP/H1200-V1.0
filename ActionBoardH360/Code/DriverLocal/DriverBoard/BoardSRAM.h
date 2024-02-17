/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:12:13 +0800
************************************************************************************************/ 
#ifndef __BOARD_SRAM_H_
#define __BOARD_SRAM_H_
#include "DriverHeaderMCU.h"

//端口初始化
void BoardSRAM_Init(void);

//自检
ERROR_SUB BoardSRAM_Check(void);

//数据写入
void BoardSRAM_WriteBuffer(uint32_t addr,uint8_t* buf,uint32_t length);

//数据读取
void BoardSRAM_ReadBuffer(uint32_t addr,uint8_t* buf,uint32_t length);


#endif















