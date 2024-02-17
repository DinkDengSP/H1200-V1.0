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
    BOARD_PORT_OUT_CPLD_CDC1        = 0,
    BOARD_PORT_OUT_CPLD_CDC2        = 1,
    BOARD_PORT_OUT_CPLD_CDC3        = 2,
    BOARD_PORT_OUT_CPLD_CDC4        = 3,
    BOARD_PORT_OUT_CPLD_CDC5        = 4,
    BOARD_PORT_OUT_CPLD_CDC6        = 5,
    BOARD_PORT_OUT_CPLD_CDC7        = 6,
    BOARD_PORT_OUT_CPLD_CDC8        = 7,
    BOARD_PORT_OUT_CPLD_CDC9        = 8,
    BOARD_PORT_OUT_CPLD_CDC10       = 9,
    BOARD_PORT_OUT_CPLD_CDC11       = 10,
    BOARD_PORT_OUT_CPLD_CDC12       = 11,
    BOARD_PORT_OUT_CPLD_CDC13       = 12,
    BOARD_PORT_OUT_CPLD_CDC14       = 13,
    BOARD_PORT_OUT_CPLD_CDC15       = 14,
    BOARD_PORT_OUT_CPLD_CDC16       = 15,
    BOARD_PORT_OUT_CPLD_CDC17       = 16,
    BOARD_PORT_OUT_CPLD_CDC18       = 17,
    BOARD_PORT_OUT_CPLD_CDC19       = 18,
    BOARD_PORT_OUT_CPLD_CDC20       = 19,
    BOARD_PORT_OUT_CPLD_CDC21       = 20,
    BOARD_PORT_OUT_CPLD_CDC22       = 21,
    BOARD_PORT_OUT_CPLD_CDC23       = 22,
    BOARD_PORT_OUT_CPLD_CDC24       = 23,
    BOARD_PORT_OUT_CPLD_CDC25       = 24,
    BOARD_PORT_OUT_CPLD_CDC26       = 25,
    BOARD_PORT_OUT_CPLD_CDC27       = 26,
    BOARD_PORT_OUT_CPLD_CDC28       = 27,
    BOARD_PORT_OUT_CPLD_CDC29       = 28,
    BOARD_PORT_OUT_CPLD_CDC30       = 29,
    BOARD_PORT_OUT_CPLD_CDC31       = 30,
    BOARD_PORT_OUT_CPLD_CDC32       = 31,
    BOARD_PORT_OUT_CPLD_CDC33       = 32,
    BOARD_PORT_OUT_CPLD_CDC34       = 33,
    BOARD_PORT_OUT_CPLD_CDC35       = 34,
    BOARD_PORT_OUT_CPLD_CDC36       = 35,
    BOARD_PORT_OUT_MCU_SEN1_PF6     = 36,
    BOARD_PORT_OUT_MCU_SEN2_PI10    = 37,
    BOARD_PORT_OUT_MCU_SEN3_PI9     = 38,
    BOARD_PORT_OUT_MCU_SEN4_PC13    = 39,
    BOARD_PORT_OUT_MCU_SEN5_PI11    = 40,
    BOARD_PORT_OUT_MCU_SEN6_PG13    = 41,
    BOARD_PORT_OUT_MCU_SEN7_PG11    = 42,
    BOARD_PORT_OUT_MCU_SEN8_PG10    = 43,
    BOARD_PORT_OUT_MCU_SEN9_PG9     = 44,
    BOARD_PORT_OUT_MCU_SEN10_PD6    = 45,
    BOARD_PORT_OUT_MCU_SEN11_PF8    = 46,
    BOARD_PORT_OUT_MCU_SEN12_PF9    = 47,
    BOARD_PORT_OUT_MCU_SEN13_PF7    = 48,
    BOARD_PORT_OUT_MCU_SEN14_PB4    = 49,
    BOARD_PORT_OUT_MCU_SEN15_PB8    = 50,
    BOARD_PORT_OUT_MCU_SEN16_PB9    = 51,
    BOARD_PORT_OUT_MCU_SEN17_PE3    = 52,
    BOARD_PORT_OUT_MCU_SEN18_PE4    = 53,
    BOARD_PORT_OUT_MCU_SEN19_PG14   = 54,
    BOARD_PORT_OUT_MCU_SEN20_PI5    = 55,
    BOARD_PORT_OUT_MCU_SEN21_PI6    = 56,
    BOARD_PORT_OUT_MCU_SEN22_PI7    = 57,
    BOARD_PORT_OUT_COUNT,
}BOARD_PORT_OUT;

//输出端口初始化全部
void BoardPortOutInitAll(void);

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex);

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value);


#endif








