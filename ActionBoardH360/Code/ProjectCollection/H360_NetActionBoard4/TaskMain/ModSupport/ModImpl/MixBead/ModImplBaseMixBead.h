/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 13:16:26 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_MIX_BEAD_H_
#define __MOD_IMPL_BASE_MIX_BEAD_H_
#include "ModImplMixBeadConfig.h"


//读取模块输入
void ModImplBaseMixBeadReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseMixBeadWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseMixBeadStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseMixBeadStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//复位
void ModImplBaseMixBeadReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀升起来
void ModImplBaseMixBeadUp(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀降下去
void ModImplBaseMixBeadDown(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗混匀转一次
void ModImplBaseMixBeadStartRotate(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//集成清洗混匀
void ModImplBaseMixBeadRotateIntegrated(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);





#endif






















