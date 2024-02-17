/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 15:43:29 +0800
************************************************************************************************/ 
#ifndef __MCU_TIMER5_ENCODER_H_
#define __MCU_TIMER5_ENCODER_H_
#include "DriverHeaderMCU.h"

//编码器初始化
void MCU_Timer5EncoderInit(void);

//编码器开始
void MCU_Timer5EncoderStart(void);

//编码器停止
void MCU_Timer5EncoderStop(void);

//编码器初始化计数值
void MCU_Timer5EncoderResetCount(void);

//设置编码器当前计数值
void MCU_Timer5EncoderSetCountCurrent(int32_t setCount);

//获取当前编码器计数值
int32_t MCU_Timer5EncoderGetCurrentCount(void);

#endif











