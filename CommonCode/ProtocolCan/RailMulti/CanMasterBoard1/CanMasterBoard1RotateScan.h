/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef _CAN_MASTER_BOARD1_ROTATE_SCAN_H_
#define _CAN_MASTER_BOARD1_ROTATE_SCAN_H_
#include "CanRailMultiDataType.h"


typedef enum INDEX_MOTOR_ROTATE_SCAN
{
    INDEX_MOTOR_ROTATE_SCAN_ROTATE  = 0X00,//旋转电机
    INDEX_MOTOR_ROTATE_SCAN_LOCK    = 0X01,//限位电机
}INDEX_MOTOR_ROTATE_SCAN;



/*************************************发送指令等待ACK************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 整体复位
void CanMasterBoard1RotateScanResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanPrepareRecvRackWhileAck(RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanRackScanRotateDirWhileAck(uint8_t scanFlag,RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//旋转扫码 整体复位
void CanMasterBoard1RotateScanWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanWaitPrepareRecvRackResult(SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanWaitRackScanRotateDirResult(SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr);

/*************************************发送指令等待完成返回************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//旋转扫码 整体复位
void CanMasterBoard1RotateScanResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanPrepareRecvRackWhileReturn(RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr);

//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanRackScanRotateDirWhileReturn(uint8_t scanFlag,RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr);

#endif // _CAN_MASTER_BOARD1_ROTATE_SCAN_H_































