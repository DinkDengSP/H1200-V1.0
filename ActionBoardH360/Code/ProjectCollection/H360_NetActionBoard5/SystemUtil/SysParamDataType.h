/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-21 10:54:33 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

//新杯机械手参数 集合
typedef struct SYS_PARAM_HAND_NEW_CUP
{
    int32_t resetCorrectUpDown;//新杯机械手升降零位修正
    int32_t resetCorrectLeftRight;//新杯机械手左右零位修正
    int32_t resetCorrectFrontBack;//新杯机械手前后零位修正
    int32_t posNewCupTrayFrontBackHoleNo1;//新杯机械手前后新杯盘1号孔坐标
    int32_t posNewCupTrayLeftRightHoleNo1;//新杯机械手左右新杯盘1号孔坐标
    int32_t posNewCupTrayFrontBackHoleNo140;//新杯机械手前后新杯盘140号孔坐标
    int32_t posNewCupTrayLeftRightHoleNo140;//新杯机械手左右新杯盘140号孔坐标
    int32_t posNewCupTrayUpDownCatchCup;//新杯机械手升降新杯盘取杯坐标
    int32_t posNewCupTrayFrontBackReaction;//新杯机械手前后反应盘坐标
    int32_t posNewCupTrayLeftRightReaction;//新杯机械手左右反应盘坐标
    int32_t posPlateReactionUpDownCatchCup;//新杯机械手升降反应盘取杯坐标
    int32_t posPlateReactionUpDownPutCup;//新杯机械手升降反应盘放杯坐标
    int32_t posFrontBackGarbage1;//新杯机械手前后垃圾桶1坐标
    int32_t posLeftRightGarbage1;//新杯机械手左右垃圾桶1坐标
    int32_t posUpDownPutCupGarbage1;//新杯机械手升降垃圾桶1丢杯坐标
    int32_t posFrontBackGarbage2;//新杯机械手前后垃圾桶2坐标
    int32_t posLeftRightGarbage2;//新杯机械手左右垃圾桶2坐标
    int32_t posUpDownPutCupGarbage2;//新杯机械手升降垃圾桶2丢杯坐标
    int32_t resverse[50];       //保留参数
}SYS_PARAM_HAND_NEW_CUP;
#define LENGTH_SYS_PARAM_HAND_NEW_CUP           DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_HAND_NEW_CUP)

//样本针参数 集合
typedef struct SYS_PARAM_NEEDLE_SAMPLE
{
    int32_t resetCorrectRotate;//样本针旋转零位修正
    int32_t resetCorrectUpDown;//样本针升降零位修正
    int32_t posRotatePowerfulClean;//样本针旋转到强洗位坐标
    int32_t posUpDownPowerfulClean;//样本针升降到强洗位坐标
    int32_t timeMsPowerfulClean;//样本针强洗时间,置换强洗液,毫秒
    int32_t timeMsPrimePowerfulClean;//样本针强洗液灌注时间,毫秒
    int32_t posRotateClean;//样本针旋转到清洗位
    int32_t posUpDownClean;//样本针升降到清洗位坐标
    int32_t posRotateSampleRack;//样本针旋转到试管架吸样位坐标
    int32_t posUpDownSampleRackFlat;//样本针吸液样本架平面高度
    int32_t posUpDownSampleRackLiquidDetectStartTubeType4;//样本针吸样探液起始点,试管类型4
    int32_t posUpDownSampleRackLiquidDetectMaxLimitTubeType4;//样本针吸样探液极限点,试管类型4 子弹头
    int32_t posRotatePlateReactionAbsorb;//样本针旋转到反应盘吸液坐标
    int32_t posUpDownPlateReactionAborbAtBottom;//样本针升降反应盘吸液杯底高度
    int32_t posUpDownPlateReactionAborbCorrect;//样本针升降反应盘吸液修正高度
    int32_t posRotatePlateReactionInject;//样本针旋转到反应盘注液坐标
    int32_t posUpDownPlateReactioninjectAtBottom;//样本针升降到反应盘注液杯底高度
    int32_t posUpDownPlateReactioninjectCorrect;//样本针升降到反应盘注液修正高度
    int32_t ulAbsorbAirAfterClean;//样本针清洗完吸空气量
    int32_t ulAbsorbAirAfterLiquidAbsorb;//样本针吸样完吸空气量
    int32_t ulReserveAirAfterInject ;//样本针注液保留空气量
    int32_t liquidDetectRealTimeComm ;//样本针探液通讯使能
    int32_t posRotateEmergency;//样本针旋转到急诊吸样位
    int32_t pumpVersion;//样本针柱塞泵版本，0：250ul2000步，1：250ul4000步
    int32_t resverse[50];       //保留参数
}SYS_PARAM_NEEDLE_SAMPLE;
#define LENGTH_SYS_PARAM_NEEDLE_SAMPLE          DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_NEEDLE_SAMPLE)

//其余辅助参数
typedef struct SYS_PARAM_OTHER
{
    int32_t baseReserve;//预留参数,现在没用
    int32_t reserveArray[50];//保留的参数,程序稳定后每次增加参数都要从这个里面减少一个参数
}SYS_PARAM_OTHER;
//参数长度
#define LENGTH_SYS_PARAM_OTHER                  DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_OTHER)

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_HAND_NEW_CUP paramHandNewCup;//清洗机械手参数
    SYS_PARAM_NEEDLE_SAMPLE paramNeedleSample;//样本针参数
    SYS_PARAM_OTHER otherParam;//其余保留参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



