/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 21:34:35 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_NEEDLE_WASH_H_
#define __CAN_MASTER_NEEDLE_WASH_H_
#include "CanMasterH360Base.h"


//步进电机序号
typedef enum INDEX_MOTOR_NEEDLE_WASH
{
    INDEX_MOTOR_NEEDLE_WASH_UPDOWN      = 0x00,//清洗针升降
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP1 = 0x01,//清洗注射泵1
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP2 = 0x02,//清洗注射泵2
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP3 = 0x03,//清洗注射泵3
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP4 = 0x04,//清洗注射泵4
    INDEX_MOTOR_NEEDLE_WASH_CLEAN_PUMP5 = 0x05,//清洗注射泵5
    INDEX_MOTOR_NEEDLE_WASH_WASH_PUMP   = 0x06,//废液泵
}INDEX_MOTOR_NEEDLE_WASH;

//清洗针堵针状态
typedef enum NEEDLE_CLEAN_BLOCK
{
    NEEDLE_CLEAN_BLOCK_NONE     = 0X00,//没堵
    NEEDLE_CLEAN_BLOCK_HALF     = 0X01,//半堵
    NEEDLE_CLEAN_BLOCK_FULL     = 0X02,//全堵
}NEEDLE_CLEAN_BLOCK;

//清洗针清洗配置
typedef struct NEEDLE_CLEAN_CONFIG
{
    FunctionalState needle1Inject;
    FunctionalState needle2Inject;
    FunctionalState needle3Inject;
    FunctionalState needle4Inject;
    FunctionalState needle5Inject;
}NEEDLE_CLEAN_CONFIG;

//磁珠清洗结果
typedef struct NEEDLE_WASH_BEAD_CLEAN_MSG
{
    NEEDLE_CLEAN_BLOCK blockState;
    uint8_t waterPercent;
    uint8_t airPercent;
}NEEDLE_WASH_BEAD_CLEAN_MSG;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleWashReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterNeedleWashWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleWashResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterNeedleWashRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//清洗针复位
void CanMasterNeedleWashResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗针灌注
void CanMasterNeedleWashPrimeWhileAck(uint8_t primeCount,SYS_RESULT_PACK* resultPackPtr);
//清洗针维护
void CanMasterNeedleWashMaintainCleanWhileAck(uint8_t maintainCount,SYS_RESULT_PACK* resultPackPtr);
//清洗针磁珠清洗一次
void CanMasterNeedleWashBeadCleanWhileAck(NEEDLE_CLEAN_CONFIG cleanConfig,SYS_RESULT_PACK* resultPackPtr);
//清洗针下降到浸泡位
void CanMasterNeedleWashUpdownToSoakWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗针阻塞清洗
void CanMasterNeedleWashBlockMaintainCleanWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗针废液泵抽一次
void CanMasterNeedleWashPumpAbsorbOnceWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterNeedleWashWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleWashWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleWashWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleWashWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗针复位
void CanMasterNeedleWashWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//清洗针灌注
void CanMasterNeedleWashWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//清洗针维护
void CanMasterNeedleWashWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr);
//清洗针磁珠清洗一次
void CanMasterNeedleWashWaitBeadCleanResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_WASH_BEAD_CLEAN_MSG* needleWashMsgPtr);
//清洗针下降到浸泡位
void CanMasterNeedleWashWaitUpdownToSoakResult(SYS_RESULT_PACK* resultPackPtr);
//清洗针阻塞清洗
void CanMasterNeedleWashWaitBlockMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr);
//清洗针废液泵抽一次
void CanMasterNeedleWashWaitPumpAbsorbOnceResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleWashReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleWashWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleWashResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleWashRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗针复位
void CanMasterNeedleWashResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗针灌注
void CanMasterNeedleWashPrimeWhileReturn(uint8_t primeCount,SYS_RESULT_PACK* resultPackPtr);
//清洗针维护
void CanMasterNeedleWashMaintainCleanWhileReturn(uint8_t maintainCount,SYS_RESULT_PACK* resultPackPtr);
//清洗针磁珠清洗一次
void CanMasterNeedleWashBeadCleanWhileReturn(NEEDLE_CLEAN_CONFIG cleanConfig,SYS_RESULT_PACK* resultPackPtr,
                                                NEEDLE_WASH_BEAD_CLEAN_MSG* needleWashMsgPtr);
//清洗针下降到浸泡位
void CanMasterNeedleWashUpdownToSoakWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗针阻塞清洗
void CanMasterNeedleWashBlockMaintainCleanWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗针废液泵抽一次
void CanMasterNeedleWashPumpAbsorbOnceWhileReturn(SYS_RESULT_PACK* resultPackPtr);




#endif
















