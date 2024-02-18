/*********************************************************************************************** 
** Author        : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 

#ifndef _CAN_MASTER_BOARD3_6_EMER_RAIL_H_
#define _CAN_MASTER_BOARD3_6_EMER_RAIL_H_


#include "CanRailMultiUtil.h"

//急诊模块电机
typedef enum INDEX_MOTOR_EM_RAIL
{
    INDEX_MOTOR_EM_RAIL_RAIL  = 0X00,//
    INDEX_MOTOR_EM_RAIL_MOVE  = 0X01,//
}INDEX_MOTOR_EM_RAIL;

//
typedef struct EM_RAIL_SENSOR_STATE
{
    SAMPLE_RACK_STATE entrance; //入口传感器状态
    SAMPLE_RACK_STATE absorb;   //吸样位传感器状态
    SAMPLE_RACK_STATE exit;     //出口传感器状态
    SAMPLE_RACK_STATE change;   //是否有架子进入变轨
}EM_RAIL_SENSOR_STATE;

/*************************************发送指令等待ACK************************************************/
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailRefreshSensorStateWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileAck(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailRackMoveInWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailRackMoveToTestWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailRackMoveWhileAck(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 放行一个架子到变轨
void CanMasterBoard3_6EmerRailPushRackToRailChangeWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr);

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailWaitRefreshSensorStateResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr);

//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailWaitMoveToTargetAbsorbPosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailWaitRackMoveInResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailWaitRackMoveToTestResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailWaitRackMoveResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道  放行一个架子到变轨
void CanMasterBoard3_6EmerRailWaitPushRackToRailChangeResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


/*************************************发送指令等待完成返回************************************************/
//急诊轨道 输入读取
void CanMasterBoard3_6EmerRailReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//急诊轨道 输出写出
void CanMasterBoard3_6EmerRailWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//急诊轨道 指定步进电机复位
void CanMasterBoard3_6EmerRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//急诊轨道 电机走步数
void CanMasterBoard3_6EmerRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//急诊轨道 整体复位
void CanMasterBoard3_6EmerRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr);

//急诊轨道 传感器状态刷新
void CanMasterBoard3_6EmerRailRefreshSensorStateWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,EM_RAIL_SENSOR_STATE *emRailSensorUnitStatePtr);

//急诊轨道 移动到指定吸样位
void CanMasterBoard3_6EmerRailMoveToTargetAbsorbPosWhileReturn(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子预进入
void CanMasterBoard3_6EmerRailRackMoveInWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子进入测试区
void CanMasterBoard3_6EmerRailRackMoveToTestWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 架子放行
void CanMasterBoard3_6EmerRailRackMoveWhileReturn(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr);

//急诊轨道 放行一个架子到变轨
void CanMasterBoard3_6EmerRailPushRackToRailChangeWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);


#endif // _CAN_MASTER_BOARD3_6_EMER_RAIL_H_

































