/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-28 15:54:37 +0800
************************************************************************************************/ 
#include "SysParamDefault.h"

//默认参数,系统温控配置
const SYS_TEMP_CONTROL_PARAM_CONFIG sysTempControlConfigDefault = {
    ENABLE,//是否使能反应盘控温
    ENABLE,//是否使能清洗盘控温
    ENABLE,//是否使能试剂盘控温
    DISABLE,//是否使能环境温度检测与相关控制
    ENABLE,//是否使能玻璃加热
    //试剂盘TEC控温模式,PID控温或者阈值控温
    TEC_MODE_TEMP_CONTROL_THRESHOLD,//TEC控温模式
    0.999,//玻璃加热功率
    20000,//等待试剂盘冷却水到位时间
    20000,//延时等待试剂盘关闭循环水和泵的时间

    DISABLE,//是否使能反应盘电流检测
    0,      //反应盘电流检测下限值
    3300,   //反应盘电流检测上限值
    
    DISABLE,//是否使能清洗盘电流检测
    0,      //清洗盘电流检测下限值
    3300,   //清洗盘电流检测上限值
    
    DISABLE,//是否使能试剂盘电流检测1
    0,      //试剂盘电流检测1电流检测下限值
    3300,   //试剂盘电流检测1电流检测上限值

    DISABLE,//是否使能试剂盘电流检测2
    0,      //试剂盘电流检测2电流检测下限值
    3300,   //试剂盘电流检测2电流检测上限值
    
    DISABLE,//是否使能试剂盘电流检测3
    0,      //试剂盘电流检测3电流检测下限值
    3300,   //试剂盘电流检测3电流检测上限值

    DISABLE,//是否使能试剂盘电流检测4
    0,      //试剂盘电流检测4电流检测下限值
    3300,   //试剂盘电流检测4电流检测上限值
    
    {0},    //保留参数
};

//默认参数,板卡运行温度限制
const BOARD_TEMP_LIMIT_PARAM boardTempLimitParamSetDefault = {
    //板上温度
    60.0f,       //板上温度上限
    -20.0f,      //板上温度下限
    {0},        //预留空间
};

//默认参数,环境运行温度限制
const ENVIR_TEMP_LIMIT_PARAM envirTempLimitParamSetDefault = {
    //环境温度
    60.0f,       //环境温度上限温度
    -20.0f,      //环境温度下限温度
    30,         //打开风扇的温度
    {0},        //预留空间
};

//默认参数,反应盘控温参数
const PLATE_REACTION_TEMP_CONTROL_PARAM plateReactionTempControlParamSetDefault = {
    //反应盘
    37.5f,       //目标温度
    0.5f,        //PID控温参数 比例
    0.1f,        //PID控温参数 积分
    0.00f,       //PID控温参数 微分
    38.5f,       //温度上限阈值,超过该数据,最大功率降温
    36.5f,       //温度下限阈值,低于该数据,最大功率升温
    0.9999f,     //输出最大功率
    0.0001f,     //输出最小功率
    70.0f,         //保护温度上限,高于该温度不控温,报警
    -20.0f,      //保护温度下限,低于该温度不控温,报警
    5,          //积分时间设定
    5,          //微分时间设定
    {0},        //预留空间
};

//默认参数,清洗盘控温参数
const PLATE_WASH_TEMP_CONTROL_PARAM plateWashTempControlParamSetDefault = {
    //清洗盘
    37.5f,       //目标温度
    0.5f,        //PID控温参数 比例
    0.1f,        //PID控温参数 积分
    0.00f,       //PID控温参数 微分
    38.5f,       //温度上限阈值,超过该数据,最大功率降温
    36.5f,       //温度下限阈值,低于该数据,最大功率升温
    0.9999f,     //输出最大功率
    0.0001f,     //输出最小功率
    70.0f,         //保护温度上限,高于该温度不控温,报警
    -20.0f,      //保护温度下限,低于该温度不控温,报警
    5,          //积分时间设定
    5,          //微分时间设定
    {0},        //预留空间
};

//默认参数,试剂盘控温参数
const PLATE_REAGENT_TEMP_CONTROL_PARAM plateReagentTempControlParamSetDefault = {
    //试剂盘
    6.0f,       //目标温度
    0.5f,        //PID控温参数 比例
    0.1f,        //PID控温参数 积分
    0.00f,       //PID控温参数 微分
    8.0f,        //温度上限阈值,超过该数据,最大功率降温
    4.0f,        //温度下限阈值,低于该数据,最大功率升温
    0.9999f,     //输出最大功率
    0.0001f,     //输出最小功率
    60.0f,         //保护温度上限,高于该温度不控温,报警
    -20.0f,      //保护温度下限,低于该温度不控温,报警
    0,          //试剂盘温控控制类型
    5,          //积分时间设定
    5,          //微分时间设定
    {0},        //预留空间
};

//默认参数,上报温度修正
const TEMP_REPORT_MODIFY_PARAM tempReportModifyParamSetDefault = {
    0.0f,//板卡温度修正
    0.0f,//环境温度修正
    0.0f,//反应盘温度修正
    0.0f,//清洗盘温度修正
    0.0f,//试剂盘温度修正
    {0},   
};

