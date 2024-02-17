/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 14:42:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-26 14:00:15 +0800
************************************************************************************************/ 
#include "ModTaskHeader.h"
#include "ModImplLiquidAuto.h"

//任务的控制块
TX_THREAD tcbModTaskLiquidAuto@".dtcm_ram";
//任务堆栈
uint64_t stackBufferModTaskLiquidAuto[STK_SIZE_MOD_TASK_LIQUID_AUTO/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueModTaskLiquidAuto@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferModTaskLiquidAuto[LENGTH_QUEUE_MOD_TASK_LIQUID_AUTO]@".dtcm_ram";


//任务相关变量初始化
static void ModTaskLiquidAutoFuncVarInit(void);
//任务消息处理
static void ModTaskLiquidAutoFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void ModTaskLiquidAutoFunc_FSM_Init(void);
//周期函数运转
static void ModTaskLiquidAutoFunc_FSM_RunPeriod(void);
#endif


//任务函数
void ModTaskLiquidAutoFuncEntry(unsigned long threadInput)
{
    //输入堆栈处理,防止报警
    (void)threadInput;
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    ModTaskLiquidAutoFuncVarInit();
    //状态机初始化
#if(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    ModTaskLiquidAutoFunc_FSM_Init();
    //上一次的外部信号检测发送的时间
	ULONG lastFSM_RunTimeTickStamp = tx_time_get();
    //更新定时时钟
    ULONG currentTimeTickStamp = tx_time_get();
    //下次消息信号量的等待时间
    ULONG nextTimeMsWaitQueueMsg = TX_WAIT_FOREVER;
    //状态机执行间隔时间检测辅助
    ULONG timeFsmRunIntervalTimeMsLiquidAuto = 0;
#endif
    while(1)
    {
        //清空消息指针
        taskMsgPtr = NULL;
#if(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(MOD_TASK_LIQUID_AUTO_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            ModTaskLiquidAutoFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(MOD_TASK_LIQUID_AUTO_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsLiquidAuto = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsLiquidAuto >= MS_TO_OS_TICK(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsLiquidAuto;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueModTaskLiquidAuto,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueModTaskLiquidAuto,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            ModTaskLiquidAutoFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}


//任务相关变量初始化
static void ModTaskLiquidAutoFuncVarInit(void)
{
    ModImplLiquidAutoVarInit();
}

//任务消息处理
static void ModTaskLiquidAutoFuncMsgProcess(void* taskMsgPtr)
{
    ModImplLiquidAutoMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(MOD_TASK_LIQUID_AUTO_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void ModTaskLiquidAutoFunc_FSM_Init(void)
{
    ModImplLiquidAutoFSM_Init();
}

//周期函数运转
void ModTaskLiquidAutoFunc_FSM_RunPeriod(void)
{
    ModImplLiquidAutoFSM_RunPeriod();
}

#endif













