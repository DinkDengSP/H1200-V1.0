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
#ifndef __APP_TASK_HEADER_H_
#define __APP_TASK_HEADER_H_
#include "TaskConfigApp.h"
#include "ModTaskHeader.h"

/********************************************应用层辅助任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferAppTaskDemo[STK_SIZE_APP_TASK_DEMO/8];
//任务控制块
extern TX_THREAD tcbAppTaskDemo;
//任务专用的消息队列
extern TX_QUEUE queueAppTaskDemo;
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskDemo[LENGTH_QUEUE_APP_TASK_DEMO];
//任务函数
void AppTaskDemoFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


#endif









