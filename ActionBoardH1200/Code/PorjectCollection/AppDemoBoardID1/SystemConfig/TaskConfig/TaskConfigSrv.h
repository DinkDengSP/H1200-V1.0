/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 15:02:35
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_SRV_H_
#define __TASK_CONFIG_SRV_H_
#include "TaskConfigCommonSrv.h"


/********************************************服务层Demo任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_DEMO_FSM_PERIOD_MS                                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑                
#define SRV_TASK_DEMO_FSM_INTERVAL_MODE                                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称              
#define NAME_SRV_TASK_DEMO                                                      "SrvTaskDemo"
//任务优先级                
#define PRIO_SRV_TASK_DEMO                                                      31
//任务堆栈大小              
#define STK_SIZE_SRV_TASK_DEMO                                                  16 * 128
//任务的消息队列长度                
#define LENGTH_QUEUE_SRV_TASK_DEMO                                              4
/**********************************************************************************************************************/



#endif








