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

//探针数据发送数组显示
void SysProbeShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);

//探针数据发送
void SrvImplBaseUtilProbeSendBuffer(uint8_t* sendBufferPtr,uint16_t bufferLength);

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


#endif








