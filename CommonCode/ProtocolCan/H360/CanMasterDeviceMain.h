/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 09:13:04 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_DEVICE_MAIN_H_
#define __CAN_MASTER_DEVICE_MAIN_H_
#include "CanMasterH360Base.h"



/*************************************发送指令等待ACK************************************************/
//吸样完成
void CanMasterDeviceMainAbsorbFinishWhileAck(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//置废轨道指定样本管
void CanMasterDeviceMainSetRailTubeDiscardWhileAck(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//状态上报到轨道
void CanMasterDeviceMainReportStateToRailWhileAck(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//锁定轨道变轨
void CanMasterDeviceMainLockRailChangeWhileAck(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
/*************************************等待指定指令完成返回************************************************/
//吸样完成
void CanMasterDeviceMainWaitAbsorbFinishResult(SYS_RESULT_PACK* resultPackPtr);
//置废轨道指定样本管
void CanMasterDeviceMainWaitSetRailTubeDiscardResult(SYS_RESULT_PACK* resultPackPtr);
//状态上报到轨道
void CanMasterDeviceMainWaitReportStateToRailResult(SYS_RESULT_PACK* resultPackPtr);
//锁定轨道变轨
void CanMasterDeviceMainWaitLockRailChangeResult(SYS_RESULT_PACK* resultPackPtr);
/*************************************发送指令等待完成返回************************************************/
//吸样完成
void CanMasterDeviceMainAbsorbFinishWhileReturn(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//置废轨道指定样本管
void CanMasterDeviceMainSetRailTubeDiscardWhileReturn(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//状态上报到轨道
void CanMasterDeviceMainReportStateToRailWhileReturn(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);
//锁定轨道变轨
void CanMasterDeviceMainLockRailChangeWhileReturn(uint8_t* sendBuf,uint16_t sendLength,SYS_RESULT_PACK* resultPackPtr);











#endif


















