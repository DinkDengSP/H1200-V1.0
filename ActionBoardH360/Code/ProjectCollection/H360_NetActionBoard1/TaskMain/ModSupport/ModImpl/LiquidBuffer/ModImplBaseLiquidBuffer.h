/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 23:45:56 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_LIQUID_BUFFER_H_
#define __MOD_IMPL_BASE_LIQUID_BUFFER_H_
#include "ModImplLiquidBufferConfig.h"

//读取模块输入
void ModImplBaseLiquidBufferReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseLiquidBufferWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseLiquidBufferStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseLiquidBufferStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//缓冲液泵阀复位
void ModImplBaseLiquidBufferReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//缓冲液灌注
void ModImplBaseLiquidBufferPrime(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//缓冲液注液
void ModImplBaseLiquidBufferInject(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//缓冲液清洗维护
void ModImplBaseLiquidBufferMaintain(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//初始化状态机
void ModImplBaseLiquidBufferFSM_Init(void);

#endif



