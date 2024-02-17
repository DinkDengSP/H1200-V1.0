/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardPowerControl.h"


//外部电源控制映射IO口
#define BOARD_POWER_CONTROL_1_PIN                 MCU_PIN_E_4
#define BOARD_POWER_CONTROL_2_PIN                 MCU_PIN_E_5
#define BOARD_POWER_CONTROL_3_PIN                 MCU_PIN_E_6
#define BOARD_POWER_CONTROL_4_PIN                 MCU_PIN_I_8
#define BOARD_POWER_CONTROL_5_PIN                 MCU_PIN_E_2
//外部电源控制上下拉模式
#define BOARD_POWER_CONTROL_1_PULL_MODE           MCU_PIN_PULL_PULLUP
#define BOARD_POWER_CONTROL_2_PULL_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_POWER_CONTROL_3_PULL_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_POWER_CONTROL_4_PULL_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_POWER_CONTROL_5_PULL_MODE           MCU_PIN_PULL_PULLUP
//外部电源控制速度
#define BOARD_POWER_CONTROL_1_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_POWER_CONTROL_2_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_POWER_CONTROL_3_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_POWER_CONTROL_4_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_POWER_CONTROL_5_SPEED               MCU_PIN_SPEED_FREQ_LOW
//外部电源控制有效值
#define BOARD_POWER_CONTROL_1_VALID_STATE         GPIO_PIN_RESET
#define BOARD_POWER_CONTROL_2_VALID_STATE         GPIO_PIN_RESET
#define BOARD_POWER_CONTROL_3_VALID_STATE         GPIO_PIN_RESET
#define BOARD_POWER_CONTROL_4_VALID_STATE         GPIO_PIN_RESET
#define BOARD_POWER_CONTROL_5_VALID_STATE         GPIO_PIN_RESET



//初始化板上电源控制,初始化默认电源全部关闭
void BoardPowerControlInit(void)
{
    MCU_PinInit(BOARD_POWER_CONTROL_1_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_POWER_CONTROL_1_PULL_MODE, BOARD_POWER_CONTROL_1_SPEED,0);
    MCU_PinInit(BOARD_POWER_CONTROL_2_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_POWER_CONTROL_2_PULL_MODE, BOARD_POWER_CONTROL_2_SPEED,0);
    MCU_PinInit(BOARD_POWER_CONTROL_3_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_POWER_CONTROL_3_PULL_MODE, BOARD_POWER_CONTROL_3_SPEED,0);
    MCU_PinInit(BOARD_POWER_CONTROL_4_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_POWER_CONTROL_4_PULL_MODE, BOARD_POWER_CONTROL_4_SPEED,0);
    MCU_PinInit(BOARD_POWER_CONTROL_5_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_POWER_CONTROL_5_PULL_MODE, BOARD_POWER_CONTROL_5_SPEED,0);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_1,PIN_STATE_INVALID);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_2,PIN_STATE_INVALID);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_3,PIN_STATE_INVALID);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_4,PIN_STATE_INVALID);
    BoardPowerControlWriteSingle(BOARD_POWER_MODULE_5,PIN_STATE_INVALID);
}

//写入指定板上电源控制
void BoardPowerControlWriteSingle(BOARD_POWER_NO index,PIN_STATE state)
{
    MCU_PIN pinIndex;
    GPIO_PinState writeValue;
    switch(index)
    {
        case BOARD_POWER_MODULE_1:
            writeValue = ((state == PIN_STATE_VALID)?BOARD_POWER_CONTROL_1_VALID_STATE:PIN_INVALID_LEVEL(BOARD_POWER_CONTROL_1_VALID_STATE));
            pinIndex = BOARD_POWER_CONTROL_1_PIN;
            break;
        case BOARD_POWER_MODULE_2:
            writeValue = ((state == PIN_STATE_VALID)?BOARD_POWER_CONTROL_2_VALID_STATE:PIN_INVALID_LEVEL(BOARD_POWER_CONTROL_2_VALID_STATE));
            pinIndex = BOARD_POWER_CONTROL_2_PIN;
            break;
        case BOARD_POWER_MODULE_3:
            writeValue = ((state == PIN_STATE_VALID)?BOARD_POWER_CONTROL_3_VALID_STATE:PIN_INVALID_LEVEL(BOARD_POWER_CONTROL_3_VALID_STATE));
            pinIndex = BOARD_POWER_CONTROL_3_PIN;
            break;
        case BOARD_POWER_MODULE_4:
            writeValue = ((state == PIN_STATE_VALID)?BOARD_POWER_CONTROL_4_VALID_STATE:PIN_INVALID_LEVEL(BOARD_POWER_CONTROL_4_VALID_STATE));
            pinIndex = BOARD_POWER_CONTROL_4_PIN;
            break;
        case BOARD_POWER_MODULE_5:
            writeValue = ((state == PIN_STATE_VALID)?BOARD_POWER_CONTROL_5_VALID_STATE:PIN_INVALID_LEVEL(BOARD_POWER_CONTROL_5_VALID_STATE));
            pinIndex = BOARD_POWER_CONTROL_5_PIN;
            break;
        default:
            return;
    }
    MCU_PinWriteSingle(pinIndex,writeValue);
}





