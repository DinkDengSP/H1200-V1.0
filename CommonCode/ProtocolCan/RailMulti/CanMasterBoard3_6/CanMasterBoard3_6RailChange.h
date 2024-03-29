/*********************************************************************************************** 
** Author        : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 

#ifndef _CAN_MASTER_BOARD3_6_RAIL_CHANGE_H_
#define _CAN_MASTER_BOARD3_6_RAIL_CHANGE_H_


#include "CanRailMultiUtil.h"

//轨道变轨电机
typedef enum INDEX_MOTOR_RAIL_CHANGE
{
    INDEX_MOTOR_RAIL_CHANGE_BLOCK   = 0X00,//
    INDEX_MOTOR_RAIL_CHANGE_MOVE    = 0X01,//
}INDEX_MOTOR_RAIL_CHANGE;

//门控状态
typedef enum RAIL_CHANGE_GATE_STATE
{
    RAIL_CHANGE_GATE_STATE_CLOSE = 0X00,
    RAIL_CHANGE_GATE_STATE_OPEN  = 0X01,
}RAIL_CHANGE_GATE_STATE;

/*************************************发送指令等待ACK************************************************/
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeMoveToTargetRailWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨  放行控制
void CanMasterBoard3_6RailChangeReleaseControlWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_GATE railGate,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangePushRackToTargetRailWhileAck(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeWaitMoveToTargetRailResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨  放行控制
void CanMasterBoard3_6RailChangeWaitReleaseControlResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangeWaitPushRackToTargetRailResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//轨道变轨 输入读取
void CanMasterBoard3_6RailChangeReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//轨道变轨 输出写出
void CanMasterBoard3_6RailChangeWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//轨道变轨 指定步进电机复位
void CanMasterBoard3_6RailChangeStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//轨道变轨 电机走步数
void CanMasterBoard3_6RailChangeStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//轨道变轨 整体复位
void CanMasterBoard3_6RailChangeResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 移动到指定轨道
void CanMasterBoard3_6RailChangeMoveToTargetRailWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 放行控制
void CanMasterBoard3_6RailChangeReleaseControlWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_GATE railGate,SYS_RESULT_PACK* resultPackPtr);

//轨道变轨 推送样本架
void CanMasterBoard3_6RailChangePushRackToTargetRailWhileReturn(RAIL_MODULE_NO railNo,RAIL_CHANGE_POS changePos,SYS_RESULT_PACK* resultPackPtr);




#endif // _CAN_MASTER_BOARD3_6_RAIL_CHANGE_H_

































