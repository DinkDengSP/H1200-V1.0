/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 16:33:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 17:10:28 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_OUT_H_
#define __BOARD_PORT_OUT_H_
#include "DriverHeaderMCU.h"

//输出端口序号
typedef enum BOARD_PORT_OUT
{
    BOARD_PORT_OUT_CPLD_CDC1                    = 0,
    BOARD_PORT_OUT_CPLD_CDC2                    = 1,
    BOARD_PORT_OUT_CPLD_CDC3                    = 2,
    BOARD_PORT_OUT_CPLD_CDC4                    = 3,
    BOARD_PORT_OUT_CPLD_CDC5                    = 4,
    BOARD_PORT_OUT_CPLD_CDC6                    = 5,
    BOARD_PORT_OUT_CPLD_CDC7                    = 6,
    BOARD_PORT_OUT_CPLD_CDC8                    = 7,
    BOARD_PORT_OUT_CPLD_CDC9                    = 8,
    BOARD_PORT_OUT_CPLD_CDC10                   = 9,
    BOARD_PORT_OUT_CPLD_CDC11                   = 10,
    BOARD_PORT_OUT_CPLD_CDC12                   = 11,
    BOARD_PORT_OUT_CPLD_CDC13                   = 12,
    BOARD_PORT_OUT_CPLD_CDC14                   = 13,
    BOARD_PORT_OUT_CPLD_CDC15                   = 14,
    BOARD_PORT_OUT_CPLD_CDC16                   = 15,
    BOARD_PORT_OUT_CPLD_CDC17                   = 16,
    BOARD_PORT_OUT_CPLD_CDC18                   = 17,
    BOARD_PORT_OUT_CPLD_CDC19                   = 18,
    BOARD_PORT_OUT_CPLD_CDC20                   = 19,
    BOARD_PORT_OUT_CPLD_CDC21                   = 20,
    BOARD_PORT_OUT_CPLD_CDC22                   = 21,
    BOARD_PORT_OUT_CPLD_CDC23                   = 22,
    BOARD_PORT_OUT_CPLD_CDC24                   = 23,
    BOARD_PORT_OUT_CPLD_CDC25                   = 24,
    BOARD_PORT_OUT_CPLD_CDC26                   = 25,
    BOARD_PORT_OUT_CPLD_CDC27                   = 26,
    BOARD_PORT_OUT_CPLD_CDC28                   = 27,
    BOARD_PORT_OUT_CPLD_CDC29                   = 28,
    BOARD_PORT_OUT_CPLD_CDC30                   = 29,
    BOARD_PORT_OUT_CPLD_CDC31                   = 30,
    BOARD_PORT_OUT_CPLD_CDC32                   = 31,
    BOARD_PORT_OUT_CPLD_CDC33                   = 32,
    BOARD_PORT_OUT_CPLD_CDC34                   = 33,
    BOARD_PORT_OUT_CPLD_CDC35                   = 34,
    BOARD_PORT_OUT_CPLD_CDC36                   = 35,
    BOARD_PORT_OUT_SLAVE_SEN17_PC4              = 36,
    BOARD_PORT_OUT_SLAVE_SEN18_PA7              = 37,
    BOARD_PORT_OUT_SLAVE_SEN19_PA6              = 38,
    BOARD_PORT_OUT_SLAVE_SEN20_PA5              = 39,
    BOARD_PORT_OUT_SLAVE_SEN21_PA4              = 40,
    BOARD_PORT_OUT_SLAVE_SEN22_PA3              = 41,
    BOARD_PORT_OUT_SLAVE_SEN23_PF8              = 42,
    BOARD_PORT_OUT_SLAVE_SEN24_PF9              = 43,
    BOARD_PORT_OUT_SLAVE_SEN25_PF10             = 44,
    BOARD_PORT_OUT_SLAVE_SEN26_PH3              = 45,
    BOARD_PORT_OUT_SLAVE_SEN27_PH2              = 46,
    BOARD_PORT_OUT_SLAVE_SEN28_PA2              = 47,
    BOARD_PORT_OUT_SLAVE_DC1_VAVLE_PC8          = 48,
    BOARD_PORT_OUT_SLAVE_DC2_RUBBISH_MOTOR1_PG9 = 49,
    BOARD_PORT_OUT_SLAVE_DC3_RUBBISH_MOTOR2_PD6 = 50,
    BOARD_PORT_OUT_SLAVE_DC4_RUBBISH_MOTOR3_PI3 = 51,
    BOARD_PORT_OUT_SLAVE_DC5_RUBBISH_MOTOR4_PI2 = 52,
    BOARD_PORT_OUT_YM1_ARM3_PB4                 = 53,
    BOARD_PORT_OUT_YM1_ARM4_PG15                = 54,
    BOARD_PORT_OUT_COUNT,
}BOARD_PORT_OUT;

//输出端口初始化全部
void BoardPortOutInitAll(void);

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex);

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value);


#endif








