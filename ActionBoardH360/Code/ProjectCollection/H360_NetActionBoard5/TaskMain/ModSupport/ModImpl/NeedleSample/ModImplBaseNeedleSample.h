/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-19 11:42:33 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_NEEDLE_SAMPLE_H_
#define __MOD_IMPL_BASE_NEEDLE_SAMPLE_H_
#include "ModImplNeedleSampleConfig.h"

//读取模块输入
void ModImplBaseNeedleSampleReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseNeedleSampleWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseNeedleSampleStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseNeedleSampleStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针复位
void ModImplBaseNeedleSampleReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针清洗液灌注
void ModImplBaseNeedleSamplePrimeNormal(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针强力清洗液灌注
void ModImplBaseNeedleSamplePrimePower(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针清洗
void ModImplBaseNeedleSampleClean(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen);

//样本针强力清洗
void ModImplBaseNeedleSampleCleanPower(uint8_t count,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针清洗维护
void ModImplBaseNeedleSampleMainTain(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);

//样本针旋转指定位置
void ModImplBaseNeedleSampleRotate(NEEDLE_SAMPLE_POS targetPos,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针吸试管样本
void ModImplBaseNeedleSampleLiquidAbsorbTube(uint32_t unitCount,int32_t lastLiquidDetectPos,TUBE_TYPE tubeType,SYS_RESULT_PACK* resultPackPtr,
                                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针吸反应盘样本
void ModImplBaseNeedleSampleLiquidAbsorbPlate(uint32_t unitCount,uint32_t cupLiquidResidueAmp1000,SYS_RESULT_PACK* resultPackPtr,
                                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//样本针注液
void ModImplBaseNeedleSampleInjectNormal(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                            uint16_t* dataBufLen);
                                            
//样本针探液校准
void ModImplBaseNeedleSampleLiquidDetectAdjust(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读样本针探液板状态
void ModImplBaseNeedleSampleLiquidDetectReadState(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen);

//读取电阻和针尖电压
void ModImplBaseNeedleSampleLiquidDetectReadResAndVol(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入电阻值
void ModImplBaseNeedleSampleLiquidDetectWriteRes(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                    uint16_t* dataBufLen);

//读取针尖基准电压
void ModImplBaseNeedleSampleLiquidDetectReadBaseVol(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读样本针升降急停高度
void ModImplBaseNeedleSampleReadNeedleUpDownEmStopPos(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//读探液板程序版本
void ModImplBaseNeedleSampleReadLiquidBoardVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

#endif


















