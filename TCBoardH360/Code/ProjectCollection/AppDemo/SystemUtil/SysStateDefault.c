/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-16 15:04:58 +0800
************************************************************************************************/ 
#include "SysState.h"

//主控信息标记
const SYS_STATE_MAIN_CONTROL_MSG mainControlMsgDefault = {
    SYSTEM_MODULE_NO_DEVICE_A,//主控模块号
    SYSTEM_BOARD_ID_MAIN_CONTROL_DEVICE,//主控板号
    RESET,//联机标记
    PROT_CONNECT_FLAG_DEBUG,//联机模式
    SYS_RUNNING_MODE_DEBUG,//系统运行模式
};


//温度传感器数据默认状态
const SYS_TEMP_SENSOR_STATE tempSensorStateSetDefault = {
    //板上温度传感器
    0.0f,   //板卡温度值
    ERROR,  //上次读取板上温度传感器的结果
    0,      //上次读取板卡温度值的时间
    0,      //上次成功读取板卡温度值的时间
    //环境温度传感器
    0.0f,   //温度传感器温度值
    ERROR,  //上次读取温度传感器的结果
    0X00,   //上次读取温度传感器的错误码
    0,      //上次读取温度值的时间
    0,      //上次成功读取温度值的时间
    0,      //上次更新温度传感器冷端温度的时间
    //反应盘温度传感器
    0.0f,   //温度传感器温度值
    ERROR,  //上次读取温度传感器的结果
    0X00,   //上次读取温度传感器的错误码
    0,      //上次读取温度值的时间
    0,      //上次成功读取温度值的时间
    0,      //上次更新温度传感器冷端温度的时
    //清洗盘温度传感器
    0.0f,   //温度传感器温度值
    ERROR,  //上次读取温度传感器的结果
    0X00,   //上次读取温度传感器的错误码
    0,      //上次读取温度值的时间
    0,      //上次成功读取温度值的时间
    0,      //上次更新温度传感器冷端温度的时
    //试剂盘温度传感器
    0.0f,   //温度传感器温度值
    ERROR,  //上次读取温度传感器的结果
    0X00,   //上次读取温度传感器的错误码
    0,      //上次读取温度值的时间
    0,      //上次成功读取温度值的时间
    0,      //上次更新温度传感器冷端温度的时
};


//反应盘控温默认状态
const SYS_TEMP_CONTROL_PID tempControlPlateReactionPIDSetDefault = {
    0.5f,//比例系数
    0.1f,//积分系数
    0.0f,//微分系数
    37.5f,//目标值
    5,//积分时间设定
    5,//系统微分时间设定
    0,//上次的积分缓存
    0,//上次的积分时间
    0,//微分上次的值缓存
    0,//微分上上次的值缓存
    0,//微分缓存个数,大于2的时候才能计算微分
    0,//上一次微分时间
};

//清洗盘控温默认状态
const SYS_TEMP_CONTROL_PID tempControlPlateWashPIDSetDefault = {
    0.5f,//比例系数
    0.1f,//积分系数
    0.0f,//微分系数
    37.5,//目标值
    5,//积分时间设定
    5,//系统微分时间设定
    0,//上次的积分缓存
    0,//上次的积分时间
    0,//微分上次的值缓存
    0,//微分上上次的值缓存
    0,//微分缓存个数,大于2的时候才能计算微分
    0,//上一次微分时间
};

//试剂盘控温默认状态
const SYS_TEMP_CONTROL_PID tempControlPlateReagentPIDSetDefault = {
    0.5f,//比例系数
    0.1f,//积分系数
    0.0f,//微分系数
    6.0f,//目标值
    5,//积分时间设定
    5,//系统微分时间设定
    0,//上次的积分缓存
    0,//上次的积分时间
    0,//微分上次的值缓存
    0,//微分上上次的值缓存
    0,//微分缓存个数,大于2的时候才能计算微分
    0,//上一次微分时间
};

//系统控温状态
const SYS_TEMP_CONTROL_SET tempControlSetDefault = {
    //环境温度控制状态
    TEMP_CONTROL_STATE_IDLE,
    //切换状态的时间
    0,
    //反应盘温度控制状态
    TEMP_CONTROL_STATE_IDLE,
    //切换状态的时间
    0,
    //清洗盘温度控制状态
    TEMP_CONTROL_STATE_IDLE,
    //切换状态的时间
    0,
    //试剂盘温度控制状态
    TEMP_CONTROL_STATE_IDLE,
    //切换状态的时间
    0,
    //环境风扇状态
    DISABLE,
    //试剂盘风扇与循环水状态
    DISABLE,
};

//系统温度上报状态
const SYS_TEMP_CONTROL_REPORT tempReportSetDefault = {
    //上报目标模块的模块号和板号
    SYSTEM_MODULE_NO_PC,
    SYSTEM_BOARD_ID_PC,
    DISABLE,//板上温度上报
    0,//上次上报的时间
    5000,//上报周期
    DISABLE,//环境温度上报
    0,//上次上报的时间
    5000,//上报周期
    DISABLE,//反应盘温度上报
    0,//上次上报的时间
    5000,//上报周期
    DISABLE,//清洗盘温度上报
    0,//上次上报的时间
    5000,//上报周期
    DISABLE,//试剂盘温度上报
    0,//上次上报的时间
    5000,//上报周期
};








