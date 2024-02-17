/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 11:10:09 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_SYS_LOG_H_
#define __SRV_IMPL_BASE_SYS_LOG_H_
#include "SrvImplBase.h"

//初始化变量
void SrvImplBaseSysLogVarInit(void);

//系统日志发送函数
void SystemPrintf(const char *format, ...);

//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//系统日志发送字符串
void SystemSendString(uint8_t* strPtr);

//系统发送数组显示
void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);

//数据发送
void SrvImplBaseSysLogSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

#endif















