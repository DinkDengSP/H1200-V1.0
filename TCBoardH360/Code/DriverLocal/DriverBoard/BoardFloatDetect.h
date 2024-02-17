/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 12:09:33 +0800
************************************************************************************************/ 
#ifndef __BOARD_FLOAT_DETECT_H_
#define __BOARD_FLOAT_DETECT_H_
#include "DriverHeaderMCU.h"

//浮球检测初始化
void BoardFloatDetectInit(void);

//获取浮球当前状态
PIN_STATE BoardFloatDetectRead(void);

#endif



