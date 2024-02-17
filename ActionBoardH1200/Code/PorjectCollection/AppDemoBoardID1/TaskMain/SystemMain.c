/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 16:05:38
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"

//导入内核时钟初始化程序
extern void CoreClockInit(void);

int main(void)
{
    //中断向量表拷贝,并且重新设置
    CoreCopyIntVectorTable(D1_ITCMRAM_BASE);
    //STM32H7xx HAL 库初始化，此时系统用的还是H7自带的64MHz，HSI时钟:
    //调用函数HAL_InitTick，初始化滴答时钟中断1ms。
    //设置NVIV优先级分组为4。
	HAL_Init();
    //内核时钟初始化
    CoreClockInit();
    //MPU配置
    MCU_MPU_ConfigInit();
    //Cache设置
    CoreDCacheSet(CORE_DCACHE_SET);
    CoreICacheSet(CORE_ICACHE_SET);
    //清空CACHE
    SCB_InvalidateICache();
    //清空DCACHE
    SCB_CleanDCache();
    //延时系统初始化
    CoreDelayInit();
    /* 内核开启前关闭HAL的时间基准,也就是临时关闭Systick定时器 */
	HAL_SuspendTick();
    /* 进入ThreadX内核 */
    tx_kernel_enter();
    //正常运行不到这里
	while(1){asm("nop");};
}

//创建启动任务
void  tx_application_define(void *first_unused_memory)
{
	//创建启动任务消息队列
    tx_queue_create(&queueSrvTaskStartHeart,"queueSrvTaskStartHeart",1,(void*)queueBufferSrvTaskStartHeart,
                        sizeof(queueBufferSrvTaskStartHeart)/sizeof(uint8_t));
	//创建启动任务
    tx_thread_create(&tcbSrvTaskStartHeart,             /* 任务控制块地址 */   
                     NAME_SRV_TASK_START_HEART,         /* 任务名 */
                     SrvTaskStartHeartFuncEntry,        /* 启动任务函数地址 */
                     0,                                 /* 传递给任务的参数 */
                     &stackBufferSrvTaskStartHeart[0],  /* 堆栈基地址 */
                     STK_SIZE_SRV_TASK_START_HEART,     /* 堆栈空间大小 */  
                     PRIO_SRV_TASK_START_HEART,         /* 任务优先级*/
                     PRIO_SRV_TASK_START_HEART,         /* 任务抢占阀值 */
                     TIME_SLICE_SRV_TASK,               /* 不开启时间片 */
                     TX_AUTO_START);                    /* 创建后立即启动 */
}









