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
#include "BoardDipLed.h"

//DIP_LED映射IO口
#define BOARD_DIP_LED_D13_PIN                 MCU_PIN_H_9
#define BOARD_DIP_LED_D15_PIN                 MCU_PIN_H_10
#define BOARD_DIP_LED_D19_PIN                 MCU_PIN_H_11
#define BOARD_DIP_LED_D21_PIN                 MCU_PIN_H_12
#define BOARD_DIP_LED_D22_PIN                 MCU_PIN_G_10
#define BOARD_DIP_LED_D25_PIN                 MCU_PIN_G_12
#define BOARD_DIP_LED_D27_PIN                 MCU_PIN_G_13
#define BOARD_DIP_LED_D26_PIN                 MCU_PIN_H_8
//DIP_LED上下拉模式
#define BOARD_DIP_LED_PULL_D13_MODE           MCU_PIN_PULL_PULLUP
#define BOARD_DIP_LED_PULL_D15_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_DIP_LED_PULL_D19_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_DIP_LED_PULL_D21_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_DIP_LED_PULL_D22_MODE           MCU_PIN_PULL_PULLUP
#define BOARD_DIP_LED_PULL_D25_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_DIP_LED_PULL_D27_MODE           MCU_PIN_PULL_PULLUP 
#define BOARD_DIP_LED_PULL_D26_MODE           MCU_PIN_PULL_PULLUP 
//DIP_LED速度
#define BOARD_DIP_LED_D13_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D15_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D19_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D21_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D22_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D25_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D27_SPEED               MCU_PIN_SPEED_FREQ_LOW
#define BOARD_DIP_LED_D26_SPEED               MCU_PIN_SPEED_FREQ_LOW
//DIP_LED有效值
#define BOARD_DIP_LED_D13_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D15_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D19_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D21_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D22_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D25_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D27_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_LED_D26_VALID_STATE         GPIO_PIN_RESET

//拨码LED单个初始化
void BoardDipLedInitSingle(BOARD_DIP_LED dipLedNo,PIN_STATE initState)
{
    GPIO_PinState initPinState;
    switch (dipLedNo)
    {
    case BOARD_DIP_LED_D13:
        MCU_PinInit(BOARD_DIP_LED_D13_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D13_MODE, BOARD_DIP_LED_D13_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D13_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D13_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D13_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D15:
        MCU_PinInit(BOARD_DIP_LED_D15_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D15_MODE, BOARD_DIP_LED_D15_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D15_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D15_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D15_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D19:
        MCU_PinInit(BOARD_DIP_LED_D19_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D19_MODE, BOARD_DIP_LED_D19_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D19_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D19_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D19_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D21:
        MCU_PinInit(BOARD_DIP_LED_D21_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D21_MODE, BOARD_DIP_LED_D21_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D21_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D21_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D21_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D22:
        MCU_PinInit(BOARD_DIP_LED_D22_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D22_MODE, BOARD_DIP_LED_D22_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D22_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D22_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D22_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D25:
        MCU_PinInit(BOARD_DIP_LED_D25_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D25_MODE, BOARD_DIP_LED_D25_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D25_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D25_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D25_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D27:
        MCU_PinInit(BOARD_DIP_LED_D27_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D27_MODE, BOARD_DIP_LED_D27_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D27_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D27_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D27_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D26:
        MCU_PinInit(BOARD_DIP_LED_D26_PIN,MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D26_MODE, BOARD_DIP_LED_D26_SPEED,0);
        initPinState = (initState == PIN_STATE_VALID)?BOARD_DIP_LED_D26_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D26_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D26_PIN,initPinState);
        break;
    default:
        break;
    }
}

//拨码LED全部初始化
void BoardDipLedInitAll(PIN_STATE initState)
{
    BoardDipLedInitSingle(BOARD_DIP_LED_D13,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D15,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D19,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D21,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D22,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D25,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D27,initState);
    BoardDipLedInitSingle(BOARD_DIP_LED_D26,initState);
}

//拨码LED单个
void BoardDipLedWriteSingle(BOARD_DIP_LED dipLedNo,PIN_STATE state)
{
    GPIO_PinState initPinState;
    switch (dipLedNo)
    {
    case BOARD_DIP_LED_D13:
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D13_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D13_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D13_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D15:
        MCU_PinInit(BOARD_DIP_LED_D15_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D15_MODE, BOARD_DIP_LED_D15_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D15_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D15_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D15_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D19:
        MCU_PinInit(BOARD_DIP_LED_D19_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D19_MODE, BOARD_DIP_LED_D19_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D19_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D19_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D19_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D21:
        MCU_PinInit(BOARD_DIP_LED_D21_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D21_MODE, BOARD_DIP_LED_D21_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D21_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D21_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D21_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D22:
        MCU_PinInit(BOARD_DIP_LED_D22_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D22_MODE, BOARD_DIP_LED_D22_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D22_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D22_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D22_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D25:
        MCU_PinInit(BOARD_DIP_LED_D25_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D25_MODE, BOARD_DIP_LED_D25_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D25_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D25_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D25_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D27:
        MCU_PinInit(BOARD_DIP_LED_D27_PIN, MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D27_MODE, BOARD_DIP_LED_D27_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D27_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D27_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D27_PIN,initPinState);
        break;
    case BOARD_DIP_LED_D26:
        MCU_PinInit(BOARD_DIP_LED_D26_PIN,MCU_PIN_MODE_OUTPUT_PP, BOARD_DIP_LED_PULL_D26_MODE, BOARD_DIP_LED_D26_SPEED,0);
        initPinState = (state == PIN_STATE_VALID)?BOARD_DIP_LED_D26_VALID_STATE:PIN_INVALID_LEVEL(BOARD_DIP_LED_D26_VALID_STATE);
        MCU_PinWriteSingle(BOARD_DIP_LED_D26_PIN,initPinState);
        break;
    default:
        break;
    }
}

//拨码LED全部写入
void BoardDipLedWriteAll(uint8_t stateAll)
{
    uint8_t indexUtil = 0;
    PIN_STATE pinState;
    for(indexUtil = 0; indexUtil < 8; indexUtil++)
    {
        pinState = (PIN_STATE)((stateAll>>indexUtil)&0X01);
        BoardDipLedWriteSingle((BOARD_DIP_LED)(BOARD_DIP_LED_D13+indexUtil),pinState);
    }
}


