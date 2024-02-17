/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-07 09:19:48 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_FLASH_IAP_H_
#define __SRV_IMPL_BASE_FLASH_IAP_H_
#include "SrvImplBase.h"

//SERVICE升级
void SrvImplBaseFlashIapServiceUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//APP升级
void SrvImplBaseFlashIapAppUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//在线升级准备
void SrvImplBaseFlashIapPrepare(uint8_t fileIapMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级数据传输
void SrvImplBaseFlashIapDataTrans(uint16_t currentPackIndex,uint16_t packDataLength,uint8_t* writeBufferPtr,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

#endif

















