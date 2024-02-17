/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 13:29:26 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

//清洗机械手参数
typedef struct SYS_PARAM_HAND_WASH
{
    int32_t updownResetCorrect;//清洗机械手升降零位修正
    int32_t rotateResetCorrect;//清洗机械手旋转零位修正
    int32_t posRotatePlateReaction;//清洗机械手旋转到反应盘坐标
    int32_t posUpdownCatchCupPlateReaction;//清洗机械手升降反应盘取杯坐标
    int32_t posUpdownPutCupPlateReaction;//清洗机械手升降反应盘放杯坐标
    int32_t posRotatePlateWash;//清洗机械手旋转到清洗盘坐标
    int32_t posUpdownCatchCupPlateWash;//清洗机械手升降清洗盘取杯坐标
    int32_t posUpdownPutCupPlateWash;//清洗机械手升降清洗盘放杯坐标
    int32_t posRotateMeasureUnit;//清洗机械手旋转到测量模块坐标
    int32_t posUpdownCatchCupOnMeasureUnitUnit;//清洗机械手升降在测量模块取杯坐标
    int32_t posUpdownPutCupOnMeasureUnitUnit;//清洗机械手升降在测量模块放杯坐标
    int32_t updownCatchMeaPosCheckEnable;//清洗机械手测量室取杯位置检测是否使能
    int32_t updownCatchMeaCorrect;//清洗机械手测量室取杯升降修正
    int32_t resverse[50];       //保留参数
}SYS_PARAM_HAND_WASH;
#define LENGTH_SYS_PARAM_HAND_WASH                  DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_HAND_WASH)

//测量室参数
typedef struct SYS_PARAM_MEASURE_MODULE
{
    int32_t doorResetCorrect;//测量上门零位修正
    int32_t windowResetCorrect;//测量侧门零位修正
    int32_t pumpResetCorrect;//激发液注射泵零位修正
    int32_t posDoorClose;//测量上门关闭坐标
    int32_t posDoorOpenHalf;//测量上门半开坐标
    int32_t posDoorOpenFull;//测量上门全开坐标
    int32_t posWindowClose;//测量侧门关闭坐标
    int32_t posWindowOpen;//测量侧门打开坐标
    int32_t lightLinearCorrectFactorA;//光子测量修正函数ax+b的a(放大1000倍)
    int32_t lightLinearCorrectFactorB;//光子测量修正函数ax+b的b(放大1000倍)
    int32_t resverse[50];       //保留参数
}SYS_PARAM_MEASURE_MODULE;
#define LENGTH_SYS_PARAM_MEASURE_MODULE             DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_MEASURE_MODULE)

//辅助参数
typedef struct SYS_PARAM_OTHER
{
    int32_t util;//辅助参数
    int32_t reserveArray[50];//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位
}SYS_PARAM_OTHER;
//参数长度
#define LENGTH_SYS_PARAM_OTHER                      DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_OTHER)  

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_HAND_WASH handWashParam;//清洗机械手参数
    SYS_PARAM_MEASURE_MODULE measureModuleParam;//测量室参数
    SYS_PARAM_OTHER otherUtilParam;//辅助参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



