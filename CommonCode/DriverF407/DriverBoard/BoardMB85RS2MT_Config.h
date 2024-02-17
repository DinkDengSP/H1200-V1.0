/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 16:07:15 +0800
************************************************************************************************/ 
#ifndef __BOARD_MB85RS2MT_CONFIG_H_
#define __BOARD_MB85RS2MT_CONFIG_H_
#include "DriverHeaderMCU.h"

//MB85操作指令集
//写入使能
#define MB85RS2MT_CMD_WREN                  0x06
//写入禁止
#define MB85RS2MT_CMD_WRDI                  0x04
//读取状态寄存器
#define MB85RS2MT_CMD_RDSR                  0x05
//写入状态寄存器
#define MB85RS2MT_CMD_WRSR                  0x01
//数据读取
#define MB85RS2MT_CMD_READ                  0x03
//数据读取快速
#define MB85RS2MT_CMD_FAST_READ             0x0B
//数据写入
#define MB85RS2MT_CMD_WRITE                 0x02
//进入睡眠
#define MB85RS2MT_CMD_SLEEP                 0xB9
//读取ID
#define MB85RS2MT_CMD_RDID                  0x9F

//状态寄存器保护
#define MB85RS2MT_STATUS_REG_BP_NONE        0X00
#define MB85RS2MT_STATUS_REG_BP_UPPER_1_4   0X01
#define MB85RS2MT_STATUS_REG_BP_UPPER_1_2   0X02
#define MB85RS2MT_STATUS_REG_BP_ALL         0X03

typedef union MB85RS2MT_STATUS_REG
{
    uint8_t regValueAll;
    struct 
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        uint8_t reserve0    : 1;
        uint8_t wel         : 1;
        uint8_t bp          : 2;
        uint8_t reserve1    : 3;
        //大端模式下most significant bits,这个是字节的最低位
        uint8_t wpen        : 1;
    }regValueSingle;
}MB85RS2MT_STATUS_REG;

#endif


