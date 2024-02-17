/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 16:29:15 +0800
************************************************************************************************/ 
#ifndef __SYS_STATE_DEFAULTH_
#define __SYS_STATE_DEFAULTH_
#include "SysStateDataType.h"

//主控信息标记
extern const SYS_STATE_MAIN_CONTROL_MSG mainControlMsgDefault;

//温度传感器数据默认状态
extern const SYS_TEMP_SENSOR_STATE tempSensorStateSetDefault;

//反应盘控温默认状态
extern const SYS_TEMP_CONTROL_PID tempControlPlateReactionPIDSetDefault;

//清洗盘控温默认状态
extern const SYS_TEMP_CONTROL_PID tempControlPlateWashPIDSetDefault;

//试剂盘控温默认状态
extern const SYS_TEMP_CONTROL_PID tempControlPlateReagentPIDSetDefault;

//系统控温状态
extern const SYS_TEMP_CONTROL_SET tempControlSetDefault;

//系统温度上报状态
extern const SYS_TEMP_CONTROL_REPORT tempReportSetDefault;

#endif
















