/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-24 13:10:38 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_NEEDLE_REAGENT_H_
#define __CAN_MASTER_LIQUID_NEEDLE_REAGENT_H_
#include "CanMasterH360Base.h"


/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidNeedleReagentReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidNeedleReagentWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleReagentResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidNeedleReagentRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//复位
void CanMasterLiquidNeedleReagentResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleReagentOpenCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleReagentCloseCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleReagentOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleReagentClosePrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleReagentOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleReagentCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleReagentWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleReagentWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleReagentWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleReagentWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleReagentWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleReagentWaitOpenCleanResult(SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleReagentWaitCloseCleanResult(SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleReagentWaitOpenPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleReagentWaitClosePrimeResult(SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleReagentWaitOpenMaintainResult(SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleReagentWaitCloseMaintainResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleReagentReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleReagentWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleReagentResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleReagentRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleReagentResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleReagentOpenCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleReagentCloseCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleReagentOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleReagentClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleReagentOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleReagentCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);


























#endif






















