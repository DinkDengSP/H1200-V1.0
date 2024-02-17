/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-09 15:02:05 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 15:33:51 +0800
************************************************************************************************/ 
#include "CommonSrvTaskHeader.h"
#include "SrvImplForwardCan.h"

//任务的控制块
TX_THREAD tcbSrvTaskForwardCan@".dtcm_ram";
//任务堆栈
uint64_t stackBufferSrvTaskForwardCan[STK_SIZE_SRV_TASK_FORWARD_CAN/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskForwardCan@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferSrvTaskForwardCan[LENGTH_QUEUE_SRV_TASK_FORWARD_CAN]@".dtcm_ram";


//任务相关变量初始化
static void SrvTaskForwardCanFuncVarInit(void);
//任务消息处理
static void SrvTaskForwardCanFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskForwardCanFunc_FSM_Init(void);
//周期函数运转
static void SrvTaskForwardCanFunc_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskForwardCanFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    SrvTaskForwardCanFuncVarInit();
    //状态机初始化
#if(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    SrvTaskForwardCanFunc_FSM_Init();
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
#if(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(SRV_TASK_FORWARD_CAN_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            SrvTaskForwardCanFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(SRV_TASK_FORWARD_CAN_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsUtil = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsUtil >= MS_TO_OS_TICK(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsUtil;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskForwardCan,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskForwardCan,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskForwardCanFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}


//任务相关变量初始化
static void SrvTaskForwardCanFuncVarInit(void)
{
    SrvImplForwardCanVarInit();
}

//任务消息处理
static void SrvTaskForwardCanFuncMsgProcess(void* taskMsgPtr)
{
    SrvImplForwardCanMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(SRV_TASK_FORWARD_CAN_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskForwardCanFunc_FSM_Init(void)
{
    SrvImplForwardCanFSM_Init();
}

//周期函数运转
void SrvTaskForwardCanFunc_FSM_RunPeriod(void)
{
    SrvImplForwardCanFSM_RunPeriod();
}

#endif













