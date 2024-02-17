/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-14 10:24:09 +0800
************************************************************************************************/ 
#ifndef __BOARD_LAMP_LED_H_
#define __BOARD_LAMP_LED_H_
#include "DriverHeaderMCU.h"

//制冷灯初始化
void BoardLampLedInit(PIN_STATE initState);

//制冷灯设置状态
void BoardLampLedSetState(PIN_STATE pinState);

#endif


