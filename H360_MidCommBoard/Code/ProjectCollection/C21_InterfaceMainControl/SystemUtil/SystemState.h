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
#ifndef __SYSTEM_STATE_H_
#define __SYSTEM_STATE_H_
#include "SystemStateDefault.h"


//系统状态初始化
void SystemStateInit(void);


/*********************************************************心跳状态*****************************************************************/

//获取系统心跳状态指针
SYSTEM_STATE_HEART* SystemStateGetCommHeart(void);

//设置系统联机状态
void SystemStateSetClientConnectState(CLIENT_CONNECT state);

//获取系统联机状态
CLIENT_CONNECT SystemStateGetClientConnectState(void);

//设置系统运行状态
void SystemStateSetSysRunMode(MODE_RUNNING state);

//获取系统运行状态
MODE_RUNNING SystemStateGetSysRunMode(void);

//设置系统模块号
void SystemStateSetSysModuleID(uint8_t moduleID);

//获取系统模块号
uint8_t SystemStateGetSysModuleID(void);




#endif

















