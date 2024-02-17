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
#ifndef __SRV_IMPL_BASE_PORT_IN_H_
#define __SRV_IMPL_BASE_PORT_IN_H_
#include "CommonSrvImplBase.h"

//输入单个读取
void SrvImplBasePortInReadSingle(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//输入区间读取        
void SrvImplBasePortInReadRange(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);  

//输入批量读取
void SrvImplBasePortInReadAny(uint8_t readSingleCount,uint8_t* readSingalIdxArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen);


#endif









