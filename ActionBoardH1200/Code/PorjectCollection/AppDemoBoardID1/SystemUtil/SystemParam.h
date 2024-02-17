/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 11:55:31
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_PARAM_H_
#define __SYSTEM_PARAM_H_
#include "SystemParamDataType.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_BOARD_INFO                 = 0X00,//板卡信息
}INDEX_MAIN_SYS_PARAM;

//板卡信息参数子序号
typedef enum INDEX_SUB_PARAM_BOARD_INFO
{
    INDEX_SUB_PARAM_BOARD_INFO_ITEM1                = 0X00,//板卡信息参数1
}INDEX_SUB_PARAM_BOARD_INFO;


//参数系统初始化
void SystemParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);

#endif








