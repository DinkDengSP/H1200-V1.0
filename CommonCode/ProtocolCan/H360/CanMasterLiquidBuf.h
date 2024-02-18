/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-25 14:10:54 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_BUF_H_
#define __CAN_MASTER_LIQUID_BUF_H_
#include "CanMasterH360Base.h"

//步进电机序号
typedef enum INDEX_MOTOR_BUFFER_SOLUTION
{
    INDEX_MOTOR_BUFFER_SOLUTION_PUMP      = 0x00,//注射泵
}INDEX_MOTOR_BUFFER_SOLUTION;

//注液端口选择
typedef enum BOTTLE_BUFFER_SOLUTION
{
    BOTTLE_BUFFER_SOLUTION_A = 0X00,//缓冲液A瓶
    BOTTLE_BUFFER_SOLUTION_B = 0X01,//缓冲液B瓶
}BOTTLE_BUFFER_SOLUTION;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidBufReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidBufWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidBufResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidBufRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//缓冲液泵阀复位
void CanMasterLiquidBufResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//缓冲液灌注
void CanMasterLiquidBufPrimeWhileAck(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);
//缓冲液注液
void CanMasterLiquidBufInjectWhileAck(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);
//缓冲液清洗维护
void CanMasterLiquidBufMaintainWhileAck(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidBufWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidBufWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidBufWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidBufWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//缓冲液泵阀复位
void CanMasterLiquidBufWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//缓冲液灌注
void CanMasterLiquidBufWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//缓冲液注液
void CanMasterLiquidBufWaitInjectResult(SYS_RESULT_PACK* resultPackPtr);
//缓冲液清洗维护
void CanMasterLiquidBufWaitMaintainResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidBufReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidBufWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidBufResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidBufRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//缓冲液泵阀复位
void CanMasterLiquidBufResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//缓冲液灌注
void CanMasterLiquidBufPrimeWhileReturn(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);
//缓冲液注液
void CanMasterLiquidBufInjectWhileReturn(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);
//缓冲液清洗维护
void CanMasterLiquidBufMaintainWhileReturn(BOTTLE_BUFFER_SOLUTION bottleSelect,SYS_RESULT_PACK* resultPackPtr);




















#endif























