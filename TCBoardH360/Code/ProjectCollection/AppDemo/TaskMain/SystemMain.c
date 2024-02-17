/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 11:10:09 +0800
************************************************************************************************/ 
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"


//导入外部时钟初始化
void CoreClockInit(void);

int main(void)
{
    //HAL 库初始化，此时系统用的还是H7自带的16MHz，HSI时钟:
    //调用函数HAL_InitTick，初始化滴答时钟中断1ms。
    //设置NVIV优先级分组为4。
	HAL_Init();
    //拷贝设置中断向量表
    CoreCopyIntVectorTable(CCMDATARAM_BASE);
    //内核时钟初始化
    CoreClockInit();
    //延时系统初始化
    CoreDelayInit();
    //内核开启前关闭HAL的时间基准,也就是临时关闭Systick定时器
	HAL_SuspendTick();
    //ICache使能
    CoreICacheSet(CORE_ICACHE_SET);
    //DCache使能
    CoreDCacheSet(CORE_DCACHE_SET);
    //Flash预取指使能
    CoreFlashPrefetchSet(CORE_FLASH_PREFETCH_SET);
    //进入ThreadX内核
    tx_kernel_enter();
    //正常运行不到这里
    while(1)
    {
        asm("nop");
    }
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













