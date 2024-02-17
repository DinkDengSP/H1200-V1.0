/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 16:33:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 17:35:00 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_IN_H_
#define __BOARD_PORT_IN_H_
#include "DriverHeaderMCU.h"

//输入端口序号
typedef enum BOARD_PORT_IN
{
   //CPLD输入
    BOARD_PORT_IN_CPLD_NOP1         = 0,
    BOARD_PORT_IN_CPLD_NOP2         = 1,
    BOARD_PORT_IN_CPLD_NOP3         = 2,
    BOARD_PORT_IN_CPLD_NOP4         = 3,
    BOARD_PORT_IN_CPLD_NOP5         = 4,
    BOARD_PORT_IN_CPLD_NOP6         = 5,
    BOARD_PORT_IN_CPLD_NOP7         = 6,
    BOARD_PORT_IN_CPLD_NOP8         = 7,
    BOARD_PORT_IN_CPLD_NOP9         = 8,
    BOARD_PORT_IN_CPLD_NOP10        = 9,
    BOARD_PORT_IN_CPLD_NOP11        = 10,
    BOARD_PORT_IN_CPLD_NOP12        = 11,
    BOARD_PORT_IN_CPLD_NOP13        = 12,
    //MCU输入   
    BOARD_PORT_IN_ARM_SEN1_PC5      = 13,
    BOARD_PORT_IN_ARM_SEN2_PB0      = 14,
    BOARD_PORT_IN_ARM_SEN3_PB1      = 15,
    BOARD_PORT_IN_ARM_SEN4_PB10     = 16,
    BOARD_PORT_IN_ARM_SEN5_PB11     = 17,
    BOARD_PORT_IN_ARM_SEN6_PH6      = 18,
    BOARD_PORT_IN_ARM_SEN7_PH7      = 19,
    BOARD_PORT_IN_ARM_SEN8_PH8      = 20,
    BOARD_PORT_IN_ARM_SEN9_PH9      = 21,
    BOARD_PORT_IN_ARM_SEN10_PH10    = 22,
    BOARD_PORT_IN_ARM_SEN11_PH11    = 23,
    BOARD_PORT_IN_ARM_SEN12_PH12    = 24,
    BOARD_PORT_IN_ARM_SEN13_PA4     = 25,
    BOARD_PORT_IN_ARM_SEN14_PA5     = 26,
    BOARD_PORT_IN_ARM_SEN15_PA6     = 27,
    BOARD_PORT_IN_ARM_SEN16_PA7     = 28,
    BOARD_PORT_IN_ARM_SEN17_PC4     = 29,
    BOARD_PORT_IN_ARM_SEN18_PH3     = 30,
    BOARD_PORT_IN_ARM_SEN19_PH2     = 31,
    BOARD_PORT_IN_ARM_SEN20_PC3     = 32,
    BOARD_PORT_IN_ARM_SEN21_PC2     = 33,
    BOARD_PORT_IN_ARM_SEN22_PC1     = 34,
    BOARD_PORT_IN_ARM_SEN23_PC0     = 35,
    BOARD_PORT_IN_ARM_SEN24_PF10    = 36,
    BOARD_PORT_IN_ARM_SEN25_PB3     = 37,
    BOARD_PORT_IN_COUNT,
    BOARD_PORT_IN_NULL          = 255,//不存在的引脚
}BOARD_PORT_IN;

//输入端口初始化
void BoardPortInInitAll(void);

//指定端口初始化
void BoardPortInInitSingle(BOARD_PORT_IN pinIndex);

//输出端口读取指定端口当前状态
GPIO_PinState BoardPortInReadState(BOARD_PORT_IN pinIndex);

#endif







