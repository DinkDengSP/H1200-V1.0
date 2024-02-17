/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-21 20:41:09 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DEFAULT_H_
#define __SYS_PARAM_DEFAULT_H_
#include "SysParamDataType.h"

//默认参数,系统温控配置
extern const SYS_TEMP_CONTROL_PARAM_CONFIG sysTempControlConfigDefault;

//默认参数,板卡运行温度限制
extern const BOARD_TEMP_LIMIT_PARAM boardTempLimitParamSetDefault;

//默认参数,环境运行温度限制
extern const ENVIR_TEMP_LIMIT_PARAM envirTempLimitParamSetDefault;

//默认参数,反应盘控温参数
extern const PLATE_REACTION_TEMP_CONTROL_PARAM plateReactionTempControlParamSetDefault;

//默认参数,清洗盘控温参数
extern const PLATE_WASH_TEMP_CONTROL_PARAM plateWashTempControlParamSetDefault;

//默认参数,试剂盘控温参数
extern const PLATE_REAGENT_TEMP_CONTROL_PARAM plateReagentTempControlParamSetDefault;

//默认参数,上报温度修正
extern const TEMP_REPORT_MODIFY_PARAM tempReportModifyParamSetDefault;



#endif





