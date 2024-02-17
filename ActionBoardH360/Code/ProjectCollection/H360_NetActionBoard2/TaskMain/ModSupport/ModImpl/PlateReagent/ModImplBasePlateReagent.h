/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 11:53:33 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_PLATE_REAGENT_H_
#define __MOD_IMPL_BASE_PLATE_REAGENT_H_
#include "ModImplPlateReagentConfig.h"

//读取模块输入
void ModImplBasePlateReagentReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBasePlateReagentWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBasePlateReagentStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBasePlateReagentStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试剂盘复位
void ModImplBasePlateReagentReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试剂盘运行到指定序号杯位,带偏移
void ModImplBasePlateReagentRun2CupWithOffset(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                uint16_t* dataBufLen);

//试剂盘运行指定数量个杯位,基于当前位置
void ModImplBasePlateReagentRunAnyCups(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试剂盘运行到指定序号杯位,带偏移
void ModImplBasePlateReagentRun2CupWithOffsetForScan(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen);


#endif












