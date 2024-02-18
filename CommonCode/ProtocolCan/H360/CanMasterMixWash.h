/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-24 13:10:38 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_MIX_WASH_H_
#define __CAN_MASTER_MIX_WASH_H_
#include "CanMasterH360Base.h"

//清洗旋转模块
typedef enum INDEX_MOTOR_MIX_WASH
{
    INDEX_MOTOR_MIX_WASH_UPDOWN = 0x00,
    INDEX_MOTOR_MIX_WASH_ROTATE = 0x01,
}INDEX_MOTOR_MIX_WASH;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterMixWashReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterMixWashWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixWashResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterMixWashRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//清洗混匀复位
void CanMasterMixWashResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀升起来
void CanMasterMixWashUpWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//清洗混匀降下去
void CanMasterMixWashDownWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀转一次
void CanMasterMixWashStartRotateWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//清洗混匀集成混匀一次
void CanMasterMixWashRotateIntegratedWhileAck(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterMixWashWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixWashWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixWashWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixWashWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗混匀复位
void CanMasterMixWashWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀升起来
void CanMasterMixWashWaitUpResult(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀降下去
void CanMasterMixWashWaitDownResult(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀转一次
void CanMasterMixWashWaitStartRotateResult(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀集成混匀一次
void CanMasterMixWashWaitRotateIntegratedResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterMixWashReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMixWashWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMixWashResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMixWashRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗混匀复位
void CanMasterMixWashResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀升起来
void CanMasterMixWashUpWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//清洗混匀降下去
void CanMasterMixWashDownWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗混匀转一次
void CanMasterMixWashStartRotateWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
//清洗混匀集成混匀一次
void CanMasterMixWashRotateIntegratedWhileReturn(MIX_MODE mixMode,SYS_RESULT_PACK* resultPackPtr);
























#endif


















