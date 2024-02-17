/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-07 10:24:17 +0800
************************************************************************************************/ 
#ifndef __SRV_IMPL_BASE_UTIL_H_
#define __SRV_IMPL_BASE_UTIL_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplSysProbeVarInit(void);
//探针数据发送函数
void SystemProbePrintf(const char *format, ...);
//探针数据发送字符串
void SystemProbeSendString(uint8_t* strPtr);
//探针数据发送缓冲区
void SystemProbeSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);

//联机
void SrvImplBaseUtilOnline(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//脱机
void SrvImplBaseUtilOffLine(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//UTC同步
void SrvImplBaseUtilUtcSync(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//心跳获取
void SrvImplBaseUtilHeartGet(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//急停
void SrvImplBaseUtilEmStop(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取版本号
void SrvImplBaseUtilGetVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//参数单个读取        
void SrvImplBaseUtilParamReadSingle(uint8_t mainIndex,uint8_t subIndex,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 

//参数批量读取
void SrvImplBaseUtilParamReadAny(uint8_t readParamPair,uint8_t* readParaPairDataArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen); 

//参数单个写入   
void SrvImplBaseUtilParamWriteSingle(uint8_t mainIndex,uint8_t subIndex,int32_t writeParam,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                        uint16_t* dataBufLen);
                                        
//参数批量写入
void SrvImplBaseUtilParamWriteAny(uint8_t writeParamPair,uint8_t* writeParamPairArrayPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                    uint16_t* dataBufLen);  

#endif

















