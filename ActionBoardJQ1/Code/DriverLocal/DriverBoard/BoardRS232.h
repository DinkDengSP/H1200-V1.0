/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 17:03:42 +0800
************************************************************************************************/ 
#ifndef __BOARD_RS_232_H_
#define __BOARD_RS_232_H_
#include "DriverHeaderMCU.h"

/*---------------------------------------------------------------------------------------*/
//RS初始化
void BoardRS232A_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS232A_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS232A_SendString(uint8_t* stringStartPtr);

//printf输出
#define BOARD_RS232A_Printf(format, ...)            MCU_Uart1Printf(format, ##__VA_ARGS__)


/*---------------------------------------------------------------------------------------*/
//RS初始化
void BoardRS232B_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS232B_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS232B_SendString(uint8_t* stringStartPtr);

//printf输出
#define BOARD_RS232B_Printf(format, ...)            MCU_Uart3Printf(format, ##__VA_ARGS__)




#endif














