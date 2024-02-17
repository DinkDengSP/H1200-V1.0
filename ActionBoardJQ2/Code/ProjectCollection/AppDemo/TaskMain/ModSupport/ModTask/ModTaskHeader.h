/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 11:29:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 11:41:49 +0800
************************************************************************************************/ 
#ifndef __MOD_TASK_CONFIG_H_
#define __MOD_TASK_CONFIG_H_
#include "TaskConfigMod.h"
#include "SrvTaskHeader.h"


/********************************************模块层辅助任务相关配置***********************************************************/
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


/********************************************模块层试管架存储仓任务相关配置***********************************************************/
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

