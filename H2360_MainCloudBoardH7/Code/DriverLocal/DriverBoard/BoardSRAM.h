/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __BOARD_SRAM_H_
#define __BOARD_SRAM_H_
#include "DriverHeaderMCU.h"

void BoardSRAM_Init(void);

ERROR_SUB BoardSRAM_ReadBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

ERROR_SUB BoardSRAM_WriteBuffer(uint32_t baseAddr,uint8_t* bufferPtr,uint32_t length);

ERROR_SUB BoardSRAM_SelfCheck(void);

#endif















