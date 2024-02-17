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
#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "DriverHeaderMCU.h"

//蜂鸣器初始化
void BoardBeepInit(PIN_STATE initState);

//蜂鸣器设置状态
void BoardBeepSetState(PIN_STATE pinState);


#endif






