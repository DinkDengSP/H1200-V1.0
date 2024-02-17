/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 18:38:52 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-26 14:47:35 +0800
************************************************************************************************/ 
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















