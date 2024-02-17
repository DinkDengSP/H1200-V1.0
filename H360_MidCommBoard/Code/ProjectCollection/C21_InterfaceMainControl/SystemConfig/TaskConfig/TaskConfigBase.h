/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_BASE_H_
#define __TASK_CONFIG_BASE_H_
#include "DriverConfigSystem.h"

//公共服务层时间片
#define TIME_SLICE_COMM_SRV_TASK                                        TX_NO_TIME_SLICE
//服务层时间片
#define TIME_SLICE_SRV_TASK                                             TX_NO_TIME_SLICE
//模块层时间片
#define TIME_SLICE_MOD_TASK                                             TX_NO_TIME_SLICE
//应用层层时间片
#define TIME_SLICE_APP_TASK                                             TX_NO_TIME_SLICE
//是否强制将转发模块号设置为下位机模块号
#define PROT_MODULE_NO_FORCE_SAME                                       CONFIG_FUNC_ENABLE

#endif





