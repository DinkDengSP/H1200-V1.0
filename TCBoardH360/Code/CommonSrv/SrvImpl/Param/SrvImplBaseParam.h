/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_PARAM_H_
#define __SRV_IMPL_BASE_PARAM_H_
#include "CommonSrvImplBase.h"

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





