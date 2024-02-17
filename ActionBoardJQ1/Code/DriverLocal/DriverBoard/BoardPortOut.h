/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 17:19:26 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_OUT_H_
#define __BOARD_PORT_OUT_H_
#include "DriverHeaderMCU.h"

//输出端口序号
typedef enum BOARD_PORT_OUT
{
    BOARD_OUT_MCU_PC4_ARM_CW1          = 0 ,//24V 1.5A
    BOARD_OUT_MCU_PC5_ARM_CCW1         = 1 ,//24V 1.5A
    BOARD_OUT_MCU_PA6_ARM_CW2          = 2 ,//24V 1.5A         
    BOARD_OUT_MCU_PA7_ARM_CCW2         = 3 ,//24V 1.5A
    BOARD_OUT_MCU_PA4_ARM_CW3          = 4 ,//24V 1.5A
    BOARD_OUT_MCU_PA5_ARM_CCW3         = 5 ,//24V 1.5A
    BOARD_OUT_MCU_PE2_ARM_DCVALVE1     = 6 ,//24V 2.0A
    BOARD_OUT_MCU_PE3_ARM_DCVALVE2     = 7 ,//24V 2.0A
    BOARD_OUT_MCU_PG11_ARM_OUT1        = 8 ,
    BOARD_OUT_MCU_PG12_ARM_OUT2        = 9 ,   
    BOARD_OUT_MCU_PG13_ARM_OUT3        = 10,
    BOARD_OUT_MCU_PG14_ARM_OUT4        = 11,
    BOARD_PORT_OUT_COUNT                   ,
}BOARD_PORT_OUT;

//输出端口初始化全部
void BoardPortOutInitAll(void);

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex);

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value);


#endif












