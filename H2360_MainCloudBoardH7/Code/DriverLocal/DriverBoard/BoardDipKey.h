/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-06 12:53:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 12:55:12 +0800
************************************************************************************************/ 
#ifndef __BOARD_DIP_KEY_H_
#define __BOARD_DIP_KEY_H_
#include "DriverHeaderMCU.h"

//按键枚举
typedef enum BOARD_DIP_KEY
{
    BOARD_DIP_KEY_S3  = 0X00,
    BOARD_DIP_KEY_S4  = 0X01,
    BOARD_DIP_KEY_S5  = 0X02,
}BOARD_DIP_KEY;

typedef union BOARD_DIP_KEY_SET
{
    uint8_t valAll;
    struct
    {//位段
        //小端模式下least significant bits,这个是字节的最低位
        PIN_STATE s3Val : 1;
        PIN_STATE s4Val : 1;
        //大端模式下most significant bits,这个是字节的最低位
        PIN_STATE s5Val : 1;
    }valSingles;//板卡上丝印标号
}BOARD_DIP_KEY_SET;

//按键单个初始化
void BoardDipKeyInitSingle(BOARD_DIP_KEY dipKeyNo);

//按键全部初始化
void BoardDipKeyInitAll(void);

//按键读取
PIN_STATE BoardDipKeyReadSingle(BOARD_DIP_KEY dipKeyNo);

//单次读取全部按键状态
void BoardDipKeyReadAll(BOARD_DIP_KEY_SET* dipKeyValAllPtr);

#endif



