/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 08:38:56 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_LIQUID_NEEDLE_REAGENT_H_
#define __MOD_IMPL_BASE_LIQUID_NEEDLE_REAGENT_H_
#include "ModImplLiquidNeedleReagentConfig.h"

//读取模块输入
void ModImplBaseLiquidNeedleReagentReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseLiquidNeedleReagentWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseLiquidNeedleReagentStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseLiquidNeedleReagentStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//复位
void ModImplBaseLiquidNeedleReagentReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//打开清洗液路
void ModImplBaseLiquidNeedleReagentOpenClean(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//关闭清洗液路
void ModImplBaseLiquidNeedleReagentCloseClean(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//打开灌注液路
void ModImplBaseLiquidNeedleReagentOpenPrime(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//关闭灌注液路
void ModImplBaseLiquidNeedleReagentClosePrime(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//打开清洗维护液路
void ModImplBaseLiquidNeedleReagentOpenMaintain(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//关闭清洗维护液路
void ModImplBaseLiquidNeedleReagentCloseMaintain(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);





#endif



