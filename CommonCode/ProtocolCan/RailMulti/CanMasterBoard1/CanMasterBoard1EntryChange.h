/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef _CAN_MASTER_BOARD1_ENTRY_CHANGE_H_
#define _CAN_MASTER_BOARD1_ENTRY_CHANGE_H_

#include "CanRailMultiDataType.h"

//步进电机序号
typedef enum INDEX_MOTOR_ENTRY_CHANGE
{
    INDEX_MOTOR_ENTRY_CHANGE_MOVE        = 0x00,//变轨平移电机
    INDEX_MOTOR_ENTRY_CHANGE_LEFT_RIGHT  = 0x01,//变轨推送电机
    INDEX_MOTOR_ENTRY_CHANGE_UPDOWN      = 0x02,//变轨升降
}INDEX_MOTOR_ENTRY_CHANGE;


typedef enum ENTRY_CHANGE_STATE
{
    ENTRY_CHANGE_STATE_IDLE                    = 0X00,
    ENTRY_CHANGE_STATE_RECEIVE_NEW_RACK        = 0X01,
    ENTRY_CHANGE_STATE_PUSH_RACK_TO_NORMAL     = 0X02,
    ENTRY_CHANGE_STATE_PUSH_RACK_TO_EMER       = 0X03,
    ENTRY_CHANGE_STATE_PUSH_RACK_TO_SAMPLE_HAND= 0X04,
    ENTRY_CHANGE_STATE_PREPARE_TO_RECEIVE_OLD  = 0X05,
    ENTRY_CHANGE_STATE_RECEIVE_OLD_RACK        = 0X06,
}ENTRY_CHANGE_STATE;

/*************************************发送指令等待ACK************************************************/
//输入变轨 输入读取
void CanMasterBoard1EntryChangeReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 输出写出
void CanMasterBoard1EntryChangeWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 指定步进电机复位
void CanMasterBoard1EntryChangeStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 电机走步数
void CanMasterBoard1EntryChangeStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 整体复位
void CanMasterBoard1EntryChangeResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 移动到指定位置
void CanMasterBoard1EntryChangeMovToTargetPosWhileAck(ENTRY_CHANGE_POS railChangePos,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 推送样本架到常规轨道
void CanMasterBoard1EntryChangePushRackToNormalRailWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 推送样本架到急诊轨道
void CanMasterBoard1EntryChangePushRackToEmRailWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 推送样本架到转运机械手
void CanMasterBoard1EntryChangePushRackToSampleHandWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 接收转运机械手架子
void CanMasterBoard1EntryChangeReceiveSampleHandRackWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 准备接收回收轨道样本架
void CanMasterBoard1EntryChangePrepareToReceiveBackRailRackWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 接收回收轨道样本架
void CanMasterBoard1EntryChangeReceiveBackRailRackWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 释放架子
void CanMasterBoard1EntryChangeReleaseRackWhileAck(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 有无试管架
void CanMasterBoard1EntryChangeIsEmptyWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//输入变轨 输入读取
void CanMasterBoard1EntryChangeWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//输入变轨 输出写出
void CanMasterBoard1EntryChangeWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//输入变轨 指定步进电机复位
void CanMasterBoard1EntryChangeWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//输入变轨 电机走步数
void CanMasterBoard1EntryChangeWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//输入变轨 整体复位
void CanMasterBoard1EntryChangeWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 移动到指定位置
void CanMasterBoard1EntryChangeWaitMovToTargetPosResult(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 推送样本架到常规轨道
void CanMasterBoard1EntryChangeWaitPushRackToNormalRailResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 推送样本架到急诊轨道
void CanMasterBoard1EntryChangeWaitPushRackToEmRailResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 推送样本架到转运机械手
void CanMasterBoard1EntryChangeWaitPushRackToSampleHandResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 接收转运机械手架子
void CanMasterBoard1EntryChangeWaitReceiveSampleHandRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 准备接收回收轨道样本架
void CanMasterBoard1EntryChangeWaitPrepareToReceiveBackRailRackResult(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 接收回收轨道样本架
void CanMasterBoard1EntryChangeWaitReceiveBackRailRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 释放架子
void CanMasterBoard1EntryChangeWaitReleaseRackResult(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 有无试管架
void CanMasterBoard1EntryChangeWaitIsEmptyResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);


/*************************************发送指令等待完成返回************************************************/
//输入变轨 输入读取
void CanMasterBoard1EntryChangeReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//输入变轨 输出写出
void CanMasterBoard1EntryChangeWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//输入变轨 指定步进电机复位
void CanMasterBoard1EntryChangeStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//输入变轨 电机走步数
void CanMasterBoard1EntryChangeStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//输入变轨 整体复位
void CanMasterBoard1EntryChangeResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 移动到指定位置
void CanMasterBoard1EntryChangeMovToTargetPosWhileReturn(ENTRY_CHANGE_POS railChangePos,SYS_RESULT_PACK* resultPackPtr);

//输入变轨 推送样本架到常规轨道
void CanMasterBoard1EntryChangePushRackToNormalRailWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 推送样本架到急诊轨道
void CanMasterBoard1EntryChangePushRackToEmRailWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 推送样本架到转运机械手
void CanMasterBoard1EntryChangePushRackToSampleHandWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 接收转运机械手架子
void CanMasterBoard1EntryChangeReceiveSampleHandRackWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨  准备接收回收轨道样本架
void CanMasterBoard1EntryChangePrepareToReceiveBackRailRackWhileReturn(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 接收回收轨道样本架
void CanMasterBoard1EntryChangeReceiveBackRailRackWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//输入变轨 释放架子
void CanMasterBoard1EntryChangeReleaseRackWhileReturn(SYS_RESULT_PACK* resultPackPtr);

//输入变轨 有无试管架
void CanMasterBoard1EntryChangeIsEmptyWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);










#endif // _CAN_MASTER_BOARD1_ENTRY_CHANGE_H_
































