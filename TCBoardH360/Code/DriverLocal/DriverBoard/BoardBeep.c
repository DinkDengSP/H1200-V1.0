/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 13:29:42 +0800
************************************************************************************************/ 
#include "BoardBeep.h"

//蜂鸣器IO
#define BOARD_BEEP_IO                   MCU_PIN_I_7
//蜂鸣器输出有效电平
#define VALID_LEVEL_BOARD_BEEP          GPIO_PIN_SET
//蜂鸣器输出无效电平
#define INVALID_LEVEL_BOARD_BEEP        (GPIO_PinState)(!VALID_LEVEL_BOARD_BEEP)

//蜂鸣器初始化
void BoardBeepInit(PIN_STATE initState)
{
    GPIO_PinState pinStateSet = GPIO_PIN_SET;
    //端口初始化,上拉
    MCU_PinInit(BOARD_BEEP_IO, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_NOPULL, MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化输出指定电平
    pinStateSet = (initState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_BEEP:INVALID_LEVEL_BOARD_BEEP;
    MCU_PinWriteSingle(BOARD_BEEP_IO,pinStateSet);
}

//蜂鸣器设置状态
void BoardBeepSetState(PIN_STATE pinState)
{
    GPIO_PinState pinStateSet = GPIO_PIN_SET;
    pinStateSet = (pinState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_BEEP:INVALID_LEVEL_BOARD_BEEP;
    MCU_PinWriteSingle(BOARD_BEEP_IO,pinStateSet);
}

















