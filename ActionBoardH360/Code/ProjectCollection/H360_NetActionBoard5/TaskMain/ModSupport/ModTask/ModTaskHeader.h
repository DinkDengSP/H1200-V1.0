/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-28 11:41:52 +0800
************************************************************************************************/ 
#ifndef __MOD_TASK_CONFIG_H_
#define __MOD_TASK_CONFIG_H_
#include "TaskConfigMod.h"
#include "SrvTaskHeader.h"


/********************************************模块层辅助任务相关配置*******************************************************/
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

/********************************************模块层 新杯机械手 任务相关配置*******************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskHandNewCup[STK_SIZE_MOD_TASK_HAND_NEW_CUP/8];
//任务控制块
extern TX_THREAD tcbModTaskHandNewCup;
//任务专用的消息队列
extern TX_QUEUE queueModTaskHandNewCup;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskHandNewCup[LENGTH_QUEUE_MOD_TASK_HAND_NEW_CUP];
//任务函数
void ModTaskHandNewCupFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 样本针 任务相关配置*******************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskNeedleSample[STK_SIZE_MOD_TASK_NEEDLE_SAMPLE/8];
//任务控制块
extern TX_THREAD tcbModTaskNeedleSample;
//任务专用的消息队列
extern TX_QUEUE queueModTaskNeedleSample;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskNeedleSample[LENGTH_QUEUE_MOD_TASK_NEEDLE_SAMPLE];
//任务函数
void ModTaskNeedleSampleFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

#endif




















