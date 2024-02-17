/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:08:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 11:45:36 +0800
************************************************************************************************/ 
#ifndef __TASK_CONFIG_SRV_H_
#define __TASK_CONFIG_SRV_H_
#include "TaskConfigBase.h"


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
/*-------------------------------------------网络连接检测--------------------------------------------*/
//是否使能内存使用率展示
#define SRV_HEART_NET_MAIN_PORT_CONNECT_CHECK_ENABLE                    CONFIG_FUNC_ENABLE
//系统内存使用率展示周期        
#define SRV_HEART_PERIOD_MS_NET_MAIN_PORT_CONNECT_CHECK                 1000
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


/********************************************网口接收任务相关配置********************************************************/
/* 网卡协议栈优先级和堆栈容量,上电先将其设置到低优先级，待网线插入后提升优先级到高优先级 */
#define PRIO_SRV_TASK_NETX_IP_PHY_HIGH                                  4
#define PRIO_SRV_TASK_NETX_IP_PHY_LOW                                   31
#define STK_SIZE_SRV_TASK_NETX_IP_PHY                                   4096u

//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_NET_FSM_PERIOD_MS                               10
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_NET_FSM_INTERVAL_MODE                           TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_NET                                        "SrvTaskSocketNet"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_NET                                        5
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_NET                                    32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_NET                                512
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET                            64
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
//是否打开UART3打印
#define SYSTEM_LOG_OUT_UART1_ENABLE                                     CONFIG_FUNC_ENABLE
//是否打开NET SOCKET LOG打印
#define SYSTEM_LOG_OUT_NET_SOCKET_LOG_ENABLE                            CONFIG_FUNC_ENABLE
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
#define SRV_TASK_UTIL_FSM_PERIOD_MS                                     20
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
#define SRV_TASK_FLASH_IAP_FSM_PERIOD_MS                                20
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



#endif


















