/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:20:15 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_C21_NET_UTIL_H_
#define __PROTOCOL_C21_NET_UTIL_H_
#include "ProtocolC21NetMacro.h"
#include "ProtocolC21NetDataType.h"
#include "ProtocolC21NetPort.h"


//写入无符号16位数据到缓存
void ProtocolC21NetUtilWriteUint16ToBuffer(uint16_t val,uint8_t* buffer);

//写入无符号32位数据到缓存
void ProtocolC21NetUtilWriteUint32ToBuffer(uint32_t val,uint8_t* buffer);

//写入无符号64位数据到缓存
void ProtocolC21NetUtilWriteUint64ToBuffer(uint64_t val,uint8_t* buffer);

//写入有符号16位数据到缓存
void ProtocolC21NetUtilWriteInt16ToBuffer(int16_t val,uint8_t* buffer);

//写入有符号32位数据到缓存
void ProtocolC21NetUtilWriteInt32ToBuffer(int32_t val,uint8_t* buffer);

//写入有符号64位数据到缓存
void ProtocolC21NetUtilWriteInt64ToBuffer(int64_t val,uint8_t* buffer);

//从协议包中获取32位无符号整型数据
uint32_t ProtocolC21NetUtilReadUint32Buffer(uint8_t* bufferPtr);

//从协议包中获取16位无符号整型数据
uint16_t ProtocolC21NetUtilReadUint16Buffer(uint8_t* bufferPtr);

//从缓存中读取64位无符号数
uint64_t ProtocolC21NetUtilReadUint64FormBuffer(uint8_t* buffer);

//从协议包中获取16位有符号整型数据
int16_t ProtocolC21NetUtilReadInt16Buffer(uint8_t* bufferPtr);

//从协议包中获取32位有符号整型数据
int32_t ProtocolC21NetUtilReadInt32Buffer(uint8_t* bufferPtr);

//从缓存中读取64位有符号数
int64_t ProtocolC21NetUtilReadInt64FormBuffer(uint8_t* bufferPtr);


#endif










