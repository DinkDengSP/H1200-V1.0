/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 15:10:17 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_HAND_NEW_CUP_H_
#define __CAN_MASTER_HAND_NEW_CUP_H_
#include "CanMasterH360Base.h"
#include "CanMasterCupTrayManage.h"

//新杯机械手
typedef enum INDEX_MOTOR_HAND_NEW_CUP
{
    INDEX_MOTOR_HAND_NEW_CUP_BACKFRONT  = 0X00,//前后运转
    INDEX_MOTOR_HAND_NEW_CUP_LEFTRIGHT  = 0X01,//左右运转
    INDEX_MOTOR_HAND_NEW_CUP_UPDOWN     = 0X02,//升降运转
}INDEX_MOTOR_HAND_NEW_CUP;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterHandNewCupReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterHandNewCupWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandNewCupResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterHandNewCupRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//新杯机械手复位
void CanMasterHandNewCupResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到新杯区
void CanMasterHandNewCupMoveToNewCupTrayWhileAck(uint8_t lineBackFront,uint8_t colLeftRight,SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到反应盘
void CanMasterHandNewCupMoveToPlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到垃圾桶
void CanMasterHandNewCupMoveToGarbageWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);
//新杯机械手新杯区取杯
void CanMasterHandNewCupCatchCupNewCupTrayWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手反应盘取杯
void CanMasterHandNewCupCatchCupPlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手反应盘放杯
void CanMasterHandNewCupPutCupPlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手垃圾桶放杯
void CanMasterHandNewCupPutCupGarbageWhileAck(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterHandNewCupWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterHandNewCupWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandNewCupWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterHandNewCupWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//新杯机械手复位
void CanMasterHandNewCupWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到新杯区
void CanMasterHandNewCupWaitMoveToNewCupTrayResult(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到反应盘
void CanMasterHandNewCupWaitMoveToPlateReactionResult(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到垃圾桶
void CanMasterHandNewCupWaitMoveToGarbageResult(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手新杯区取杯
void CanMasterHandNewCupWaitCatchCupNewCupTrayResult(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手反应盘取杯
void CanMasterHandNewCupWaitCatchCupPlateReactionResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//新杯机械手反应盘放杯
void CanMasterHandNewCupWaitPutCupPlateReactionResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//新杯机械手垃圾桶放杯
void CanMasterHandNewCupWaitPutCupGarbageResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterHandNewCupReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterHandNewCupWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandNewCupResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterHandNewCupRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//新杯机械手复位
void CanMasterHandNewCupResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到新杯区
void CanMasterHandNewCupMoveToNewCupTrayWhileReturn(uint8_t lineBackFront,uint8_t colLeftRight,SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到反应盘
void CanMasterHandNewCupMoveToPlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手移动到垃圾桶
void CanMasterHandNewCupMoveToGarbageWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);
//新杯机械手新杯区取杯
void CanMasterHandNewCupCatchCupNewCupTrayWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//新杯机械手反应盘取杯
void CanMasterHandNewCupCatchCupPlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//新杯机械手反应盘放杯
void CanMasterHandNewCupPutCupPlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//新杯机械手垃圾桶放杯
void CanMasterHandNewCupPutCupGarbageWhileReturn(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr);
























#endif

















