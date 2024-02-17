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
#ifndef __SRV_IMPL_BASE_UTIL_H_
#define __SRV_IMPL_BASE_UTIL_H_
#include "SrvImplBase.h"

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

//设置运行模式
void SrvImplBaseUtilSetRunningMode(SYS_RUNNING_MODE sysRunningMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif









