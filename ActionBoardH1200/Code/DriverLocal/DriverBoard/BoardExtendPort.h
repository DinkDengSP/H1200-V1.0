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
#ifndef __BOARD_EXTEND_PORT_H_
#define __BOARD_EXTEND_PORT_H_
#include "DriverHeaderMCU.h"
#include "BoardTCA9555PWR.h"

//扩展输出端口
typedef enum BOARD_EXTEND_PORT_OUT_PIN
{
    BOARD_EXTEND_PORT_OUT_PIN_1     = 0,
    BOARD_EXTEND_PORT_OUT_PIN_2     = 1,
    BOARD_EXTEND_PORT_OUT_PIN_3     = 2,
    BOARD_EXTEND_PORT_OUT_PIN_4     = 3,
    BOARD_EXTEND_PORT_OUT_PIN_5     = 4,
    BOARD_EXTEND_PORT_OUT_PIN_6     = 5,
    BOARD_EXTEND_PORT_OUT_PIN_7     = 6,
    BOARD_EXTEND_PORT_OUT_PIN_8     = 7,
    BOARD_EXTEND_PORT_OUT_PIN_9     = 8,
    BOARD_EXTEND_PORT_OUT_PIN_10    = 9,
    BOARD_EXTEND_PORT_OUT_PIN_11    = 10,
    BOARD_EXTEND_PORT_OUT_PIN_12    = 11,
    BOARD_EXTEND_PORT_OUT_PIN_13    = 12,
    BOARD_EXTEND_PORT_OUT_PIN_14    = 13,
    BOARD_EXTEND_PORT_OUT_PIN_15    = 14,
    BOARD_EXTEND_PORT_OUT_PIN_16    = 15,
}BOARD_EXTEND_PORT_OUT_PIN;

//扩展输入端口
typedef enum BOARD_EXTEND_PORT_IN_PIN
{
    BOARD_EXTEND_PORT_IN_PIN_1      = 0,
    BOARD_EXTEND_PORT_IN_PIN_2      = 1,
    BOARD_EXTEND_PORT_IN_PIN_3      = 2,
    BOARD_EXTEND_PORT_IN_PIN_4      = 3,
    BOARD_EXTEND_PORT_IN_PIN_5      = 4,
    BOARD_EXTEND_PORT_IN_PIN_6      = 5,
    BOARD_EXTEND_PORT_IN_PIN_7      = 6,
    BOARD_EXTEND_PORT_IN_PIN_8      = 7,
    BOARD_EXTEND_PORT_IN_PIN_9      = 8,
    BOARD_EXTEND_PORT_IN_PIN_10     = 9,
    BOARD_EXTEND_PORT_IN_PIN_11     = 10,
    BOARD_EXTEND_PORT_IN_PIN_12     = 11,
    BOARD_EXTEND_PORT_IN_PIN_13     = 12,
    BOARD_EXTEND_PORT_IN_PIN_14     = 13,
    BOARD_EXTEND_PORT_IN_PIN_15     = 14,
    BOARD_EXTEND_PORT_IN_PIN_16     = 15,
}BOARD_EXTEND_PORT_IN_PIN;

//扩展输入端口状态集合
typedef struct BOARD_EXTEND_PORT_IN_SET
{
    TCA9555_PORT_VAL_SET port1ValSet;
}BOARD_EXTEND_PORT_IN_SET;


//扩展端口初始化
void BoardExtendPortInit(void);

//配置扩展端口默认值
ERROR_SUB BoardExtendPortConfigDefault(void);

//输出端口输出指定值
ERROR_SUB BoardExtendPortOutWriteSingle(BOARD_EXTEND_PORT_OUT_PIN outPin,GPIO_PinState outValue);

//输出端口获取当前输出值
ERROR_SUB BoardExtendPortOutReadSingleCurrent(BOARD_EXTEND_PORT_OUT_PIN outPin,GPIO_PinState* outValuePtr);

//输出端口切换状态
ERROR_SUB BoardExtendPortOutToogleSingle(BOARD_EXTEND_PORT_OUT_PIN outPin);

//获取输入端口状态
ERROR_SUB BoardExtendPortInSingle(BOARD_EXTEND_PORT_IN_PIN inPin,GPIO_PinState* inValuePtr);

//单次获取全部扩展端口讯息
ERROR_SUB BoardExtendPortInAll(BOARD_EXTEND_PORT_IN_SET* portInValSetPtr);

#endif





