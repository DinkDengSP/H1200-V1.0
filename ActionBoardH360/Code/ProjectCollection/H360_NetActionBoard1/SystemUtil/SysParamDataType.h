/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 17:56:39 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555A5

//清洗盘参数
typedef struct SYS_PARAM_PLATE_WASH
{
    int32_t resetCorrect;//清洗盘零位修正
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_PLATE_WASH;
//参数长度
#define LENGTH_SYS_PARAM_PLATE_WASH                 DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_PLATE_WASH)

//清洗摇匀参数
typedef struct SYS_PARAM_MIX_WASH
{
    int32_t resetCorrectUpDown;//清洗摇匀升降零位修正
    int32_t resetCorrectRotate;//清洗摇匀旋转零位修正
    int32_t upPosMode1;//清洗摇匀模式1升降高度
    int32_t rotateTimeMsMode1;//清洗摇匀模式1摇匀时间,毫秒
    int32_t upPosMode2;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode2;//清洗摇匀模式2摇匀时间,毫秒
    int32_t upPosMode3;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode3;//清洗摇匀模式3摇匀时间,毫秒
    int32_t upPosMode4;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode4;//清洗摇匀模式4摇匀时间,毫秒
    int32_t upPosMode5;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode5;//清洗摇匀模式5摇匀时间,毫秒
    int32_t upPosMode6;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode6;//清洗摇匀模式6摇匀时间,毫秒
    int32_t upPosMode7;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode7;//清洗摇匀模式7摇匀时间,毫秒
    int32_t upPosMode8;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode8;//清洗摇匀模式8摇匀时间,毫秒
    int32_t upPosMode9;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode9;//清洗摇匀模式9摇匀时间,毫秒
    int32_t upPosMode10;//清洗摇匀模式2升降高度
    int32_t rotateTimeMsMode10;//清洗摇匀模式10摇匀时间,毫秒
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_MIX_WASH;
//参数长度
#define LENGTH_SYS_PARAM_MIX_WASH                   DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_MIX_WASH)

//清洗针参数
typedef struct SYS_PARAM_NEEDLE_WASH
{
    int32_t resetCorrectUpDown;//清洗针升降零位修正
    int32_t posCupTopNeedleUpDown;//清洗针升降到杯口坐标
    int32_t posCupBottomNeedleUpDown;//清洗针升降到杯底坐标
    int32_t ulAbsorbWashPump1;//清洗针注射泵1吸液量,微升
    int32_t ulAbsorbWashPump2;//清洗针注射泵2吸液量,微升
    int32_t ulAbsorbWashPump3;//清洗针注射泵3吸液量,微升
    int32_t ulAbsorbWashPump4;//清洗针注射泵4吸液量,微升
    int32_t ulAbsorbWashPump5;//清洗针注射泵5吸液量,微升
    int32_t timeMsWaitWasteLiquidAtCupBottom;//清洗针在杯底等待抽废液时间,毫秒
    int32_t timeMsWaitInjectStableAtCupTop;//清洗针在杯口等待注液稳定时间,毫秒
    int32_t timeMsWaitWasteLiquidAtPrime;//清洗针灌注一次等抽废液完成时间,毫秒
    int32_t posUpdownToSoak;//清洗针升降到浸泡位
    int32_t maintainCleanTime;//维护清洗时间
    int32_t threeHoleNeedle2_5InjectDelayMs;//清洗针2-5杯底注液延时时间
    int32_t threeHoleNeedle1InjectWaitUpMs;//清洗针1注液等待针抬起时间 
    int32_t blockUpThrehold;//清洗针堵针阈值上限   
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_NEEDLE_WASH;
//参数长度
#define LENGTH_SYS_PARAM_NEEDLE_WASH                DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_NEEDLE_WASH)


//缓冲液注液参数
typedef struct SYS_PARAM_BUFFER_INJECT
{
    int32_t resetCorrectPump;//缓冲液注射泵零位修正
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_BUFFER_INJECT;
//参数长度
#define LENGTH_SYS_PARAM_BUFFER_INJECT              DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_BUFFER_INJECT)

//液路自动化参数
typedef struct SYS_PARAM_LIQUID_AUTO
{
    int32_t paramReserve;//预留参数,现在没用
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_LIQUID_AUTO;
//参数长度
#define LENGTH_SYS_PARAM_LIQUID_AUTO                DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_LIQUID_AUTO)

//其余辅助参数
typedef struct SYS_PARAM_OTHER
{
    int32_t baseReserve;//预留参数,现在没用
    int32_t reserveArray[50];//,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_OTHER;
//参数长度
#define LENGTH_SYS_PARAM_OTHER                      DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_LIQUID_AUTO)

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_PLATE_WASH paramPlateWash;//清洗盘参数
    SYS_PARAM_MIX_WASH paramMixWash;//清洗摇匀参数
    SYS_PARAM_NEEDLE_WASH paramNeedleWash;//清洗针参数
    SYS_PARAM_BUFFER_INJECT paramBufferInject;//缓冲液注液参数
    SYS_PARAM_LIQUID_AUTO paramLiquidAuto;//液路自动化参数
    SYS_PARAM_OTHER paramOther;//其余辅助参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM                            DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



