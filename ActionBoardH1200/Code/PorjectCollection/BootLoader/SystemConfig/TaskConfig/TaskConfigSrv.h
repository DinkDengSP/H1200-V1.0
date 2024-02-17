/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 16:21:01
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_SRV_H_
#define __TASK_CONFIG_SRV_H_
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"


/******************************************************启动任务********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_START_HEART_FSM_PERIOD_MS                              10
//状态机执行间隔逻辑
#define SRV_TASK_START_HEART_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//启动任务
#define NAME_SRV_TASK_START_HEART                                      "SrvTaskStartHeart"
//任务优先级
#define PRIO_SRV_TASK_START_HEART                                       1
//任务堆栈大小
#define STK_SIZE_SRV_TASK_START_HEART                                   16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_START_HEART                               16
/*-------------------------------------运行时状态统计-------------------------------------------*/
//是否使能系统运行状态统计
#define SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE                        CONFIG_FUNC_ENABLE
//主板任务执行情况输出显示周期      
#define SRV_HEART_PERIOD_MS_THREAD_INFO_SHOW                            60000
//CPU实时使用率统计周期     
#define SRV_HEART_PERIOD_MS_THREAD_INFO_STATE                           1000
/*----------------------------------------LED定时显示-------------------------------------------*/
//是否打开LED心跳显示
#define SRV_HEART_LED_SHOW_ENABLE                                       CONFIG_FUNC_ENABLE
//LED心跳周期       
#define SRV_HEART_PERIOD_MS_HEART_LED_SHOW                              200
//LED心跳使用的灯序号       
#define SRV_HEART_PERIOD_MS_HEART_LED                                   BOARD_LED_YELLOW
/*----------------------------------------RTC时间显示---------------------------------------------*/
//是否使能RTC时间展示
#define SRV_HEART_RTC_TIMER_SHOW_ENABLE                                 CONFIG_FUNC_ENABLE
//系统内存使用率展示周期        
#define SRV_HEART_PERIOD_MS_RTC_TIMER_SHOW                              60000
/*-----------------------------------------独立看门狗---------------------------------------------*/
//是否使能独立看门狗
#define SRV_HEART_IWDG_ENABLE                                           CONFIG_FUNC_DISABLE
//独立看门狗溢出周期        
#define SRV_HEART_IWDG_OVER_FLOW_TIME                                   1000
/*-------------------------------------------窗口看门狗--------------------------------------------*/
//是否使能窗口看门狗
#define SRV_HEART_WWDG_ENABLE                                           CONFIG_FUNC_DISABLE
//窗口看门狗溢出周期        
#define SRV_HEART_WWDG_OVER_FLOW_TIME                                   SRV_TASK_START_HEART_FSM_PERIOD_MS
/*-------------------------------------------内存使用率--------------------------------------------*/
//是否使能内存使用率展示
#define SRV_HEART_MEM_USAGE_SHOW_ENABLE                                 CONFIG_FUNC_ENABLE
//系统内存使用率展示周期        
#define SRV_HEART_PERIOD_MS_MEM_USAGE_SHOW                              60000
/**********************************************************************************************************************/

/********************************************UART接收任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//是否启用SOCKET UART
#define SRV_COMM_SOCKET_UART_ENABLE                                     CONFIG_FUNC_ENABLE
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_UART_FSM_PERIOD_MS                              20
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_UART_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_UART                                       "SrvTaskSocketUart"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_UART                                       2
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_UART                                   32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_UART                               128
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_UART                           8
/**********************************************************************************************************************/

/********************************************CAN1接收任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//是否启用SOCKET CAN1
#define SRV_COMM_SOCKET_CAN1_ENABLE                                     CONFIG_FUNC_ENABLE
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_CAN1_FSM_PERIOD_MS                              20
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_CAN1_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_CAN1                                       "SrvTaskSocketCan1"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_CAN1                                       3
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_CAN1                                   32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_CAN1                               MCU_CAN1_MSG_FIFO_LENGTH
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN1                           8
/**********************************************************************************************************************/

/********************************************日志任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SYS_LOG_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//系统日志单条最大长度
#define BLOCK_SIZE_SYSTEM_LOG                                           2048
//单行显示缓存数据长度          
#define SYS_BUF_SHOW_SINGLE_LINE                                        20
//状态机执行间隔逻辑
#define SRV_TASK_SYS_LOG_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志任务
#define NAME_SRV_TASK_SYS_LOG                                           "SrvTaskSysLog"
//任务优先级
#define PRIO_SRV_TASK_SYS_LOG                                           6
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SYS_LOG                                       32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SYS_LOG                                   1024
//是否打开RTT日志打印
#define SYSTEM_LOG_OUT_RTT_ENABLE                                       CONFIG_FUNC_ENABLE
//是否打开UART打印
#define SYSTEM_LOG_OUT_UART_ENABLE                                      CONFIG_FUNC_DISABLE
//是否打开CAN打印
#define SYSTEM_LOG_OUT_CAN1_SOCKET                                      CONFIG_FUNC_DISABLE
/**********************************************************************************************************************/


/********************************************定时查询任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_TIMER_FSM_PERIOD_MS                                    20
//状态机执行间隔逻辑
#define SRV_TASK_TIMER_FSM_INTERVAL_MODE                                TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_TIMER                                             "SrvTaskTimer"
//任务优先级
#define PRIO_SRV_TASK_TIMER                                             7
//任务堆栈大小
#define STK_SIZE_SRV_TASK_TIMER                                         16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_TIMER                                     8
/**********************************************************************************************************************/

/********************************************辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_UTIL_FSM_PERIOD_MS                                     SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_UTIL_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_UTIL                                              "SrvTaskUtil"
//任务优先级    
#define PRIO_SRV_TASK_UTIL                                              8
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_UTIL                                          16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_UTIL                                      1024
/**********************************************************************************************************************/

/********************************************在线升级任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_FLASH_IAP_FSM_PERIOD_MS                                SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_FLASH_IAP_FSM_INTERVAL_MODE                            TASK_FSM_TIME_COUNT_MODE_BEFORE
//日志      
#define NAME_SRV_TASK_FLASH_IAP                                         "SrvTaskFlashIap"
//任务优        
#define PRIO_SRV_TASK_FLASH_IAP                                         9
//任务堆栈      
#define STK_SIZE_SRV_TASK_FLASH_IAP                                     16 * 128
//任务的消息队列        
#define LENGTH_QUEUE_SRV_TASK_FLASH_IAP                                 8
/**********************************************************************************************************************/

/********************************************端口输入任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_IN_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PORT_IN_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PORT_IN                                           "SrvTaskPortIn"
//任务优先级    
#define PRIO_SRV_TASK_PORT_IN                                           10
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PORT_IN                                       16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PORT_IN                                   8
/**********************************************************************************************************************/

/********************************************端口输出相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_OUT_FSM_PERIOD_MS                                 SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PORT_OUT_FSM_INTERVAL_MODE                             TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PORT_OUT                                          "SrvTaskPortOut"
//任务优先级    
#define PRIO_SRV_TASK_PORT_OUT                                          11
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PORT_OUT                                      16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PORT_OUT                                  8
/**********************************************************************************************************************/

/********************************************参数读写任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PARAM_FSM_PERIOD_MS                                     SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PARAM_FSM_INTERVAL_MODE                                 TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PARAM                                              "SrvTaskParam"
//任务优先级    
#define PRIO_SRV_TASK_PARAM                                              12
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PARAM                                          16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PARAM                                      8
/**********************************************************************************************************************/











#endif








