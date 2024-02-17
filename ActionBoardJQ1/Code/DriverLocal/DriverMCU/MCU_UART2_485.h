/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 17:20:02 +0800
************************************************************************************************/ 
#ifndef __MCU_UART2_485_H_
#define __MCU_UART2_485_H_
#include "DriverHeaderCore.h"

//获取互斥锁
void MCU_UART2_GetLock(void);

//释放互斥锁
void MCU_UART2_ReleaseLock(void);

//串口初始化
void MCU_UART2_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode);

//配置串口回调函数
void MCU_UART2_ConfigCallBack(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack);

//串口发送数组
void MCU_UART2_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength);

//串口发送字符串,常规模式下发送
void MCU_UART2_SendString(uint8_t* stringStartPtr);

//串口发送字符串,中断中发送
void MCU_UART2_SendStringInt(uint8_t* stringStartPtr);

//串口打印,带格式化
void MCU_UART2_Printf(const char *format, ...);

//串口接收到的数据释放
void MCU_UART2_RecvDatRelease(UART_RECV_PACK* uartMsgRecvPtr);

#endif



