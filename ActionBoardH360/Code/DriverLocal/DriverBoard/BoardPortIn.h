/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 16:33:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 09:02:58 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_IN_H_
#define __BOARD_PORT_IN_H_
#include "DriverHeaderMCU.h"

//输入端口序号
typedef enum BOARD_PORT_IN
{
    //CPLD输入
    BOARD_PORT_IN_CPLD_NOP1                 = 0,
    BOARD_PORT_IN_CPLD_NOP2                 = 1,
    BOARD_PORT_IN_CPLD_NOP3                 = 2,
    BOARD_PORT_IN_CPLD_NOP4                 = 3,
    BOARD_PORT_IN_CPLD_NOP5                 = 4,
    BOARD_PORT_IN_CPLD_NOP6                 = 5,
    BOARD_PORT_IN_CPLD_NOP7                 = 6,
    BOARD_PORT_IN_CPLD_NOP8                 = 7,
    BOARD_PORT_IN_CPLD_NOP9                 = 8,
    BOARD_PORT_IN_CPLD_NOP10                = 9,
    BOARD_PORT_IN_CPLD_NOP11                = 10,
    BOARD_PORT_IN_CPLD_NOP12                = 11,
    BOARD_PORT_IN_CPLD_NOP13                = 12,
    //MCU输入   
    BOARD_PORT_IN_ARM_SEN1_PI10             = 13,
    BOARD_PORT_IN_ARM_SEN2_PI9              = 14,
    BOARD_PORT_IN_ARM_SEN3_PC13             = 15,
    BOARD_PORT_IN_ARM_SEN4_PI8              = 16,
    BOARD_PORT_IN_ARM_SEN5_PE6              = 17,
    BOARD_PORT_IN_ARM_SEN6_PE5              = 18,
    BOARD_PORT_IN_ARM_SEN7_PE4              = 19,
    BOARD_PORT_IN_ARM_SEN8_PE3              = 20,
    BOARD_PORT_IN_ARM_SEN9_PE2              = 21,
    BOARD_PORT_IN_ARM_SEN10_PI7             = 22,
    BOARD_PORT_IN_ARM_SEN11_PI6             = 23,
    BOARD_PORT_IN_ARM_SEN12_PI5             = 24,
    BOARD_PORT_IN_ARM_SEN13_PI4             = 25,
    BOARD_PORT_IN_ARM_SEN14_PB9             = 26,
    BOARD_PORT_IN_ARM_SEN15_PB8             = 27,
    BOARD_PORT_IN_ARM_SEN16_PB7             = 28,
    BOARD_PORT_IN_SLAVE_ARM_SEN1_PH12       = 29,
    BOARD_PORT_IN_SLAVE_ARM_SEN2_PH11       = 30,
    BOARD_PORT_IN_SLAVE_ARM_SEN3_PH10       = 31,
    BOARD_PORT_IN_SLAVE_ARM_SEN4_PH9        = 32,
    BOARD_PORT_IN_SLAVE_ARM_SEN5_PH8        = 33,
    BOARD_PORT_IN_SLAVE_ARM_SEN6_PH7        = 34,
    BOARD_PORT_IN_SLAVE_ARM_SEN7_PH6        = 35,
    BOARD_PORT_IN_SLAVE_ARM_SEN8_PB11       = 36,
    BOARD_PORT_IN_SLAVE_ARM_SEN9_PB10       = 37,
    BOARD_PORT_IN_SLAVE_ARM_SEN10_PC0       = 38,
    BOARD_PORT_IN_SLAVE_ARM_SEN11_PC1       = 39,
    BOARD_PORT_IN_SLAVE_ARM_SEN12_PC2       = 40,
    BOARD_PORT_IN_SLAVE_ARM_SEN13_PC3       = 41,
    BOARD_PORT_IN_SLAVE_ARM_SEN14_PB1       = 42,
    BOARD_PORT_IN_SLAVE_ARM_SEN15_PB0       = 43,
    BOARD_PORT_IN_SLAVE_ARM_SEN16_PC5       = 44,
    BOARD_PORT_IN_FRUBBISH1_PH15            = 45,
    BOARD_PORT_IN_FRUBBISH2_PH14            = 46,
    BOARD_PORT_IN_FRUBBISH3_PH13            = 47,
    BOARD_PORT_IN_FRUBBISH4_PC9             = 48,
    BOARD_PORT_IN_FRUBBISH_OVER1_PI0        = 49,
    BOARD_PORT_IN_FRUBBISH_OVER2_PI1        = 50,
    BOARD_PORT_IN_PR1_ARM1_PG14             = 51,
    BOARD_PORT_IN_PR1_ARM2_PG13             = 52,
    BOARD_PORT_IN_PR2_ARM1_PG11             = 53,
    BOARD_PORT_IN_PR2_ARM2_PG10             = 54,
    BOARD_PORT_IN_YM1_ARM1_PB6              = 55,
    BOARD_PORT_IN_YM1_ARM2_PB5              = 56,
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







