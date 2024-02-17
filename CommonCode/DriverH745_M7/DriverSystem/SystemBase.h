/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-29 18:06:55 +0800
************************************************************************************************/ 
#ifndef __SYSTEM_BASE_H_
#define __SYSTEM_BASE_H_
#include "DriverHeaderBoard.h"
#include "DriverHeaderDevice.h"

//任务周期函数不存在的宏定义,如果存在状态机,直接将对应的时间定义即可
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

/*--------------------------------------------系统消息相关---------------------------------------------------*/
//系统消息头
typedef enum SYS_MSG_PACK_SRC
{
    SYS_MSG_PACK_SRC_LOG            = 0,//日志消息
    SYS_MSG_PACK_SRC_CAN1_CMD       = 1,//CAN指令
    SYS_MSG_PACK_SRC_CAN1_UPLOAD    = 2,//CAN上报数据
    SYS_MSG_PACK_SRC_CAN2_CMD       = 3,//CAN指令
    SYS_MSG_PACK_SRC_CAN2_UPLOAD    = 4,//CAN上报数据
    SYS_MSG_PACK_SRC_IPC_CMD        = 5,//IPC指令消息
    SYS_MSG_PACK_SRC_IPC_UPLOAD     = 6,//IPC上报消息
    SYS_MSG_PACK_SRC_NET_CMD        = 7,//网口指令消息
    SYS_MSG_PACK_SRC_NET_UPLOAD     = 8,//网口上传消息
    SYS_MSG_PACK_SRC_UART_CMD       = 9,//串口指令消息
    SYS_MSG_PACK_SRC_UART_UPLOAD    = 10,//串口数据上报消息
    SYS_MSG_PACK_SRC_RPC_CMD        = 11,//多核指令消息
    SYS_MSG_PACK_SRC_RPC_UPLOAD     = 12,//多核数据上报消息
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


/*--------------------------------------------系统输入输出,电机控制---------------------------------------------------*/
//模块内部电机复位配置
typedef struct MODULE_STEP_MOTOR_RESET_CONFIG
{
    uint16_t motorNo;//步进电机映射
    uint8_t resetCorrectMainIndex;//步进电机复位修正参数的主序号
    uint8_t resetCorrectSubIndex;//步进电机复位修正参数的子序号
}MODULE_STEP_MOTOR_RESET_CONFIG;

//模块输入单元映射
typedef struct MODULE_INPUT_MAP_UNIT
{
    uint16_t inputIndex;//输入序号
    GPIO_PinState validState;//支持状态
}MODULE_INPUT_MAP_UNIT;
//保留输入,当模块没有输入的时候用这个占位
#define MODULE_BASE_INPUT_PIN_RESERVE                       0XFFFF
#define VALID_LEVEL_MODULE_BASE_INPUT_PIN_RESERVE           GPIO_PIN_RESET

//模块输出单元映射
typedef struct MODULE_OUTPUT_MAP_UNIT
{
    uint16_t outputIndex;
    GPIO_PinState validState;
}MODULE_OUTPUT_MAP_UNIT;
//保留输出,当模块没有输出的时候用这个占位
#define MODULE_BASE_OUTPUT_PIN_RESERVE                      0XFFFF
#define VALID_LEVEL_MODULE_BASE_OUTPUT_PIN_RESERVE          GPIO_PIN_RESET

#endif





