/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 15:52:43
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_TASK_HEADER_H_
#define __SRV_TASK_HEADER_H_
#include "TaskConfigSrv.h"
#include "CommonSrvTaskHeader.h"

/********************************************模块层辅助任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskDemo[STK_SIZE_SRV_TASK_DEMO/8];
//任务控制块
extern TX_THREAD tcbSrvTaskDemo;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskDemo;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskDemo[LENGTH_QUEUE_SRV_TASK_DEMO];
//任务函数
void SrvTaskDemoFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


#endif










