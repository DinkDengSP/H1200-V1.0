/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-24 13:10:38 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_HAND_WASH_H_
#define __CAN_MASTER_HAND_WASH_H_
#include "CanMasterH360Base.h"

//清洗机械手模块电机
typedef enum INDEX_MOTOR_HAND_WASH
{
    INDEX_MOTOR_HAND_WASH_ROTATE  = 0X00,//旋转
    INDEX_MOTOR_HAND_WASH_UPDOWN  = 0X01,//升降
}INDEX_MOTOR_HAND_WASH;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterHandWashReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterHandWashWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandWashResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterHandWashRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//清洗机械手复位
void CanMasterHandWashResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到反应盘
void CanMasterHandWashRotatePlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到清洗盘
void CanMasterHandWashRotatePlateWashWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到测量模块
void CanMasterHandWashRotateMeasureModuleWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手反应盘取杯
void CanMasterHandWashCatchCupPlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手反应盘放杯
void CanMasterHandWashPutCupPlateReactionWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手清洗盘取杯
void CanMasterHandWashCatchCupPlateWashWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手清洗盘放杯
void CanMasterHandWashPutCupPlateWashWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手测量模块取杯
void CanMasterHandWashCatchCupMeasureModuleWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手测量模块放杯
void CanMasterHandWashPutCupMeasureModuleWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterHandWashWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterHandWashWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandWashWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterHandWashWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗机械手复位
void CanMasterHandWashWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到反应盘
void CanMasterHandWashWaitRotatePlateReactionResult(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到清洗盘
void CanMasterHandWashWaitRotatePlateWashResult(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到测量模块
void CanMasterHandWashWaitRotateMeasureModuleResult(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手反应盘取杯
void CanMasterHandWashWaitCatchCupPlateReactionResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//清洗机械手反应盘放杯
void CanMasterHandWashWaitPutCupPlateReactionResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//清洗机械手清洗盘取杯
void CanMasterHandWashWaitCatchCupPlateWashResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateWashSensorStatePtr);
//清洗机械手清洗盘放杯
void CanMasterHandWashWaitPutCupPlateWashResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateWashSensorStatePtr);
//清洗机械手测量模块取杯
void CanMasterHandWashWaitCatchCupMeasureModuleResult(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手测量模块放杯
void CanMasterHandWashWaitPutCupMeasureModuleResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterHandWashReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterHandWashWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterHandWashResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterHandWashRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//清洗机械手复位
void CanMasterHandWashResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到反应盘
void CanMasterHandWashRotatePlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到清洗盘
void CanMasterHandWashRotatePlateWashWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手旋转到测量模块
void CanMasterHandWashRotateMeasureModuleWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手反应盘取杯
void CanMasterHandWashCatchCupPlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//清洗机械手反应盘放杯
void CanMasterHandWashPutCupPlateReactionWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateReactionSensorStatePtr);
//清洗机械手清洗盘取杯
void CanMasterHandWashCatchCupPlateWashWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateWashSensorStatePtr);
//清洗机械手清洗盘放杯
void CanMasterHandWashPutCupPlateWashWhileReturn(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* plateWashSensorStatePtr);
//清洗机械手测量模块取杯
void CanMasterHandWashCatchCupMeasureModuleWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//清洗机械手测量模块放杯
void CanMasterHandWashPutCupMeasureModuleWhileReturn(SYS_RESULT_PACK* resultPackPtr);

























#endif


















