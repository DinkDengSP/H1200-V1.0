/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 16:34:43 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

//反应盘参数
typedef struct SYS_PARAM_PLATE_REACTION
{
    int32_t resetCorrect;//反应盘零位修正
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_PLATE_REACTION;
//参数长度
#define LENGTH_SYS_PARAM_PLATE_REACTION                 DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_PLATE_REACTION)

//磁珠摇匀参数
typedef struct SYS_PARAM_MIX_BEAD
{
    int32_t resetCorrectUpDown;//磁珠摇匀升降零位修正
    int32_t resetCorrectRotate;//磁珠摇匀旋转零位修正
    int32_t upPosMode1;//磁珠摇匀模式1升降高度
    int32_t rotateTimeMsMode1;//磁珠摇匀模式1摇匀时间,毫秒
    int32_t upPosMode2;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode2;//磁珠摇匀模式2摇匀时间,毫秒
    int32_t upPosMode3;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode3;//磁珠摇匀模式3摇匀时间,毫秒
    int32_t upPosMode4;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode4;//磁珠摇匀模式4摇匀时间,毫秒
    int32_t upPosMode5;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode5;//磁珠摇匀模式5摇匀时间,毫秒
    int32_t upPosMode6;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode6;//磁珠摇匀模式6摇匀时间,毫秒
    int32_t upPosMode7;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode7;//磁珠摇匀模式7摇匀时间,毫秒
    int32_t upPosMode8;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode8;//磁珠摇匀模式8摇匀时间,毫秒
    int32_t upPosMode9;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode9;//磁珠摇匀模式9摇匀时间,毫秒
    int32_t upPosMode10;//磁珠摇匀模式2升降高度
    int32_t rotateTimeMsMode10;//磁珠摇匀模式10摇匀时间,毫秒
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_MIX_BEAD;
//参数长度
#define LENGTH_SYS_PARAM_MIX_BEAD                   DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_MIX_BEAD)

//试剂摇匀参数
typedef struct SYS_PARAM_MIX_REAGENT
{
    int32_t resetCorrectUpDown;//试剂摇匀升降零位修正
    int32_t resetCorrectRotate;//试剂摇匀旋转零位修正
    int32_t upPosMode1;//试剂摇匀模式1升降高度
    int32_t rotateTimeMsMode1;//试剂摇匀模式1摇匀时间,毫秒
    int32_t upPosMode2;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode2;//试剂摇匀模式2摇匀时间,毫秒
    int32_t upPosMode3;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode3;//试剂摇匀模式3摇匀时间,毫秒
    int32_t upPosMode4;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode4;//试剂摇匀模式4摇匀时间,毫秒
    int32_t upPosMode5;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode5;//试剂摇匀模式5摇匀时间,毫秒
    int32_t upPosMode6;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode6;//试剂摇匀模式6摇匀时间,毫秒
    int32_t upPosMode7;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode7;//试剂摇匀模式7摇匀时间,毫秒
    int32_t upPosMode8;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode8;//试剂摇匀模式8摇匀时间,毫秒
    int32_t upPosMode9;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode9;//试剂摇匀模式9摇匀时间,毫秒
    int32_t upPosMode10;//试剂摇匀模式2升降高度
    int32_t rotateTimeMsMode10;//试剂摇匀模式10摇匀时间,毫秒
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_MIX_REAGENT;
//参数长度
#define LENGTH_SYS_PARAM_MIX_REAGENT                   DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_MIX_REAGENT)

//试剂针参数
typedef struct SYS_PARAM_NEEDLE_REAGENT
{
    int32_t rotateResetCorrect;//试剂针旋转零位修正
    int32_t upDownResetCorrect;//试剂针升降零位修正
    int32_t posRotateToClean;//试剂针旋转到清洗位坐标
    int32_t posUpDownClean;//试剂针升降到清洗位坐标
    int32_t posRotateToR1;//试剂针旋转到R1坐标
    int32_t posUpDownR1LiquidDetectMaxLimit;//试剂针升降R1吸样探液极限点
    int32_t posRotateToR2;//试剂针旋转到R2坐标
    int32_t posUpDownR2LiquidDetectMaxLimit;//试剂针升降R2吸样探液极限点
    int32_t posRotateToPlateReaction;//试剂针旋转到反应盘排样坐标
    int32_t posUpDownInjectCupBottom;//试剂针升降反应盘排样杯底高度
    int32_t correctionUpDownInject;//试剂针升降反应盘排样修正高度
    int32_t ulAbsorbAirAfterClean;//试剂针清洗完吸空气量
    int32_t ulAbsorbAirAfterLiquidAbsorb;//试剂针吸样完吸空气量
    int32_t ulReserveAirAfterInject;//试剂针注液保留空气量
    int32_t liquidDetectRealTimeComm;//试剂针探液通讯使能 
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_NEEDLE_REAGENT;
//参数长度
#define LENGTH_SYS_PARAM_NEEDLE_REAGENT                DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_NEEDLE_REAGENT)

//其余辅助参数
typedef struct SYS_PARAM_OTHER
{
    int32_t baseReserve;//预留参数,现在没用
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_OTHER;
//参数长度
#define LENGTH_SYS_PARAM_OTHER                      DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_OTHER)

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_PLATE_REACTION paramPlateReaction;//反应盘
    SYS_PARAM_MIX_BEAD paramMixBead;//磁珠混匀
    SYS_PARAM_MIX_REAGENT paramMixReagent;//试剂混匀
    SYS_PARAM_NEEDLE_REAGENT paramNeedleReagent;//试剂针
    SYS_PARAM_OTHER paramOther;//其余辅助参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM                                DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



