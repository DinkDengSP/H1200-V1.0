/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-24 13:10:38 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_NEEDLE_BEAD_H_
#define __CAN_MASTER_LIQUID_NEEDLE_BEAD_H_
#include "CanMasterH360Base.h"



/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidNeedleBeadReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidNeedleBeadWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleBeadResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidNeedleBeadRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//复位
void CanMasterLiquidNeedleBeadResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleBeadOpenCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleBeadCloseCleanWhileAck(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleBeadOpenPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleBeadClosePrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleBeadOpenMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleBeadCloseMaintainWhileAck(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleBeadWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleBeadWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleBeadWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleBeadWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleBeadWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleBeadWaitOpenCleanResult(SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleBeadWaitCloseCleanResult(SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleBeadWaitOpenPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleBeadWaitClosePrimeResult(SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleBeadWaitOpenMaintainResult(SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleBeadWaitCloseMaintainResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidNeedleBeadReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidNeedleBeadWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidNeedleBeadResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidNeedleBeadRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//复位
void CanMasterLiquidNeedleBeadResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开清洗液路
void CanMasterLiquidNeedleBeadOpenCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗液路
void CanMasterLiquidNeedleBeadCloseCleanWhileReturn(NEEDLE_CLEAN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开灌注液路
void CanMasterLiquidNeedleBeadOpenPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭灌注液路
void CanMasterLiquidNeedleBeadClosePrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//打开清洗维护液路
void CanMasterLiquidNeedleBeadOpenMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//关闭清洗维护液路
void CanMasterLiquidNeedleBeadCloseMaintainWhileReturn(NEEDLE_MAINTAIN_OPT opt,SYS_RESULT_PACK* resultPackPtr);























#endif


















