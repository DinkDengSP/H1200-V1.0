/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 15:36:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 15:37:38 +0800
************************************************************************************************/ 
#ifndef __BOARD_RS485_H_
#define __BOARD_RS485_H_
#include "DriverHeaderMCU.h"

/*---------------------------------------------------------------------------------------*/
//RS初始化
void BoardRS485_Init(MCU_UartRecvIntProcFunc recvCallBack);

//RS发送缓存
void BoardRS485_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS485_SendString(uint8_t* stringStartPtr);

//printf输出
#define BOARD_RS485_Printf(format, ...)            MCU_Uart2Printf(format, ##__VA_ARGS__)


#endif














