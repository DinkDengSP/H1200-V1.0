/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-14 19:34:15 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-15 08:56:33 +0800
************************************************************************************************/ 
#include "SrvTaskHeader.h"
#include "SrvImplDemo.h"

//任务的控制块
TX_THREAD tcbSrvTaskDemo@".ccram";
//任务堆栈
uint64_t stackBufferSrvTaskDemo[STK_SIZE_SRV_TASK_DEMO/8]@".ccram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskDemo@".ccram";
//消息缓冲区
uint32_t queueBufferSrvTaskDemo[LENGTH_QUEUE_SRV_TASK_DEMO]@".ccram";


//任务相关变量初始化
static void SrvTaskDemoFuncVarInit(void);
//任务消息处理
static void SrvTaskDemoFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(SRV_TASK_DEMO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskDemoFunc_FSM_Init(void);
//周期函数运转
static void SrvTaskDemoFunc_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskDemoFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    SrvTaskDemoFuncVarInit();
    //状态机初始化
#if(SRV_TASK_DEMO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    SrvTaskDemoFunc_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
    //下次消息信号量的等待时间
    ULONG nextTimeMsWaitQueueMsg = TX_WAIT_FOREVER;
    //状态机执行间隔时间检测辅助
    ULONG timeFsmRunIntervalTimeMsDemo = 0;
#endif
    while(1)
    {
        //清空消息指针
        taskMsgPtr = NULL;
#if(SRV_TASK_DEMO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(SRV_TASK_DEMO_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(SRV_TASK_DEMO_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            SrvTaskDemoFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(SRV_TASK_DEMO_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsDemo = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsDemo >= MS_TO_OS_TICK(SRV_TASK_DEMO_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_DEMO_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsDemo;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(SRV_TASK_DEMO_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_DEMO_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskDemo,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskDemo,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskDemoFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}


//任务相关变量初始化
static void SrvTaskDemoFuncVarInit(void)
{
    SrvImplDemoVarInit();
}

//任务消息处理
static void SrvTaskDemoFuncMsgProcess(void* taskMsgPtr)
{
    SrvImplDemoMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(SRV_TASK_DEMO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskDemoFunc_FSM_Init(void)
{
    SrvImplDemoFSM_Init();
}

//周期函数运转
void SrvTaskDemoFunc_FSM_RunPeriod(void)
{
    SrvImplDemoFSM_RunPeriod();
}

#endif








