/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 12:09:33 +0800
************************************************************************************************/ 
#ifndef __BOARD_BEEP_H_
#define __BOARD_BEEP_H_
#include "DriverHeaderMCU.h"

//蜂鸣器初始化
void BoardBeepInit(PIN_STATE initState);

//蜂鸣器设置状态
void BoardBeepSetState(PIN_STATE pinState);


#endif


















