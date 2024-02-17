/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-20 11:53:28 +0800
************************************************************************************************/ 
#include "SysState.h"
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"
#include "SrvTaskHeader.h"

//系统状态指针
static SYS_STATE* sysStatePtr = NULL;
//初始化系统状态为默认值
static void SysStateSetDefault(void);

//系统状态初始化
void SysStateInit(void)
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
    //主控信息设定
    UserMemCopy((void*)&(sysStatePtr->mainControlMsg),(void*)(&mainControlMsgDefault),LENGTH_SYS_STATE_MAIN_CONTROL_MSG);
    //默认参数设定
    UserMemCopy((void*)&(sysStatePtr->tempSensorStateSet),(void*)(&tempSensorStateSetDefault),LENGTH_SYS_TEMP_SENSOR_STATE);
    //PID参数初始化
    UserMemCopy((void*)&(sysStatePtr->tempControlPlateReactionPIDSet),(void*)(&tempControlPlateReactionPIDSetDefault),
                    SIZE_SYS_TEMP_CONTROL_PID);
    UserMemCopy((void*)&(sysStatePtr->tempControlPlateWashPIDSet),(void*)(&tempControlPlateWashPIDSetDefault),
                    SIZE_SYS_TEMP_CONTROL_PID);
    UserMemCopy((void*)&(sysStatePtr->tempControlPlateReagentPIDSet),(void*)(&tempControlPlateReagentPIDSetDefault),
                    SIZE_SYS_TEMP_CONTROL_PID);
    //系统控温状态初始化
    UserMemCopy((void*)&(sysStatePtr->tempControlSet),(void*)(&tempControlSetDefault),
                    SIZE_SYS_TEMP_CONTROL_SET);
    //系统温度上报功能初始化
    UserMemCopy((void*)&(sysStatePtr->tempReportSet),(void*)(&tempReportSetDefault),
                    SIZE_SYS_TEMP_CONTROL_REPORT);
}

//获取主控信息集合
SYS_STATE_MAIN_CONTROL_MSG* SysStateGetMainControlMsgPtr(void)
{
    return &(sysStatePtr->mainControlMsg);
}

//获取温度传感器数据集合指针
SYS_TEMP_SENSOR_STATE* SysStateGetSensorSetPtr(void)
{
    return &(sysStatePtr->tempSensorStateSet);
}

//获取反应盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateReactionPIDSetPtr(void)
{
    return &(sysStatePtr->tempControlPlateReactionPIDSet);
}

//获取清洗盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateWashPIDSetPtr(void)
{
    return &(sysStatePtr->tempControlPlateWashPIDSet);
}

//获取试剂盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateReagentPIDSetPtr(void)
{
    return &(sysStatePtr->tempControlPlateReagentPIDSet);
}

//获取系统控温状态
SYS_TEMP_CONTROL_SET* SysStateGetTempControlSetPtr(void)
{
    return &(sysStatePtr->tempControlSet);
}

//温控状态和描述映射
typedef struct TEMP_CONTROL_STATE_MAP_STR
{
    TEMP_CONTROL_STATE state;
    uint8_t* despStr;
}TEMP_CONTROL_STATE_MAP_STR;
//NETX状态映射表
static const TEMP_CONTROL_STATE_MAP_STR TempControlStrMapArray[] = {
    {TEMP_CONTROL_STATE_IDLE                   ,"IDLE              "},
    {TEMP_CONTROL_STATE_DISABLE                ,"DISABLE           "},
    {TEMP_CONTROL_STATE_OFF                    ,"OFF               "},
    {TEMP_CONTROL_STATE_RUNNING_WAIT_WATER     ,"RUNNING_WAIT_WATER"},
    {TEMP_CONTROL_STATE_RUNNING_MAX_POWER      ,"RUNNING_MAX_POWER "},
    {TEMP_CONTROL_STATE_RUNNING_MIN_POWER      ,"RUNNING_MIN_POWER "},
    {TEMP_CONTROL_STATE_RUNNING_PID            ,"RUNNING_PID       "},
    {TEMP_CONTROL_STATE_RUNNING_NORMAL         ,"RUNNING_NORMAL    "},
    {TEMP_CONTROL_STATE_STOP_FLOAT             ,"STOP_FLOAT        "},
    {TEMP_CONTROL_STATE_STOP_BOARD_SENSOR      ,"STOP_BOARD_SENSOR "},
    {TEMP_CONTROL_STATE_STOP_BOARD_HIGH        ,"STOP_BOARD_HIGH   "},
    {TEMP_CONTROL_STATE_STOP_BOARD_LOW         ,"STOP_BOARD_LOW    "},
    {TEMP_CONTROL_STATE_STOP_SENSOR_ERR        ,"STOP_SENSOR_ERR   "},
    {TEMP_CONTROL_STATE_STOP_SENSOR_HIGH       ,"STOP_SENSOR_HIGH  "},
    {TEMP_CONTROL_STATE_STOP_SENSOR_LOW        ,"STOP_SENSOR_LOW   "},
    {TEMP_CONTROL_STATE_STOP_CURRENT           ,"STOP_CURRENT      "},
    {TEMP_CONTROL_STATE_STOP_ENVIR             ,"STOP_ENVIR        "},
};
#define LENGTH_TEMP_CONTROL_STATE_MAP_STR       (sizeof(TempControlStrMapArray)/sizeof(TempControlStrMapArray[0]))
//未知状态描述表
static const uint8_t TempControlStrUnknow[] = "Unknow ControlState";

//通过控制状态获取描述字符串
uint8_t* SysStateControlGetDesp(TEMP_CONTROL_STATE controlState)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_TEMP_CONTROL_STATE_MAP_STR;indexUtil++)
    {
        if(controlState == TempControlStrMapArray[indexUtil].state)
        {
            return (uint8_t*)(TempControlStrMapArray[indexUtil].despStr);
        }
    }
    return (uint8_t*)(TempControlStrUnknow);
}


//获取上报配置指针
SYS_TEMP_CONTROL_REPORT* SysStateGetTempControlReportSetPtr(void)
{
    return &(sysStatePtr->tempReportSet);
}

