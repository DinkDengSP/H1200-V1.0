/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 08:57:56 +0800
************************************************************************************************/ 
#ifndef __BOARD_ALARM_H_
#define __BOARD_ALARM_H_
#include "DriverHeaderMCU.h"

//报警灯序号
typedef enum BOARD_ALARM
{
    BOARD_ALARM_RED       = 0X00,//红灯
    BOARD_ALARM_GREEN     = 0X01,//绿灯
    BOARD_ALARM_YELLOW    = 0X02,//黄灯
    BOARD_ALARM_BUZZER    = 0X03,//喇叭
}BOARD_ALARM;


//一键全部关闭
void BoardAlarmAllOff(void);

//报警设置状态
void BoardAlarmWrite(BOARD_ALARM alarmPin,PIN_STATE setState);


#endif















