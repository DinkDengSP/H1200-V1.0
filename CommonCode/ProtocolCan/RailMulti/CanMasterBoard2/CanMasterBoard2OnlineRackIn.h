/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef _CAN_MASTER_BOARD2_ONLINE_RACK_IN_H_
#define _CAN_MASTER_BOARD2_ONLINE_RACK_IN_H_


#include "CanRailMultiDataType.h"

/*************************************发送指令等待ACK************************************************/
//联机接口进架 输入读取
void CanMasterBoard2OnlineRackInReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 输出写出
void CanMasterBoard2OnlineRackInWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInReleaseControlWhileAck(ONLINE_IN_GATE gateCtrl,SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//联机接口进架  输入读取
void CanMasterBoard2OnlineRackInWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//联机接口进架  输出写出
void CanMasterBoard2OnlineRackInWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInWaitReleaseControlResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//联机接口进架  输入读取
void CanMasterBoard2OnlineRackInReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//联机接口进架  输出写出
void CanMasterBoard2OnlineRackInWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//联机接口进架 指定步进电机复位
void CanMasterBoard2OnlineRackInStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口进架 电机走步数
void CanMasterBoard2OnlineRackInStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口进架 整体复位
void CanMasterBoard2OnlineRackInResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr);

//联机接口进架 放行控制
void CanMasterBoard2OnlineRackInReleaseControlWhileReturn(ONLINE_IN_GATE gateCtrl,SYS_RESULT_PACK* resultPackPtr);


#endif // _CAN_MASTER_BOARD2_ONLINE_RACK_IN_H_


































