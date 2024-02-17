/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-27 10:53:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 17:22:54 +0800
************************************************************************************************/ 
#include "BoardIdentity.h"

static uint8_t boardID = 0;

//按键映射IO口
#define BOARD_IDENTITY1_PIN                 MCU_PIN_B_12   
#define BOARD_IDENTITY2_PIN                 MCU_PIN_B_13  
#define BOARD_IDENTITY3_PIN                 MCU_PIN_B_14
#define BOARD_IDENTITY4_PIN                 MCU_PIN_B_15
//按键上下拉模式
#define BOARD_IDENTITY1_PULL_MODE           MCU_PIN_PULL_PULLDOWN
#define BOARD_IDENTITY2_PULL_MODE           MCU_PIN_PULL_PULLDOWN 
#define BOARD_IDENTITY3_PULL_MODE           MCU_PIN_PULL_PULLDOWN 
#define BOARD_IDENTITY4_PULL_MODE           MCU_PIN_PULL_PULLDOWN 
//按键速度
#define BOARD_IDENTITY1_SPEED               MCU_PIN_SPEED_FREQ_MEDIUM
#define BOARD_IDENTITY2_SPEED               MCU_PIN_SPEED_FREQ_MEDIUM
#define BOARD_IDENTITY3_SPEED               MCU_PIN_SPEED_FREQ_MEDIUM
#define BOARD_IDENTITY4_SPEED               MCU_PIN_SPEED_FREQ_MEDIUM
//按键有效值
#define BOARD_IDENTITY1_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY2_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY3_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY4_VALID_STATE         GPIO_PIN_SET

//屏蔽板号0
uint8_t BoardIdentityPortGet(void)
{
    GPIO_PinState pinValueSingle;
    uint8_t localIdentity = 0;
    pinValueSingle = MCU_PinReadSingle(BOARD_IDENTITY4_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY4_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = MCU_PinReadSingle(BOARD_IDENTITY3_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY3_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = MCU_PinReadSingle(BOARD_IDENTITY2_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY2_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = MCU_PinReadSingle(BOARD_IDENTITY1_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY1_VALID_STATE)?1:0;

    boardID = localIdentity;
    return boardID;
}

//板卡ID识别
void BoardIdentityPortInit(void)
{
    MCU_PinInit(BOARD_IDENTITY1_PIN, MCU_PIN_MODE_INPUT, BOARD_IDENTITY1_PULL_MODE, BOARD_IDENTITY1_SPEED,0);
    MCU_PinInit(BOARD_IDENTITY2_PIN, MCU_PIN_MODE_INPUT, BOARD_IDENTITY2_PULL_MODE, BOARD_IDENTITY2_SPEED,0);
    MCU_PinInit(BOARD_IDENTITY3_PIN, MCU_PIN_MODE_INPUT, BOARD_IDENTITY3_PULL_MODE, BOARD_IDENTITY3_SPEED,0);
    MCU_PinInit(BOARD_IDENTITY4_PIN, MCU_PIN_MODE_INPUT, BOARD_IDENTITY4_PULL_MODE, BOARD_IDENTITY4_SPEED,0);
    //获取板卡ID
    boardID = BoardIdentityPortGet();
}

//获取板卡ID
uint8_t BoardIdentityGet(void)
{
    return boardID;
}

