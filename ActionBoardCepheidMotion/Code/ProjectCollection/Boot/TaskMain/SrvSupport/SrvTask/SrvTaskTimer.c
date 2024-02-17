/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 08:52:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 10:20:17 +0800
************************************************************************************************/ 
#include "SrvTaskHeader.h"
#include "SrvImplTimer.h"

//任务的控制块
TX_THREAD tcbSrvTaskTimer@".ccram";
//任务堆栈
uint64_t stackBufferSrvTaskTimer[STK_SIZE_SRV_TASK_TIMER/8]@".ccram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskTimer@".ccram";
//消息缓冲区
uint32_t queueBufferSrvTaskTimer[LENGTH_QUEUE_SRV_TASK_TIMER]@".ccram";


//任务相关变量初始化
static void SrvTaskTimerFuncVarInit(void);
//任务消息处理
static void SrvTaskTimerFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(SRV_TASK_TIMER_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskTimerFunc_FSM_Init(void);
//周期函数运转
static void SrvTaskTimerFunc_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskTimerFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    SrvTaskTimerFuncVarInit();
    //状态机初始化
#if(SRV_TASK_TIMER_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    SrvTaskTimerFunc_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
    //下次消息信号量的等待时间
    ULONG nextTimeMsWaitQueueMsg = TX_WAIT_FOREVER;
    //状态机执行间隔时间检测辅助
    ULONG timeFsmRunIntervalTimeMsUtil = 0;
#endif
    while(1)
    {
        //清空消息指针
        taskMsgPtr = NULL;
#if(SRV_TASK_TIMER_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(SRV_TASK_TIMER_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(SRV_TASK_TIMER_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            SrvTaskTimerFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(SRV_TASK_TIMER_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsUtil = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsUtil >= MS_TO_OS_TICK(SRV_TASK_TIMER_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_TIMER_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsUtil;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(SRV_TASK_TIMER_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_TIMER_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskTimer,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskTimer,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskTimerFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}


//任务相关变量初始化
static void SrvTaskTimerFuncVarInit(void)
{
    SrvImplTimerVarInit();
}

//任务消息处理
static void SrvTaskTimerFuncMsgProcess(void* taskMsgPtr)
{
    SrvImplTimerMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(SRV_TASK_TIMER_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskTimerFunc_FSM_Init(void)
{
    SrvImplTimerFSM_Init();
}

//周期函数运转
void SrvTaskTimerFunc_FSM_RunPeriod(void)
{
    SrvImplTimerFSM_RunPeriod();
}

#endif













