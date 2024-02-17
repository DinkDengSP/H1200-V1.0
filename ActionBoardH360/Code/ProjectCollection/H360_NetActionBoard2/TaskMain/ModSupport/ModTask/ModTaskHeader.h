/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 10:25:38 +0800
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

/********************************************模块层磁珠针任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskNeedleBead[STK_SIZE_MOD_TASK_NEEDLE_BEAD/8];
//任务控制块
extern TX_THREAD tcbModTaskNeedleBead;
//任务专用的消息队列
extern TX_QUEUE queueModTaskNeedleBead;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskNeedleBead[LENGTH_QUEUE_MOD_TASK_NEEDLE_BEAD];
//任务函数
void ModTaskNeedleBeadFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/


/********************************************模块层试剂盘任务相关配置***********************************************************/
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


/********************************************模块层杯栈管理任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferModTaskCupTrayManage[STK_SIZE_MOD_TASK_CUP_TRAY_MANAGE/8];
//任务控制块
extern TX_THREAD tcbModTaskCupTrayManage;
//任务专用的消息队列
extern TX_QUEUE queueModTaskCupTrayManage;
//任务的消息队列缓冲区
extern uint32_t queueBufferModTaskCupTrayManage[LENGTH_QUEUE_MOD_TASK_CUP_TRAY_MANAGE];
//任务函数
void ModTaskCupTrayManageFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/



#endif




















