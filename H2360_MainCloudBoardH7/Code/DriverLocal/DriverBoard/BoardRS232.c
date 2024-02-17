/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-07 09:48:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-07 09:52:47 +0800
************************************************************************************************/ 
#include "BoardRS232.h"
#include "DriverConfigBoard.h"

/*******************************************RS1_UART1****************************************************************/
//RS232初始化
void BoardRS1Uart1_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART1_Init(BOARD_RS1_UART1_BAUD,BOARD_RS1_UART1_LENGTH,BOARD_RS1_UART1_STOP_BIT,BOARD_RS1_UART1_CHECK_MODE,
                        BOARD_RS1_UART1_HARD_CONTROL);
    //回调函数注册
    MCU_UART1_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS232发送缓存
void BoardRS1Uart1_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART1_SendBuffer(bufferStartPtr,sendLength);
}

//RS232发送字符串
void BoardRS1Uart1_SendString(uint8_t* stringStartPtr)
{
    MCU_UART1_SendString(stringStartPtr);
}








