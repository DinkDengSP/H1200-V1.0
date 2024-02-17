/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 22:50:03 +0800
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

/********************************************模块层 液路自动化 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidAuto[STK_SIZE_MOD_TASK_LIQUID_AUTO/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidAuto;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidAuto;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidAuto[LENGTH_QUEUE_MOD_TASK_LIQUID_AUTO];
//任务函数
void ModTaskLiquidAutoFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 缓冲液液路 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidBuffer[STK_SIZE_MOD_TASK_LIQUID_BUFFER/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidBuffer;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidBuffer;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidBuffer[LENGTH_QUEUE_MOD_TASK_LIQUID_BUFFER];
//任务函数
void ModTaskLiquidBufferFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 激发液液路 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidExcita[STK_SIZE_MOD_TASK_LIQUID_EXCITA/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidExcita;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidExcita;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidExcita[LENGTH_QUEUE_MOD_TASK_LIQUID_EXCITA];
//任务函数
void ModTaskLiquidExcitaFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 磁珠针液路 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidNeedleBead[STK_SIZE_MOD_TASK_LIQUID_NEEDLE_BEAD/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidNeedleBead;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidNeedleBead;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidNeedleBead[LENGTH_QUEUE_MOD_TASK_LIQUID_NEEDLE_BEAD];
//任务函数
void ModTaskLiquidNeedleBeadFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 试剂针液路 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidNeedleReagent[STK_SIZE_MOD_TASK_LIQUID_NEEDLE_REAGENT/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidNeedleReagent;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidNeedleReagent;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidNeedleReagent[LENGTH_QUEUE_MOD_TASK_LIQUID_NEEDLE_REAGENT];
//任务函数
void ModTaskLiquidNeedleReagentFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 样本针液路 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskLiquidNeedleSample[STK_SIZE_MOD_TASK_LIQUID_NEEDLE_SAMPLE/8];
//任务控制块
extern TX_THREAD tcbModTaskLiquidNeedleSample;
//任务专用的消息队列
extern TX_QUEUE queueModTaskLiquidNeedleSample;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskLiquidNeedleSample[LENGTH_QUEUE_MOD_TASK_LIQUID_NEEDLE_SAMPLE];
//任务函数
void ModTaskLiquidNeedleSampleFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 清洗混匀 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskMixWash[STK_SIZE_MOD_TASK_MIX_WASH/8];
//任务控制块
extern TX_THREAD tcbModTaskMixWash;
//任务专用的消息队列
extern TX_QUEUE queueModTaskMixWash;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskMixWash[LENGTH_QUEUE_MOD_TASK_MIX_WASH];
//任务函数
void ModTaskMixWashFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 清洗针 任务相关配置*************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskNeedleWash[STK_SIZE_MOD_TASK_NEEDLE_WASH/8];
//任务控制块
extern TX_THREAD tcbModTaskNeedleWash;
//任务专用的消息队列
extern TX_QUEUE queueModTaskNeedleWash;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskNeedleWash[LENGTH_QUEUE_MOD_TASK_NEEDLE_WASH];
//任务函数
void ModTaskNeedleWashFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/

/********************************************模块层 清洗盘 任务相关配置*************************************************/
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


#endif




















