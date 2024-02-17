/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 10:49:12 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_MIX_BEAD_H_
#define __CAN_MASTER_MIX_BEAD_H_
#include "CanMasterH360Base.h"

//磁珠混匀模块
typedef enum INDEX_MOTOR_MIX_BEAD
{
    INDEX_MOTOR_MIX_BEAD_UPDOWN = 0x00,//升降
    INDEX_MOTOR_MIX_BEAD_ROTATE = 0x01,//旋转
}INDEX_MOTOR_MIX_BEAD;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterMixBeadReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterMixBeadWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixBeadResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterMixBeadRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀复位
void CanMasterMixBeadResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀升起来
void CanMasterMixBeadUpWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀降下去
void CanMasterMixBeadDownWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀转一次
void CanMasterMixBeadStartRotateWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀集成混匀一次
void CanMasterMixBeadRotateIntegratedWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterMixBeadWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixBeadWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixBeadWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixBeadWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//磁珠混匀复位
void CanMasterMixBeadWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀升起来
void CanMasterMixBeadWaitUpResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀降下去
void CanMasterMixBeadWaitDownResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀转一次
void CanMasterMixBeadWaitStartRotateResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀集成混匀一次
void CanMasterMixBeadWaitRotateIntegratedResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterMixBeadReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixBeadWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixBeadResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixBeadRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//磁珠混匀复位
void CanMasterMixBeadResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀升起来
void CanMasterMixBeadUpWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀降下去
void CanMasterMixBeadDownWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀转一次
void CanMasterMixBeadStartRotateWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//磁珠混匀集成混匀一次
void CanMasterMixBeadRotateIntegratedWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
























#endif




















