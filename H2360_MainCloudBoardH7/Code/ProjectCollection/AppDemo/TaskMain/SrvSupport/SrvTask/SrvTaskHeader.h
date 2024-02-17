/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __SRV_TASK_HEADER_H_
#define __SRV_TASK_HEADER_H_
#include "TaskConfigSrv.h"
#include "CommonSrvTaskHeader.h"

/********************************************DEMO任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskDemo[STK_SIZE_SRV_TASK_DEMO/8];
//任务控制块
extern TX_THREAD tcbSrvTaskDemo;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskDemo;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskDemo[LENGTH_QUEUE_SRV_TASK_DEMO];
//任务函数
void SrvTaskDemoFuncEntry(ULONG threadInput);
/***************************************************************************************************************/


#endif

















