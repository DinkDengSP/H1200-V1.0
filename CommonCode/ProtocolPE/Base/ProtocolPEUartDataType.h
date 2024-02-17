/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 17:09:37 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_PE_UART_DATA_TYPE_H_
#define __PROTOCOL_PE_UART_DATA_TYPE_H_
#include "ProtocolPEUartMacro.h"
#include "ADT_CircularPtrList.h"

//数据包发送结构体
typedef struct PE_UART_PACKAGE
{
    uint16_t cmdCode;//接收数据指令码
    uint16_t dataBodyLength;//接收有效数据长度
    uint8_t* dataBodyBufferPtr;//接收到的有效数据缓存
}PE_UART_PACKAGE;

//数据包接收缓存
typedef struct PE_UART_RECV_CACHE
{
    uint32_t lastRecvTimeStamp;//本指令数据包的接收时间
    uint16_t recvDataLength;//接收到的指令长度,包含指令的头
    uint16_t recvBodyLength;//指令包体的长度,也就是有效的数据长度
    uint8_t recvDataBuf[PROTOCOL_PE_UART_SINGLE_PACK_DATA_MAX];//缓存的数据,包含指令头部数据
}PE_UART_RECV_CACHE;

//指令等待数据缓冲
typedef struct PE_UART_CMD_WAIT_PACK
{
    uint16_t cmdValue;//指令码
    uint16_t cmdParamBufferLength;//指令参数长度
    uint8_t* cmdParamBufferPtr;//指令参数缓存
    uint32_t lastSendTimeTickMs;//上一次发送的时间
    uint8_t retryCount;
}PE_UART_CMD_WAIT_PACK;

//UART初始化信息集合
typedef struct PROTOCOL_PE_UART_INIT_SET
{
    TX_MUTEX mutexSocketDataAccess;//SOCKET数据访问互斥信号量
    //发送缓存
    ADT_CIRCULAR_PTR_LIST_T listSendCmdWaitAckPtr;//发送指令包等待ACK列表缓存指针
}PROTOCOL_PE_UART_INIT_SET;

#endif









