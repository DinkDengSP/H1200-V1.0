/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 18:59:10 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 19:17:15 +0800
************************************************************************************************/ 
#ifndef __PROT_UTIL_H_
#define __PROT_UTIL_H_
#include "DriverConfigBase.h"
#include "ErrorCode.h"

//写入无符号16位数据到缓存
void ProtUtilWriteUint16ToBuffer(uint16_t val,uint8_t* buffer);

//写入无符号32位数据到缓存
void ProtUtilWriteUint32ToBuffer(uint32_t val,uint8_t* buffer);

//写入无符号64位数据到缓存
void ProtUtilWriteUint64ToBuffer(uint64_t val,uint8_t* buffer);

//写入有符号16位数据到缓存
void ProtUtilWriteInt16ToBuffer(int16_t val,uint8_t* buffer);

//写入有符号32位数据到缓存
void ProtUtilWriteInt32ToBuffer(int32_t val,uint8_t* buffer);

//写入有符号64位数据到缓存
void ProtUtilWriteInt64ToBuffer(int64_t val,uint8_t* buffer);

//从协议包中获取32位无符号整型数据
uint32_t ProtUtilReadUint32Buffer(uint8_t* bufferPtr);

//从协议包中获取16位无符号整型数据
uint16_t ProtUtilReadUint16Buffer(uint8_t* bufferPtr);

//从缓存中读取64位无符号数
uint64_t ProtUtilReadUint64Buffer(uint8_t* buffer);

//从协议包中获取32位有符号整型数据
int32_t ProtUtilReadInt32Buffer(uint8_t* bufferPtr);

//从协议包中获取16位有符号整型数据
int16_t ProtUtilReadInt16Buffer(uint8_t* bufferPtr);

//从缓存中读取64位有符号数
int64_t ProtUtilReadInt64Buffer(uint8_t* bufferPtr);

//获取错误代码中的板号
uint8_t ProtUtilGetBoardIdFromErrorCode(ERROR_SUB errorCodeSrc);

//获取错误代码中的错误类型
ERROR_SUB ProtUtilGetErrorTypeFromErrorCode(ERROR_SUB errorCodeSrc);



#endif
















