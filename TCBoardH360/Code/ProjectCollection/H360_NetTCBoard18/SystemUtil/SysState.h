/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-17 15:42:54 +0800
************************************************************************************************/ 
#ifndef __SYS_STATE_H_
#define __SYS_STATE_H_
#include "SysStateDefault.h"

//系统状态初始化
void SysStateInit(void);

//获取温度传感器数据集合指针
SYS_TEMP_SENSOR_STATE* SysStateGetSensorSetPtr(void);

//获取反应盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateReactionPIDSetPtr(void);

//获取清洗盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateWashPIDSetPtr(void);

//获取试剂盘PID控温系数
SYS_TEMP_CONTROL_PID* SysStateGetPlateReagentPIDSetPtr(void);

//获取系统控温状态
SYS_TEMP_CONTROL_SET* SysStateGetTempControlSetPtr(void);

//通过控制状态获取描述字符串
uint8_t* SysStateControlGetDesp(TEMP_CONTROL_STATE controlState);

//获取上报配置指针
SYS_TEMP_CONTROL_REPORT* SysStateGetTempControlReportSetPtr(void);

//获取主控信息集合
SYS_STATE_MAIN_CONTROL_MSG* SysStateGetMainControlMsgPtr(void);

#endif

















