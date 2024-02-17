/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:19:37 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PORT_IN_H_
#define __CAN_MASTER_PORT_IN_H_
#include "CanMasterUtil.h"

/*************************************发送指令等待ACK************************************************/
//输入单个读取
void CanMasterPortInReadSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,SYS_RESULT_PACK* resultPackPtr);
//输入区间读取
void CanMasterPortInReadRangeWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t startPinIndex,
                                                uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr);
//输入批量读取
void CanMasterPortInReadAnyWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArray,
                                                uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//输入单个读取
void CanMasterPortInWaitReadSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStatePtr);
//输入区间读取
void CanMasterPortInWaitReadRangeResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStateArrayPtr);
//输入批量读取
void CanMasterPortInWaitReadAnyResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState* pinStateArrayPtr);

/*************************************发送指令等待完成返回************************************************/
//输入单个读取
void CanMasterPortInReadSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr);
//输入区间读取
void CanMasterPortInReadRangeWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t startPinIndex,uint8_t pinCount,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStateArrayPtr);
//输入批量读取
void CanMasterPortInReadAnyWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArray,uint8_t pinCount,
                                                    SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStateArrayPtr);


#endif


























