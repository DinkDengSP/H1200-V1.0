/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-27 13:19:36 +0800
************************************************************************************************/ 
#ifndef __MOD_TASK_CONFIG_H_
#define __MOD_TASK_CONFIG_H_
#include "TaskConfigMod.h"
#include "SrvTaskHeader.h"


/********************************************模块层辅助任务相关配置********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskUtil[STK_SIZE_MOD_TASK_UTIL/8];
//任务控制块
extern TX_THREAD tcbModTaskUtil;
//任务专用的消息队列
extern TX_QUEUE queueModTaskUtil;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskUtil[LENGTH_QUEUE_MOD_TASK_UTIL];
//任务函数
void ModTaskUtilFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 清洗机械手 任务相关配置*******************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskHandWash[STK_SIZE_MOD_TASK_HAND_WASH/8];
//任务控制块
extern TX_THREAD tcbModTaskHandWash;
//任务专用的消息队列
extern TX_QUEUE queueModTaskHandWash;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskHandWash[LENGTH_QUEUE_MOD_TASK_HAND_WASH];
//任务函数
void ModTaskHandWashFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 测量模块 任务相关配置*******************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskMeasureModule[STK_SIZE_MOD_TASK_MEASURE_MODULE/8];
//任务控制块
extern TX_THREAD tcbModTaskMeasureModule;
//任务专用的消息队列
extern TX_QUEUE queueModTaskMeasureModule;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskMeasureModule[LENGTH_QUEUE_MOD_TASK_MEASURE_MODULE];
//任务函数
void ModTaskMeasureModuleFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/



#endif




















