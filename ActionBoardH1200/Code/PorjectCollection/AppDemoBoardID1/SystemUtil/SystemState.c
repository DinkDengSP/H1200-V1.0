/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 11:05:39
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SystemState.h"
#include "SystemStateDefault.h"
#include "DriverConfigSystem.h"

//系统状态指针
static SYS_STATE* sysStatePtr = NULL;
//初始化系统状态为默认值
static void SysStateSetDefault(void);

//状态系统初始化
void SystemStateInit(void)
{
    //首先申请内存
    sysStatePtr = NULL;
    //申请内存
    sysStatePtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_STATE,LENGTH_SYS_STATE);
    //校验不通过,设定为默认配置
    SysStateSetDefault();
}


//初始化系统状态为默认值
static void SysStateSetDefault(void)
{
    sysStatePtr->stateInitFlag = SYS_STATE_INIT_FLAG;
    //默认参数设定
    UserMemCopy((void*)&(sysStatePtr->stepMotorState),(void*)(&sysStepMotorStateDefault),LENGTH_SYS_STATE_STEP_MOTOR);
    UserMemCopy((void*)&(sysStatePtr->mainControlMsg),(void*)(&mainControlMsgDefault),LENGTH_SYS_STATE_MAIN_CONTROL_MSG);
}

//获取电机状态集合
SYS_STATE_STEP_MOTOR* SysStateGetStepMotorStatePtr(void)
{
    return &(sysStatePtr->stepMotorState);
}

//获取主控信息集合
SYS_STATE_MAIN_CONTROL_MSG* SysStateGetMainControlMsgPtr(void)
{
    return &(sysStatePtr->mainControlMsg);
}







