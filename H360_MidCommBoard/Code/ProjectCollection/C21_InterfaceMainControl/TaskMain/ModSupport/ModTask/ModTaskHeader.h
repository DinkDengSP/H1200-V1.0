/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 11:37:01
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __MOD_TASK_HEADER_H_
#define __MOD_TASK_HEADER_H_
#include "TaskConfigMod.h"
#include "SrvTaskHeader.h"

/********************************************模块层辅助任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskDemo[STK_SIZE_MOD_TASK_DEMO/8];
//任务控制块
extern TX_THREAD tcbModTaskDemo;
//任务专用的消息队列
extern TX_QUEUE queueModTaskDemo;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskDemo[LENGTH_QUEUE_MOD_TASK_DEMO];
//任务函数
void ModTaskDemoFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


/********************************************模块层样本架移动任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskRackMove[STK_SIZE_MOD_TASK_RACK_MOVE/8];
//任务控制块
extern TX_THREAD tcbModTaskRackMove;
//任务专用的消息队列
extern TX_QUEUE queueModTaskRackMove;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskRackMove[LENGTH_QUEUE_MOD_TASK_RACK_MOVE];
//任务函数
void ModTaskRackMoveFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层样本架装载辅助查询任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskRackStore[STK_SIZE_MOD_TASK_RACK_STORE/8];
//任务控制块
extern TX_THREAD tcbModTaskRackStore;
//任务专用的消息队列
extern TX_QUEUE queueModTaskRackStore;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskRackStore[LENGTH_QUEUE_MOD_TASK_RACK_STORE];
//任务函数
void ModTaskRackStoreFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/



#endif






