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
#ifndef __BOARD_DIP_LED_H_
#define __BOARD_DIP_LED_H_
#include "DriverHeaderMCU.h"

typedef enum BOARD_DIP_LED
{
    BOARD_DIP_LED_D13  = 0X00,
    BOARD_DIP_LED_D15  = 0X01,
    BOARD_DIP_LED_D19  = 0X02,
    BOARD_DIP_LED_D21  = 0X03,
    BOARD_DIP_LED_D22  = 0X04,
    BOARD_DIP_LED_D25  = 0X05,
    BOARD_DIP_LED_D27  = 0X06,
    BOARD_DIP_LED_D26  = 0X07,
}BOARD_DIP_LED;

//拨码LED单个初始化
void BoardDipLedInitSingle(BOARD_DIP_LED dipLedNo,PIN_STATE initState);

//拨码LED全部初始化
void BoardDipLedInitAll(PIN_STATE initState);

//拨码LED单个
void BoardDipLedWriteSingle(BOARD_DIP_LED dipLedNo,PIN_STATE state);

//拨码LED全部写入
void BoardDipLedWriteAll(uint8_t stateAll);




#endif



