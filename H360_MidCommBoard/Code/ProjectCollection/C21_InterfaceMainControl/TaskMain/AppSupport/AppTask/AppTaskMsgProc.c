/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 11:29:12
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "AppTaskHeader.h"
#include "AppImplMsgProc.h"

//任务的控制块
TX_THREAD tcbAppTaskMsgProc@".dtcm_ram";
//任务堆栈
uint64_t stackBufferAppTaskMsgProc[STK_SIZE_APP_TASK_MSG_PROC/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueAppTaskMsgProc@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferAppTaskMsgProc[LENGTH_QUEUE_APP_TASK_MSG_PROC]@".dtcm_ram";


//任务相关变量初始化
static void AppTaskMsgProcFuncVarInit(void);
//任务消息处理
static void AppTaskMsgProcFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(APP_TASK_MSG_PROC_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void AppTaskMsgProcFunc_FSM_Init(void);
//周期函数运转
static void AppTaskMsgProcFunc_FSM_RunPeriod(void);
#endif


//任务函数
void AppTaskMsgProcFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    AppTaskMsgProcFuncVarInit();
    //状态机初始化
#if(APP_TASK_MSG_PROC_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    AppTaskMsgProcFunc_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
    //下次消息信号量的等待时间
    ULONG nextTimeMsWaitQueueMsg = TX_WAIT_FOREVER;
    //状态机执行间隔时间检测辅助
    ULONG timeFsmRunIntervalTimeMsMsgProc = 0;
#endif
    while(1)
    {
        //清空消息指针
        taskMsgPtr = NULL;
#if(APP_TASK_MSG_PROC_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(APP_TASK_MSG_PROC_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(APP_TASK_MSG_PROC_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            AppTaskMsgProcFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(APP_TASK_MSG_PROC_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsMsgProc = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsMsgProc >= MS_TO_OS_TICK(APP_TASK_MSG_PROC_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间-小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(APP_TASK_MSG_PROC_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsMsgProc;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(APP_TASK_MSG_PROC_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(APP_TASK_MSG_PROC_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueAppTaskMsgProc,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueAppTaskMsgProc,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            AppTaskMsgProcFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}


//任务相关变量初始化
static void AppTaskMsgProcFuncVarInit(void)
{
    AppImplMsgProcVarInit();
}

//任务消息处理
static void AppTaskMsgProcFuncMsgProcess(void* taskMsgPtr)
{
    AppImplMsgProcMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(APP_TASK_MSG_PROC_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void AppTaskMsgProcFunc_FSM_Init(void)
{
    AppImplMsgProcFSM_Init();
}

//周期函数运转
void AppTaskMsgProcFunc_FSM_RunPeriod(void)
{
    AppImplMsgProcFSM_RunPeriod();
}

#endif





