/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:h
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-26 15:22:34 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_OUT_H_
#define __BOARD_PORT_OUT_H_
#include "DriverHeaderMCU.h"

//输出端口序号
typedef enum BOARD_PORT_OUT
{
    BOARD_OUT_MCU_PB1_RBF_READYR          = 0,//
    BOARD_OUT_MCU_PF12_RBF_ENABLER        = 1,//
    BOARD_OUT_MCU_PF11_RBF_EXISTR         = 2,//  
    BOARD_OUT_MCU_PE11_RBF_REQUESTS       = 3,//
    BOARD_OUT_MCU_PE12_RBF_MOVES          = 4,//
    BOARD_OUT_MCU_PG11_RED_LED            = 5,//
    BOARD_OUT_MCU_PG12_GREEN_LED          = 6,//
    BOARD_OUT_MCU_PG13_YELLOW_LED         = 7,//
    BOARD_OUT_MCU_PG14_BUZZER             = 8,//
    BOARD_OUT_MCU_PC4_RBF_AU_LED          = 9,//
    BOARD_PORT_OUT_COUNT                     ,//
}BOARD_PORT_OUT;

//输出端口初始化全部
void BoardPortOutInitAll(void);

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex);

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value);


#endif

