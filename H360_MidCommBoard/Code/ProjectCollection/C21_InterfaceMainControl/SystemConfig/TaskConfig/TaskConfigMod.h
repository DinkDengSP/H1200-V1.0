/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_MOD_H_
#define __TASK_CONFIG_MOD_H_
#include "TaskConfigSrv.h"

/********************************************模块层辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define MOD_TASK_DEMO_FSM_PERIOD_MS                                     SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define MOD_TASK_DEMO_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_MOD_TASK_DEMO                                              "ModTaskDemo"
//任务优先级
#define PRIO_MOD_TASK_DEMO                                              32
//任务堆栈大小
#define STK_SIZE_MOD_TASK_DEMO                                          16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_MOD_TASK_DEMO                                      8
/**********************************************************************************************************************/


/********************************************模块层样本架移动任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define MOD_TASK_RACK_MOVE_FSM_PERIOD_MS                                SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define MOD_TASK_RACK_MOVE_FSM_INTERVAL_MODE                            TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_MOD_TASK_RACK_MOVE                                         "ModTaskRackMove"
//任务优先级
#define PRIO_MOD_TASK_RACK_MOVE                                         33
//任务堆栈大小
#define STK_SIZE_MOD_TASK_RACK_MOVE                                     32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_MOD_TASK_RACK_MOVE                                 8
/**********************************************************************************************************************/

/********************************************模块层样本架装载辅助查询任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define MOD_TASK_RACK_STORE_FSM_PERIOD_MS                               SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑        
#define MOD_TASK_RACK_STORE_FSM_INTERVAL_MODE                           TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务      
#define NAME_MOD_TASK_RACK_STORE                                        "ModTaskRackStore"
//任务优先级        
#define PRIO_MOD_TASK_RACK_STORE                                        34
//任务堆栈大小      
#define STK_SIZE_MOD_TASK_RACK_STORE                                    32 * 128
//任务的消息队列长度        
#define LENGTH_QUEUE_MOD_TASK_RACK_STORE                                8
/**********************************************************************************************************************/


#endif







