/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 13:19:30 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_PLATE_REACTION_H_
#define __MOD_IMPL_BASE_PLATE_REACTION_H_
#include "ModImplPlateReactionConfig.h"

//读取模块输入
void ModImplBasePlateReactionReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBasePlateReactionWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBasePlateReactionStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBasePlateReactionStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//复位
void ModImplBasePlateReactionReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//正向运转到指定杯位
void ModImplBasePlateReactionRunIndexCupPositive(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//正向运转指定数量杯位
void ModImplBasePlateReactionRunAnyCupsPositive(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//反向运转到指定杯位
void ModImplBasePlateReactionRunIndexCupNegative(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//反向运转指定数量杯位
void ModImplBasePlateReactionRunAnyCupsNegative(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);





#endif






















