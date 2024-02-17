/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 15:09:16 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-28 11:53:43 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_LIQUID_AUTO_H_
#define __MOD_IMPL_BASE_LIQUID_AUTO_H_
#include "ModImplLiquidAutoConfig.h"

//初始化纯水叶轮脉冲检测
void ModImplBaseLiquidAutoRegisterPureWaterTrigSensor(void);

//液路自动化状态机初始化
void ModImplBaseLiquidAutoFSM_Init(void);

//液路状态机周期自行
void ModImplBaseLiquidAutoFSM_RunPeriod(void);

//读取模块输入
void ModImplBaseLiquidAutoReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseLiquidAutoWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseLiquidAutoStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseLiquidAutoStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//初始化
void ModImplBaseLiquidAutoInit(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//强制排一次废液
void ModImplBaseLiquidAutoForceWasteClearOnce(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//自动排废液打开
void ModImplBaseLiquidAutoEnableAutoWasteClear(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//自动排废液关闭
void ModImplBaseLiquidAutoDisableAutoWasteClear(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//查看排废液状态
void ModImplBaseLiquidAutoCheckAutoWasteClearState(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//关闭稀释
void ModImplBaseLiquidAutoCloseDilution(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//打开稀释
void ModImplBaseLiquidAutoOpenDilution(DILUTION_CH setChannel,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读取自动稀释纯水脉冲数
void ModImplBaseLiquidAutoGetLiquidDilutionPureWaterPulse(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//清除自动稀释纯水脉冲数
void ModImplBaseLiquidAutoResetLiquidDilutionPureWaterPulse(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读取自动稀释纯水触发标记
void ModImplBaseLiquidAutoGetLiquidDilutionPureWaterTrigFlag(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//注册自动稀释纯水脉冲数
void ModImplBaseLiquidAutoRegisterLiquidDilutionPureWaterPulse(uint32_t trigCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif



