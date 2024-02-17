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
#include "BoardRS2.h"
#include "DriverConfigBoard.h"


//RS232初始化
void BoardRS2Uart1_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART1_Init(BOARD_RS2_UART1_BAUD,BOARD_RS2_UART1_LENGTH,BOARD_RS2_UART1_STOP_BIT,BOARD_RS2_UART1_CHECK_MODE,
                        BOARD_RS2_UART1_HARD_CONTROL);
    //回调函数注册
    MCU_UART1_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS232发送缓存
void BoardRS2Uart1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART1_SendBuffer(bufferStartPtr,sendLength);
}

//RS232发送字符串
void BoardRS2Uart1_SendString(uint8_t* stringStartPtr)
{
    MCU_UART1_SendString(stringStartPtr);
}


