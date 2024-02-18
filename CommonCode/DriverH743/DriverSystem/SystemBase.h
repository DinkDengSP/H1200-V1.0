/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 15:56:01 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_BASE_H_
#define __SYSTEM_BASE_H_
#include "DriverHeaderBoard.h"

//任务周期函数不存在的宏定义
#define SYS_TASK_FSM_NOT_EXIST                              0
//操作系统状态机执行策略,0表示状态机执行间隔时间按照进入状态机开始计算,不包含状态机的执行时间,两次执行时间不固定
//因为状态机本身有执行时间,例如设置间隔20MS,状态机执行时间10MS,那么选择模式0,状态机执行完成之后再过10MS就会执行下一次状态机
#define TASK_FSM_TIME_COUNT_MODE_BEFORE                     0
//1表示从退出上一次状态机开始计算,两个状态机执行间隔固定,例如设置间隔20MS,状态机执行时间10MS,那么选择模式1,
//状态机执行完成之后再过20MS就会执行下一次状态机,相当于状态机执行周期变成了30MS一次
#define TASK_FSM_TIME_COUNT_MODE_END                        1


//任务函数定义
typedef void (*SystemTaskFuncEntryPtr)(unsigned long threadInput);
//操作系统任务配置
typedef struct SYSTEM_TASK_CREATE_CONFIG
{
    TX_THREAD* tcbTaskPtr;                  //任务TCB控制块的指针
    CHAR* taskNamePtr;                      //任务的名称
    SystemTaskFuncEntryPtr taskFuncEntryPtr;//任务函数地址
    unsigned long threadInput;              //任务输入参数
    void* taskStkBasePtr;                   //任务堆栈指针
    unsigned long taskStkSize;              //任务堆栈长度
    uint32_t priority;                      //任务优先级
    uint32_t preempt_threshold;             //抢占阈值优先级,当抢占阈值为任务优先级的时候禁止任何任务抢占
    unsigned long  time_slice;              //任务时间片长度,只有开启了任务时间片才能使用,否则定义为TX_NO_TIME_SLICE
    uint32_t auto_start;                    //任务创建后是否自动启动,启动使用TX_AUTO_START,不启动使用TX_DONT_START
}SYSTEM_TASK_CREATE_CONFIG;

#ifdef NX_INCLUDE_USER_DEFINE_FILE
/*--------------------------------------------网络协议栈相关---------------------------------------------------*/
//Socket初始化标记  
#define SOCKET_NET_INIT_FLAG        0XAA55A55A

//NETX网卡状态和描述映射
typedef struct NETX_STATE_MAP_STR
{
    ULONG state;
    uint8_t* despStr;
}NETX_STATE_MAP_STR;

//Socket的状态
typedef struct NX_SOCKET_INFO
{
    ULONG tcp_packets_sent;
    ULONG tcp_bytes_sent;
    ULONG tcp_packets_received;
    ULONG tcp_bytes_received;
    ULONG tcp_retransmit_packets;
    ULONG tcp_packets_queued;
    ULONG tcp_checksum_errors;
    ULONG tcp_socket_state;
    ULONG tcp_transmit_queue_depth;
    ULONG tcp_transmit_window;
    ULONG tcp_receive_window;
}NX_SOCKET_INFO;
#define LENGTH_NX_SOCKET_INFO       (sizeof(NX_SOCKET_INFO)/sizeof(uint8_t))

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SocketNetStateGetSocketState(ULONG state);

#endif


/*--------------------------------------------系统消息相关---------------------------------------------------*/
//系统消息头
typedef enum SYS_MSG_PACK_SRC
{
    SYS_MSG_PACK_SRC_LOG                = 0,//日志消息
    SYS_MSG_PACK_SRC_CAN1_CMD           = 1,//CAN指令
    SYS_MSG_PACK_SRC_CAN1_UPLOAD        = 2,//CAN上报数据
    SYS_MSG_PACK_SRC_CAN2_CMD           = 3,//CAN指令
    SYS_MSG_PACK_SRC_CAN2_UPLOAD        = 4,//CAN上报数据
    SYS_MSG_PACK_SRC_IPC_CMD            = 5,//IPC指令消息
    SYS_MSG_PACK_SRC_IPC_UPLOAD         = 6,//IPC上报消息
    SYS_MSG_PACK_SRC_NET_CMD            = 7,//网口指令消息
    SYS_MSG_PACK_SRC_NET_UPLOAD         = 8,//网口上传消息
    SYS_MSG_PACK_SRC_UART_CMD           = 9,//串口指令消息
    SYS_MSG_PACK_SRC_UART_UPLOAD        = 10,//串口数据上报消息
    SYS_MSG_PACK_SRC_RPC_CMD            = 11,//多核指令消息
    SYS_MSG_PACK_SRC_RPC_UPLOAD         = 12,//多核数据上报消息
    SYS_MSG_PACK_SRC_NET5500_CMD        = 13,//网口W5500指令消息
    SYS_MSG_PACK_SRC_NET5500_UPLOAD     = 14,//网口W5500上传消息
    SYS_MSG_PACK_SRC_PE_UART_CMD        = 15,//PE串口指令消息
    SYS_MSG_PACK_SRC_PE_UART_UPLOAD     = 16,//PE串口上传消息
}SYS_MSG_PACK_SRC;

typedef struct SYS_MSG
{
    SYS_MSG_PACK_SRC msgSrc;//消息源
    void* msgBodyPtr;//消息体指针
}SYS_MSG;

//日志数据结构
typedef struct SYS_LOG_MSG
{
    uint8_t* logDataBuffer;//输出日志缓冲区
    uint16_t logDataLength;//输出日志长度
}SYS_LOG_MSG;
#define LENGTH_SYS_LOG_MSG      (sizeof(SYS_LOG_MSG)/sizeof(uint8_t))


#endif












