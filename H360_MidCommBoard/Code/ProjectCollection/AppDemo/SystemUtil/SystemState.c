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
#include "SystemState.h"
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"
#include "SrvTaskHeader.h"

//系统状态指针
static SYSTEM_STATE* systemStatePtr = NULL;
//初始化系统状态为默认值
static void SystemStateSetDefault(void);


//系统状态初始化
void SystemStateInit(void)
{
    //首先申请内存
    systemStatePtr = NULL;
    //申请内存
    systemStatePtr = UserMemMallocWhileSuccess(MEM_REGION_SYSTEM_STATE,LENGTH_SYSTEM_STATE);
    //校验不通过,设定为默认配置
    SystemStateSetDefault();
}

//初始化系统状态为默认值
static void SystemStateSetDefault(void)
{
    systemStatePtr->stateInitFlag = SYSTEM_STATE_INIT_FLAG;
    //默认参数设定
    UserMemCopy((void*)&(systemStatePtr->sysHeartState),(void*)(&sysHeartStateDefault),LENGTH_SYSTEM_STATE_HEART);
}

/*********************************************************心跳状态*****************************************************************/

//获取系统心跳状态指针
SYSTEM_STATE_HEART* SystemStateGetCommHeart(void)
{
    return &(systemStatePtr->sysHeartState);
}

//设置系统联机状态
void SystemStateSetClientConnectState(CLIENT_CONNECT state)
{
    systemStatePtr->sysHeartState.clientConnectState = state;
}

//获取系统联机状态
CLIENT_CONNECT SystemStateGetClientConnectState(void)
{
    return systemStatePtr->sysHeartState.clientConnectState;
}

//设置系统运行状态
void SystemStateSetSysRunMode(MODE_RUNNING state)
{
    systemStatePtr->sysHeartState.runningMode = state;
}

//获取系统运行状态
MODE_RUNNING SystemStateGetSysRunMode(void)
{
    return systemStatePtr->sysHeartState.runningMode;
}

//设置系统模块号
void SystemStateSetSysModuleID(uint8_t moduleID)
{
    systemStatePtr->sysHeartState.moduleID = moduleID;
}

//获取系统模块号
uint8_t SystemStateGetSysModuleID(void)
{
    return systemStatePtr->sysHeartState.moduleID;
}













