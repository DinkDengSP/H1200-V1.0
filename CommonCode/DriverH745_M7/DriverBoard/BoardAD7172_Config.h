/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-19 13:21:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-19 14:01:39 +0800
************************************************************************************************/ 
#ifndef __BOARD_AD7172_CONFIG_H_
#define __BOARD_AD7172_CONFIG_H_
#include "DriverHeaderMCU.h"

//寄存器读写标志位
typedef enum BOARD_AD7172_FLAG
{
    BOARD_AD7172_FLAG_WRITE = 0X00,//写入操作
    BOARD_AD7172_FLAG_READ  = 0X01,//读取操作
}BOARD_AD7172_FLAG;


//芯片寄存器地址
typedef enum BOARD_AD7172_REG_ADDR
{
    BOARD_AD7172_REG_ADDR_STATE                     = 0X00,//状态寄存器
    BOARD_AD7172_REG_ADDR_ADC_MODE                  = 0X01,//ADC模式寄存器
    BOARD_AD7172_REG_ADDR_INTERFACE_MODE            = 0X02,//接口模式寄存器
    BOARD_AD7172_REG_ADDR_REG_CHECK                 = 0X03,//寄存器检查寄存器
    BOARD_AD7172_REG_ADDR_DATA                      = 0X04,//数据寄存器
    BOARD_AD7172_REG_ADDR_IO_CONF                   = 0X06,//IO配置寄存器
    BOARD_AD7172_REG_ADDR_ID                        = 0X07,//ID寄存器
}BOARD_AD7172_REG_ADDR;


#endif


