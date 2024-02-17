/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 12:09:33 +0800
************************************************************************************************/ 
#ifndef __BOARD_RS_H_
#define __BOARD_RS_H_
#include "DriverHeaderMCU.h"

/*---------------------------------------------------------------------------------------*/
//RS初始化
void BoardRS1_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS1_SendString(uint8_t* stringStartPtr);

//printf输出
#define BoardRS1_Printf(format, ...)            MCU_Uart1Printf(format, ##__VA_ARGS__)


#endif


















