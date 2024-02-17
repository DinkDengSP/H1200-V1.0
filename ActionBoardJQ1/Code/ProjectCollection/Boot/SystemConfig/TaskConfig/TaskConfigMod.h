/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-25 10:59:43 +0800
************************************************************************************************/ 
#ifndef __TASK_CONFIG_MOD_H_
#define __TASK_CONFIG_MOD_H_
#include "TaskConfigSrv.h"

//模块层时间片
#define TIME_SLICE_MOD_TASK                                             TX_NO_TIME_SLICE

/********************************************模块层辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define MOD_TASK_UTIL_FSM_PERIOD_MS                                     20
//状态机执行间隔逻辑
#define MOD_TASK_UTIL_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_MOD_TASK_UTIL                                              "ModTaskUtil"
//任务优先级
#define PRIO_MOD_TASK_UTIL                                              20
//任务堆栈大小
#define STK_SIZE_MOD_TASK_UTIL                                          16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_MOD_TASK_UTIL                                      8
/**********************************************************************************************************************/



#endif























