/*********************************************************************************************** 
** Author        : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 

#ifndef _CAN_MASTER_BOARD3_6_RECYCLE_RAIL_H_
#define _CAN_MASTER_BOARD3_6_RECYCLE_RAIL_H_


#include "CanRailMultiUtil.h"


//回收轨道电机
typedef enum INDEX_MOTOR_RECYCLE_RAIL
{
    INDEX_MOTOR_RECYCLE_RAIL_MOVE  = 0X00,//
}INDEX_MOTOR_RECYCLE_RAIL;

/*************************************发送指令等待ACK************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailMovRackToExitWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailReceiveRackWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailPushRackToNextWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr);

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailWaitMovRackToExitResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr);

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailWaitReceiveRackResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackEntryPtr);

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailWaitPushRackToNextResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//回收轨道 输入读取
void CanMasterBoard3_6RecycleRailReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//回收轨道 输出写出
void CanMasterBoard3_6RecycleRailWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//回收轨道 指定步进电机复位
void CanMasterBoard3_6RecycleRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//回收轨道 电机走步数
void CanMasterBoard3_6RecycleRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//回收轨道 整体复位
void CanMasterBoard3_6RecycleRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr);

//回收轨道 回收架子到出口
void CanMasterBoard3_6RecycleRailMovRackToExitWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr);

//回收轨道 接收架子
void CanMasterBoard3_6RecycleRailReceiveRackWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackEntryPtr);

//回收轨道 推送架子到下一模块
void CanMasterBoard3_6RecycleRailPushRackToNextWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE *rackExistPtr);


#endif // _CAN_MASTER_BOARD3_6_RECYCLE_RAIL_H_

































