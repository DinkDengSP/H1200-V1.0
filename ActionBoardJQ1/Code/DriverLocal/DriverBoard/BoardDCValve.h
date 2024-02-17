/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 08:57:56 +0800
************************************************************************************************/ 
#ifndef __BOARD_DC_VALVE_H_
#define __BOARD_DC_VALVE_H_
#include "DriverHeaderMCU.h"

//直流电磁阀控制
typedef enum BOARD_DC_VALVE
{
    BOARD_DC_VALVE_1     = 0X00,
    BOARD_DC_VALVE_2     = 0X01,
}BOARD_DC_VALVE;


//一键全部关闭
void BoardDCValveAllOff(void);

//报警设置状态
void BoardDCValveWrite(BOARD_DC_VALVE dcValve,PIN_STATE setState);


#endif













