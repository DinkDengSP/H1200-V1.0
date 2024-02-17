/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 09:36:16
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseHeart.h"
#include "SrvTaskHeader.h"
#include "SrvImplSocketNet8700.h"
#include "SocketNetUtilLAN8700.h"

/*------------------------------------------------------系统运行状态统计------------------------------------------------------*/
#if(SRV_HEART_THREAD_PERFORMANCE_STAT_ENABLE != CONFIG_FUNC_DISABLE)
//上次统计任务信息的时间
static int32_t lastTickThreadPerformanceStat = 0;
//上次显示系统任务情况的时间
static int32_t lastTickThreadPerformanceShow = 0;

/* CPU百分比 */
__IO double OSCPUUsage;    
//系统统计的各项时间  
EXECUTION_TIME profileThreadTotalTime, profileIsrTime, profileIdleTime; 
//计算CPU实时使用率的变量
EXECUTION_TIME TolTime, IdleTime, deltaTolTime, deltaIdleTime; 

//线程信息统计任务初始化
void SrvImplHeartSubThreadPerformanceStateInit(void)
{
    //上次显示系统任务情况的时间
    lastTickThreadPerformanceShow = tx_time_get();
    //上次统计任务信息的时间
    lastTickThreadPerformanceStat = tx_time_get();  
    //线程总消耗时间
    _tx_execution_thread_total_time_get(&profileThreadTotalTime);
    //中断时间
    _tx_execution_isr_time_get(&profileIsrTime);
    //空闲时间
    _tx_execution_idle_time_get(&profileIdleTime);
    /* 计算CPU利用率 */
	IdleTime = profileIdleTime;
	TolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime; 
}

//线程信息统计任务周期运转
void SrvImplHeartSubThreadPerformanceStateRunPeriod(void)
{
    /* 定义一个任务控制块指针 */
    TX_THREAD      *p_tcb;	
    EXECUTION_TIME threadRunTime;
    if(tx_time_get() - lastTickThreadPerformanceStat >= MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_THREAD_INFO_STATE))
    {
        //时间更新
        lastTickThreadPerformanceStat = tx_time_get();
        //线程总消耗时间
        _tx_execution_thread_total_time_get(&profileThreadTotalTime);
        //中断时间
        _tx_execution_isr_time_get(&profileIsrTime);
        //空闲时间
        _tx_execution_idle_time_get(&profileIdleTime);
        //实时计算变化增量
        deltaIdleTime = profileIdleTime - IdleTime;
        deltaTolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime - TolTime;
        OSCPUUsage = (double)deltaIdleTime/deltaTolTime;
        OSCPUUsage = 100- OSCPUUsage*100;
        IdleTime = profileIdleTime;
        TolTime = profileThreadTotalTime + profileIsrTime + profileIdleTime;
    }
    if(tx_time_get() - lastTickThreadPerformanceShow >= MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_THREAD_INFO_SHOW))
    {
        lastTickThreadPerformanceShow = tx_time_get();  
        p_tcb = &tcbSrvTaskStartHeart;
        //线程总消耗时间
        _tx_execution_thread_total_time_get(&profileThreadTotalTime);
        //中断时间
        _tx_execution_isr_time_get(&profileIsrTime);
        //空闲时间
        _tx_execution_idle_time_get(&profileIdleTime);
        /* 打印标题 */
        SystemPrintf("=============================AzureComponent BootLoader==============================\r\n");
        SystemPrintf("CPU usage = %5.2f%%\r\n", OSCPUUsage);
        SystemPrintf("TotalTaskRunTime = %.9fs\r\n", (double)profileThreadTotalTime/SystemCoreClock);
        SystemPrintf("IdleTaskRunTime = %.9fs\r\n", (double)profileIdleTime/SystemCoreClock);
        SystemPrintf("IntRunTime = %.9fs\r\n", (double)profileIsrTime/SystemCoreClock);
        SystemPrintf("SystemRunTime = %.9fs\r\n", (double)(profileThreadTotalTime + profileIsrTime + profileIdleTime)/SystemCoreClock);	
        SystemPrintf("====================================================================================\r\n");
        SystemPrintf("   Prio     StackSize   CurStack    MaxStack   RunTime   RunCount   Taskname\r\n");
        /* 遍历任务控制列表TCB list)，打印所有的任务的优先级和名称 */
        while (p_tcb != (TX_THREAD *)0) 
        {
            //获取线程运行时间
            _tx_execution_thread_time_get(p_tcb,&threadRunTime);
            //打印线程执行情况
            SystemPrintf("   %2d        %5d      %5d       %5d      %.3f S     %5d     %s\r\n", 
                        p_tcb->tx_thread_priority,
                        p_tcb->tx_thread_stack_size,
                        (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_ptr,
                        (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_highest_ptr,
                        (double)(threadRunTime)/SystemCoreClock,
                        p_tcb->tx_thread_run_count,
                        p_tcb->tx_thread_name);
            //下一个
            p_tcb = p_tcb->tx_thread_created_next;
            if(p_tcb == &tcbSrvTaskStartHeart) break;
        }
        SystemPrintf("====================================================================================\r\n");
    }
}
#endif
/*--------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------系统RTC时间显示-------------------------------------------------------*/
#if(SRV_HEART_RTC_TIMER_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//上次RTC时间显示时间戳
static int32_t lastHeartRtcTimerShowTimeTick = 0;
//系统RTC时间显示初始化
void SrvImplHeartSubRtcTimerShowInit(void)
{
    lastHeartRtcTimerShowTimeTick = tx_time_get();
}

//系统RTC时间显示周期运行
void SrvImplHeartSubRtcTimerShowRunPeriod(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(tx_time_get() - lastHeartRtcTimerShowTimeTick < MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_RTC_TIMER_SHOW))
    {
        return;
    }
    lastHeartRtcTimerShowTimeTick = tx_time_get();
    //时间展示
    RTC_TIMER* rtcTimerPtr = NULL;
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimerPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("MCU_RTC_GetTimeRecordSync Failed,Code: 0X%08X\r\n",errorSub);
        return;
    }
    //更新文件系统时间
    fx_system_date_set(rtcTimerPtr->year,rtcTimerPtr->month,rtcTimerPtr->day);
    fx_system_time_set(rtcTimerPtr->hour,rtcTimerPtr->minute,rtcTimerPtr->second);
    //当前时间打印
    SystemPrintf("CurrentWeek: %d,CurrentTime %04d-%02d-%02d,%02d:%02d:%02d\r\n",rtcTimerPtr->weekDay,
                    rtcTimerPtr->year,rtcTimerPtr->month,rtcTimerPtr->day,rtcTimerPtr->hour,rtcTimerPtr->minute,
                    rtcTimerPtr->second);
}

#endif
/*--------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------内存使用率展示-------------------------------------------------------*/
#if(SRV_HEART_MEM_USAGE_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//上次内存使用率展示时间戳
static int32_t lastHeartMemUsageShowTimeTick = 0;
//系统内存使用率展示初始化
void SrvImplHeartSubMemUsageShowInit(void)
{
    lastHeartMemUsageShowTimeTick = tx_time_get();
}

//系统内存使用率展示周期运行
void SrvImplHeartSubMemUsageShowRunPeriod(void)
{
    if(tx_time_get() - lastHeartMemUsageShowTimeTick < MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_MEM_USAGE_SHOW))
    {
        return;
    }
    lastHeartMemUsageShowTimeTick = tx_time_get();
    uint8_t memUsage = 0;
#if(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM != 0)
    memUsage = UserMemPerUsed(MEM_ITCM_SRAM);
    SystemPrintf("MemManage SramItcmUsage: %d%% ,TotalLength: %d KB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_ITCM_SRAM/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM != 0)
    memUsage = UserMemPerUsed(MEM_DTCM_SRAM);
    SystemPrintf("MemManage SramDtcmUsage: %d%% ,TotalLength: %d KB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_DTCM_SRAM/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_AXI_SRAM != 0)
    memUsage = UserMemPerUsed(MEM_AXI_SRAM);
    SystemPrintf("MemManage sramAxiUsage: %d%% ,TotalLength: %d KB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_AXI_SRAM/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_D2_SRAM1 != 0)
    memUsage = UserMemPerUsed(MEM_D2_SRAM1);
    SystemPrintf("MemManage d2SramUsage: %d%% ,TotalLength: %d KB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_D2_SRAM1/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_D3_SRAM4 != 0)
    memUsage = UserMemPerUsed(MEM_D3_SRAM4);
    SystemPrintf("MemManage d3SramUsage: %d%% ,TotalLength: %d KB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_D3_SRAM4/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM1 != 0)
    memUsage = UserMemPerUsed(MEM_SDRAM1);
    SystemPrintf("MemManage Sdram1Usage: %d%% ,TotalLength: %d MB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_SDRAM1/1024/1024));
#endif
#if(MEM_MANAGE_BYTE_SIZE_SDRAM2 != 0)
    memUsage = UserMemPerUsed(MEM_SDRAM2);
    SystemPrintf("MemManage Sdram2Usage: %d%% ,TotalLength: %d MB\r\n",memUsage,(MEM_MANAGE_BYTE_SIZE_SDRAM2/1024/1024));
#endif
}
#endif
/*--------------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------独立看门狗------------------------------------------------------------*/
#if(SRV_HEART_IWDG_ENABLE != CONFIG_FUNC_DISABLE)
//上次独立看门狗喂狗时间戳
static int32_t lastHeartIwdgFeedTimeTick = 0;
//运行时独立看门狗初始化
void SrvImplHeartSubIwdgInit(void)
{
    lastHeartIwdgFeedTimeTick = tx_time_get();
    //关闭全部的LED
    MCU_IWDG_Init(SRV_HEART_IWDG_OVER_FLOW_TIME);
}

//独立看门狗定时喂狗
void SrvImplHeartSubIwdgFeedRunPeriod(void)
{
    if(tx_time_get() - lastHeartIwdgFeedTimeTick < (SRV_HEART_IWDG_OVER_FLOW_TIME/2))
    {
        return;
    }
    lastHeartIwdgFeedTimeTick = tx_time_get();
    //独立看门狗喂狗
    MCU_IWDG_Feed();
}
#endif
/*--------------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------窗口看门狗------------------------------------------------------------*/
#if(SRV_HEART_WWDG_ENABLE != CONFIG_FUNC_DISABLE)
//上次窗口看门狗喂狗时间戳
static int32_t lastTickHeartWwdgFeed = 0;
#if(MCU_WWDG_ENABLE_EWI_SET != CONFIG_FUNC_DISABLE)
//窗口看门狗喂狗回调函数
void SrvImplHeartWWDG_EarlyCallBack(void)
{
    asm("nop");
}
#endif
//运行时窗口看门狗初始化
void SrvImplHeartSubWwdgInit(void)
{
    lastTickHeartWwdgFeed = tx_time_get();
    //初始化WWDG
#if(MCU_WWDG_ENABLE_EWI_SET != CONFIG_FUNC_DISABLE)
    MCU_WWDG_Init(SrvImplHeartWWDG_EarlyCallBack);
#else
    MCU_WWDG_Init();
#endif
}
//运行时窗口看门狗定时喂狗
void SrvImplHeartSubWwdgFeedRunPeriod(void)
{
    if(tx_time_get() - lastTickHeartWwdgFeed < (SRV_HEART_WWDG_OVER_FLOW_TIME/2))
    {
        return;
    }
    lastTickHeartWwdgFeed = tx_time_get();
    //窗口看门狗喂狗
    MCU_WWDG_Feed();
}
#endif
/*--------------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------系统运行状态灯显示------------------------------------------------------*/
#if(SRV_HEART_LED_SHOW_ENABLE != CONFIG_FUNC_DISABLE)
//上次LED心跳时间
static int32_t lastHeartLedRunTimeTick = 0;
//初始化LED定时展示
void SrvImplHeartSubLedShowInit(void)
{
    //上次LED心跳时间
    lastHeartLedRunTimeTick = tx_time_get();
}
//LED定时闪烁
void SrvImplHeartSubLedShowRunPeriod(void)
{
    if(tx_time_get() - lastHeartLedRunTimeTick >= MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_HEART_LED_SHOW))
    {
        lastHeartLedRunTimeTick = tx_time_get();
        BoardLedToogle(SRV_HEART_PERIOD_MS_HEART_LED);
        //用拨码按键控制拨码灯光
        BOARD_DIP_KEY_SET dipKeySetAll;
        BoardDipKeyReadAll(&dipKeySetAll);
        //设置灯光
        BoardDipLedWriteAll(dipKeySetAll.valAll);
        //检查网卡连接,连接成功绿灯亮
        if((SUCCESS == SocketNetUtilLAN8700CheckBaseInitOver())&&(NX_TCP_ESTABLISHED == SrvImplSocketNetLAN8700MainGetState()))
        {
            BoardLedWrite(BOARD_LED_YELLOW,PIN_STATE_VALID);
        }
        else
        {
            BoardLedWrite(BOARD_LED_YELLOW,PIN_STATE_INVALID);
        }
    }
}
#endif






