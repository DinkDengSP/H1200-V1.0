/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-31 15:47:25
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_SRV_H_
#define __TASK_CONFIG_SRV_H_
#include "TaskConfigCommonSrv.h"

/********************************************服务层辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_DEMO_FSM_PERIOD_MS                                     100
//状态机执行间隔逻辑
#define SRV_TASK_DEMO_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_SRV_TASK_DEMO                                              "SrvTaskDemo"
//任务优先级
#define PRIO_SRV_TASK_DEMO                                              20
//任务堆栈大小
#define STK_SIZE_SRV_TASK_DEMO                                          16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_DEMO                                      8
/**********************************************************************************************************************/


#endif






