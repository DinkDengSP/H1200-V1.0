/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 09:13:04 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PLATE_WASH_H_
#define __CAN_MASTER_PLATE_WASH_H_
#include "CanMasterH360Base.h"

//电机序号
typedef enum INDEX_MOTOR_PLATE_WASH
{
    INDEX_MOTOR_PLATE_WASH_ROTATE = 0x00,
}INDEX_MOTOR_PLATE_WASH;

//清洗盘信息
typedef struct PLATE_WASH_MSG
{
    uint8_t cupIndex;//当前序号
    PIN_STATE cupExistState;//光纤传感器检杯序号
}PLATE_WASH_MSG;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterPlateWashReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterPlateWashWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateWashResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterPlateWashRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//清洗盘复位
void CanMasterPlateWashResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗盘运行指定个杯位
void CanMasterPlateWashRunAnyCupsWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr);
//清洗盘运行到指定序号杯位
void CanMasterPlateWashRun2CupWithIndexWhileAck(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterPlateWashWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateWashWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateWashWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateWashWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗盘复位
void CanMasterPlateWashWaitResetResult(SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);
//清洗盘运行指定个杯位
void CanMasterPlateWashWaitRunAnyCupsResult(SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);
//清洗盘运行到指定序号杯位
void CanMasterPlateWashWaitRun2CupWithIndexResult(SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterPlateWashReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateWashWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateWashResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateWashRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗盘复位
void CanMasterPlateWashResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);
//清洗盘运行指定个杯位
void CanMasterPlateWashRunAnyCupsWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);
//清洗盘运行到指定序号杯位
void CanMasterPlateWashRun2CupWithIndexWhileReturn(uint8_t targetCup,SYS_RESULT_PACK* resultPackPtr,PLATE_WASH_MSG* msgSetPtr);




















#endif


















