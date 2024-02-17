/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-06 10:47:33 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-07 10:35:08 +0800
************************************************************************************************/ 
#include "BoardLed.h"

//LED RED端口
#define PIN_LED_RED                     MCU_PIN_A_5
//LED RED 有效电平      
#define VALID_LEVEL_LED_RED             GPIO_PIN_RESET
//LED RED 端口模式      
#define PULL_MODE_LED_RED               MCU_PIN_PULL_PULLUP
//LED RED 端口速度      
#define SPEED_LED_RED                   MCU_PIN_SPEED_FREQ_MEDIUM

//LED GREEN端口
#define PIN_LED_GREEN                   MCU_PIN_A_6
//LED GREEN 有效电平        
#define VALID_LEVEL_LED_GREEN           GPIO_PIN_RESET
//LED GREEN 端口模式        
#define PULL_MODE_LED_GREEN             MCU_PIN_PULL_PULLUP
//LED GREEN 端口速度        
#define SPEED_LED_GREEN                 MCU_PIN_SPEED_FREQ_MEDIUM

//LED YELLOW端口
#define PIN_LED_YELLOW                  MCU_PIN_A_4
//LED YELLOW 有效电平       
#define VALID_LEVEL_LED_YELLOW          GPIO_PIN_RESET
//LED YELLOW 端口模式       
#define PULL_MODE_LED_YELLOW            MCU_PIN_PULL_PULLUP
//LED YELLOW 端口速度       
#define SPEED_LED_YELLOW                MCU_PIN_SPEED_FREQ_MEDIUM

//LED TEST1端口
#define PIN_LED_TEST1                   MCU_PIN_H_5
//LED TEST1 有效电平        
#define VALID_LEVEL_LED_TEST1           GPIO_PIN_RESET
//LED TEST1 端口模式        
#define PULL_MODE_LED_TEST1             MCU_PIN_PULL_PULLUP
//LED TEST1 端口速度        
#define SPEED_LED_TEST1                 MCU_PIN_SPEED_FREQ_MEDIUM

//LED TEST2端口
#define PIN_LED_TEST2                   MCU_PIN_H_4
//LED TEST2 有效电平        
#define VALID_LEVEL_LED_TEST2           GPIO_PIN_RESET
//LED TEST2 端口模式        
#define PULL_MODE_LED_TEST2             MCU_PIN_PULL_PULLUP
//LED TEST2 端口速度        
#define SPEED_LED_TEST2                 MCU_PIN_SPEED_FREQ_MEDIUM



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

//BOARD_LED_TEST1
static void BoardLedInitTest1(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_TEST1)):VALID_LEVEL_LED_TEST1;
    //初始化
    MCU_PinInit(PIN_LED_TEST1,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_LED_TEST1,SPEED_LED_TEST1,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_LED_TEST1,ioState);
}

static void BoardLedWriteTest1(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_TEST1)):VALID_LEVEL_LED_TEST1;
    //写入数值
    MCU_PinWriteSingle(PIN_LED_TEST1,ioState);
}

static void BoardLedToogleTest1(void)
{
    MCU_PinToogleSingle(PIN_LED_TEST1);
}

//BOARD_LED_TEST2
static void BoardLedInitTest2(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_TEST2)):VALID_LEVEL_LED_TEST2;
    //初始化
    MCU_PinInit(PIN_LED_TEST2,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_LED_TEST2,SPEED_LED_TEST2,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_LED_TEST2,ioState);
}

static void BoardLedWriteTest2(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_LED_TEST2)):VALID_LEVEL_LED_TEST2;
    //写入数值
    MCU_PinWriteSingle(PIN_LED_TEST2,ioState);
}

static void BoardLedToogleTest2(void)
{
    MCU_PinToogleSingle(PIN_LED_TEST2);
}

//初始化函数数组
static const BoardLedInitFuncPtr BoardLedInitFuncPtrArray[] = {
    BoardLedInitRed,
    BoardLedInitGreen,
    BoardLedInitYellow,
    BoardLedInitTest1,
    BoardLedInitTest2,
};

//写入数值函数数组
static const BoardLedWriteFuncPtr BoardLedWriteFuncPtrArray[] = {
    BoardLedWriteRed,
    BoardLedWriteGreen,
    BoardLedWriteYellow,
    BoardLedWriteTest1,
    BoardLedWriteTest2,
};  

//翻转状态函数数组
static const BoardLedToogleFuncPtr BoardLedToogleFuncPtrArray[] = {
    BoardLedToogleRed,
    BoardLedToogleGreen,
    BoardLedToogleYellow,
    BoardLedToogleTest1,
    BoardLedToogleTest2,
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















