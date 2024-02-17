/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:58:37 +0800
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

/********************************************模块层反应盘控温任务相关配置***********************************************************/
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

/********************************************模块层清洗盘控温控温任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskPlateWash[STK_SIZE_MOD_TASK_PLATE_WASH/8];
//任务控制块
extern TX_THREAD tcbModTaskPlateWash;
//任务专用的消息队列
extern TX_QUEUE queueModTaskPlateWash;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskPlateWash[LENGTH_QUEUE_MOD_TASK_PLATE_WASH];
//任务函数
void ModTaskPlateWashFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层试剂盘控温任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskPlateReagent[STK_SIZE_MOD_TASK_PLATE_REAGENT/8];
//任务控制块
extern TX_THREAD tcbModTaskPlateReagent;
//任务专用的消息队列
extern TX_QUEUE queueModTaskPlateReagent;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskPlateReagent[LENGTH_QUEUE_MOD_TASK_PLATE_REAGENT];
//任务函数
void ModTaskPlateReagentFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层环境温度控温任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskEnvironment[STK_SIZE_MOD_TASK_ENVIRONMENT/8];
//任务控制块
extern TX_THREAD tcbModTaskEnvironment;
//任务专用的消息队列
extern TX_QUEUE queueModTaskEnvironment;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskEnvironment[LENGTH_QUEUE_MOD_TASK_ENVIRONMENT];
//任务函数
void ModTaskEnvironmentFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


#endif













