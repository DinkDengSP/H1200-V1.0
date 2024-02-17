/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 14:12:43
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "BoardLed.h"

//LED RED端口
#define PIN_LED_RED                     MCU_PIN_H_10
//LED RED 有效电平      
#define VALID_LEVEL_LED_RED             GPIO_PIN_SET
//LED RED 端口模式      
#define PULL_MODE_LED_RED               MCU_PIN_PULL_PULLUP
//LED RED 端口速度      
#define SPEED_LED_RED                   MCU_PIN_SPEED_FREQ_MEDIUM

//LED GREEN端口
#define PIN_LED_GREEN                   MCU_PIN_H_8
//LED GREEN 有效电平        
#define VALID_LEVEL_LED_GREEN           GPIO_PIN_SET
//LED GREEN 端口模式        
#define PULL_MODE_LED_GREEN             MCU_PIN_PULL_PULLUP
//LED GREEN 端口速度        
#define SPEED_LED_GREEN                 MCU_PIN_SPEED_FREQ_MEDIUM

//LED YELLOW端口
#define PIN_LED_YELLOW                  MCU_PIN_H_9
//LED YELLOW 有效电平       
#define VALID_LEVEL_LED_YELLOW          GPIO_PIN_SET
//LED YELLOW 端口模式       
#define PULL_MODE_LED_YELLOW            MCU_PIN_PULL_PULLUP
//LED YELLOW 端口速度       
#define SPEED_LED_YELLOW                MCU_PIN_SPEED_FREQ_MEDIUM

//函数指针类型
//端口初始化
typedef void (*BoardLedInitFuncPtr)(PIN_STATE initState);
//写入数据
typedef void (*BoardLedWriteFuncPtr)(PIN_STATE state);
//切换状态
typedef void (*BoardLedToogleFuncPtr)(void);

//BOARD_LED_RED
static void BoardLedInitRed(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_RED)):VALID_LEVEL_LED_RED;
    //初始化
    MCU_PinInit(PIN_LED_RED,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_LED_RED,SPEED_LED_RED,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_LED_RED,ioState);
}

static void BoardLedWriteRed(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_RED)):VALID_LEVEL_LED_RED;
    //写入数值
    MCU_PinWriteSingle(PIN_LED_RED,ioState);
}

static void BoardLedToogleRed(void)
{
    MCU_PinToogleSingle(PIN_LED_RED);
}

//BOARD_LED_GREEN
static void BoardLedInitGreen(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_GREEN)):VALID_LEVEL_LED_GREEN;
    //初始化
    MCU_PinInit(PIN_LED_GREEN,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_LED_GREEN,SPEED_LED_GREEN,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_LED_GREEN,ioState);
}

static void BoardLedWriteGreen(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_GREEN)):VALID_LEVEL_LED_GREEN;
    //写入数值
    MCU_PinWriteSingle(PIN_LED_GREEN,ioState);
}

static void BoardLedToogleGreen(void)
{
    MCU_PinToogleSingle(PIN_LED_GREEN);
}

//BOARD_LED_YELLOW
static void BoardLedInitYellow(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_YELLOW)):VALID_LEVEL_LED_YELLOW;
    //初始化
    MCU_PinInit(PIN_LED_YELLOW,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_LED_YELLOW,SPEED_LED_YELLOW,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_LED_YELLOW,ioState);
}

static void BoardLedWriteYellow(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_YELLOW)):VALID_LEVEL_LED_YELLOW;
    //写入数值
    MCU_PinWriteSingle(PIN_LED_YELLOW,ioState);
}

static void BoardLedToogleYellow(void)
{
    MCU_PinToogleSingle(PIN_LED_YELLOW);
}

//初始化函数数组
static const BoardLedInitFuncPtr BoardLedInitFuncPtrArray[] = {
    BoardLedInitRed,
    BoardLedInitGreen,
    BoardLedInitYellow,
};

//写入数值函数数组
static const BoardLedWriteFuncPtr BoardLedWriteFuncPtrArray[] = {
    BoardLedWriteRed,
    BoardLedWriteGreen,
    BoardLedWriteYellow,
};  

//翻转状态函数数组
static const BoardLedToogleFuncPtr BoardLedToogleFuncPtrArray[] = {
    BoardLedToogleRed,
    BoardLedToogleGreen,
    BoardLedToogleYellow,
};

//初始化LED状态
void BoardLedInit(BOARD_LED ledPin,PIN_STATE initState)
{
    BoardLedInitFuncPtrArray[ledPin](initState);
}

//设置指定LED状态
void BoardLedWrite(BOARD_LED ledPin,PIN_STATE setState)
{
    BoardLedWriteFuncPtrArray[ledPin](setState);
}

//翻转LED状态
void BoardLedToogle(BOARD_LED ledPin)
{
    BoardLedToogleFuncPtrArray[ledPin]();
}





