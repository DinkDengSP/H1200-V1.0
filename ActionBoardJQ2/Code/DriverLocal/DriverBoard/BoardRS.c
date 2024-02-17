/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 15:03:39 +0800
************************************************************************************************/ 
#include "BoardRS.h"
#include "DriverConfigBoard.h"


/****************************************************************************************************/
//RS初始化
void BoardRS1_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART1_Init(BOARD_RS1_BAUD,BOARD_RS1_LENGTH,BOARD_RS1_STOP_BIT,BOARD_RS1_CHECK_MODE,
                        BOARD_RS1_HARD_CONTROL);
    //回调函数注册
    MCU_UART1_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART1_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS1_SendString(uint8_t* stringStartPtr)
{
    MCU_UART1_SendString(stringStartPtr);
}


/****************************************************************************************************/
//RS初始化
void BoardRS2_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART3_Init(BOARD_RS2_BAUD,BOARD_RS2_LENGTH,BOARD_RS2_STOP_BIT,BOARD_RS2_CHECK_MODE,
                        BOARD_RS2_HARD_CONTROL);
    //回调函数注册
    MCU_UART3_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS2_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART3_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS2_SendString(uint8_t* stringStartPtr)
{
    MCU_UART3_SendString(stringStartPtr);
}



