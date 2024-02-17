/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:17:52 +0800
************************************************************************************************/ 
#ifndef __MCU_RTT_H_
#define __MCU_RTT_H_
#include "DriverHeaderCore.h"

//RTT初始化
void MCU_RTT_Init(void);

//获取锁
void MCU_RTT_GetLock(void);

//释放锁
void MCU_RTT_ReleaseLock(void);

//RTT发送数组
void MCU_RTT_SendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//RTT发送数组
void MCU_RTT_SendBufferInt(uint8_t* bufferPtr,uint16_t bufferLength);

//RTT发送字符串,不能在中断中使用
void MCU_RTT_SendString(uint8_t* stringPtr);

//RTT发送字符串,可以在中断中使用
void MCU_RTT_SendStringInt(uint8_t* stringPtr);

//RTT打印
void MCU_RTT_Printf(const char *format, ...);

#endif





















