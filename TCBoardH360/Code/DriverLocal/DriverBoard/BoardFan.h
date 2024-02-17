/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-14 10:39:51 +0800
************************************************************************************************/ 
#ifndef __BOARD_FAN_H_
#define __BOARD_FAN_H_
#include "DriverHeaderMCU.h"

typedef enum BOARD_FAN
{
    BOARD_FAN_MAIN_MOTOR    = 0X00,
    BOARD_FAN_RELAY_OUT1    = 0X01,
    BOARD_FAN_RELAY_OUT2    = 0X02,
    BOARD_FAN_RELAY_OUT3    = 0X03,
}BOARD_FAN;

//系统风扇单个初始化
void BoardFanInitSingle(BOARD_FAN fan,PIN_STATE initState);

//系统风扇全部初始化
void BoardFanInitAll(PIN_STATE initState);

//系统风扇单个写入
void BoardFanWriteSingle(BOARD_FAN fan,PIN_STATE setState);


#endif



