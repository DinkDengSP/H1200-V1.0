/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 09:40:59
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_FLASH_IAP_H_
#define __SRV_IMPL_BASE_FLASH_IAP_H_
#include "SrvImplBase.h"


//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级数据传输
void SrvImplBaseFlashIapDataTrans(uint16_t currentPackIndex,uint16_t packDataLength,uint8_t* writeBufferPtr,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif









