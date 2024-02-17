/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 19:29:40 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_NEEDLE_SAMPLE_H_
#define __CAN_MASTER_LIQUID_NEEDLE_SAMPLE_H_
#include "CanMasterH360Base.h"



/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidNeedleSampleReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidNeedleSampleWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleSampleResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidNeedleSampleRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//复位
void CanMasterLiquidNeedleSampleResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开常规清洗液路
void CanMasterLiquidNeedleSampleOpenNormalCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭常规清洗液路
void CanMasterLiquidNeedleSampleCloseNormalCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开强力清洗液路
void CanMasterLiquidNeedleSampleOpenPowerCleanWhileAck(SYS_RESULT_PACK* resultPackPtr);
//关闭强力清洗液路
void CanMasterLiquidNeedleSampleClosePowerCleanWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleSampleOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleSampleClosePrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleSampleOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleSampleCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleSampleWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleSampleWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleSampleWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleSampleWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleSampleWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//打开常规清洗液路
void CanMasterLiquidNeedleSampleWaitOpenNormalCleanResult(SYS_RESULT_PACK* resultPackPtr);
//关闭常规清洗液路
void CanMasterLiquidNeedleSampleWaitCloseNormalCleanResult(SYS_RESULT_PACK* resultPackPtr);
//打开强力清洗液路
void CanMasterLiquidNeedleSampleWaitOpenPowerCleanResult(SYS_RESULT_PACK* resultPackPtr);
//关闭强力清洗液路
void CanMasterLiquidNeedleSampleWaitClosePowerCleanResult(SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleSampleWaitOpenPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleSampleWaitClosePrimeResult(SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleSampleWaitOpenMaintainResult(SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleSampleWaitCloseMaintainResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleSampleReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleSampleWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleSampleResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleSampleRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleSampleResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开常规清洗液路
void CanMasterLiquidNeedleSampleOpenNormalCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭常规清洗液路
void CanMasterLiquidNeedleSampleCloseNormalCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开强力清洗液路
void CanMasterLiquidNeedleSampleOpenPowerCleanWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//关闭强力清洗液路
void CanMasterLiquidNeedleSampleClosePowerCleanWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleSampleOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleSampleClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleSampleOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleSampleCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);






#endif


















