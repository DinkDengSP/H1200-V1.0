/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 14:43:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 14:47:24 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_FLASH_IAP_H_
#define __SRV_IMPL_BASE_FLASH_IAP_H_
#include "SrvImplBase.h"

//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在线升级数据传输
void SrvImplBaseFlashIapDataTrans(uint16_t currentPackIndex,uint16_t packDataLength,uint8_t* writeBufferPtr,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

#endif














