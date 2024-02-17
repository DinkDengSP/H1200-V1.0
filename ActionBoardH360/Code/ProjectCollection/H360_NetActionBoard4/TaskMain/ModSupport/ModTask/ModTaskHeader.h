/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-27 16:06:19 +0800
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

/********************************************模块层 磁珠混匀 任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskMixBead[STK_SIZE_MOD_TASK_MIX_BEAD/8];
//任务控制块
extern TX_THREAD tcbModTaskMixBead;
//任务专用的消息队列
extern TX_QUEUE queueModTaskMixBead;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskMixBead[LENGTH_QUEUE_MOD_TASK_MIX_BEAD];
//任务函数
void ModTaskMixBeadFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 试剂混匀 任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskMixReagent[STK_SIZE_MOD_TASK_MIX_REAGENT/8];
//任务控制块
extern TX_THREAD tcbModTaskMixReagent;
//任务专用的消息队列
extern TX_QUEUE queueModTaskMixReagent;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskMixReagent[LENGTH_QUEUE_MOD_TASK_MIX_REAGENT];
//任务函数
void ModTaskMixReagentFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 试剂针 任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskNeedleReagent[STK_SIZE_MOD_TASK_NEEDLE_REAGENT/8];
//任务控制块
extern TX_THREAD tcbModTaskNeedleReagent;
//任务专用的消息队列
extern TX_QUEUE queueModTaskNeedleReagent;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskNeedleReagent[LENGTH_QUEUE_MOD_TASK_NEEDLE_REAGENT];
//任务函数
void ModTaskNeedleReagentFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 反应盘 任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskPlateReaction[STK_SIZE_MOD_TASK_PLATE_REACTION/8];
//任务控制块
extern TX_THREAD tcbModTaskPlateReaction;
//任务专用的消息队列
extern TX_QUEUE queueModTaskPlateReaction;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskPlateReaction[LENGTH_QUEUE_MOD_TASK_PLATE_REACTION];
//任务函数
void ModTaskPlateReactionFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


#endif




















