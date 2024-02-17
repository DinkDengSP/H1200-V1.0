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
#include "BoardRS1.h"
#include "DriverConfigBoard.h"

//RS初始化
void BoardRS1Uart3_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART3_Init(BOARD_RS1_COMM_BAUD,BOARD_RS1_COMM_LENGTH,BOARD_RS1_COMM_STOP_BIT,BOARD_RS1_COMM_CHECK_MODE,
                        BOARD_RS1_COMM_HARD_CONTROL);
    //回调函数注册
    MCU_UART3_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS1Uart3_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART3_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS1Uart3_SendString(uint8_t* stringStartPtr)
{
    MCU_UART3_SendString(stringStartPtr);
}





