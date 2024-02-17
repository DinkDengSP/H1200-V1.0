/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 14:42:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 14:44:07 +0800
************************************************************************************************/ 
#ifndef __BOARD_RS_232_H_
#define __BOARD_RS_232_H_
#include "DriverHeaderMCU.h"

/*---------------------------------------232A--RS1----6针--------------------------------------------*/
//RS初始化
void BoardRS232A_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS232A_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS232A_SendString(uint8_t* stringStartPtr);

//printf输出
#define BOARD_RS232A_Printf(format, ...)            MCU_Uart2Printf(format, ##__VA_ARGS__)


/*-------------------------------------------232B--RS2-------4针-------------------------------------*/
//RS初始化
void BoardRS232B_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//RS发送缓存
void BoardRS232B_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//RS发送字符串
void BoardRS232B_SendString(uint8_t* stringStartPtr);

//printf输出
#define BOARD_RS232B_Printf(format, ...)            MCU_Uart1Printf(format, ##__VA_ARGS__)

#endif













