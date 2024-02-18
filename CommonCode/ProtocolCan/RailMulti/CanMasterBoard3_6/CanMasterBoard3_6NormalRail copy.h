/*********************************************************************************************** 
** Author        : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 

#ifndef _CAN_MASTER_BOARD3_6_EM_RAIL_H_
#define _CAN_MASTER_BOARD3_6_EM_RAIL_H_


#include "CanRailMultiUtil.h"

/*************************************发送指令等待ACK************************************************/
//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,int32_t* coordinatePtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,int32_t* coordinatePtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


/*************************************发送指令等待完成返回************************************************/
//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,int32_t* coordinatePtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* coordinatePtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


#endif // _CAN_MASTER_BOARD3_6_EM_RAIL_H_

































