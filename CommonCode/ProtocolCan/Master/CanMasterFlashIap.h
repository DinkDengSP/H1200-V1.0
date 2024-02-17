/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 10:56:30 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_FLASH_IAP_H_
#define __CAN_MASTER_FLASH_IAP_H_
#include "CanMasterUtil.h"


/*************************************发送指令等待ACK************************************************/
//升级准备
void CanMasterFlashIapPrepareWhileAck(uint8_t targetModule,uint8_t targetBoardID,BOARD_IAP_MODE iapMode,
                                                SYS_RESULT_PACK* resultPackPtr);
//升级开始
void CanMasterFlashIapStartWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteTotal,uint32_t packTotal,
                                                uint32_t singlePackLength,SYS_RESULT_PACK* resultPackPtr);
//升级数据
void CanMasterFlashIapDataWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint16_t packIndex,uint8_t* dataBufPtr,
                                            uint16_t dataLength,SYS_RESULT_PACK* resultPackPtr);
//升级校验
void CanMasterFlashIapCheckWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint32_t crcCode,
                                                SYS_RESULT_PACK* resultPackPtr);
//升级强制跳转
void CanMasterFlashIapJmpWhileAck(uint8_t targetModule,uint8_t targetBoardID,BOARD_FORCE_JMP jmpTarget,
                                            SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//升级准备
void CanMasterFlashIapPrepareWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//升级开始
void CanMasterFlashIapStartWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//升级数据
void CanMasterFlashIapDataWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//升级校验
void CanMasterFlashIapCheckWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//升级强制跳转
void CanMasterFlashIapJmpWaitResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//升级准备
void CanMasterFlashIapPrepareWhileReturn(uint8_t targetModule,uint8_t targetBoardID,BOARD_IAP_MODE iapMode,
                                                SYS_RESULT_PACK* resultPackPtr);
//升级开始
void CanMasterFlashIapStartWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteTotal,uint32_t packTotal,
                                                uint32_t singlePackLength,SYS_RESULT_PACK* resultPackPtr);
//升级数据
void CanMasterFlashIapDataWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint16_t packIndex,uint8_t* dataBufPtr,
                                            uint16_t dataLength,SYS_RESULT_PACK* resultPackPtr);
//升级校验
void CanMasterFlashIapCheckWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint32_t crcCode,
                                            SYS_RESULT_PACK* resultPackPtr);
//升级强制跳转
void CanMasterFlashIapJmpWhileReturn(uint8_t targetModule,uint8_t targetBoardID,BOARD_FORCE_JMP jmpTarget,
                                            SYS_RESULT_PACK* resultPackPtr);



#endif



















