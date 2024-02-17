/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 10:58:20
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


/********************************************应用层消息处理任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferAppTaskMsgProc[STK_SIZE_APP_TASK_MSG_PROC/8];
//任务控制块
extern TX_THREAD tcbAppTaskMsgProc;
//任务专用的消息队列
extern TX_QUEUE queueAppTaskMsgProc;
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskMsgProc[LENGTH_QUEUE_APP_TASK_MSG_PROC];
//任务函数
void AppTaskMsgProcFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************应用层流程处理任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferAppTaskTestFlow[STK_SIZE_APP_TASK_TEST_FLOW/8];
//任务控制块
extern TX_THREAD tcbAppTaskTestFlow;
//任务专用的消息队列
extern TX_QUEUE queueAppTaskTestFlow;
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskTestFlow[LENGTH_QUEUE_APP_TASK_TEST_FLOW];
//任务函数
void AppTaskTestFlowFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/




#endif









