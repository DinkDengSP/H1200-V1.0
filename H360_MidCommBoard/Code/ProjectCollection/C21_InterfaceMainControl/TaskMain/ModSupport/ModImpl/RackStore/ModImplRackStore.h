/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 14:11:43
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __MOD_IMPL_RACK_STORE_H_
#define __MOD_IMPL_RACK_STORE_H_
#include "ModImplBase.h"

//任务全局对象初始化
void ModImplRackStoreVarInit(void);

//任务接收消息处理
void ModImplRackStoreMsgProcess(void* taskMsgPtr);

//任务状态机初始化
void ModImplRackStoreFSM_Init(void);

//任务状态机周期运行
void ModImplRackStoreFSM_RunPeriod(void);




#endif







