/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-31 14:05:03
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_PARAM_DATA_TYPE_H_
#define __SYSTEM_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYSTEM_PARAM_DEFAULT_FLAG              0XAA5555AA

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYSTEM_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
}SYSTEM_PARAM;
//参数长度
#define LENGTH_SYSTEM_PARAM     DIM_STRUCT_LENGTH_BYTE(SYSTEM_PARAM)


#endif


