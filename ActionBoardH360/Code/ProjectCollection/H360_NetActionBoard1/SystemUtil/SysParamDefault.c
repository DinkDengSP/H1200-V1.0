/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 17:07:17 +0800
************************************************************************************************/ 
#include "SysParamDefault.h"


//清洗盘参数
const SYS_PARAM_PLATE_WASH paramPlateWashDefault = {
    -218,//清洗盘零位修正
    {0}, //保留的参数
};

//清洗摇匀参数
const SYS_PARAM_MIX_WASH paramMixWashDefault = {
    0,   //清洗摇匀升降零位修正
    0,   //清洗摇匀旋转零位修正
    2250,//清洗摇匀模式1升降高度
    4000,//清洗摇匀模式1摇匀时间,毫秒
    2470,//清洗摇匀模式2升降高度
    4000,//清洗摇匀模式2摇匀时间,毫秒
    2470,//清洗摇匀模式3升降高度
    4000,//清洗摇匀模式3摇匀时间,毫秒
    2470,//清洗摇匀模式4升降高度
    4000,//清洗摇匀模式4摇匀时间,毫秒
    2470,//清洗摇匀模式5升降高度
    4000,//清洗摇匀模式5摇匀时间,毫秒
    2470,//清洗摇匀模式6升降高度
    4000,//清洗摇匀模式6摇匀时间,毫秒
    2470,//清洗摇匀模式7升降高度
    4000,//清洗摇匀模式7摇匀时间,毫秒
    2470,//清洗摇匀模式7升降高度
    4000,//清洗摇匀模式8摇匀时间,毫秒
    2470,//清洗摇匀模式8升降高度
    4000,//清洗摇匀模式9摇匀时间,毫秒
    2470,//清洗摇匀模式10升降高度
    4000,//清洗摇匀模式10摇匀时间,毫秒
    {0}, //保留的参数
};

//清洗针参数
const SYS_PARAM_NEEDLE_WASH paramNeedleWashDefault = {
    0,   //清洗针升降零位修正
    2000,//清洗针升降到杯口坐标
    4680,//清洗针升降到杯底坐标
    400, //清洗针注射泵1吸液量,微升
    500, //清洗针注射泵2吸液量,微升
    600, //清洗针注射泵3吸液量,微升
    600, //清洗针注射泵4吸液量,微升
    600, //清洗针注射泵5吸液量,微升
    1000,//清洗针在杯底等待抽废液时间,毫秒
    50,  //清洗针在杯口等待注液稳定时间,毫秒
    1000,//清洗针灌注一次等抽废液完成时间,毫秒
    0,//清洗针升降到浸泡位
    0,//维护清洗时间
    0,//清洗针2-5杯底注液等待时间
    0,//清洗针1注液前等待针针抬起时间
    0,//清洗针堵针阈值上限
    {0}, //保留的参数
};

//缓冲液注液参数
const SYS_PARAM_BUFFER_INJECT paramBufferInjectDefault = {
    0,  //缓冲液注射泵零位修正
    {0},//保留的参数
};

//液路自动化参数
const SYS_PARAM_LIQUID_AUTO paramLiquidAutoDefault = {
    0,//预留参数,现在没用
    {0},  //保留的参数
};

//其余辅助参数
const SYS_PARAM_OTHER paramOtherDefault = {
    0,//预留参数,现在没用
    {0},//保留的参数
};
