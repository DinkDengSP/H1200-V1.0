/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:20:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 21:29:13 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PLATE_REAGENT_H_
#define __CAN_MASTER_PLATE_REAGENT_H_
#include "CanMasterH360Base.h"

//试剂盘模块电机
typedef enum INDEX_MOTOR_PLATE_REAGENT
{
    INDEX_MOTOR_PLATE_REAGENT_ROTATE  = 0X00,     
}INDEX_MOTOR_PLATE_REAGENT;

//试剂盘的偏移信息
typedef enum PLATE_REAGENT_OFFSET
{
    PLATE_REAGENT_OFFSET_NONE               = 0X00,//试剂盘无偏移
    PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R1  = 0X01,//试剂盘旋转到对准试剂针R1位置
    PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_R2  = 0X02,//试剂盘旋转到对准试剂针R2位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R1     = 0X03,//试剂盘旋转到对阵磁珠针R1位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_R2     = 0X04,//试剂盘旋转到对阵磁珠针R2位置
    PLATE_REAGENT_OFFSET_NEEDLE_BEAD_BEAD   = 0X05,//试剂盘旋转到对阵磁珠针BEAD位置
    PLATE_REAGENT_OFFSET_QR_SCAN            = 0X06,//试剂盘旋转到扫码位置
}PLATE_REAGENT_OFFSET;

//扫码偏移
typedef enum PLATE_REAGENT_SCAN_OFFSET
{
    PLATE_REAGENT_SCAN_OFFSET_NONE          = 0,//无偏移
    PLATE_REAGENT_SCAN_OFFSET_START         = 1,//试剂盘移动扫码起始点
    PLATE_REAGENT_SCAN_OFFSET_END           = 2,//试剂盘移动扫码结束点
}PLATE_REAGENT_SCAN_OFFSET;

//试剂盘返回信息
typedef struct PLATE_REAGENT_MSG_SET
{
    uint8_t currentCupIndex;
    PLATE_REAGENT_OFFSET currentOffset;
}PLATE_REAGENT_MSG_SET;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterPlateReagentReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterPlateReagentWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReagentResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterPlateReagentRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//试剂盘复位
void CanMasterPlateReagentResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentRun2CupWithOffsetWhileAck(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,SYS_RESULT_PACK* resultPackPtr);
//试剂盘运行指定数量个杯位,基于当前位置
void CanMasterPlateReagentRunAnyCupsWhileAck(uint8_t cups,SYS_RESULT_PACK* resultPackPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentRun2CupWithOffsetForScanWhileAck(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterPlateReagentWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateReagentWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReagentWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateReagentWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂盘复位
void CanMasterPlateReagentWaitResetResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentWaitRun2CupWithOffsetResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行指定数量个杯位,基于当前位置
void CanMasterPlateReagentWaitRunAnyCupsResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentWaitRun2CupWithOffsetForScanResult(SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterPlateReagentReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterPlateReagentWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterPlateReagentResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterPlateReagentRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂盘复位
void CanMasterPlateReagentResetWhileReturn(SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentRun2CupWithOffsetWhileReturn(uint8_t targetCupIndex,PLATE_REAGENT_OFFSET targetOffset,SYS_RESULT_PACK* resultPackPtr,
                                                            PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行指定数量个杯位,基于当前位置
void CanMasterPlateReagentRunAnyCupsWhileReturn(uint8_t cups,SYS_RESULT_PACK* resultPackPtr,PLATE_REAGENT_MSG_SET* msgSetPtr);
//试剂盘运行到指定序号杯位,带偏移
void CanMasterPlateReagentRun2CupWithOffsetForScanWhileReturn(uint8_t targetCupIndex,PLATE_REAGENT_SCAN_OFFSET scanOffset,SYS_RESULT_PACK* resultPackPtr
                                                                ,PLATE_REAGENT_MSG_SET* msgSetPtr);




















#endif













