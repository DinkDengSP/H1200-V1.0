/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 13:18:13 +0800
************************************************************************************************/ 
#include "SysParamDefault.h"

//清洗机械手参数
const SYS_PARAM_HAND_WASH handWashParamDefault = {
    0,//清洗机械手升降零位修正
    -16,//清洗机械手旋转零位修正
    1846,//清洗机械手旋转到反应盘坐标
    1890,//清洗机械手升降反应盘取杯坐标
    1830,//清洗机械手升降反应盘放杯坐标
    -16,//清洗机械手旋转到清洗盘坐标
    1880,//清洗机械手升降清洗盘取杯坐标
    1820,//清洗机械手升降清洗盘放杯坐标
    835,//清洗机械手旋转到测量模块坐标
    3330,//清洗机械手升降在测量模块取杯坐标
    3300,//清洗机械手升降在测量模块放杯坐标
    0,//清洗机械手码盘版本测量室取杯升降修正
    0,//清洗机械手测量室取杯位置检测是否使能
    {0},//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位      
};

//测量室参数
const SYS_PARAM_MEASURE_MODULE measureModuleParamDefault = {
    -80,//测量上门零位修正
    -75,//测量侧门零位修正
    0,//激发液注射泵零位修正
    -80,//测量上门关闭坐标
    2500,//测量上门半开坐标
    3150,//测量上门全开坐标
    -75,//测量侧门关闭坐标
    2610,//测量侧门打开坐标
    1000,//光子测量修正函数ax+b的a(放大1000倍)
    0,//光子测量修正函数ax+b的b(放大1000倍)
    {0},//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位      
};

//辅助参数
const SYS_PARAM_OTHER otherUtilParamDefault = {
    0,//辅助参数
    {0},//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位      
};



