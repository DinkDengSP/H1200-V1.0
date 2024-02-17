/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_IMPL_BASE_PARAM_H_
#define __SRV_IMPL_BASE_PARAM_H_
#include "SrvImplBase.h"

//参数单个读取        
void SrvImplBaseParamReadSingle(uint8_t mainIndex,uint8_t subIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//参数批量读取
void SrvImplBaseParamReadAny(uint8_t readParamPair,uint8_t* readParaPairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen); 

//参数单个写入   
void SrvImplBaseParamWriteSingle(uint8_t mainIndex,uint8_t subIndex,int32_t writeParam,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);
                                        
//参数批量写入
void SrvImplBaseParamWriteAny(uint8_t writeParamPair,uint8_t* writeParamPairArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen);  


#endif




