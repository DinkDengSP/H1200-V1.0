/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 16:32:57
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardRS2.h"
#include "DriverConfigBoard.h"

//RS初始化
void BoardRS2Uart6_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART6_Init(BOARD_RS2_COMM_BAUD,BOARD_RS2_COMM_LENGTH,BOARD_RS2_COMM_STOP_BIT,BOARD_RS2_COMM_CHECK_MODE,
                        BOARD_RS2_COMM_HARD_CONTROL);
    //回调函数注册
    MCU_UART6_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS2Uart6_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART6_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS2Uart6_SendString(uint8_t* stringStartPtr)
{
    MCU_UART6_SendString(stringStartPtr);
}






