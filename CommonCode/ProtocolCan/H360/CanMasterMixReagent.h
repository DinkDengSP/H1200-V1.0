/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-24 13:40:11 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_MIX_REAGENT_H_
#define __CAN_MASTER_MIX_REAGENT_H_
#include "CanMasterH360Base.h"

//试剂混匀模块
typedef enum INDEX_MOTOR_MIX_REAGENT
{
    INDEX_MOTOR_MIX_REAGENT_UPDOWN = 0x00,//升降
    INDEX_MOTOR_MIX_REAGENT_ROTATE = 0x01,//旋转
}INDEX_MOTOR_MIX_REAGENT;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterMixReagentReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterMixReagentWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixReagentResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterMixReagentRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//试剂混匀复位
void CanMasterMixReagentResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀升起来
void CanMasterMixReagentUpWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//试剂混匀降下去
void CanMasterMixReagentDownWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀转一次
void CanMasterMixReagentStartRotateWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//试剂混匀集成混匀一次
void CanMasterMixReagentRotateIntegratedWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterMixReagentWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixReagentWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixReagentWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixReagentWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂混匀复位
void CanMasterMixReagentWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀升起来
void CanMasterMixReagentWaitUpResult(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀降下去
void CanMasterMixReagentWaitDownResult(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀转一次
void CanMasterMixReagentWaitStartRotateResult(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀集成混匀一次
void CanMasterMixReagentWaitRotateIntegratedResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterMixReagentReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixReagentWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixReagentResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixReagentRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂混匀复位
void CanMasterMixReagentResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀升起来
void CanMasterMixReagentUpWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//试剂混匀降下去
void CanMasterMixReagentDownWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//试剂混匀转一次
void CanMasterMixReagentStartRotateWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//试剂混匀集成混匀一次
void CanMasterMixReagentRotateIntegratedWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
























#endif



















