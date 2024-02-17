/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SYSTEM_STATE_H_
#define __SYSTEM_STATE_H_
#include "SystemStateDataType.h"

//状态系统初始化
void SystemStateInit();

//获取电机状态集合
SYS_STATE_STEP_MOTOR* SysStateGetStepMotorStatePtr(void);

//获取主控信息集合
SYS_STATE_MAIN_CONTROL_MSG* SysStateGetMainControlMsgPtr(void);

#endif








