/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 10:25:38 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_SYS_LOG_H_
#define __SRV_IMPL_BASE_SYS_LOG_H_
#include "SrvImplBase.h"

void SrvImplBaseSysLogInit(void);

//系统日志发送函数
void SystemPrintf(const char *format, ...);

//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//系统日志发送字符串
void SystemSendString(uint8_t* strPtr);

//系统发送数组显示
void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);


#endif






















