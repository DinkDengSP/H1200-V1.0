/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "SrvImplStartHeart.h"
#include "SystemVersion.h"

//任务的控制块
TX_THREAD tcbSrvTaskStartHeart@".dtcm_ram";
//任务堆栈
uint64_t stackBufferSrvTaskStartHeart[STK_SIZE_SRV_TASK_START_HEART/8]@".dtcm_ram";
//任务消息队列控制块
TX_QUEUE queueSrvTaskStartHeart@".dtcm_ram";
//消息缓冲区
uint32_t queueBufferSrvTaskStartHeart[LENGTH_QUEUE_SRV_TASK_START_HEART]@".dtcm_ram";


//任务相关变量初始化
static void SrvTaskStartHeartFuncVarInit(void);
//任务消息处理
static void SrvTaskStartHeartFuncMsgProcess(void* taskMsgPtr);

//定义执行周期,定义周期函数
#if(SRV_TASK_START_HEART_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskStartHeartFunc_FSM_Init(void);
//周期函数运转
static void SrvTaskStartHeartFunc_FSM_RunPeriod(void);
#endif


//任务函数
void SrvTaskStartHeartFuncEntry(ULONG threadInput)
{
    (void)threadInput;
    //设置中断分组
    CoreNVIC_SetGroupDefault();
    //延时函数重新初始化
    CoreDelayInit();
	//内核开启后，恢复HAL里的时间基准,打开SysTick中断
    HAL_ResumeTick();
    //系统设备初始化
    SrvImplStartHeartDeviceInit();
    //公共服务层数据初始化
    SrvImplStartHeartCreateCommonSrvData();
    //创建公共服务层任务
    SrvImplStartHeartCreateCommonSrvTask();
    //服务层数据初始化
    SrvImplStartHeartCreateSrvData();
    //服务层任务初始化
    SrvImplStartHeartCreateSrvTask();
    //模块层数据初始化
    SrvImplStartHeartCreateModuleData();
    //模块层任务初始化
    SrvImplStartHeartCreateModuleTask();
    //应用层数据初始化
    SrvImplStartHeartCreateAppData();
    //应用层任务初始化
    SrvImplStartHeartCreateAppTask();
    //系统API调用结果
    uint32_t txResult = TX_SUCCESS;
    //任务消息队列
    void* taskMsgPtr = NULL;
    //任务相关变量初始化
    SrvTaskStartHeartFuncVarInit();
    //延时等待任务创建及系统任务体系初始化
    CoreDelayMs(20);
    //系统启动完成,打印编译信息
    SystemPrintf("STM32H743IIT6_M7 ThreadX MidComm App Start\r\n");
    SystemPrintf("Module No: %d,Board ID: %d\r\n",SELF_MODULE_NO,SELF_BOARD_ID);
    //打印编译信息
    SystemPrintf("Compile Data: %s\r\n",__DATE__);
    SystemPrintf("Compile Time: %s\r\n",__TIME__);
    //打印当前文件版本
    SystemPrintf("CurrentVersion %03d.%03d.%03d - SvnCode: %d\r\n",SYSTEM_VERSION_MAIN,SYSTEM_VERSION_SUB,
                    SYSTEM_VERSION_TEMP,SYSTEM_DEV_VERSION_STORE);
#if(SRV_TASK_START_HEART_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
    SrvTaskStartHeartFunc_FSM_Init();
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
#if(SRV_TASK_START_HEART_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
        //定义自动周期运行,检查当前时间
        currentTimeTickStamp = tx_time_get();
        if(currentTimeTickStamp - lastFSM_RunTimeTickStamp >= MS_TO_OS_TICK(SRV_TASK_START_HEART_FSM_PERIOD_MS))
        {
            //执行之前更新时间
#if(SRV_TASK_START_HEART_FSM_INTERVAL_MODE == TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //执行周期函数
            SrvTaskStartHeartFunc_FSM_RunPeriod();
            //执行之后更新时间
#if(SRV_TASK_START_HEART_FSM_INTERVAL_MODE != TASK_FSM_TIME_COUNT_MODE_BEFORE)
            lastFSM_RunTimeTickStamp = tx_time_get();
#endif
            //更新当前时间
            currentTimeTickStamp = tx_time_get();
        }
        //计算状态机执行间隔时间
        timeFsmRunIntervalTimeMsUtil = currentTimeTickStamp - lastFSM_RunTimeTickStamp;
        //计算下一次调度延时
        if(timeFsmRunIntervalTimeMsUtil >= MS_TO_OS_TICK(SRV_TASK_START_HEART_FSM_PERIOD_MS))
        {
            //当前状态机执行间隔时间大于周期等待时间了,立刻执行下一次状态机,不等待
            nextTimeMsWaitQueueMsg = TX_NO_WAIT;
        }
        else 
        {
            //当前状态机执行间隔时间小于周期等待时间了,等待一段时间进行下一次状态机检测
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_START_HEART_FSM_PERIOD_MS) - timeFsmRunIntervalTimeMsUtil;
        }
        //防溢出
        if(nextTimeMsWaitQueueMsg > MS_TO_OS_TICK(SRV_TASK_START_HEART_FSM_PERIOD_MS))
        {
            nextTimeMsWaitQueueMsg = MS_TO_OS_TICK(SRV_TASK_START_HEART_FSM_PERIOD_MS);
        }
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskStartHeart,(void*)(&taskMsgPtr),nextTimeMsWaitQueueMsg);
#else
        //申请信号量
        txResult = tx_queue_receive(&queueSrvTaskStartHeart,(void*)(&taskMsgPtr),TX_WAIT_FOREVER);
#endif
        if((txResult == TX_SUCCESS)&&(taskMsgPtr != NULL))
        {
            //对消息进行处理
            SrvTaskStartHeartFuncMsgProcess(taskMsgPtr);
            //清空指针
            taskMsgPtr = NULL;
        }
    }
}



//任务相关变量初始化
static void SrvTaskStartHeartFuncVarInit(void)
{
    SrvImplStartHeartVarInit();
}

//任务消息处理
static void SrvTaskStartHeartFuncMsgProcess(void* taskMsgPtr)
{
    SrvImplStartHeartMsgProcess(taskMsgPtr);
}


//定义执行周期,定义周期函数
#if(SRV_TASK_START_HEART_FSM_PERIOD_MS != TASK_FSM_NOT_EXIST)
//周期运行函数初始化
static void SrvTaskStartHeartFunc_FSM_Init(void)
{
    SrvImplStartHeartFSM_Init();
}

//周期函数运转
void SrvTaskStartHeartFunc_FSM_RunPeriod(void)
{
    SrvImplStartHeartFSM_RunPeriod();
}

#endif









