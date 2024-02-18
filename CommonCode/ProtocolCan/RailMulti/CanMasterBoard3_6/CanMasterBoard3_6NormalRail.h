/*********************************************************************************************** 
** Author        : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(RAIL_MODULE_NO railNo,邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 

#ifndef _CAN_MASTER_BOARD3_6_NORMAL_RAIL_H_
#define _CAN_MASTER_BOARD3_6_NORMAL_RAIL_H_


#include "CanRailMultiUtil.h"

// 常规轨道电机
typedef enum INDEX_MOTOR_NORMAL_RAIL
{
    INDEX_MOTOR_NORMAL_RAIL_ABSORB_BLOCK    = 0X00,//常规轨道吸样挡叉电机
    INDEX_MOTOR_NORMAL_RAIL_WAIT_BLOCK      = 0X01,//常规轨道待测挡叉电机
    INDEX_MOTOR_NORMAL_RAIL_BUFFER_BLOCK    = 0X02,//常规轨道缓存挡叉电机
    INDEX_MOTOR_NORMAL_RAIL_MOVE            = 0X03,//常规轨道皮带电机
}INDEX_MOTOR_NORMAL_RAIL;

//
typedef struct NORMAL_RAIL_SENSOR_STATE
{
    SAMPLE_RACK_STATE entrance; //入口传感器状态
    SAMPLE_RACK_STATE buffer;   //缓存位传感器状态
    SAMPLE_RACK_STATE wait;     //待测位传感器状态
    SAMPLE_RACK_STATE absorb;   //吸样位传感器状态
    SAMPLE_RACK_STATE exit;     //出口传感器状态
    SAMPLE_RACK_STATE change;   //是否有架子进入变轨
}NORMAL_RAIL_SENSOR_STATE;

/*************************************发送指令等待ACK************************************************/
//常规轨道 输入读取
void CanMasterBoard3_6NormalRailReadInputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 输出写出
void CanMasterBoard3_6NormalRailWriteOutputWhileAck(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 指定步进电机复位
void CanMasterBoard3_6NoramlRailStepMotorResetWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 电机走步数
void CanMasterBoard3_6NoramlRailStepMotorRunStepsWhileAck(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 整体复位
void CanMasterBoard3_6NoramlRailResetAllWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 传感器状态刷新
void CanMasterBoard3_6NormalRailFlushSensorStateWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 移动到指定吸样位
void CanMasterBoard3_6NormalRailMovToTargetAbsorbPosWhileAck(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 接收架子
void CanMasterBoard3_6NormalRailRackMovInWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子移动到缓存
void CanMasterBoard3_6NormalRailRackMoveToBufferWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子放行
void CanMasterBoard3_6NormalRailRackMoveWhileAck(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//常规轨道 输入读取
void CanMasterBoard3_6NormalRailWaitReadInputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//常规轨道 输出写出
void CanMasterBoard3_6NormalRailWaitWriteOutputResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//常规轨道 指定步进电机复位
void CanMasterBoard3_6NoramlRailWaitStepMotorResetResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//常规轨道 电机走步数
void CanMasterBoard3_6NoramlRailWaitStepMotorRunStepsResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//常规轨道 整体复位
void CanMasterBoard3_6NoramlRailWaitResetAllResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,NORMAL_RAIL_SENSOR_STATE *normalRailSensorUnitPtr);

//常规轨道 传感器状态刷新
void CanMasterBoard3_6NormalRailWaitFlushSensorStateResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,NORMAL_RAIL_SENSOR_STATE *normalRailSensorUnitPtr);

//常规轨道 移动到指定吸样位
void CanMasterBoard3_6NormalRailWaitMovToTargetAbsorbPosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 接收架子
void CanMasterBoard3_6NormalRailWaitRackMoveInResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子移动到缓存
void CanMasterBoard3_6NormalRailWaitRackMoveToBufferResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子放行
void CanMasterBoard3_6NormalRailWaitRackMoveResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//常规轨道 输入读取
void CanMasterBoard3_6NormalRailReadInputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//常规轨道 输出写出
void CanMasterBoard3_6NormalRailWriteOutputWhileReturn(RAIL_MODULE_NO railNo,uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//常规轨道 指定步进电机复位
void CanMasterBoard3_6NormalRailStepMotorResetWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);
     
//常规轨道 电机走步数
void CanMasterBoard3_6NormalRailStepMotorRunStepsWhileReturn(RAIL_MODULE_NO railNo,uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//常规轨道 整体复位
void CanMasterBoard3_6NormalRailResetAllWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,NORMAL_RAIL_SENSOR_STATE *normalRailSensorUnitPtr);

//常规轨道 传感器状态刷新
void CanMasterBoard3_6NormalRailFlushSensorStateWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,NORMAL_RAIL_SENSOR_STATE *normalRailSensorUnitPtr);

//常规轨道 移动到指定吸样位
void CanMasterBoard3_6NormalRailMovToTargetAbsorbPosWhileReturn(RAIL_MODULE_NO railNo,ABSORB_POS pos,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 接收架子
void  CanMasterBoard3_6NormalRailRackMovInWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子移动到缓存
void  CanMasterBoard3_6NormalRailRackMoveToBufferWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

//常规轨道 架子放行
void CanMasterBoard3_6NormalRailRackMoveWhileReturn(RAIL_MODULE_NO railNo,RACK_MOVE releaseType,SYS_RESULT_PACK* resultPackPtr);


#endif // _CAN_MASTER_BOARD3_6_NORMAL_RAIL_H_

































