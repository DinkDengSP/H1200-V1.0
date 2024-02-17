/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 18:02:59
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_H_
#define __SRV_IMPL_BASE_H_
#include "DriverHeaderSystem.h"
#include "DriverConfigSystem.h"
#include "SystemParam.h"
#include "SystemState.h"

//系统日志发送函数
extern void SystemPrintf(const char *format, ...);

//系统日志发送缓冲区
extern void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//系统日志发送字符串
extern void SystemSendString(uint8_t* strPtr);

//系统发送数组显示
extern void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);

#endif








