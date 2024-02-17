/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 11:40:42 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_MEASURE_MODULE_H_
#define __MOD_IMPL_BASE_MEASURE_MODULE_H_
#include "ModImplMeasureModuleConfig.h"

//读取模块输入
void ModImplBaseMeasureModuleReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseMeasureModuleWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseMeasureModuleStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseMeasureModuleStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块状态机初始化
void ModImplBaseMeasureModuleFSM_Init(void);

//测量模块状态机周期运转
void ModImplBaseMeasureModuleFSM_RunPeriod(void);

//测量模块复位
void ModImplBaseMeasureModuleReset(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量上门全开
void ModImplBaseMeasureModuleDoorOpenFull(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量上门半开
void ModImplBaseMeasureModuleDoorOpenHalf(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量上门关闭
void ModImplBaseMeasureModuleDoorClose(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量侧门打开
void ModImplBaseMeasureModuleWindowOpen(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量侧门关闭
void ModImplBaseMeasureModuleWindowClose(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//激发液灌注
void ModImplBaseMeasureModulePrimeOnce(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//激发液注液
void ModImplBaseMeasureModuleInjectOnce(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块清洗维护
void ModImplBaseMeasureModuleInjectMaintain(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块暗计数
void ModImplBaseMeasureModuleReadDark(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块读本底
void ModImplBaseMeasureModuleBackGround(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块注液测光
void ModImplBaseMeasureModuleReadLightInject(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块集成单次测量
void ModImplBaseMeasureModuleReadLightComplex(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,SYS_RESULT_PACK* resultPackPtr,
                                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//测量模块读光子变化率
void ModImplBaseMeasureModuleReadLightRate(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                uint16_t* dataBufLen);

#endif












