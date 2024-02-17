/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 09:54:52 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_IN_H_
#define __BOARD_PORT_IN_H_
#include "DriverHeaderMCU.h"

//输入端口序号
typedef enum BOARD_PORT_IN
{
    BOARD_PORT_IN_PH6_SEN1      = 0 ,
    BOARD_PORT_IN_PH7_SEN2      = 1 ,
    BOARD_PORT_IN_PH8_SEN3      = 2 ,
    BOARD_PORT_IN_PH9_SEN4      = 3 ,
    BOARD_PORT_IN_PG0_SEN5      = 4 ,
    BOARD_PORT_IN_PG1_SEN6      = 5 ,
    BOARD_PORT_IN_PE7_SEN7      = 6 ,
    BOARD_PORT_IN_PE12_SEN8     = 7 ,
    BOARD_PORT_IN_PF12_SEN9     = 8 ,
    BOARD_PORT_IN_PF13_SEN10    = 9 ,
    BOARD_PORT_IN_PF14_SEN11    = 10,
    BOARD_PORT_IN_PF15_SEN12    = 11,
    BOARD_PORT_IN_PA3_SEN13     = 12,
    BOARD_PORT_IN_PB0_SEN14     = 13,
    BOARD_PORT_IN_PB1_SEN15     = 14,
    BOARD_PORT_IN_PF11_SEN16    = 15,
    BOARD_PORT_IN_COUNT             ,
    BOARD_PORT_IN_NULL          = 255,
}BOARD_PORT_IN;

//输入端口初始化
void BoardPortInInitAll(void);

//指定端口初始化
void BoardPortInInitSingle(BOARD_PORT_IN pinIndex);

//输出端口读取指定端口当前状态
GPIO_PinState BoardPortInReadState(BOARD_PORT_IN pinIndex);

#endif










