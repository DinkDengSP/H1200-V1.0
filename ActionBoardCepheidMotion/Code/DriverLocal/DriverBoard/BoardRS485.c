/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 15:36:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 15:50:58 +0800
************************************************************************************************/ 
#include "BoardRS485.h"
#include "DriverConfigBoard.h"

//485 RE端口
#define PIN_RS485_RE                    MCU_PIN_E_4
//485 RE 有效电平   
#define VALID_LEVEL_RS485_RE            GPIO_PIN_RESET
//485 RE 端口模式   
#define PULL_MODE_RS485_RE              MCU_PIN_PULL_PULLUP
//485 RE 端口速度   
#define SPEED_RS485_RE                  MCU_PIN_SPEED_FREQ_MEDIUM

//485 DE端口
#define PIN_RS485_DE                    MCU_PIN_E_3
//485 DE 有效电平           
#define VALID_LEVEL_RS485_DE            GPIO_PIN_SET
//485 DE 端口模式           
#define PULL_MODE_RS485_DE              MCU_PIN_PULL_PULLUP
//485 DE 端口速度           
#define SPEED_RS485_DE                  MCU_PIN_SPEED_FREQ_MEDIUM

//进入接收模式
static void BoardRS485_EnterRecvMode(void)
{
    //设置接收有效
    MCU_PinWriteSingle(PIN_RS485_RE,VALID_LEVEL_RS485_RE);
    //设置发送无效
    MCU_PinWriteSingle(PIN_RS485_DE,PIN_INVALID_LEVEL(VALID_LEVEL_RS485_DE));
}

//进入发送模式
static void BoardRS485_EnterSendMode(void)
{
    //设置发送有效
    MCU_PinWriteSingle(PIN_RS485_DE,VALID_LEVEL_RS485_DE);
    //设置接收无效
    MCU_PinWriteSingle(PIN_RS485_RE,PIN_INVALID_LEVEL(VALID_LEVEL_RS485_RE));
}

/****************************************************************************************************/
//RS初始化
void BoardRS485_Init(MCU_UartRecvIntProcFunc recvCallBack)
{
    //初始化控制IO
    MCU_PinInit(PIN_RS485_RE,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_RS485_RE,SPEED_RS485_RE,0);
    MCU_PinInit(PIN_RS485_DE,MCU_PIN_MODE_OUTPUT_PP,PULL_MODE_RS485_DE,SPEED_RS485_DE,0);
    //进入接收模式
    BoardRS485_EnterRecvMode();
    //串口初始化
    MCU_UART4_Init(BOARD_RS485_BAUD,BOARD_RS485_LENGTH,BOARD_RS485_STOP_BIT,BOARD_RS485_CHECK_MODE,BOARD_RS485_HARD_CONTROL);
    //回调函数注册
    MCU_UART4_ConfigCallBack(recvCallBack,BoardRS485_EnterSendMode,BoardRS485_EnterRecvMode);
}

//RS发送缓存
void BoardRS485_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    MCU_UART4_SendBuffer(bufferStartPtr,sendLength);
}

//RS发送字符串
void BoardRS485_SendString(uint8_t* stringStartPtr)
{
    MCU_UART4_SendString(stringStartPtr);
}














