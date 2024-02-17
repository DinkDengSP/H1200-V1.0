/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 11:33:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 11:33:42 +0800
************************************************************************************************/ 
#ifndef __BOARD_TCA9555PWR_CONFIG_H_
#define __BOARD_TCA9555PWR_CONFIG_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

//寄存器地址
//输入值
#define REG_ADDR_TCA9555_INPUT_VAL_LOW                  0X00
#define REG_ADDR_TCA9555_INPUT_VAL_HIGH                 0X01
//输出值
#define REG_ADDR_TCA9555_OUTPUT_VAL_LOW                 0X02
#define REG_ADDR_TCA9555_OUTPUT_VAL_HIGH                0X03
//极性
#define REG_ADDR_TCA9555_POLARITY_VAL_LOW               0X04
#define REG_ADDR_TCA9555_POLARITY_VAL_HIGH              0X05
//方向
#define REG_ADDR_TCA9555_DIR_VAL_LOW                    0X06
#define REG_ADDR_TCA9555_DIR_VAL_HIGH                   0X07

//设置端口方向
typedef enum TCA9555_PORT_DIR
{
    TCA9555_PORT_DIR_OUT    = 0X00,//端口设置为输出
    TCA9555_PORT_DIR_IN     = 0X01,//端口设置为输入
}TCA9555_PORT_DIR;

//设置端口极性反转
typedef enum TCA9555_PORT_POLARITY
{
    TCA9555_PORT_POLARITY_RETAIN    = 0X00,//端口极性保持
    TCA9555_PORT_POLARITY_INVERT    = 0X01,//端口极性反转
}TCA9555_PORT_POLARITY;

//端口序号
typedef enum TCA9555_PIN
{
    TCA9555_PIN_1       = 0X00,
    TCA9555_PIN_2       = 0X01,
    TCA9555_PIN_3       = 0X02,
    TCA9555_PIN_4       = 0X03,
    TCA9555_PIN_5       = 0X04,
    TCA9555_PIN_6       = 0X05,
    TCA9555_PIN_7       = 0X06,
    TCA9555_PIN_8       = 0X07,
    TCA9555_PIN_9       = 0X08,
    TCA9555_PIN_10      = 0X09,
    TCA9555_PIN_11      = 0X0A,
    TCA9555_PIN_12      = 0X0B,
    TCA9555_PIN_13      = 0X0C,
    TCA9555_PIN_14      = 0X0D,
    TCA9555_PIN_15      = 0X0E,
    TCA9555_PIN_16      = 0X0F,
}TCA9555_PIN;

//TCA9555端口方向讯息
typedef union TCA9555_PORT_DIR_SET
{
    uint16_t regVal2Byte;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        TCA9555_PORT_DIR pin1Dir        : 1;
        TCA9555_PORT_DIR pin2Dir        : 1;
        TCA9555_PORT_DIR pin3Dir        : 1;
        TCA9555_PORT_DIR pin4Dir        : 1;
        TCA9555_PORT_DIR pin5Dir        : 1;
        TCA9555_PORT_DIR pin6Dir        : 1;
        TCA9555_PORT_DIR pin7Dir        : 1;
        TCA9555_PORT_DIR pin8Dir        : 1;
        TCA9555_PORT_DIR pin9Dir        : 1;
        TCA9555_PORT_DIR pin10Dir       : 1;
        TCA9555_PORT_DIR pin11Dir       : 1;
        TCA9555_PORT_DIR pin12Dir       : 1;
        TCA9555_PORT_DIR pin13Dir       : 1;
        TCA9555_PORT_DIR pin14Dir       : 1;
        TCA9555_PORT_DIR pin15Dir       : 1;
        //大端模式下most significant bits,这个是字节的最低位
        TCA9555_PORT_DIR pin16Dir       : 1;
    }regValueSingle;
}TCA9555_PORT_DIR_SET;

//TCA9555端口极性反转信息
typedef union TCA9555_PORT_POLARITY_SET
{
    uint16_t regVal2Byte;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        TCA9555_PORT_POLARITY pin1Polarity        : 1;
        TCA9555_PORT_POLARITY pin2Polarity        : 1;
        TCA9555_PORT_POLARITY pin3Polarity        : 1;
        TCA9555_PORT_POLARITY pin4Polarity        : 1;
        TCA9555_PORT_POLARITY pin5Polarity        : 1;
        TCA9555_PORT_POLARITY pin6Polarity        : 1;
        TCA9555_PORT_POLARITY pin7Polarity        : 1;
        TCA9555_PORT_POLARITY pin8Polarity        : 1;
        TCA9555_PORT_POLARITY pin9Polarity        : 1;
        TCA9555_PORT_POLARITY pin10Polarity       : 1;
        TCA9555_PORT_POLARITY pin11Polarity       : 1;
        TCA9555_PORT_POLARITY pin12Polarity       : 1;
        TCA9555_PORT_POLARITY pin13Polarity       : 1;
        TCA9555_PORT_POLARITY pin14Polarity       : 1;
        TCA9555_PORT_POLARITY pin15Polarity       : 1;
        //大端模式下most significant bits,这个是字节的最高位
        TCA9555_PORT_POLARITY pin16Polarity       : 1;
    }regValueSingle;
}TCA9555_PORT_POLARITY_SET;

//端口状态
typedef union TCA9555_PORT_VAL_SET
{
    uint16_t regVal2Byte;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        GPIO_PinState pin1Value        : 1;
        GPIO_PinState pin2Value        : 1;
        GPIO_PinState pin3Value        : 1;
        GPIO_PinState pin4Value        : 1;
        GPIO_PinState pin5Value        : 1;
        GPIO_PinState pin6Value        : 1;
        GPIO_PinState pin7Value        : 1;
        GPIO_PinState pin8Value        : 1;
        GPIO_PinState pin9Value        : 1;
        GPIO_PinState pin10Value       : 1;
        GPIO_PinState pin11Value       : 1;
        GPIO_PinState pin12Value       : 1;
        GPIO_PinState pin13Value       : 1;
        GPIO_PinState pin14Value       : 1;
        GPIO_PinState pin15Value       : 1;
        //大端模式下most significant bits,这个是字节的最高位
        GPIO_PinState pin16Value       : 1;
    }regValueSingle;
}TCA9555_PORT_VAL_SET;


#endif



