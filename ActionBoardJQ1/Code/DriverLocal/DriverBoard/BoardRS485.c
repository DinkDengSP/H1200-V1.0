/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 17:51:39 +0800
************************************************************************************************/ 
#include "BoardRS485.h"
#include "DriverConfigBoard.h"


/****************************************************************************************************/
//RS初始化
void BoardRS485_Init(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    //串口初始化
    MCU_UART2_Init(BOARD_RS485_BAUD,BOARD_RS485_LENGTH,BOARD_RS485_STOP_BIT,BOARD_RS485_CHECK_MODE);
    //回调函数注册
    MCU_UART2_ConfigCallBack(recvCallBack,sendStartCallBack,sendOverCallBack);
}

//RS发送缓存
void BoardRS485_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART2_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS485_SendString(uint8_t* stringStartPtr)
{
    MCU_UART2_SendString(stringStartPtr);
}
