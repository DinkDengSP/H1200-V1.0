/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 09:55:52 +0800
************************************************************************************************/ 
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












