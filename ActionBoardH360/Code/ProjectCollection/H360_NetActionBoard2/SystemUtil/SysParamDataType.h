/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 15:11:16 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555A5

//试剂盘参数
typedef struct SYS_PARAM_PLATE_REAGENT
{
    int32_t rotateResetCorrect;//试剂盘零位修正
    int32_t offsetNeedleReagentWithR1;//试剂盘相对试剂针到R1偏移
    int32_t offsetNeedleReagentWithR2;//试剂盘相对试剂针到R2偏移
    int32_t offsetNeedleBeadWithR1;//试剂盘相对磁珠针到R1偏移
    int32_t offsetNeedleBeadWithR2;//试剂盘相对磁珠针到R2偏移
    int32_t offsetNeedleBeadWithBead;//试剂盘相对磁珠针到磁珠偏移
    int32_t offsetWithQRScan;//试剂盘到扫码位偏移
    int32_t reserveArray[50];//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位
}SYS_PARAM_PLATE_REAGENT;
//参数长度
#define LENGTH_SYS_PARAM_PLATE_REAGENT     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_PLATE_REAGENT)

//杯栈管理参数
typedef struct SYS_PARAM_CUP_TRAY_MANAGE
{
    int32_t newCupTrayStackResetCorrect;//新杯盘升降电机零位修正
    int32_t emptyCupTrayStackResetCorrect;//空杯盘升降电机零位修正
    int32_t clampResetCorrect;//夹手电机零位修正
    int32_t pushResetCorrect;//推杆电机零位修正
    int32_t posClampOpen;//夹手打开坐标
    int32_t posClampClose;//夹手关闭坐标
    int32_t posPushTrayToTest;//推杆推新盘到测试位坐标
    int32_t offsetTopToSensorWhenNewCupTrayFull;//新杯栈满时,顶部到检测位偏移
    int32_t offsetTopToPaltformWhenNewCupTrayFull;//新杯栈满时,顶部到平台偏移
    int32_t spacingLayerNewCupTray;//新杯栈上升一个杯盘的偏移
    int32_t offsetTopToSensorWhenEmptyCupTrayFull;//空杯栈满时,顶部到检测位偏移
    int32_t offsetTopToPaltformWhenEmptyCupTrayFull;//空杯栈满时,顶部到承接平台偏移
    int32_t spacingLayerEmptyCupTray;//空杯栈上升一个杯盘的偏移
    int32_t reserveArray[50];//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位
}SYS_PARAM_CUP_TRAY_MANAGE;
//参数长度
#define LENGTH_SYS_PARAM_CUP_TRAY_MANAGE     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_CUP_TRAY_MANAGE) 

//磁珠针参数
typedef struct SYS_PARAM_NEEDLE_BEAD
{
    int32_t rotateResetCorrect;//磁珠针旋转零位修正
    int32_t upDownResetCorrect;//磁珠针升降零位修正
    int32_t posRotateToClean;//磁珠针旋转到清洗位坐标
    int32_t posUpDownClean;//磁珠针升降到清洗位坐标
    int32_t posRotateToR1;//磁珠针旋转到R1坐标
    int32_t posUpDownR1LiquidDetectMaxLimit;//磁珠针升降R1吸样探液极限点
    int32_t posRotateToR2;//磁珠针旋转到R2坐标
    int32_t posUpDownR2LiquidDetectMaxLimit;//磁珠针升降R2吸样探液极限点
    int32_t posRotateToBead;//磁珠针旋转到磁珠位坐标
    int32_t posUpDownBeadLiquidDetectMaxLimit;//磁珠针升降磁珠位吸样探液极限点
    int32_t posRotateToPlateReaction;//磁珠针旋转到反应盘排样坐标
    int32_t posUpDownInjectAtButtom;//磁珠针升降反应盘排样杯底高度
    int32_t posUpDownInjectCorrect;//磁珠针升降反应盘排样修正高度
    int32_t ulAbsorbAirAfterClean;//磁珠针清洗完吸空气量
    int32_t ulAbsorbAirAfterLiquidAbsorb;//磁珠针吸样完吸空气量
    int32_t ulReserveAirAfterInject;//磁珠针注液保留空气量
    int32_t liquidDetectRealTimeComm;//磁珠针探液通讯使能
    int32_t posUpDownLittleBeadMix;//小磁针吸吐混匀高度
    int32_t reserveArray[50];//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位
}SYS_PARAM_NEEDLE_BEAD;
//参数长度
#define LENGTH_SYS_PARAM_NEEDLE_BEAD     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_NEEDLE_BEAD)

//辅助参数
typedef struct SYS_PARAM_OTHER
{
    int32_t util;//辅助参数
    int32_t reserveArray[50];//保留的参数,程序稳定后增加参数就从这个数组大小中减少参数,防止参数错位
}SYS_PARAM_OTHER;
//参数长度
#define LENGTH_SYS_PARAM_OTHER     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM_OTHER)  

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
    SYS_PARAM_PLATE_REAGENT plateReagentParam;//试剂盘参数
    SYS_PARAM_CUP_TRAY_MANAGE cupTrayManageParam;//杯栈管理参数
    SYS_PARAM_NEEDLE_BEAD needleBeadParam;//磁珠针参数
    SYS_PARAM_OTHER otherUtilParam;//辅助参数
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif



