/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-06 12:53:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 14:46:52 +0800
************************************************************************************************/
#include "BoardDipKey.h"
//按键映射IO口
#define BOARD_DIP_KEY_S3_PIN                 MCU_PIN_I_4
#define BOARD_DIP_KEY_S4_PIN                 MCU_PIN_I_5
#define BOARD_DIP_KEY_S5_PIN                 MCU_PIN_I_6
//按键上下拉模式
#define BOARD_DIP_KEY_PULL_S3_MODE           MCU_PIN_PULL_NOPULL
#define BOARD_DIP_KEY_PULL_S4_MODE           MCU_PIN_PULL_NOPULL 
#define BOARD_DIP_KEY_PULL_S5_MODE           MCU_PIN_PULL_NOPULL 
//按键速度
#define BOARD_DIP_KEY_S3_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S4_SPEED               MCU_PIN_SPEED_FREQ_HIGH
#define BOARD_DIP_KEY_S5_SPEED               MCU_PIN_SPEED_FREQ_HIGH
//按键有效值
#define BOARD_DIP_KEY_S3_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S4_VALID_STATE         GPIO_PIN_RESET
#define BOARD_DIP_KEY_S5_VALID_STATE         GPIO_PIN_RESET

//按键单个初始化
void BoardDipKeyInitSingle(BOARD_DIP_KEY dipKeyNo)
{
    switch (dipKeyNo)
    {
    case BOARD_DIP_KEY_S3:
        MCU_PinInit(BOARD_DIP_KEY_S3_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S3_MODE, BOARD_DIP_KEY_S3_SPEED,0);
        break;
    case BOARD_DIP_KEY_S4:
        MCU_PinInit(BOARD_DIP_KEY_S4_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S4_MODE, BOARD_DIP_KEY_S4_SPEED,0);
        break;
    case BOARD_DIP_KEY_S5:
        MCU_PinInit(BOARD_DIP_KEY_S5_PIN, MCU_PIN_MODE_INPUT, BOARD_DIP_KEY_PULL_S5_MODE, BOARD_DIP_KEY_S5_SPEED,0);
        break;
    default:
        break;
    }
}

//按键全部初始化
void BoardDipKeyInitAll(void)
{
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S3);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S4);
    BoardDipKeyInitSingle(BOARD_DIP_KEY_S5);
}

//按键读取
PIN_STATE BoardDipKeyReadSingle(BOARD_DIP_KEY dipKeyNo)
{
    GPIO_PinState keyValueSingle;
    switch (dipKeyNo)
    {
    case BOARD_DIP_KEY_S3:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S3_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S3_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S4:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S4_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S4_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
    case BOARD_DIP_KEY_S5:
        keyValueSingle = MCU_PinReadSingle(BOARD_DIP_KEY_S5_PIN);
        return (keyValueSingle == BOARD_DIP_KEY_S5_VALID_STATE)?PIN_STATE_VALID:PIN_STATE_INVALID;
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
    dipKeyValAllPtr->valSingles.s3Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S3);
    dipKeyValAllPtr->valSingles.s4Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S4);
    dipKeyValAllPtr->valSingles.s5Val  = BoardDipKeyReadSingle(BOARD_DIP_KEY_S5);
}








