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
#ifndef __BOARD_POWER_CONTROL_H_
#define __BOARD_POWER_CONTROL_H_
#include "DriverHeaderMCU.h"

//板上电源控制端口
typedef enum BOARD_POWER_NO
{
    BOARD_POWER_MODULE_1 = 0X00,
    BOARD_POWER_MODULE_2 = 0X01,
    BOARD_POWER_MODULE_3 = 0X02,
    BOARD_POWER_MODULE_4 = 0X03,
    BOARD_POWER_MODULE_5 = 0X04,
}BOARD_POWER_NO;

//初始化板上电源控制,初始化默认电源全部关闭
void BoardPowerControlInit(void);

//写入指定板上电源控制
void BoardPowerControlWriteSingle(BOARD_POWER_NO index,PIN_STATE state);



#endif






