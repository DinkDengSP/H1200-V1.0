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
#ifndef __BOARD_PORT_IN_H_
#define __BOARD_PORT_IN_H_
#include "DriverHeaderMCU.h"

//输入端口序号
typedef enum BOARD_PORT_IN
{
    BOARD_PORT_IN_ARM_PF10_SEN_1        = 0,
    BOARD_PORT_IN_ARM_PI11_SEN_2        = 1,
    BOARD_PORT_IN_ARM_PI10_SEN_3        = 2,
    BOARD_PORT_IN_ARM_PI9_SEN_4         = 3,
    BOARD_PORT_IN_ARM_PC13_SEN_5        = 4,
    BOARD_PORT_IN_ARM_PI8_SEN_6         = 5,
    BOARD_PORT_IN_ARM_PE3_SEN_7         = 6,
    BOARD_PORT_IN_ARM_PI7_SEN_8         = 7,
    BOARD_PORT_IN_ARM_PI6_SEN_9         = 8,
    BOARD_PORT_IN_ARM_PI5_SEN_10        = 9,
    BOARD_PORT_IN_ARM_PI4_SEN_11        = 10,
    BOARD_PORT_IN_ARM_PB9_SEN_12        = 11,
    BOARD_PORT_IN_ARM_PB3_SEN_13        = 12,
    BOARD_PORT_IN_ARM_PG9_SEN_14        = 13,
    BOARD_PORT_IN_ARM_PC12_SEN_16       = 14,
    BOARD_PORT_IN_ARM_PA15_SEN_17       = 15,
    BOARD_PORT_IN_SMBUS_IN16_SEN_18     = 16,
    BOARD_PORT_IN_SMBUS_IN15_SEN_19     = 17,
    BOARD_PORT_IN_SMBUS_IN14_SEN_20     = 18,
    BOARD_PORT_IN_SMBUS_IN13_SEN_21     = 19,
    BOARD_PORT_IN_SMBUS_IN12            = 20,
    BOARD_PORT_IN_SMBUS_IN11            = 21,
    BOARD_PORT_IN_SMBUS_IN10            = 22,
    BOARD_PORT_IN_SMBUS_IN9             = 23,
    BOARD_PORT_IN_SMBUS_IN8             = 24,
    BOARD_PORT_IN_SMBUS_IN7             = 25,
    BOARD_PORT_IN_SMBUS_IN6             = 26,
    BOARD_PORT_IN_SMBUS_IN5             = 27,
    BOARD_PORT_IN_SMBUS_IN4             = 28,
    BOARD_PORT_IN_SMBUS_IN3             = 29,
    BOARD_PORT_IN_SMBUS_IN2             = 30,
    BOARD_PORT_IN_SMBUS_IN1             = 31,
    BOARD_PORT_IN_COUNT                 = 32,
    BOARD_PORT_IN_NULL                  = 255,
}BOARD_PORT_IN;

typedef union BOARD_PORT_IN_SET
{
    uint64_t portInSetVal8Byte;
    struct
    {
        //小端模式下least significant bits,这个是字节的最低位
        GPIO_PinState boardPortInArmPf10Sen1Val     : 1;
        GPIO_PinState boardPortInArmPi11Sen2Val     : 1;
        GPIO_PinState boardPortInArmPi10Sen3Val     : 1;
        GPIO_PinState boardPortInArmPi9Sen4Val      : 1;
        GPIO_PinState boardPortInArmPc13Sen5Val     : 1;
        GPIO_PinState boardPortInArmPi8Sen6Val      : 1;
        GPIO_PinState boardPortInArmPe3Sen7Val      : 1;
        GPIO_PinState boardPortInArmPi7Sen8Val      : 1;
        GPIO_PinState boardPortInArmPi6Sen9Val      : 1;
        GPIO_PinState boardPortInArmPi5Sen10Val     : 1;
        GPIO_PinState boardPortInArmPi4Sen11Val     : 1;
        GPIO_PinState boardPortInArmPb9Sen12Val     : 1;
        GPIO_PinState boardPortInArmPb3Sen13Val     : 1;
        GPIO_PinState boardPortInArmPg9Sen14Val     : 1;
        GPIO_PinState boardPortInArmPc12Sen16Val    : 1;
        GPIO_PinState boardPortInArmPa15Sen17Val    : 1;
        GPIO_PinState boardPortInSmbusIn16Sen18Val  : 1;
        GPIO_PinState boardPortInSmbusIn15Sen19Val  : 1;
        GPIO_PinState boardPortInSmbusIn14Sen20Val  : 1;
        GPIO_PinState boardPortInSmbusIn13Sen21Val  : 1;
        GPIO_PinState boardPortInSmbusIn12          : 1;
        GPIO_PinState boardPortInSmbusIn11          : 1;
        GPIO_PinState boardPortInSmbusIn10          : 1;
        GPIO_PinState boardPortInSmbusIn9           : 1;
        GPIO_PinState boardPortInSmbusIn8           : 1;
        GPIO_PinState boardPortInSmbusIn7           : 1;
        GPIO_PinState boardPortInSmbusIn6           : 1;
        GPIO_PinState boardPortInSmbusIn5           : 1;
        GPIO_PinState boardPortInSmbusIn4           : 1;
        GPIO_PinState boardPortInSmbusIn3           : 1;
        GPIO_PinState boardPortInSmbusIn2           : 1;
        //大端模式下most significant bits,这个是字节的最高位
        GPIO_PinState boardPortInSmbusIn1           : 1;
    }portInSetValSingle;
}BOARD_PORT_IN_SET;

//输入端口初始化
void BoardPortInInitAll(void);

//指定端口初始化
void BoardPortInInitSingle(BOARD_PORT_IN pinIndex);

//输出端口读取指定端口当前状态
GPIO_PinState BoardPortInReadState(BOARD_PORT_IN pinIndex);

//输入端口一次性读取全部信息
void BoardPortInAll(BOARD_PORT_IN_SET* portInSetPtr);


#endif


