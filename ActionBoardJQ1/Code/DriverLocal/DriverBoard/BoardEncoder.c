/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 14:51:33 +0800
************************************************************************************************/ 
#include "BoardEncoder.h"

//编码器1初始化
void BoardEncoder1Init(void)
{
    MCU_Timer5EncoderInit();
}
//编码器1复位
void BoardEncoder1ResetCount(void)
{
    MCU_Timer5EncoderResetCount();
}
//获取当前编码器1计数值
int32_t BoardEncoder1GetCurrentCount(void)
{
    return MCU_Timer5EncoderGetCurrentCount();
}

//编码器2初始化
void BoardEncoder2Init(void)
{
    MCU_Timer2EncoderInit();
}
//编码器2复位
void BoardEncoder2ResetCount(void)
{
    MCU_Timer2EncoderResetCount();
}
//获取当前编码器2计数值
int32_t BoardEncoder2GetCurrentCount(void)
{
    return MCU_Timer2EncoderGetCurrentCount();
}

//编码器初始化
void BoardEncoderInit(ENCODER_NO encoderNo)
{
    if(encoderNo == ENCODER_NO_1)
    {
        BoardEncoder1Init();
    }
    else
    {
        BoardEncoder2Init();
    }
}

//编码器复位
void BoardEncoderResetCount(ENCODER_NO encoderNo)
{
    if(encoderNo == ENCODER_NO_1)
    {
        BoardEncoder1ResetCount();
    }
    else
    {
        BoardEncoder2ResetCount();
    }
}

//获取当前编码器计数值
int32_t BoardEncoderGetCurrentCount(ENCODER_NO encoderNo)
{
    if(encoderNo == ENCODER_NO_1)
    {
        return BoardEncoder1GetCurrentCount();
    }
    else if(encoderNo == ENCODER_NO_2)
    {
        return BoardEncoder2GetCurrentCount();
    }
    else
    {
        return 0;
    }
}














