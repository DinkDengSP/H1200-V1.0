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
#ifndef __APP_IMPL_TEST_FLOW_H_
#define __APP_IMPL_TEST_FLOW_H_
#include "AppImplBase.h"

//任务全局对象初始化
void AppImplTestFlowVarInit(void);

//任务接收消息处理
void AppImplTestFlowMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void AppImplTestFlowFSM_Init(void);

//任务状态机周期运行
void AppImplTestFlowFSM_RunPeriod(void);




#endif













