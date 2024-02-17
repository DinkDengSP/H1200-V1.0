/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:12:13 +0800
************************************************************************************************/ 
#ifndef __BOARD_LED_H_
#define __BOARD_LED_H_
#include "DriverHeaderMCU.h"

//LED灯序号
typedef enum BOARD_LED
{
    BOARD_LED_RED       = 0X00,
    BOARD_LED_GREEN     = 0X01,
    BOARD_LED_YELLOW    = 0X02,
}BOARD_LED;


//led初始化
void BoardLedInit(BOARD_LED ledPin,PIN_STATE initState);

//LED设置状态
void BoardLedWrite(BOARD_LED ledPin,PIN_STATE setState);

//LED切换状态
void BoardLedToogle(BOARD_LED ledPin);

#endif














