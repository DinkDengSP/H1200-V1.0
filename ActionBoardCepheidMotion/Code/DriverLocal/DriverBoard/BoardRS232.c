/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 14:42:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 14:51:37 +0800
************************************************************************************************/ 
#include "BoardRS232.h"
#include "DriverConfigBoard.h"


/****************************************************************************************************/
//RS初始化
void BoardRS232A_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART2_Init(BOARD_RS232A_BAUD,BOARD_RS232A_LENGTH,BOARD_RS232A_STOP_BIT,BOARD_RS232A_CHECK_MODE,
                        BOARD_RS232A_HARD_CONTROL);
    //回调函数注册
    MCU_UART2_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS232A_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART2_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS232A_SendString(uint8_t* stringStartPtr)
{
    MCU_UART2_SendString(stringStartPtr);
}


/****************************************************************************************************/
//RS初始化
void BoardRS232B_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART1_Init(BOARD_RS232B_BAUD,BOARD_RS232B_LENGTH,BOARD_RS232B_STOP_BIT,BOARD_RS232B_CHECK_MODE,
                        BOARD_RS232B_HARD_CONTROL);
    //回调函数注册
    MCU_UART1_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS232B_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART1_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS232B_SendString(uint8_t* stringStartPtr)
{
    MCU_UART1_SendString(stringStartPtr);
}

/****************************************************************************************************/
//RS初始化
void BoardRS232C_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART3_Init(BOARD_RS232C_BAUD,BOARD_RS232C_LENGTH,BOARD_RS232C_STOP_BIT,BOARD_RS232C_CHECK_MODE,
                        BOARD_RS232C_HARD_CONTROL);
    //回调函数注册
    MCU_UART3_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS232C_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART3_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS232C_SendString(uint8_t* stringStartPtr)
{
    MCU_UART3_SendString(stringStartPtr);
}

/****************************************************************************************************/
//RS初始化
void BoardRS232D_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART6_Init(BOARD_RS232D_BAUD,BOARD_RS232D_LENGTH,BOARD_RS232D_STOP_BIT,BOARD_RS232D_CHECK_MODE,
                        BOARD_RS232D_HARD_CONTROL);
    //回调函数注册
    MCU_UART6_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS232D_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART6_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS232D_SendString(uint8_t* stringStartPtr)
{
    MCU_UART6_SendString(stringStartPtr);
}
















