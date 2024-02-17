/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 11:37:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_PORT_OUT_H_
#define __SRV_IMPL_BASE_PORT_OUT_H_
#include "SrvImplBase.h"

//输出单个写入            
void SrvImplBasePortOutWriteSingle(uint8_t writeSingleIndex,uint8_t writeSingleVal,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);  

//输出批量写入 
void SrvImplBasePortOutWriteAny(uint8_t writeSinglePair,uint8_t* writePairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);  


#endif









