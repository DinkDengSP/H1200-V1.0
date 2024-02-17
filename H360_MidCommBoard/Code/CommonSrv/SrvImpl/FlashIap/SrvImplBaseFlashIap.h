/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_FLASH_IAP_H_
#define __SRV_IMPL_BASE_FLASH_IAP_H_
#include "CommonSrvImplBase.h"

//BOOT升级
void SrvImplBaseFlashIapBootUpdata(BOARD_IAP_RECODER* iapRecordPtr);

//在线升级准备
void SrvImplBaseFlashIapPrepare(uint8_t fileIapMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级数据传输
void SrvImplBaseFlashIapDataTrans(uint16_t currentPackIndex,uint16_t packDataLength,uint8_t* writeBufferPtr,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

#endif








