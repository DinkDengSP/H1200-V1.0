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
#include "BoardDipKey.h"

//按键映射IO口
#define BOARD_DIP_KEY_S1_PIN                 MCU_PIN_H_14 
#define BOARD_DIP_KEY_S2_PIN                 MCU_PIN_A_15
#define BOARD_DIP_KEY_S3_PIN                 MCU_PIN_I_5
#define BOARD_DIP_KEY_S4_PIN                 MCU_PIN_I_7
#define BOARD_DIP_KEY_S5_PIN                 MCU_PIN_H_15
#define BOARD_DIP_KEY_S6_PIN                 MCU_PIN_H_13
#define BOARD_DIP_KEY_S7_PIN                 MCU_PIN_I_4
#define BOARD_DIP_KEY_S8_PIN                MCU_PIN_I_6
//按键上下拉模式
#define BOARD_DIP_KEY_PULL_S1_MODE           MCU_PIN_PULL_NOPULL
#define BOARD_DIP_KEY_PULL_S2_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S3_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S4_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S5_MODE           MCU_PIN_PULL_NOPULL
#define BOARD_DIP_KEY_PULL_S6_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S7_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S8_MODE          MCU_PIN_PULL_NOPULL 
//按键速度
#define BOARD_DIP_KEY_S1_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S2_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S3_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S4_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S5_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S6_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S7_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S8_SPEED              MCU_PIN_SPEED_FREQ_HIGH
//按键有效值
#define BOARD_DIP_KEY_S1_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S2_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S3_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S4_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S5_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S6_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S7_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S8_VALID_STATE        GPIO_PIN_RESET

//按键单个初始化
void BoardDipKeyInitSingle(BOARD_DIP_KEY dipKeyNo)
{
    switch (dipKeyNo)
    {
    case BOARD_DIP_KEY_S1:
        MCU_PinInit(BOARD_DIP_KEY_S1_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S1_MODE, BOARD_DIP_KEY_S1_SPEED,0);
        break;
    case BOARD_DIP_KEY_S2:
        MCU_PinInit(BOARD_DIP_KEY_S2_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S2_MODE, BOARD_DIP_KEY_S2_SPEED,0);
        break;
    case BOARD_DIP_KEY_S3:
        MCU_PinInit(BOARD_DIP_KEY_S3_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S3_MODE, BOARD_DIP_KEY_S3_SPEED,0);
        break;
    case BOARD_DIP_KEY_S4:
        MCU_PinInit(BOARD_DIP_KEY_S4_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S4_MODE, BOARD_DIP_KEY_S4_SPEED,0);
        break;
    case BOARD_DIP_KEY_S5:
        MCU_PinInit(BOARD_DIP_KEY_S5_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S5_MODE, BOARD_DIP_KEY_S5_SPEED,0);
        break;
    case BOARD_DIP_KEY_S6:
        MCU_PinInit(BOARD_DIP_KEY_S6_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S6_MODE, BOARD_DIP_KEY_S6_SPEED,0);
        break;
    case BOARD_DIP_KEY_S7:
        MCU_PinInit(BOARD_DIP_KEY_S7_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S7_MODE, BOARD_DIP_KEY_S7_SPEED,0);
        break;
    case BOARD_DIP_KEY_S8:
        MCU_PinInit(BOARD_DIP_KEY_S8_PIN,MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S8_MODE, BOARD_DIP_KEY_S8_SPEED,0);
        break;
    default:
        break;
    }
}

//按键全部初始化
void BoardDipKeyInitAll(void)
{
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S1);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S2);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S3);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S4);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S5);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S6);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S7);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S8);
}

//按键读取
PIN_STATE BoardDipKeyReadSingle(BOARD_DIP_KEY dipKeyNo)
{
    GPIO_PinState keyValueSingle;
    switch (dipKeyNo)
    {
    case BOARD_DIP_KEY_S1:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S1_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S1_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S2:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S2_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S2_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S3:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S3_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S3_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S4:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S4_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S4_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S5:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S5_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S5_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S6:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S6_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S6_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S7:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S7_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S7_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S8:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S8_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S8_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    default:
        return PIN_STATE_INVALID;
    }
}

//单次读取全部按键状态
void BoardDipKeyReadAll(BOARD_DIP_KEY_SET* dipKeyValAllPtr)
{
    //先设置为初始值
    dipKeyValAllPtr->valAll = 0X00;
    //然后设置各个域的实际值
    dipKeyValAllPtr->valSingles.s1Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S1);
    dipKeyValAllPtr->valSingles.s2Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S2);
    dipKeyValAllPtr->valSingles.s3Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S3);
    dipKeyValAllPtr->valSingles.s4Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S4);
    dipKeyValAllPtr->valSingles.s5Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S5);
    dipKeyValAllPtr->valSingles.s6Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S6);
    dipKeyValAllPtr->valSingles.s7Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S7);
    dipKeyValAllPtr->valSingles.s8Val = BoardDipKeyReadSingle(BOARD_DIP_KEY_S8);
}





