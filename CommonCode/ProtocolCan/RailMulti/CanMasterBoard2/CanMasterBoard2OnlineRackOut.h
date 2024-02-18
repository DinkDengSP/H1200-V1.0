/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef _CAN_MASTER_BOARD2_ONLINE_RACK_OUT_H_
#define _CAN_MASTER_BOARD2_ONLINE_RACK_OUT_H_


#include "CanRailMultiDataType.h"

/*************************************发送指令等待ACK************************************************/
//联机接口出架 输入读取
void CanMasterBoard2OnlineRackOutReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 输出写出
void CanMasterBoard2OnlineRackOutWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 指定步进电机复位
void CanMasterBoard2OnlineRackOutStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 电机走步数
void CanMasterBoard2OnlineRackOutStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 整体复位
void CanMasterBoard2OnlineRackOutResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 放行控制
void CanMasterBoard2OnlineRackOutReleaseControlWhileAck(SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//联机接口出架 输入读取
void CanMasterBoard2OnlineRackOutWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//联机接口出架 输出写出
void CanMasterBoard2OnlineRackOutWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//联机接口出架 指定步进电机复位
void CanMasterBoard2OnlineRackOutWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口出架 电机走步数
void CanMasterBoard2OnlineRackOutWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口出架 整体复位
void CanMasterBoard2OnlineRackOutWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 放行控制
void CanMasterBoard2OnlineRackOutWaitReleaseControlResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//联机接口出架 输入读取
void CanMasterBoard2OnlineRackOutReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//联机接口出架 输出写出
void CanMasterBoard2OnlineRackOutWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//联机接口出架 指定步进电机复位
void CanMasterBoard2OnlineRackOutStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口出架 电机走步数
void CanMasterBoard2OnlineRackOutStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//联机接口出架 整体复位
void CanMasterBoard2OnlineRackOutResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr);

//联机接口出架 放行控制
void CanMasterBoard2OnlineRackOutReleaseControlWhileReturn(SYS_RESULT_PACK* resultPackPtr);


#endif // _CAN_MASTER_BOARD2_ONLINE_RACK_OUT_H_


































