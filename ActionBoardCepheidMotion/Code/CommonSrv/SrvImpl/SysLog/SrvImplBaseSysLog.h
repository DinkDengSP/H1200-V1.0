/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 09:55:52 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_SYS_LOG_H_
#define __SRV_IMPL_BASE_SYS_LOG_H_
#include "CommonSrvImplBase.h"


//初始化日志端口
void SrvImplBaseSysLogVarInit(void);

//系统日志发送函数
void SystemPrintf(const char *format, ...);

//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//系统日志发送字符串
void SystemSendString(uint8_t* strPtr);

//系统发送数组显示
void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);


#endif












