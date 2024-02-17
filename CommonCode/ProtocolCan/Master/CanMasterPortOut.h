/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:30 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PORT_OUT_H_
#define __CAN_MASTER_PORT_OUT_H_
#include "CanMasterUtil.h"

/*************************************发送指令等待ACK************************************************/
//输出单个写入
void CanMasterPortOutWriteSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,GPIO_PinState pinState,
                                            SYS_RESULT_PACK* resultPackPtr);
//输出批量写入
void CanMasterPortOutWriteAnyWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArrayPtr,GPIO_PinState* pinStateArrayPtr,
                                                    uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//输出单个写入
void CanMasterPortOutWaitWriteSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//输出批量写入
void CanMasterPortOutWaitWriteAnyResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//输出单个写入
void CanMasterPortOutWriteSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t pinIndex,GPIO_PinState pinState,
                                                    SYS_RESULT_PACK* resultPackPtr);
//输出批量写入
void CanMasterPortOutWriteAnyWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* pinIndexArrayPtr,GPIO_PinState* pinStateArrayPtr,
                                                    uint8_t pinCount,SYS_RESULT_PACK* resultPackPtr);


#endif



















