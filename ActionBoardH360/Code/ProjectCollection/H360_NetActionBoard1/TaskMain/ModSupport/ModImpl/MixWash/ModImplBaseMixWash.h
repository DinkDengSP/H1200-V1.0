/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 09:22:41 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_MIX_WASH_H_
#define __MOD_IMPL_BASE_MIX_WASH_H_
#include "ModImplMixWashConfig.h"

//读取模块输入
void ModImplBaseMixWashReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseMixWashWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseMixWashStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseMixWashStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//复位
void ModImplBaseMixWashReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀升起来
void ModImplBaseMixWashUp(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀降下去
void ModImplBaseMixWashDown(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀转一次
void ModImplBaseMixWashStartRotate(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//集成清洗混匀
void ModImplBaseMixWashRotateIntegrated(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);



#endif



