/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-19 16:56:57 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_H360_BASE_H_
#define __CAN_MASTER_H360_BASE_H_
#include "CanMasterH360Macro.h"


//杯中最大液体量
#define H360_CUP_MAX_CAPACITY_UL                            1400
//系统彩蛋值,针
#define H360_EASTER_EGGS_NEEDLE_VAL1                        1989
//系统彩蛋值,针
#define H360_EASTER_EGGS_NEEDLE_VAL2                        1121

//系统彩蛋值,测量模块
#define H360_EASTER_EGGS_MEASURE_MODULE_DARK                1120
//系统彩蛋值,测量模块
#define H360_EASTER_EGGS_MEASURE_MODULE_BKGND               1989
//系统彩蛋值,测量模块
#define H360_EASTER_EGGS_MEASURE_MODULE_LIGHT               19891120

//注液高度
typedef enum NEEDLE_INJECT_LEVEL
{
    NEEDLE_INJECT_LEVEL_1_SHALLOWEST    = 0x00,//排液位置最浅
    NEEDLE_INJECT_LEVEL_2               = 0X01,
    NEEDLE_INJECT_LEVEL_3               = 0X02,
    NEEDLE_INJECT_LEVEL_4               = 0X03,
    NEEDLE_INJECT_LEVEL_5               = 0X04,
    NEEDLE_INJECT_LEVEL_6               = 0X05,
    NEEDLE_INJECT_LEVEL_7               = 0X06,
    NEEDLE_INJECT_LEVEL_8_DEEPEST       = 0X07,//排液位置最深
}NEEDLE_INJECT_LEVEL;

//吸液高度
typedef enum NEEDLE_ABSORB_LEVEL
{
    NEEDLE_ABSORB_LEVEL_1_SHALLOWEST    = 0x00,//吸液位置最浅
    NEEDLE_ABSORB_LEVEL_2               = 0X01,
    NEEDLE_ABSORB_LEVEL_3               = 0X02,
    NEEDLE_ABSORB_LEVEL_4               = 0X03,
    NEEDLE_ABSORB_LEVEL_5               = 0X04,
    NEEDLE_ABSORB_LEVEL_6               = 0X05,
    NEEDLE_ABSORB_LEVEL_7               = 0X06,
    NEEDLE_ABSORB_LEVEL_8_DEEPEST       = 0X07,//吸液位置最深
}NEEDLE_ABSORB_LEVEL;

//混匀模式,每个混匀有多种混匀模式,具体需要底层下位机动作版自己定义
typedef enum MIX_MODE
{
    MIX_MODE_1   = 0X00,
    MIX_MODE_2   = 0X01,
    MIX_MODE_3   = 0X02,
    MIX_MODE_4   = 0X03,
    MIX_MODE_5   = 0X04,
    MIX_MODE_6   = 0X05,
    MIX_MODE_7   = 0X06,
    MIX_MODE_8   = 0X07,
    MIX_MODE_9   = 0X08,
    MIX_MODE_10  = 0X09,
    MIX_MODE_MAX = MIX_MODE_10,
}MIX_MODE;

//混匀升起来的函数指针
typedef void (*ModImplMixUpFuncPtr)(SYS_RESULT_PACK* resultPackPtr);
//混匀旋转不带升降的函数指针
typedef void (*ModImplMixStartRotateFuncPtr)(SYS_RESULT_PACK* resultPackPtr);
//混匀集成混匀的函数指针
typedef void (*ModImplMixRotateIntegratedFuncPtr)(SYS_RESULT_PACK* resultPackPtr);

//针清洗配置
typedef enum NEEDLE_CLEAN_OPT
{
    NEEDLE_CLEAN_OPT_IN     = 0X01,//内壁清洗
    NEEDLE_CLEAN_OPT_OUT    = 0X02,//外壁清洗
    NEEDLE_CLEAN_OPT_ALL    = 0X03,//内外壁同时清洗
}NEEDLE_CLEAN_OPT;

//针灌注配置
typedef enum NEEDLE_PRIME_OPT
{
    NEEDLE_PRIME_OPT_IN     = 0X01,//内壁灌注
    NEEDLE_PRIME_OPT_OUT    = 0X02,//外壁灌注
    NEEDLE_PRIME_OPT_ALL    = 0X03,//内外壁同时灌注
}NEEDLE_PRIME_OPT;

//针维护配置
typedef enum NEEDLE_MAINTAIN_OPT
{
    NEEDLE_MAINTAIN_OPT_IN     = 0X01,//内壁清洗维护
    NEEDLE_MAINTAIN_OPT_OUT    = 0X02,//外壁清洗维护
    NEEDLE_MAINTAIN_OPT_ALL    = 0X03,//内外壁同时清洗维护
}NEEDLE_MAINTAIN_OPT;

//针清洗时间
typedef enum NEEDLE_CLEAN_TIME
{
    NEEDLE_CLEAN_TIME_1 = 0x00,//长清洗
    NEEDLE_CLEAN_TIME_2 = 0x01,//短清洗
    NEEDLE_CLEAN_TIME_3 = 0x02,
    NEEDLE_CLEAN_TIME_4 = 0x03,
    NEEDLE_CLEAN_TIME_5 = 0x04,
}NEEDLE_CLEAN_TIME;

//探液板配置
typedef enum LIQUID_DETECT_CONFIG
{
    LIQUID_DETECT_CONFIG_1  = 0,
    LIQUID_DETECT_CONFIG_2  = 1,
    LIQUID_DETECT_CONFIG_3  = 2,
    LIQUID_DETECT_CONFIG_4  = 3,
    LIQUID_DETECT_CONFIG_5  = 4,
    LIQUID_DETECT_CONFIG_6  = 5,
    LIQUID_DETECT_CONFIG_7  = 6,
    LIQUID_DETECT_CONFIG_8  = 7,
    LIQUID_DETECT_CONFIG_9  = 8,
    LIQUID_DETECT_CONFIG_10 = 9,
    LIQUID_DETECT_CONFIG_MAX = LIQUID_DETECT_CONFIG_10,
}LIQUID_DETECT_CONFIG;

//探液结果
typedef enum LIQUID_FUNC_RESULT
{
    LIQUID_FUNC_RESULT_OK               = 0X00,//探液功能正常打开
    LIQUID_FUNC_RESULT_OPEN_FAIL        = 0X01,//探液功能打开失败,打开探液针信号一直为高
    LIQUID_FUNC_RESULT_BASE_50_V        = 0x20,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_45_50_V     = 0x21,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_40_45_V     = 0x22,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_35_40_V     = 0x23,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_30_35_V     = 0x24,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_29_30_V     = 0x25,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_20_21_V     = 0x26,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_15_20_V     = 0x27,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_10_15_V     = 0x28,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_5_10_V      = 0x29,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_0_5_V       = 0x2A,//液面探测基准电压异常
    LIQUID_FUNC_RESULT_BASE_0_V         = 0x2B,//液面探测基准电压异常
}LIQUID_FUNC_RESULT;

//探液级别
typedef enum LIQUID_DETECT_STAGE
{
    LIQUID_DETECT_STAGE_NONE    = 0X00,//无探液信息
    LIQUID_DETECT_STAGE_LARGE   = 0X01,//大液量
    LIQUID_DETECT_STAGE_SMALL   = 0X02,//小液量
}LIQUID_DETECT_STAGE;

//移液针液面探测结果
typedef enum LIQUID_NEEDLE_LIQUID_DETECT_RESULT
{
    LIQUID_NEEDLE_LIQUID_DETECT_FAILED   = 0X00, //移液针液面探测失败
    LIQUID_NEEDLE_LIQUID_DETECT_SUCCESS  = 0X01, //移液针液面探测成功
}LIQUID_NEEDLE_LIQUID_DETECT_RESULT;

//用于区分调用IPC的源头
typedef enum CMD_SOURCE_TEST_FLOW
{
    CMD_SOURCE_TEST_FLOW_MAIN   = 0X00,//主流程调用
    CMD_SOURCE_TEST_FLOW_SUB    = 0X01,//子流程调用
}CMD_SOURCE_TEST_FLOW;

//探液段落
typedef enum LIQUID_DETECT_SEGMENT
{
    LIQUID_DETECT_SEGMENT_HIGH          = 0X00,//高
    LIQUID_DETECT_SEGMENT_LOW           = 0x01,//低
}LIQUID_DETECT_SEGMENT;

#endif















