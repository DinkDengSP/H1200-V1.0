/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardIdentity.h"
#include "BoardPortIn.h"

static uint8_t boardID = 0;

//按键映射IO口
#define BOARD_IDENTITY0_PIN                 BOARD_PORT_IN_SMBUS_IN1   
#define BOARD_IDENTITY1_PIN                 BOARD_PORT_IN_SMBUS_IN2  
#define BOARD_IDENTITY2_PIN                 BOARD_PORT_IN_SMBUS_IN3
#define BOARD_IDENTITY3_PIN                 BOARD_PORT_IN_SMBUS_IN4
//按键有效值
#define BOARD_IDENTITY0_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY1_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY2_VALID_STATE         GPIO_PIN_SET
#define BOARD_IDENTITY3_VALID_STATE         GPIO_PIN_SET

uint8_t BoardIdentityPortGet(void)
{
    GPIO_PinState pinValueSingle;
    uint8_t localIdentity = 0;

    pinValueSingle = BoardPortInReadState(BOARD_IDENTITY3_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY3_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = BoardPortInReadState(BOARD_IDENTITY2_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY2_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = BoardPortInReadState(BOARD_IDENTITY1_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY1_VALID_STATE)?1:0;
    localIdentity <<= 1;

    pinValueSingle = BoardPortInReadState(BOARD_IDENTITY0_PIN);
    localIdentity += (pinValueSingle == BOARD_IDENTITY0_VALID_STATE)?1:0;

    boardID = localIdentity;
    return boardID;
}

//板卡ID识别
void BoardIdentityPortInit(void)
{
    //获取板卡ID
    boardID = BoardIdentityPortGet();
}

//获取板卡ID
uint8_t BoardIdentityGet(void)
{
    return boardID;
}



