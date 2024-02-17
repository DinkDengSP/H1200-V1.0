/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_PORT_OUT_H_
#define __BOARD_PORT_OUT_H_
#include "DriverHeaderMCU.h"

//板卡端口输出
typedef enum BOARD_PORT_OUT
{
    BOARD_PORT_OUT_SMBUS_OUT1_IO_OUT1       = 0,
    BOARD_PORT_OUT_SMBUS_OUT2_IO_OUT2       = 1,
    BOARD_PORT_OUT_SMBUS_OUT3_IO_OUT3       = 2,
    BOARD_PORT_OUT_SMBUS_OUT4_IO_OUT4       = 3,
    BOARD_PORT_OUT_SMBUS_OUT5_IO_OUT5       = 4,
    BOARD_PORT_OUT_SMBUS_OUT6_IO_OUT6       = 5,
    BOARD_PORT_OUT_SMBUS_OUT7_IO_OUT7       = 6,
    BOARD_PORT_OUT_SMBUS_OUT8_IO_OUT8       = 7,
    BOARD_PORT_OUT_SMBUS_OUT9_IO_OUT9       = 8,
    BOARD_PORT_OUT_SMBUS_OUT10_IO_OUT10     = 9,
    BOARD_PORT_OUT_SMBUS_OUT11_IO_OUT11     = 10,
    BOARD_PORT_OUT_SMBUS_OUT12_IO_OUT12     = 11,
    BOARD_PORT_OUT_SMBUS_OUT13_IO_OUT13     = 12,
    BOARD_PORT_OUT_SMBUS_OUT14_IO_OUT14     = 13,
    BOARD_PORT_OUT_SMBUS_OUT15_IO_OUT15     = 14,
    BOARD_PORT_OUT_SMBUS_OUT16_IO_OUT16     = 15,
    BOARD_PORT_OUT_COUNT                    = 16,
    BOARD_PORT_OUT_NULL                     = 255,
}BOARD_PORT_OUT;

//输出端口初始化全部
void BoardPortOutInitAll(void);

//输出端口初始化单个
void BoardPortOutInitSingle(BOARD_PORT_OUT pinIndex);

//输出端口写入单个端口状态
void BoardPortOutWriteSingleState(BOARD_PORT_OUT pinIndex,GPIO_PinState value);

//端口反转指定输出状态
void BoardPortOutToogleSingleState(BOARD_PORT_OUT pinIndex);


#endif





