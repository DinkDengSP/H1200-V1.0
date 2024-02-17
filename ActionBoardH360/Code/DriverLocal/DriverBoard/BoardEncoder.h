/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-10 15:22:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 15:48:50 +0800
************************************************************************************************/ 
/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 10:59:43 +0800
************************************************************************************************/ 
#ifndef __BOARD_ENCODER_H_
#define __BOARD_ENCODER_H_
#include "DriverHeaderMCU.h"

//编码器序号
typedef enum ENCODER_NO
{
    ENCODER_NO_1    = 0x00,
    ENCODER_NO_2    = 0x01,
}ENCODER_NO;

//编码器1初始化
void BoardEncoder1Init(void);
//编码器1复位
void BoardEncoder1ResetCount(void);
//获取当前编码器1计数值
int32_t BoardEncoder1GetCurrentCount(void);
//设置当前编码器的值
void BoardEncoder1SetCurrentCount(int32_t setCount);

//编码器2初始化
void BoardEncoder2Init(void);
//编码器2复位
void BoardEncoder2ResetCount(void);
//获取当前编码器2计数值
int32_t BoardEncoder2GetCurrentCount(void);
//设置当前编码器的值
void BoardEncoder2SetCurrentCount(int32_t setCount);

//编码器初始化
void BoardEncoderInit(ENCODER_NO encoderNo);
//编码器复位
void BoardEncoderResetCount(ENCODER_NO encoderNo);
//获取当前编码器计数值
int32_t BoardEncoderGetCurrentCount(ENCODER_NO encoderNo);
//设置当前编码器计数值
void BoardEncoderSetCurrentCount(ENCODER_NO encoderNo,int32_t setCount);


#endif












