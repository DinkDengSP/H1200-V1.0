/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 10:03:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 13:47:58 +0800
************************************************************************************************/ 
#ifndef __TASK_CONFIG_COMMON_SRV_H_
#define __TASK_CONFIG_COMMON_SRV_H_
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"


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
#define STK_SIZE_SRV_TASK_START_HEART                                   8 * 128
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
#define SRV_HEART_PERIOD_MS_HEART_LED_SHOW                              500
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
#define SRV_TASK_SOCKET_CAN1_FSM_PERIOD_MS                              20
//状态机执行间隔逻辑
#define SRV_TASK_SOCKET_CAN1_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称
#define NAME_SRV_TASK_SOCKET_CAN1                                       "SrvTaskSocketCan1"
//任务优先级
#define PRIO_SRV_TASK_SOCKET_CAN1                                       2
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SOCKET_CAN1                                   16 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_SOCKET_CAN1                               MCU_CAN1_MSG_FIFO_LENGTH
//SOCKET ACK发送消息队列
#define LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN1                           8
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
#define PRIO_SRV_TASK_UTIL                                              4
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_UTIL                                          8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_UTIL                                      1024
//系统探针是用于系统信号采集用的
#define BLOCK_SIZE_SYSTEM_PROBE                                         2048
/**********************************************************************************************************************/

/********************************************输入读取任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_IN_FSM_PERIOD_MS                                  SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PORT_IN_FSM_INTERVAL_MODE                              TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PORT_IN                                           "SrvTaskPortIn"
//任务优先级    
#define PRIO_SRV_TASK_PORT_IN                                           5
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PORT_IN                                       8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PORT_IN                                   5
/**********************************************************************************************************************/

/********************************************输出写入任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PORT_OUT_FSM_PERIOD_MS                                 SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PORT_OUT_FSM_INTERVAL_MODE                             TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PORT_OUT                                          "SrvTaskPortOut"
//任务优先级    
#define PRIO_SRV_TASK_PORT_OUT                                          6
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PORT_OUT                                      8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PORT_OUT                                  5
/**********************************************************************************************************************/

/********************************************参数读写任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_PARAM_FSM_PERIOD_MS                                    SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_PARAM_FSM_INTERVAL_MODE                                TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_PARAM                                             "SrvTaskParam"
//任务优先级    
#define PRIO_SRV_TASK_PARAM                                             7
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_PARAM                                         8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_PARAM                                     5
/**********************************************************************************************************************/

/********************************************步进电机1任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR1_FSM_PERIOD_MS                               SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR1_FSM_INTERVAL_MODE                           TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR1                                        "SrvTaskStepMotor1"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR1                                        8
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR1                                    8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR1                                5
/**********************************************************************************************************************/

/********************************************步进电机2任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR2_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR2_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR2                                       "SrvTaskStepMotor2"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR2                                       9
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR2                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR2                               5
/**********************************************************************************************************************/

/********************************************步进电机3任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR3_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR3_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR3                                       "SrvTaskStepMotor3"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR3                                       10
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR3                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR3                               5
/**********************************************************************************************************************/

/********************************************步进电机4任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR4_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR4_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR4                                       "SrvTaskStepMotor4"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR4                                       11
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR4                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR4                               5
/**********************************************************************************************************************/

/********************************************步进电机5任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR5_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR5_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR5                                       "SrvTaskStepMotor5"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR5                                       12
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR5                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR5                               5
/**********************************************************************************************************************/

/********************************************步进电机6任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR6_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR6_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR6                                       "SrvTaskStepMotor6"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR6                                       13
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR6                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR6                               5
/**********************************************************************************************************************/

/********************************************步进电机7任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR7_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR7_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR7                                       "SrvTaskStepMotor7"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR7                                       14
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR7                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR7                               5
/**********************************************************************************************************************/

/********************************************步进电机8任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR8_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR8_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR8                                       "SrvTaskStepMotor8"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR8                                       15
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR8                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR8                               5
/**********************************************************************************************************************/

/********************************************步进电机9任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR9_FSM_PERIOD_MS                              SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR9_FSM_INTERVAL_MODE                          TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR9                                       "SrvTaskStepMotor9"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR9                                       16
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR9                                   8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR9                               5
/**********************************************************************************************************************/

/********************************************步进电机10任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR10_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR10_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR10                                      "SrvTaskStepMotor10"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR10                                      17
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR10                                  8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR10                              5
/**********************************************************************************************************************/

/********************************************步进电机11任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR11_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR11_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR11                                      "SrvTaskStepMotor11"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR11                                      18
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR11                                  8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR11                              5
/**********************************************************************************************************************/

/********************************************步进电机12任务相关配置***********************************************************/
//自动运行程序的执行周期,单位毫秒,当不需要周期执行函数的时候,将该值定义为 SYS_TASK_FSM_NOT_EXIST
//否则定义成想要运行的周期
#define SRV_TASK_STEP_MOTOR12_FSM_PERIOD_MS                             SYS_TASK_FSM_NOT_EXIST
//状态机执行间隔逻辑    
#define SRV_TASK_STEP_MOTOR12_FSM_INTERVAL_MODE                         TASK_FSM_TIME_COUNT_MODE_BEFORE
//任务名称  
#define NAME_SRV_TASK_STEP_MOTOR12                                      "SrvTaskStepMotor12"
//任务优先级    
#define PRIO_SRV_TASK_STEP_MOTOR12                                      19
//任务堆栈大小  
#define STK_SIZE_SRV_TASK_STEP_MOTOR12                                  8 * 128
//任务的消息队列长度    
#define LENGTH_QUEUE_SRV_TASK_STEP_MOTOR12                              5
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
#define PRIO_SRV_TASK_SYS_LOG                                           20
//任务堆栈大小
#define STK_SIZE_SRV_TASK_SYS_LOG                                       8 * 128
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
#define PRIO_SRV_TASK_TIMER                                             21
//任务堆栈大小
#define STK_SIZE_SRV_TASK_TIMER                                         8 * 128
//任务的消息队列长度
#define LENGTH_QUEUE_SRV_TASK_TIMER                                     8
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
#define PRIO_SRV_TASK_FLASH_IAP                                         22
//任务堆栈      
#define STK_SIZE_SRV_TASK_FLASH_IAP                                     8 * 128
//任务的消息队列        
#define LENGTH_QUEUE_SRV_TASK_FLASH_IAP                                 8
/**********************************************************************************************************************/


#endif



