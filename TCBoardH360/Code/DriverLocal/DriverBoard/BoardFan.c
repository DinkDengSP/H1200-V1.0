/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-14 11:01:59 +0800
************************************************************************************************/ 
#include "BoardFan.h"

//函数指针类型
//端口初始化
typedef void (*BoardFanInitFuncPtr)(PIN_STATE initState);
//写入数据
typedef void (*BoardFanWriteFuncPtr)(PIN_STATE state);


//FAN_MAIN_MOTOR端口
#define PIN_FAN_MAIN_MOTOR                          MCU_PIN_E_8
//FAN_MAIN_MOTOR有效电平        
#define VALID_LEVEL_FAN_MAIN_MOTOR                  GPIO_PIN_RESET
//FAN_MAIN_MOTOR端口模式        
#define PULL_MODE_FAN_MAIN_MOTOR                    MCU_PIN_PULL_PULLUP
//FAN_MAIN_MOTOR端口速度        
#define SPEED_FAN_MAIN_MOTOR                        MCU_PIN_SPEED_FREQ_MEDIUM

//FAN_RELAY_OUT1端口
#define PIN_FAN_RELAY_OUT1                          MCU_PIN_E_7
//FAN_RELAY_OUT1有效电平        
#define VALID_LEVEL_FAN_RELAY_OUT1                  GPIO_PIN_RESET
//FAN_RELAY_OUT1端口模式        
#define PULL_MODE_FAN_RELAY_OUT1                    MCU_PIN_PULL_PULLUP
//FAN_RELAY_OUT1端口速度        
#define SPEED_FAN_RELAY_OUT1                        MCU_PIN_SPEED_FREQ_MEDIUM

//FAN_RELAY_OUT2端口
#define PIN_FAN_RELAY_OUT2                          MCU_PIN_G_1
//FAN_RELAY_OUT2有效电平        
#define VALID_LEVEL_FAN_RELAY_OUT2                  GPIO_PIN_RESET
//FAN_RELAY_OUT2端口模式        
#define PULL_MODE_FAN_RELAY_OUT2                    MCU_PIN_PULL_PULLUP
//FAN_RELAY_OUT2端口速度        
#define SPEED_FAN_RELAY_OUT2                        MCU_PIN_SPEED_FREQ_MEDIUM

//FAN_RELAY_OUT3端口
#define PIN_FAN_RELAY_OUT3                          MCU_PIN_G_0
//FAN_RELAY_OUT3有效电平        
#define VALID_LEVEL_FAN_RELAY_OUT3                  GPIO_PIN_RESET
//FAN_RELAY_OUT3端口模式        
#define PULL_MODE_FAN_RELAY_OUT3                    MCU_PIN_PULL_PULLUP
//FAN_RELAY_OUT3端口速度        
#define SPEED_FAN_RELAY_OUT3                        MCU_PIN_SPEED_FREQ_MEDIUM


//FAN_MAIN_MOTOR
static void BoardFanInitMainMotor(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_MAIN_MOTOR)):VALID_LEVEL_FAN_MAIN_MOTOR;
    //初始化
    MCU_PinInit(PIN_FAN_MAIN_MOTOR,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_FAN_MAIN_MOTOR,SPEED_FAN_MAIN_MOTOR,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_FAN_MAIN_MOTOR,ioState);
}
//FAN_MAIN_MOTOR
static void BoardFanWriteMainMotor(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_MAIN_MOTOR)):VALID_LEVEL_FAN_MAIN_MOTOR;
    //写入数值
    MCU_PinWriteSingle(PIN_FAN_MAIN_MOTOR,ioState);
}

//FAN_RELAY_OUT1
static void BoardFanInitRelayOut1(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT1)):VALID_LEVEL_FAN_RELAY_OUT1;
    //初始化
    MCU_PinInit(PIN_FAN_RELAY_OUT1,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_FAN_RELAY_OUT1,SPEED_FAN_RELAY_OUT1,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT1,ioState);
}
//FAN_RELAY_OUT1
static void BoardFanWriteRelayOut1(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT1)):VALID_LEVEL_FAN_RELAY_OUT1;
    //写入数值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT1,ioState);
}


//FAN_RELAY_OUT2
static void BoardFanInitRelayOut2(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT2)):VALID_LEVEL_FAN_RELAY_OUT2;
    //初始化
    MCU_PinInit(PIN_FAN_RELAY_OUT2,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_FAN_RELAY_OUT2,SPEED_FAN_RELAY_OUT2,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT2,ioState);
}
//FAN_RELAY_OUT2
static void BoardFanWriteRelayOut2(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT2)):VALID_LEVEL_FAN_RELAY_OUT2;
    //写入数值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT2,ioState);
}


//FAN_RELAY_OUT3
static void BoardFanInitRelayOut3(PIN_STATE initState)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (initState == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT3)):VALID_LEVEL_FAN_RELAY_OUT3;
    //初始化
    MCU_PinInit(PIN_FAN_RELAY_OUT3,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_FAN_RELAY_OUT3,SPEED_FAN_RELAY_OUT3,0);
    //写入初始化值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT3,ioState);
}
//FAN_RELAY_OUT3
static void BoardFanWriteRelayOut3(PIN_STATE state)
{
    GPIO_PinState ioState;
    //确定写入状态
    ioState = (state == PIN_STATE_INVALID) ? ((GPIO_PinState)(!VALID_LEVEL_FAN_RELAY_OUT3)):VALID_LEVEL_FAN_RELAY_OUT3;
    //写入数值
    MCU_PinWriteSingle(PIN_FAN_RELAY_OUT3,ioState);
}

//初始化函数数组
static const BoardFanInitFuncPtr BoardFanInitFuncPtrArray[] = {
    BoardFanInitMainMotor,
    BoardFanInitRelayOut1,
    BoardFanInitRelayOut2,
    BoardFanInitRelayOut3,
};

//写入数值函数数组
static const BoardFanWriteFuncPtr BoardFanWriteFuncPtrArray[] = {
    BoardFanWriteMainMotor,
    BoardFanWriteRelayOut1,
    BoardFanWriteRelayOut2,
    BoardFanWriteRelayOut3,
};  


//系统风扇单个初始化
void BoardFanInitSingle(BOARD_FAN fan,PIN_STATE initState)
{
    BoardFanInitFuncPtrArray[fan](initState);
}

//系统风扇全部初始化
void BoardFanInitAll(PIN_STATE initState)
{
    uint8_t indexUtil = 0;
    for(indexUtil = BOARD_FAN_MAIN_MOTOR;indexUtil <= BOARD_FAN_RELAY_OUT3;indexUtil++)
    {
        BoardFanInitFuncPtrArray[indexUtil](initState);
    }
}

//系统风扇单个写入
void BoardFanWriteSingle(BOARD_FAN fan,PIN_STATE setState)
{
    BoardFanWriteFuncPtrArray[fan](setState);
}










