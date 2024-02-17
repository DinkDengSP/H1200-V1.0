/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-14 09:10:23 +0800
************************************************************************************************/ 
#include "BoardLampLed.h"

//制冷灯IO
#define BOARD_LAMP_LED_IO                   MCU_PIN_E_11
//制冷灯输出有效电平
#define VALID_LEVEL_BOARD_LAMP_LED          GPIO_PIN_RESET
//制冷灯输出无效电平
#define INVALID_LEVEL_BOARD_LAMP_LED        (GPIO_PinState)(!VALID_LEVEL_BOARD_LAMP_LED)


//制冷灯初始化
void BoardLampLedInit(PIN_STATE initState)
{
    GPIO_PinState pinStateSet = GPIO_PIN_SET;
    //端口初始化,上拉
    MCU_PinInit(BOARD_LAMP_LED_IO, MCU_PIN_MODE_OUTPUT_PP, MCU_PIN_PULL_PULLUP, MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化输出指定电平
    pinStateSet = (initState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_LAMP_LED:INVALID_LEVEL_BOARD_LAMP_LED;
    MCU_PinWriteSingle(BOARD_LAMP_LED_IO,pinStateSet);
}

//制冷灯设置状态
void BoardLampLedSetState(PIN_STATE pinState)
{
    GPIO_PinState pinStateSet = GPIO_PIN_SET;
    pinStateSet = (pinState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_LAMP_LED:INVALID_LEVEL_BOARD_LAMP_LED;
    MCU_PinWriteSingle(BOARD_LAMP_LED_IO,pinStateSet);
}
