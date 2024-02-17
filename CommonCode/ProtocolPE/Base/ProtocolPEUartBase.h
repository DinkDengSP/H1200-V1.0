/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:23:34 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_PE_UART_BASE_H_
#define __PROTOCOL_PE_UART_BASE_H_
#include "ProtocolPEUartUtil.h"

//初始化SOCKET
void ProtocolPEUartSocketInit(void);

//检查SOCKET是否已经初始化
FlagStatus ProtocolPEUartSocketInitQuery(void);

//SOCKET数据包接收
void ProtocolPEUartSocketPackRecv(UART_RECV_PACK* uartRecvPackPtr,uint32_t currentTickMs);

//SOCKET1内部周期性数据处理
void ProtocolPEUartRunPeriod(uint32_t currentTickMs);

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB ProtocolPEUartSendCmdOnly(uint16_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength);

//SOCKET发送指令ACK包
ERROR_SUB ProtocolPEUartSendAckCmd(void);


#endif





