/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:20:15 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_PE_UART_UTIL_H_
#define __PROTOCOL_PE_UART_UTIL_H_
#include "ProtocolPEUartMacro.h"
#include "ProtocolPEUartDataType.h"
#include "ProtocolPEUartPort.h"


//写入无符号16位数据到缓存
void ProtocolPEUartUtilWriteUint16ToBuffer(uint16_t val,uint8_t* buffer);

//写入无符号32位数据到缓存
void ProtocolPEUartUtilWriteUint32ToBuffer(uint32_t val,uint8_t* buffer);

//写入无符号64位数据到缓存
void ProtocolPEUartUtilWriteUint64ToBuffer(uint64_t val,uint8_t* buffer);

//写入有符号16位数据到缓存
void ProtocolPEUartUtilWriteInt16ToBuffer(int16_t val,uint8_t* buffer);

//写入有符号32位数据到缓存
void ProtocolPEUartUtilWriteInt32ToBuffer(int32_t val,uint8_t* buffer);

//写入有符号64位数据到缓存
void ProtocolPEUartUtilWriteInt64ToBuffer(int64_t val,uint8_t* buffer);

//从协议包中获取32位无符号整型数据
uint32_t ProtocolPEUartUtilReadUint32Buffer(uint8_t* bufferPtr);

//从协议包中获取16位无符号整型数据
uint16_t ProtocolPEUartUtilReadUint16Buffer(uint8_t* bufferPtr);

//从缓存中读取64位无符号数
uint64_t ProtocolPEUartUtilReadUint64FormBuffer(uint8_t* buffer);

//从协议包中获取16位有符号整型数据
int16_t ProtocolPEUartUtilReadInt16Buffer(uint8_t* bufferPtr);

//从协议包中获取32位有符号整型数据
int32_t ProtocolPEUartUtilReadInt32Buffer(uint8_t* bufferPtr);

//从缓存中读取64位有符号数
int64_t ProtocolPEUartUtilReadInt64FormBuffer(uint8_t* bufferPtr);


#endif










