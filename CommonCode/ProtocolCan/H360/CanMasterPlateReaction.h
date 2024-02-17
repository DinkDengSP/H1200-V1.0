/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 10:04:30 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PLATE_REACTION_H_
#define __CAN_MASTER_PLATE_REACTION_H_
#include "CanMasterH360Base.h"

//反应盘模块电机
typedef enum INDEX_MOTOR_PLATE_REACTION
{
    INDEX_MOTOR_PLATE_REACTION_ROTATE  = 0X00,     
}INDEX_MOTOR_PLATE_REACTION;

//反应盘结果数据
typedef struct PLATE_REACTION_MSG
{
    uint8_t currentNewCupIndex;//当前新杯位反应盘序号
    PIN_STATE cupExistNewPos;//新杯位是否有杯
    PIN_STATE cupExistOldPos;//旧杯位是否有杯
}PLATE_REACTION_MSG;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterPlateReactionReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterPlateReactionWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReactionResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterPlateReactionRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//反应盘复位
void CanMasterPlateReactionResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//反应盘正向运转到指定杯位
void CanMasterPlateReactionRunIndexCupPositiveWhileAck(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr);
//反应盘正向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsPositiveWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr);
//反应盘反向运转到指定杯位
void CanMasterPlateReactionRunIndexCupNegativeWhileAck(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr);
//反应盘反向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsNegativeWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterPlateReactionWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateReactionWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReactionWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateReactionWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//反应盘复位
void CanMasterPlateReactionWaitResetWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘正向运转到指定杯位
void CanMasterPlateReactionWaitRunIndexCupPositiveWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘正向运转指定数量杯位
void CanMasterPlateReactionWaitRunAnyCupsPositiveWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘反向运转到指定杯位
void CanMasterPlateReactionWaitRunIndexCupNegativeWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘反向运转指定数量杯位
void CanMasterPlateReactionWaitRunAnyCupsNegativeWhileResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterPlateReactionReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateReactionWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReactionResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateReactionRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//反应盘复位
void CanMasterPlateReactionResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘正向运转到指定杯位
void CanMasterPlateReactionRunIndexCupPositiveWhileReturn(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘正向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsPositiveWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘反向运转到指定杯位
void CanMasterPlateReactionRunIndexCupNegativeWhileReturn(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);
//反应盘反向运转指定数量杯位
void CanMasterPlateReactionRunAnyCupsNegativeWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_REACTION_MSG* msgSetPtr);




















#endif

















