/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 08:57:19 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-28 08:57:56 +0800
************************************************************************************************/ 
#include "BoardDCValve.h"
#include "BoardPortOut.h"
#include "DriverConfigBoard.h"


//一键全部关闭
void BoardDCValveAllOff(void)
{
    BoardDCValveWrite(BOARD_DC_VALVE_1,PIN_STATE_INVALID);
    BoardDCValveWrite(BOARD_DC_VALVE_2,PIN_STATE_INVALID);
}

//报警设置状态
void BoardDCValveWrite(BOARD_DC_VALVE dcValve,PIN_STATE setState)
{
    GPIO_PinState ioRealState;
    switch(dcValve)
    {
        case BOARD_DC_VALVE_1:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_DC_VALVE1:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_VALVE1);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PE2_ARM_DCVALVE1,ioRealState);
            break;
        case BOARD_DC_VALVE_2:
            ioRealState = (setState == PIN_STATE_VALID)?VALID_LEVEL_BOARD_DC_VALVE2:PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_DC_VALVE2);
            BoardPortOutWriteSingleState(BOARD_OUT_MCU_PE3_ARM_DCVALVE2,ioRealState);
            break;
        default:
            break;
    }
}









