/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:h
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-24 15:56:55 +0800
************************************************************************************************/ 
#ifndef __BOARD_PORT_IN_H_
#define __BOARD_PORT_IN_H_
#include "DriverHeaderMCU.h"

//输入端口序号
typedef enum BOARD_PORT_IN
{
    BOARD_PORT_IN_PG0_SEN1          = 0 ,
    BOARD_PORT_IN_PF15_SEN2         = 1 ,
    BOARD_PORT_IN_PE13_SEN3         = 2 ,
    BOARD_PORT_IN_PE14_SEN4         = 3 ,
    BOARD_PORT_IN_PE15_SEN5         = 4 ,
    BOARD_PORT_IN_PH8_SEN6          = 5 ,
    BOARD_PORT_IN_PH9_SEN7          = 6 ,
    BOARD_PORT_IN_PH10_SEN8         = 7 ,
    BOARD_PORT_IN_PH11_SEN9         = 8 ,
    BOARD_PORT_IN_PH7_SEN10         = 9 ,
    BOARD_PORT_IN_PH6_SEN11         = 10,
    BOARD_PORT_IN_PH12_SEN12        = 11,
    BOARD_PORT_IN_PD2_SEN13         = 12,
    BOARD_PORT_IN_PD3_SEN14         = 13,
    BOARD_PORT_IN_PD4_SEN15         = 14,
    BOARD_PORT_IN_PD5_SEN16         = 15,
    BOARD_PORT_IN_PD6_SEN17         = 16,
    BOARD_PORT_IN_PD7_SEN18         = 17,
    BOARD_PORT_IN_PG9_SEN19         = 18,
    BOARD_PORT_IN_PG10_SEN20        = 19,
    BOARD_PORT_IN_PB3_SEN21         = 20,
    BOARD_PORT_IN_PB4_SEN22         = 21,
    BOARD_PORT_IN_PB5_SEN23         = 22,
    BOARD_PORT_IN_PB6_SEN24         = 23,
    BOARD_PORT_IN_PB8_SEN25         = 24,
    BOARD_PORT_IN_PB9_SEN26         = 25,
    BOARD_PORT_IN_PE0_SEN27         = 26,
    BOARD_PORT_IN_PE1_SEN28         = 27,
    BOARD_PORT_IN_PE2_SEN29         = 28,
    BOARD_PORT_IN_PE3_SEN30         = 29,
    BOARD_PORT_IN_PF0_SEN31         = 30,
    BOARD_PORT_IN_PF1_SEN32         = 31,  
    BOARD_PORT_IN_PF2_SEN33         = 32,
    BOARD_PORT_IN_PF3_SEN34         = 33,
    BOARD_PORT_IN_PF4_SEN35         = 34,  
    BOARD_PORT_IN_PF5_SEN36         = 35,  
    BOARD_PORT_IN_PI4_SEN37         = 36,
    BOARD_PORT_IN_PI5_SEN38         = 37,
    BOARD_PORT_IN_PI6_SEN39         = 38,
    BOARD_PORT_IN_PI7_SEN40         = 39,
    BOARD_PORT_IN_PC5_RBF_REQUESTR  = 40,
    BOARD_PORT_IN_PB0_RBF_MOVER     = 41,    
    BOARD_PORT_IN_PF13_RBF_READYS   = 42,
    BOARD_PORT_IN_PG1_RBF_ENABLES   = 43,
    BOARD_PORT_IN_PE7_RBF_EXISTS    = 44,
    BOARD_PORT_IN_COUNT              ,
    BOARD_PORT_IN_NULL              = 255,
}BOARD_PORT_IN;

//输入端口初始化
void BoardPortInInitAll(void);

//指定端口初始化
void BoardPortInInitSingle(BOARD_PORT_IN pinIndex);

//输出端口读取指定端口当前状态
GPIO_PinState BoardPortInReadState(BOARD_PORT_IN pinIndex);

#endif



