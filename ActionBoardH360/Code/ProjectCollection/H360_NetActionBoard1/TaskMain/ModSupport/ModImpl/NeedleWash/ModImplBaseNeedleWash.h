/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-05 13:20:25 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_NEEDLE_WASH_H_
#define __MOD_IMPL_BASE_NEEDLE_WASH_H_
#include "ModImplNeedleWashConfig.h"

//读取模块输入
void ModImplBaseNeedleWashReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseNeedleWashWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseNeedleWashStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseNeedleWashStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针复位
void ModImplBaseNeedleWashReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针灌注
void ModImplBaseNeedleWashPrime(uint8_t primeCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针维护
void ModImplBaseNeedleWashMaintain(uint8_t maintainCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针磁珠清洗一次
void ModImplBaseNeedleWashBeadClean(uint8_t beadCleanConfig,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针下降到浸泡位
void ModImplBaseNeedleWashUpDownToSock(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针阻塞清洗
void ModImplBaseNeedleWashBlockMaintainClean(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清洗针废液泵抽一次
void ModImplBaseNeedleWashPumpAbsorbOnce(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif



