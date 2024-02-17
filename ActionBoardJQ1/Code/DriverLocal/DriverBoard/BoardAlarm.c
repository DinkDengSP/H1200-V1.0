/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 08:57:56 +0800
************************************************************************************************/ 
#include "BoardAlarm.h"
#include "DriverConfigBoard.h"
#include "BoardPortOut.h"

//一键全部关闭
void BoardAlarmAllOff(void)
{
    BoardAlarmWrite(BOARD_ALARM_RED,PIN_STATE_INVALID);
    BoardAlarmWrite(BOARD_ALARM_GREEN,PIN_STATE_INVALID);
    BoardAlarmWrite(BOARD_ALARM_YELLOW,PIN_STATE_INVALID);
    BoardAlarmWrite(BOARD_ALARM_BUZZER,PIN_STATE_INVALID);
}

//报警设置状态
void BoardAlarmWrite(BOARD_ALARM alarmPin,PIN_STATE setState)
{
    GPIO_PinState ioRealState;
    switch(alarmPin)
    {
        case BOARD_ALARM_RED:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_ALARM_LED_RED:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_ALARM_LED_RED);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PG11_ARM_OUT1,ioRealState);
            break;
        case BOARD_ALARM_GREEN:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_ALARM_GREEN_RED:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_ALARM_GREEN_RED);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PG12_ARM_OUT2,ioRealState);
            break;
        case BOARD_ALARM_YELLOW:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_ALARM_YELLOW_RED:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_ALARM_YELLOW_RED);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PG13_ARM_OUT3,ioRealState);
            break;
        case BOARD_ALARM_BUZZER:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_ALARM_BUZZER:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_ALARM_BUZZER);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PG14_ARM_OUT4,ioRealState);
            break;
        default:
            break;
    }
}









