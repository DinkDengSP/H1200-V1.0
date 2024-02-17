/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-28 19:11:03 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 12:54:21 +0800
************************************************************************************************/ 
#ifndef __TASK_CONFIG_COMMON_SRV_H_
#define __TASK_CONFIG_COMMON_SRV_H_
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"


//服务层时间片
#define TIME_SLICE_SRV_TASK                                             TX_NO_TIME_SLICE


/******************************************************启动任务********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_START_HEART_FSM_PERIOD_MS                              20
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
//RTC时间展示展示周期        
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

/********************************************CAN1接收任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SOCKET_CAN1_FSM_PERIOD_MS                              10
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_CAN1_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_CAN1                                       "SrvTaskSocketCan1"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_CAN1                                       2
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_CAN1                                   32 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_CAN1                               MCU_CAN1_MSG_FIFO_LENGTH
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN1                           8
/**********************************************************************************************************************/

/********************************************端口输入任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_IN_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PORT_IN_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PORT_IN                                           "SrvTaskPortIn"
//任务优先级
#define PRIO_SRV_TASK_PORT_IN                                           4
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PORT_IN                                       16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PORT_IN                                   8
/**********************************************************************************************************************/

/********************************************端口输出任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_OUT_FSM_PERIOD_MS                                 SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PORT_OUT_FSM_INTERVAL_MODE                             TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PORT_OUT                                          "SrvTaskPortOut"
//任务优先级
#define PRIO_SRV_TASK_PORT_OUT                                          5
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PORT_OUT                                      16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PORT_OUT                                  8
/**********************************************************************************************************************/

/********************************************参数读写任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PARAM_RW_FSM_PERIOD_MS                                 SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_PARAM_RW_FSM_INTERVAL_MODE                             TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_PARAM_RW                                          "SrvTaskParamRW"
//任务优先级
#define PRIO_SRV_TASK_PARAM_RW                                          6
//任务堆栈大小
#define STK_SIZE_SRV_TASK_PARAM_RW                                      16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_PARAM_RW                                  8
/**********************************************************************************************************************/

/********************************************步进电机1任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR_1_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_STEP_MOTOR_1_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_STEP_MOTOR_1                                      "SrvTaskStepMotor1"
//任务优先级
#define PRIO_SRV_TASK_STEP_MOTOR_1                                      7
//任务堆栈大小
#define STK_SIZE_SRV_TASK_STEP_MOTOR_1                                  16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR_1                              8
/**********************************************************************************************************************/

/********************************************步进电机2任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR_2_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_STEP_MOTOR_2_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_STEP_MOTOR_2                                      "SrvTaskStepMotor2"
//任务优先级
#define PRIO_SRV_TASK_STEP_MOTOR_2                                      8
//任务堆栈大小
#define STK_SIZE_SRV_TASK_STEP_MOTOR_2                                  16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR_2                              8
/**********************************************************************************************************************/

/********************************************步进电机3任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR_3_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_STEP_MOTOR_3_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_STEP_MOTOR_3                                      "SrvTaskStepMotor3"
//任务优先级
#define PRIO_SRV_TASK_STEP_MOTOR_3                                      9
//任务堆栈大小
#define STK_SIZE_SRV_TASK_STEP_MOTOR_3                                  16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR_3                              8
/**********************************************************************************************************************/

/********************************************步进电机4任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR_4_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_STEP_MOTOR_4_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_STEP_MOTOR_4                                      "SrvTaskStepMotor4"
//任务优先级
#define PRIO_SRV_TASK_STEP_MOTOR_4                                      10
//任务堆栈大小
#define STK_SIZE_SRV_TASK_STEP_MOTOR_4                                  16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR_4                              8
/**********************************************************************************************************************/

/********************************************步进电机5任务相关配置********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR_5_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_STEP_MOTOR_5_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_STEP_MOTOR_5                                      "SrvTaskStepMotor5"
//任务优先级
#define PRIO_SRV_TASK_STEP_MOTOR_5                                      11
//任务堆栈大小
#define STK_SIZE_SRV_TASK_STEP_MOTOR_5                                  16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR_5                              8
/**********************************************************************************************************************/

/********************************************日志任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_SYS_LOG_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑
#define SRV_TASK_SYS_LOG_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SYS_LOG                                           "SrvTaskSysLog"
//任务优先级
#define PRIO_SRV_TASK_SYS_LOG                                           12
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SYS_LOG                                       16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SYS_LOG                                   1024
//系统日志单条最大长度
#define BLOCK_SIZE_SYSTEM_LOG                                           2048
//单行显示缓存数据长度          
#define SYS_BUF_SHOW_SINGLE_LINE                                        20
//是否打开RTT日志打印
#define SYSTEM_LOG_OUT_RTT_ENABLE                                       CONFIG_FUNC_ENABLE
//是否打开UART1打印
#define SYSTEM_LOG_OUT_UART1_ENABLE                                     CONFIG_FUNC_DISABLE
//是否打开UART3打印
#define SYSTEM_LOG_OUT_UART3_ENABLE                                     CONFIG_FUNC_DISABLE
//是否使用CAN1 SOCKET1打印
#define SYSTEM_LOG_OUT_SOCKET_CAN1_ENABLE                               CONFIG_FUNC_ENABLE
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
#define PRIO_SRV_TASK_TIMER                                             13
//任务堆栈大小
#define STK_SIZE_SRV_TASK_TIMER                                         16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_TIMER                                     8
/**********************************************************************************************************************/

/********************************************辅助任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_UTIL_FSM_PERIOD_MS                                    SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_UTIL_FSM_INTERVAL_MODE                                TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_UTIL                                             "SrvTaskUtil"
//任务优先级    
#define PRIO_SRV_TASK_UTIL                                             14
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_UTIL                                         16 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_UTIL                                     1024
//系统探针是用于系统信号采集用的
#define BLOCK_SIZE_SYSTEM_PROBE                                        2048
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
#define STK_SIZE_SRV_TASK_FLASH_IAP                                     16 * 128
//任务的消息队列        
#define LENGTH_QUEUE_SRV_TASK_FLASH_IAP                                 8
/**********************************************************************************************************************/




#endif


