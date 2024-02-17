/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-21 08:55:34 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//浮点参数放大系数
#define AMP_FACTOR_SYS_PARAM            (100.0f)

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_BOARD_TEMP_LIMIT= 0x00,//板上
    INDEX_MAIN_SYS_PARAM_ENVIR_TEMP_LIMIT= 0x01,//环境
    INDEX_MAIN_SYS_PARAM_PLATE_REACTION  = 0x02,//反应盘
    INDEX_MAIN_SYS_PARAM_PLATE_WASH      = 0x03,//清洗盘
    INDEX_MAIN_SYS_PARAM_PLATE_REAGENT   = 0x04,//试剂盘
    INDEX_MAIN_SYS_PARAM_SENSOR          = 0x05,//传感器温度
    INDEX_MAIN_SYS_PARAM_REPORT_MODIFY   = 0X06,//显示修正
}INDEX_MAIN_SYS_PARAM;

//参数读写子序号,板上温度限制
typedef enum INDEX_SUB_PARAM_BOARD_TEMP_LIMIT
{
    INDEX_SUB_PARAM_BOARD_TEMP_MAX_LIMIT = 0X00,//保护温度上
    INDEX_SUB_PARAM_BOARD_TEMP_MIN_LIMIT = 0X01,//保护温度下
}INDEX_SUB_PARAM_BOARD_TEMP_LIMIT;

//参数读写子序号,环境温度限制
typedef enum INDEX_SUB_PARAM_ENVIR_TEMP_LIMIT
{
    INDEX_SUB_PARAM_ENVIR_MAX_LIMIT     = 0X00,//保护温度上
    INDEX_SUB_PARAM_ENVIR_MIN_LIMIT     = 0X01,//保护温度下
    INDEX_SUB_PARAM_ENVIR_TEMP_OPEN_FAN = 0X02,//风扇打开温度
}INDEX_SUB_PARAM_ENVIR_TEMP_LIMIT;

//参数读写子序号,反应盘
typedef enum INDEX_SUB_PARAM_PLATE_REACTION
{
    INDEX_SUB_PARAM_PLATE_REACTION_TARGET_TEMP       = 0X00,//目标温度
    INDEX_SUB_PARAM_PLATE_REACTION_KP                = 0X01,//比例系数
    INDEX_SUB_PARAM_PLATE_REACTION_KI                = 0X02,//积分系数
    INDEX_SUB_PARAM_PLATE_REACTION_KD                = 0X03,//微分系数
    INDEX_SUB_PARAM_PLATE_REACTION_HIGH_THRESHOLD    = 0X04,//全速降温上阈值
    INDEX_SUB_PARAM_PLATE_REACTION_LOW_THRESHOLD     = 0X05,//全速升温下阈值
    INDEX_SUB_PARAM_PLATE_REACTION_MAX_POWER         = 0X06,//最大输出功率
    INDEX_SUB_PARAM_PLATE_REACTION_MIN_POWER         = 0X07,//最小输出功率
    INDEX_SUB_PARAM_PLATE_REACTION_HIGH_PROTECT      = 0X08,//保护温度上
    INDEX_SUB_PARAM_PLATE_REACTION_LOW_PROTECT       = 0X09,//保护温度下
    INDEX_SUB_PARAM_PLATE_REACTION_KI_TIME_SECOND    = 0X0A,//积分时间,秒
    INDEX_SUB_PARAM_PLATE_REACTION_KD_TIME_SECOND    = 0X0B,//微分时间,秒
}INDEX_SUB_PARAM_PLATE_REACTION;

//参数读写子序号,清洗盘
typedef enum INDEX_SUB_PARAM_PLATE_WASH
{
    INDEX_SUB_PARAM_PLATE_WASH_TARGET_TEMP     = 0X00,//目标温度
    INDEX_SUB_PARAM_PLATE_WASH_KP              = 0X01,//比例系数
    INDEX_SUB_PARAM_PLATE_WASH_KI              = 0X02,//积分系数
    INDEX_SUB_PARAM_PLATE_WASH_KD              = 0X03,//微分系数
    INDEX_SUB_PARAM_PLATE_WASH_HIGH_THRESHOLD  = 0X04,//全速降温上阈值
    INDEX_SUB_PARAM_PLATE_WASH_LOW_THRESHOLD   = 0X05,//全速升温下阈值
    INDEX_SUB_PARAM_PLATE_WASH_MAX_POWER       = 0X06,//最大输出功率
    INDEX_SUB_PARAM_PLATE_WASH_MIN_POWER       = 0X07,//最小输出功率
    INDEX_SUB_PARAM_PLATE_WASH_HIGH_PROTECT    = 0X08,//保护温度上
    INDEX_SUB_PARAM_PLATE_WASH_LOW_PROTECT     = 0X09,//保护温度下
    INDEX_SUB_PARAM_PLATE_WASH_KI_TIME_SECOND  = 0X0A,//积分时间,秒
    INDEX_SUB_PARAM_PLATE_WASH_KD_TIME_SECOND  = 0X0B,//微分时间,秒
}INDEX_SUB_PARAM_PLATE_WASH;

//参数读写子序号,试剂盘
typedef enum INDEX_SUB_PARAM_PLATE_REAGENT
{
    INDEX_SUB_PARAM_PLATE_REAGENT_TARGET_TEMP       = 0X00,//目标温度
    INDEX_SUB_PARAM_PLATE_REAGENT_KP                = 0X01,//比例系数
    INDEX_SUB_PARAM_PLATE_REAGENT_KI                = 0X02,//积分系数
    INDEX_SUB_PARAM_PLATE_REAGENT_KD                = 0X03,//微分系数
    INDEX_SUB_PARAM_PLATE_REAGENT_HIGH_THRESHOLD    = 0X04,//全速降温上阈值
    INDEX_SUB_PARAM_PLATE_REAGENT_LOW_THRESHOLD     = 0X05,//全速升温下阈值
    INDEX_SUB_PARAM_PLATE_REAGENT_MAX_POWER         = 0X06,//最大输出功率
    INDEX_SUB_PARAM_PLATE_REAGENT_MIN_POWER         = 0X07,//最小输出功率
    INDEX_SUB_PARAM_PLATE_REAGENT_HIGH_PROTECT      = 0X08,//保护温度上
    INDEX_SUB_PARAM_PLATE_REAGENT_LOW_PROTECT       = 0X09,//保护温度下
    INDEX_SUB_PARAM_PLATE_REAGENT_TEMP_CTRL_TYPE    = 0x0A,//试剂盘温度控制类型
    INDEX_SUB_PARAM_PLATE_REAGENT_KI_TIME_SECOND    = 0X0B,//积分时间,秒
    INDEX_SUB_PARAM_PLATE_REAGENT_KD_TIME_SECOND    = 0X0C,//微分时间,秒
}INDEX_SUB_PARAM_PLATE_REAGENT;

//参数读写子序号,温度传感器
typedef enum INDEX_SUB_PARAM_SENSOR
{
    INDEX_SUB_PARAM_SENSOR_BOARD                = 0X00,//板上温度
    INDEX_SUB_PARAM_SENSOR_ENVIR                = 0X01,//环境温度
    INDEX_SUB_PARAM_SENSOR_PLATE_REACTION       = 0X02,//反应盘温度
    INDEX_SUB_PARAM_SENSOR_PLATE_WASH           = 0X03,//清洗盘温度
    INDEX_SUB_PARAM_SENSOR_PLATE_REAGENT        = 0X04,//试剂盘温度
}INDEX_SUB_PARAM_SENSOR;

//参数读写子序号,显示修正
typedef enum INDEX_SUB_PARAM_REPORT_MODIFY
{
    INDEX_SUB_PARAM_REPORT_MODIFY_BOARD_TEMP            = 0X00,//板上温度上报修正
    INDEX_SUB_PARAM_REPORT_MODIFY_ENVIR_TEMP            = 0X01,//环境温度上报修正
    INDEX_SUB_PARAM_REPORT_MODIFY_PLATE_REACTION_TEMP   = 0X02,//反应盘温度上报修正
    INDEX_SUB_PARAM_REPORT_MODIFY_PLATE_WASH_TEMP       = 0X03,//清洗盘温度上报修正
    INDEX_SUB_PARAM_REPORT_MODIFY_PLATE_REAGENT_TEMP    = 0X04,//试剂盘温度上报修正
}INDEX_SUB_PARAM_REPORT_MODIFY;


//初始化系统参数
void SysParamInit(void);

//获取系统参数指针
SYS_PARAM* SysParamGetAllPtr(void);

//获取温控配置参数指针
SYS_TEMP_CONTROL_PARAM_CONFIG* SysParamGetTempControlConfigPtr(void);

//获取板卡运行温度限制参数指针
BOARD_TEMP_LIMIT_PARAM* SysParamGetBoardTempLimitParamSetPtr(void);

//获取环境运行温度限制参数指针
ENVIR_TEMP_LIMIT_PARAM* SysParamGetEnvirTempLimitParamSetPtr(void);

//获取反应盘控温参数指针
PLATE_REACTION_TEMP_CONTROL_PARAM* SysParamGetPlateReactionTempControlParamSetPtr(void);

//获取清洗盘控温参数指针
PLATE_WASH_TEMP_CONTROL_PARAM* SysParamGetPlateWashTempControlParamSetPtr(void);

//获取试剂盘控温参数指针
PLATE_REAGENT_TEMP_CONTROL_PARAM* SysParamGetPlateReagentTempControlParamSetPtr(void);

//获取上报温度修正参数
TEMP_REPORT_MODIFY_PARAM* SysParamGetReportModifyParamSetPtr(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);



#endif








