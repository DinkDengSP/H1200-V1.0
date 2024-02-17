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
#ifndef __BOARD_RS3_H_
#define __BOARD_RS3_H_
#include "DriverHeaderMCU.h"

//UART1,六针有供电

//RS初始化
void BoardRS3Uart1_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS3Uart1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS3Uart1_SendString(uint8_t* stringStartPtr);

//printf输出
#define BoardRS3Uart1_Printf(format, ...)           MCU_Uart1Printf(format, ##__VA_ARGS__)


#endif



