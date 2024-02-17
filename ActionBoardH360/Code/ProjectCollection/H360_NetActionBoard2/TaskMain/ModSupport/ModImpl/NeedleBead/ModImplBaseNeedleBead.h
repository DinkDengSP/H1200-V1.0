/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 11:08:17 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_NEEDLE_BEAD_H_
#define __MOD_IMPL_BASE_NEEDLE_BEAD_H_
#include "ModImplNeedleBeadConfig.h"

//读取模块输入
void ModImplBaseNeedleBeadReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseNeedleBeadWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseNeedleBeadStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseNeedleBeadStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针复位,带零位偏移 
void ModImplBaseNeedleBeadReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针清洗
void ModImplBaseNeedleBeadClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针灌注
void ModImplBaseNeedleBeadPrime(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针清洗维护
void ModImplBaseNeedleBeadMaintainClean(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针旋转指定位置
void ModImplBaseNeedleBeadRotate(NEEDLE_BEAD_POS targetPos,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针吸液
void ModImplBaseNeedleBeadLiquidAbsorb(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,SYS_RESULT_PACK* resultPackPtr,
                                            uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针注液
void ModImplBaseNeedleBeadLiquidInject(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针探液校准
void ModImplBaseNeedleBeadLiquidDetectAdjust(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读磁珠针探液板状态
void ModImplBaseNeedleBeadLiquidDetectReadState(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针读取针尖电阻和电压
void ModImplBaseNeedleBeadLiquidDetectReadResAndVol(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针写入探液板电阻
void ModImplBaseNeedleBeadLiquidDetectWriteRes(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针读取针尖基准电压
void ModImplBaseNeedleBeadLiquidDetectReadBaseVol(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针液量余量检查
void ModImplBaseNeedleBeadLiquidResidueCheck(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针升降急停高度读取
void ModImplBaseNeedleBeadReadNeedleUpDownEmStopPos(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//磁珠针小磁珠吸吐混匀
void ModImplBaseNeedleBeadLittleBeadMix(uint32_t mixLiquidUnit,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读取探液板版本号
void ModImplBaseNeedleBeadReadLiquidBoardVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif


















