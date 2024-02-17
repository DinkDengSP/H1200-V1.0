/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-27 10:53:57 +0800
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


/*---------------------------------------------------------------------------------------*/
//RS初始化
void BoardRS2_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS2_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS2_SendString(uint8_t* stringStartPtr);

//printf输出
#define BoardRS2_Printf(format, ...)            MCU_Uart3Printf(format, ##__VA_ARGS__)




#endif



