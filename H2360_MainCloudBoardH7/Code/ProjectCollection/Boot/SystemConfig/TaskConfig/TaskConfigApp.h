/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:08:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:41:23 +0800
************************************************************************************************/ 
#ifndef __TASK_CONFIG_APP_H_
#define __TASK_CONFIG_APP_H_
#include "TaskConfigMod.h"

/********************************************应用层辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define APP_TASK_UTIL_FSM_PERIOD_MS                                     20
//状态机执行间隔逻辑
#define APP_TASK_UTIL_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_APP_TASK_UTIL                                              "AppTaskUtil"
//任务优先级
#define PRIO_APP_TASK_UTIL                                              48
//任务堆栈大小
#define STK_SIZE_APP_TASK_UTIL                                          16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_APP_TASK_UTIL                                      8
/**********************************************************************************************************************/


#endif

















