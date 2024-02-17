/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 14:27:02
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __TASK_CONFIG_COMMON_SRV_H_
#define __TASK_CONFIG_COMMON_SRV_H_
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
#define SRV_HEART_PERIOD_MS_HEART_LED                                   BOARD_LED_GREEN
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
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_UART_FSM_PERIOD_MS                              10
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_UART_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_UART                                       "SrvTaskSocketUart"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_UART                                       2
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_UART                                   16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_UART                               128
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_UART                           8
/**********************************************************************************************************************/

/********************************************CAN1接收任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_CAN1_FSM_PERIOD_MS                              10
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_CAN1_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_CAN1                                       "SrvTaskSocketCan1"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_CAN1                                       3
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_CAN1                                   16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_CAN1                               MCU_CAN1_MSG_FIFO_LENGTH
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN1                           8
/**********************************************************************************************************************/

/********************************************CAN2接收任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_CAN2_FSM_PERIOD_MS                              10
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_CAN2_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_CAN2                                       "SrvTaskSocketCan2"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_CAN2                                       4
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_CAN2                                   16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_CAN2                               MCU_CAN2_MSG_FIFO_LENGTH
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN2                           8
/**********************************************************************************************************************/

/********************************************网口接收任务相关配置********************************************************/
/* 网卡协议栈优先级和堆栈容量,上电先将其设置到低优先级，待网线插入后提升优先级到高优先级 */
#define PRIO_SRV_TASK_NETX_IP_8700_PHY_HIGH                             5
#define PRIO_SRV_TASK_NETX_IP_8700_PHY_LOW                              30
#define STK_SIZE_SRV_TASK_NETX_IP_8700_PHY                              4096u

#if(NETX_FILE_FTP_SERVICE_MODE != CONFIG_FUNC_DISABLE)
#define STK_SIZE_SRV_TASK_NETX_FTP_SOCKET                               4096u
#define PRIO_SRV_TASK_NETX_IP_PHY_FTP                                   NX_FTP_SERVER_PRIORITY
#endif

//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_NET_8700_FSM_PERIOD_MS                          20
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_NET_8700_FSM_INTERVAL_MODE                      TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_NET_8700                                   "SrvTaskSocketNet8700"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_NET_8700                                   6
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_NET_8700                               16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_NET_8700                           512
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET_8700                       32
/**********************************************************************************************************************/


/********************************************网口5500接收任务相关配置*************************************************/
/* 网卡协议栈优先级和堆栈容量,上电先将其设置到低优先级，待网线插入后提升优先级到高优先级 */
#define PRIO_SRV_TASK_NETX_IP_5500_PHY_HIGH                             7
#define PRIO_SRV_TASK_NETX_IP_5500_PHY_LOW                              29
#define STK_SIZE_SRV_TASK_NETX_IP_5500_PHY                              4096u

//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_NET_5500_FSM_PERIOD_MS                          20
//状态机执行间隔逻辑    
#define SRV_TASK_SOCKET_NET_5500_FSM_INTERVAL_MODE                      TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_SOCKET_NET_5500                                   "SrvTaskSSocket5500"
//任务优先级    
#define PRIO_SRV_TASK_SOCKET_NET_5500                                   8
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_SOCKET_NET_5500                               16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_SOCKET_NET_5500                           512
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET_5500                       32
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
#define PRIO_SRV_TASK_SYS_LOG                                           9
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SYS_LOG                                       32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SYS_LOG                                   1024
//是否打开RTT日志打印
#define SYSTEM_LOG_OUT_RTT_ENABLE                                       CONFIG_FUNC_ENABLE
//是否打开UART打印
#define SYSTEM_LOG_OUT_UART_ENABLE                                      CONFIG_FUNC_ENABLE
//是否打开NET LAN8700打印
#define SYSTEM_LOG_OUT_NET_8700_SOCKET                                  CONFIG_FUNC_ENABLE
//是否打开NET 5500打印
#define SYSTEM_LOG_OUT_NET_5500_SOCKET                                  CONFIG_FUNC_ENABLE
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
#define PRIO_SRV_TASK_UTIL                                              10
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_UTIL                                          16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_UTIL                                      1024
//系统探针单条最大长度
#define BLOCK_SIZE_SYSTEM_PROBE                                         2048
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
#define PRIO_SRV_TASK_TIMER                                             11
//任务堆栈大小
#define STK_SIZE_SRV_TASK_TIMER                                         8 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_TIMER                                     8
/**********************************************************************************************************************/

/********************************************输入读取任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_IN_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PORT_IN_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PORT_IN                                           "SrvTaskPortIn"
//任务优先级
#define PRIO_SRV_TASK_PORT_IN                                           12
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PORT_IN                                       8 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PORT_IN                                   8
/**********************************************************************************************************************/

/********************************************输出写入任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_OUT_FSM_PERIOD_MS                                 SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PORT_OUT_FSM_INTERVAL_MODE                             TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PORT_OUT                                          "SrvTaskPortOut"
//任务优先级
#define PRIO_SRV_TASK_PORT_OUT                                          13
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PORT_OUT                                      8 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PORT_OUT                                  8
/**********************************************************************************************************************/

/********************************************参数读写任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PARAM_FSM_PERIOD_MS                                    SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PARAM_FSM_INTERVAL_MODE                                TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PARAM                                             "SrvTaskParam"
//任务优先级
#define PRIO_SRV_TASK_PARAM                                             14
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PARAM                                         8 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PARAM                                     8
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
#define PRIO_SRV_TASK_FLASH_IAP                                         15
//任务堆栈      
#define STK_SIZE_SRV_TASK_FLASH_IAP                                     8 * 128
//任务的消息队列        
#define LENGTH_QUEUE_SRV_TASK_FLASH_IAP                                 8
/**********************************************************************************************************************/

/********************************************CAN数据透传任务相关配置******************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_FORWARD_SOCKET_CAN_FSM_PERIOD_MS                       SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_FORWARD_SOCKET_CAN_FSM_INTERVAL_MODE                   TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_FORWARD_SOCKET_CAN                                "SrvTaskForwardSocketCan"
//任务优先级    
#define PRIO_SRV_TASK_FORWARD_SOCKET_CAN                                16
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_FORWARD_SOCKET_CAN                            8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_CAN                        1024
//CAN数据透传是否展示日志
#define LOG_SHOW_SWITCH_FORWARD_SOCKET_CAN                              CONFIG_FUNC_ENABLE
/**********************************************************************************************************************/

/********************************************NET数据透传任务相关配置******************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_FORWARD_SOCKET_NET_FSM_PERIOD_MS                       SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_FORWARD_SOCKET_NET_FSM_INTERVAL_MODE                   TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_FORWARD_SOCKET_NET                                "SrvTaskForwardSocketNet"
//任务优先级    
#define PRIO_SRV_TASK_FORWARD_SOCKET_NET                                17
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_FORWARD_SOCKET_NET                            8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_NET                        1024
//NET数据透传是否展示日志
#define LOG_SHOW_SWITCH_FORWARD_SOCKET_NET                              CONFIG_FUNC_ENABLE
/**********************************************************************************************************************/

/********************************************UART数据透传任务相关配置******************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_FORWARD_SOCKET_UART_FSM_PERIOD_MS                      SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_FORWARD_SOCKET_UART_FSM_INTERVAL_MODE                  TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_FORWARD_SOCKET_UART                               "SrvTaskForwardSocketUart"
//任务优先级    
#define PRIO_SRV_TASK_FORWARD_SOCKET_UART                               18
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_FORWARD_SOCKET_UART                           8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_UART                       1024
//UART数据透传是否展示日志
#define LOG_SHOW_SWITCH_FORWARD_SOCKET_UART                             CONFIG_FUNC_DISABLE
/**********************************************************************************************************************/

#endif



















