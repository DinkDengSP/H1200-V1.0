/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-04 13:20:15 +0800
************************************************************************************************/ 
#ifndef __PE_UART_OUMENG_INTER_DATA_TYPE_H_
#define __PE_UART_OUMENG_INTER_DATA_TYPE_H_
#include "PEUartOumengInterMacro.h"

//设备当前状态
typedef enum PE_UART_DEVICE_STATE
{
    PE_UART_DEVICE_STATE_NORMAL  = 0x00,//PE设备正常
    PE_UART_DEVICE_STATE_FAULT   = 0x01,//PE设备故障
}PE_UART_DEVICE_STATE;

//空架子状态
typedef enum PE_UART_EMPTY_RANK_STATE
{
    PE_UART_EMPTY_RANK_STATE_NOT_READY       = 0,//架子还没准备好
    PE_UART_EMPTY_RANK_STATE_READY_TUBE_POS1 = 1,//一号试管位准备好了
    PE_UART_EMPTY_RANK_STATE_READY_TUBE_POS2 = 2,//二号试管位准备好了
    PE_UART_EMPTY_RANK_STATE_READY_TUBE_POS3 = 3,//三号试管位准备好了
    PE_UART_EMPTY_RANK_STATE_READY_TUBE_POS4 = 4,//四号试管位准备好了
    PE_UART_EMPTY_RANK_STATE_READY_TUBE_POS5 = 5,//五号试管位准备好了
}PE_UART_EMPTY_RANK_STATE;

//试管位置
typedef enum PE_UART_TUBE_POS
{
    PE_UART_TUBE_POS_1 = 0X01,
    PE_UART_TUBE_POS_2 = 0X02,
    PE_UART_TUBE_POS_3 = 0X03,
    PE_UART_TUBE_POS_4 = 0X04,
    PE_UART_TUBE_POS_5 = 0X05,
}PE_UART_TUBE_POS;

//试管返回许可
typedef enum PE_UART_TUBE_RETURN
{
    PE_UART_TUBE_RETURN_NOT_ALLOW   = 0X00,//不允许返回
    PE_UART_TUBE_RETURN_ALLOW       = 0X01,//允许返回
}PE_UART_TUBE_RETURN;

//试管存在状态
typedef enum PE_UART_TUBE_STATE
{
    PE_UART_TUBE_STATE_NOT_EXIST    = 0X00,//不存在
    PE_UART_TUBE_STATE_EXIST        = 0X01,//存在
}PE_UART_TUBE_STATE;



#endif









