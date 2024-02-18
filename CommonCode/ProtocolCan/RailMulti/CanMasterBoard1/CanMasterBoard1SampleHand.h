/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef _CAN_MASTER_BOARD1_SAMPLE_HAND_H_
#define _CAN_MASTER_BOARD1_SAMPLE_HAND_H_

#include "CanRailMultiDataType.h"


//步进电机序号
typedef enum INDEX_MOTOR_SAMPLE_HAND
{
    INDEX_MOTOR_SAMPLE_HAND_MOVE        = 0X00, //平移电机
    INDEX_MOTOR_SAMPLE_HAND_LEFT_RIGHT  = 0X01, //左右推送电机
    INDEX_MOTOR_SAMPLE_HAND_UP_DOWN     = 0X02, //升降电机
}INDEX_MOTOR_SAMPLE_HAND;

/*************************************发送指令等待ACK************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 输出写出
void CanMasterBoard1SampleHandWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 电机走步数
void CanMasterBoard1SampleHandStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 整体复位
void CanMasterBoard1SampleHandResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr);

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandMoveToTargetPosWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 架子扫描
void CanMasterBoard1SampleHandRackExistScanWhileAck(SYS_RESULT_PACK* resultPackPtr);

//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandCatchTargetRackWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandPutTargetRackWhileAck(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileAck(uint8_t trayPos,uint8_t rackPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileAck(SAMPLE_HAND_POS mainCatchPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 抓取指定位置放到指定位置 非样本仓
void CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileAck(SAMPLE_HAND_POS mainCatchPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileAck(SYS_RESULT_PACK* resultPackPtr);

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileAck(SYS_RESULT_PACK* resultPackPtr);

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandOpenOrCloseScanWhileAck(uint8_t action,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//转运机械手 输出写出
void CanMasterBoard1SampleHandWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//转运机械手 电机走步数
void CanMasterBoard1SampleHandWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//转运机械手 整体复位
void CanMasterBoard1SampleHandWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandWaitMoveToTargetPosResult(SYS_RESULT_PACK* resultPackPtr);

//转运机械手 架子扫描
void CanMasterBoard1SampleHandWaitRackExistScanResult(SYS_RESULT_PACK* resultPackPtr,uint8_t *scanLengthPtr,uint8_t **trayRackExistPtr);

//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandWaitCatchTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandWaitPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandWaitCatchTrayPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandWaitCatchTargetRackPutTrayResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取指定位置放到指定位置 非样本仓
void CanMasterBoard1SampleHandWaitCatchTargetPutTargetRackResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandWaitCatchRotatePutEntryChangeAndScanResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr);

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandWaitRackPutEntryChangeAndScanResult(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr);

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandWaitOpenOrCloseScanResult(SYS_RESULT_PACK* resultPackPtr);




/*************************************发送指令等待完成返回************************************************/
//转运机械手 输入读取
void CanMasterBoard1SampleHandReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr);

//转运机械手 输出写出
void CanMasterBoard1SampleHandWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex);

//转运机械手 指定步进电机复位
void CanMasterBoard1SampleHandStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//转运机械手 电机走步数
void CanMasterBoard1SampleHandStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr);

//转运机械手 整体复位
void CanMasterBoard1SampleHandResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 样本转运机械手运行到指定位置
void CanMasterBoard1SampleHandMoveToTargetPosWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr);

//转运机械手 架子扫描
void CanMasterBoard1SampleHandRackExistScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t *scanLengthPtr,uint8_t **trayRackExistPtr);

//转运机械手 抓取指定位置样本架
void CanMasterBoard1SampleHandCatchTargetRackWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 架子放回指定位置
void CanMasterBoard1SampleHandPutTargetRackWhileReturn(SAMPLE_HAND_POS mainPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取样本仓样本架放到指定位置
void CanMasterBoard1SampleHandCatchTrayPutTargetRackWhileReturn(uint8_t trayPos,uint8_t rackPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取指定位置样本架放到样本仓
void CanMasterBoard1SampleHandCatchTargetRackPutTrayWhileReturn(SAMPLE_HAND_POS mainCatchPos,uint8_t trayPos,uint8_t rackPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手 抓取指定位置放到指定位置 非样本仓
void CanMasterBoard1SampleHandCatchTargetPutTargetRackWhileReturn(SAMPLE_HAND_POS mainCatchPos,SAMPLE_HAND_POS mainPutPos,SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr);

//转运机械手  抓取旋转扫码试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandCatchRotatePutEntryChangeAndScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr);

//转运机械手 手上试管架放入输入变轨变轨并扫码
void CanMasterBoard1SampleHandRackPutEntryChangeAndScanWhileReturn(SYS_RESULT_PACK* resultPackPtr,SAMPLE_RACK_STATE* rackExistPtr,uint8_t *codeExistPtr,uint8_t **codeBuffPtr);

//转运机械手 扫码枪控制
void CanMasterBoard1SampleHandOpenOrCloseScanWhileReturn(uint8_t action,SYS_RESULT_PACK* resultPackPtr);










#endif // _CAN_MASTER_BOARD1_SAMPLE_HAND_H_






























