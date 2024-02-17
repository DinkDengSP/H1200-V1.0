/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 11:33:28
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_PARAM_DATA_TYPE_H_
#define __SYSTEM_PARAM_DATA_TYPE_H_
#include "DriverHeaderSystem.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

/*---------------------------------------------------------------------------------------------------------------*/
//板卡信息参数
typedef struct PARAM_BOARD_INFO
{
    int32_t boardInfoItem1;//板卡信息参数1
    int32_t reserve[250];//预留251个参数位置
}PARAM_BOARD_INFO;

//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    PARAM_BOARD_INFO paramBoardInfo;//板卡信息参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif







