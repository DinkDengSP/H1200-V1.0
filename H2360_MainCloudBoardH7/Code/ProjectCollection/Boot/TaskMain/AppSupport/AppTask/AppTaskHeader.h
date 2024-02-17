/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 11:24:45 +0800
************************************************************************************************/ 
#ifndef __APP_TASK_HEADER_H_
#define __APP_TASK_HEADER_H_
#include "TaskConfigApp.h"
#include "ModTaskHeader.h"

/********************************************应用层辅助任务相关配置***********************************************************/
//任务堆栈
extern uint64_t stackBufferAppTaskUtil[STK_SIZE_APP_TASK_UTIL/8];
//任务控制块
extern TX_THREAD tcbAppTaskUtil;
//任务专用的消息队列
extern TX_QUEUE queueAppTaskUtil;
//任务的消息队列缓冲区
extern uint32_t queueBufferAppTaskUtil[LENGTH_QUEUE_APP_TASK_UTIL];
//任务函数
void AppTaskUtilFuncEntry(ULONG threadInput);
/**********************************************************************************************************************/



#endif
















