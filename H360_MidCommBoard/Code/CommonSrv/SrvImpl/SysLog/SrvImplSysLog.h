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
#ifndef __SRV_IMPL_SYS_LOG_H_
#define __SRV_IMPL_SYS_LOG_H_
#include "CommonSrvImplBase.h"

//任务全局对象初始化
void SrvImplSysLogVarInit(void);

//任务接收消息处理
void SrvImplSysLogMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void SrvImplSysLogFSM_Init(void);

//任务状态机周期运行
void SrvImplSysLogFSM_RunPeriod(void);

#endif










