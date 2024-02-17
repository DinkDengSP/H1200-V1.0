/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-31 15:04:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-01 09:03:14 +0800
************************************************************************************************/ 
#include "BoardHpTimeStamp.h"
#include "DriverConfigBoard.h"
#include "tx_mutex.h"

//定时器句柄
static TIM_HandleTypeDef boardHpTimeStampHandle;
//溢出计数值
static uint32_t boardHpTimeStampOverFlowCount;
//标识初始化标记
static TX_MUTEX mutexBoardHpTimeStamp;


//Timer周期回调
static void BoardHpTimeStampPeriodCallBack(TIM_HandleTypeDef *htim)
{
    boardHpTimeStampOverFlowCount++;
}

// 底层复位
static void BoardHpTimeStampMspDeInit(void)
{
    //使能定时器时钟
    BOARD_HP_TIME_STAMP_TIMER_CLK_ENABLE();
    //模块复位
    BOARD_HP_TIME_STAMP_TIMER_FORCE_RESET();
    //取消复位
    BOARD_HP_TIME_STAMP_TIMER_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(BOARD_HP_TIME_STAMP_TIMER_IRQ);
}

// 底层初始化
static void BoardHpTimeStampMspInit(void)
{
    //使能定时器时钟
    BOARD_HP_TIME_STAMP_TIMER_CLK_ENABLE();
    // debug的时候停止定时器时钟
    BOARD_HP_TIME_STAMP_TIMER_DEBUG_FREEZE(); 
}

//高精度时基初始化
void BoardHpTimeStampInit(void)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0}; 
    if(mutexBoardHpTimeStamp.tx_mutex_id == TX_MUTEX_ID)
    {
        return;
    }
    //初始化之后不需要再次初始化
    tx_mutex_create(&mutexBoardHpTimeStamp,"mutexBoardHpTimeStamp",TX_INHERIT);
    //选择定时器
    boardHpTimeStampHandle.Instance = BOARD_HP_TIME_STAMP_TIMER;
    //定时器回调函数复位
    TIM_ResetCallback(&boardHpTimeStampHandle);
    //定时器底层复位
    BoardHpTimeStampMspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&boardHpTimeStampHandle);
    //预分频器
    boardHpTimeStampHandle.Init.Prescaler   = BOARD_HP_TIME_STAMP_TIM_PRESCALER;
    // 计数模式:向上计数
    boardHpTimeStampHandle.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    boardHpTimeStampHandle.Init.Period      = BOARD_HP_TIME_STAMP_PERIOD;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    boardHpTimeStampHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //初始化定时器底层
    BoardHpTimeStampMspInit();
    //初始化定时器
    HAL_TIM_Base_Init(&boardHpTimeStampHandle); 
    //清除定时器标记
    __HAL_TIM_CLEAR_IT(&boardHpTimeStampHandle,TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_FLAG(&boardHpTimeStampHandle,TIM_FLAG_UPDATE);
    //设置中断优先级
    HAL_NVIC_SetPriority(BOARD_HP_TIME_STAMP_TIMER_IRQ, BOARD_HP_TIME_STAMP_PREE_PRI, 
                            BOARD_HP_TIME_STAMP_SUB_PRI);
    //打开中断
    HAL_NVIC_EnableIRQ(BOARD_HP_TIME_STAMP_TIMER_IRQ);     
    //选择内部时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&boardHpTimeStampHandle, &sClockSourceConfig);   
    //没有触发
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; 
    //禁用主/从模式
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&boardHpTimeStampHandle, &sMasterConfig);   
    //回调函数关联
    boardHpTimeStampHandle.PeriodElapsedCallback = BoardHpTimeStampPeriodCallBack;     
    //计数清零
    boardHpTimeStampOverFlowCount = 0;
    //启动时间戳
    HAL_TIM_Base_Start_IT(&boardHpTimeStampHandle);
}

//获取当前时间戳
void BoardHpTimeStampGetStamp(BOARD_HP_TIME_STAMP* timeStampPtr)
{
    //获取当前计数
    timeStampPtr->subCount = __HAL_TIM_GET_COUNTER(&boardHpTimeStampHandle);
    //获取溢出次数
    timeStampPtr->mainCount = boardHpTimeStampOverFlowCount;
}

//复位时间戳结构体
void BoardHpTimeStampResetStampData(BOARD_HP_TIME_STAMP* timeStampPtr)
{
    timeStampPtr->subCount = 0;
    timeStampPtr->mainCount = 0;
}

//拷贝时间戳让两个时间戳一致
void BoardHpTimeStampCopy(BOARD_HP_TIME_STAMP* dstPtr,BOARD_HP_TIME_STAMP* srcPtr)
{
    dstPtr->subCount = srcPtr->subCount;
    dstPtr->mainCount = srcPtr->mainCount;
}

//计算两个时间戳之间的差值
int64_t BoardHpTimeStampCalcDiff(BOARD_HP_TIME_STAMP* newStampPtr,BOARD_HP_TIME_STAMP* oldStampPtr)
{
    int64_t oldStamp = 0;
    int64_t newStamp = 0;
    //旧时间戳
    oldStamp += oldStampPtr->mainCount;
    oldStamp *= BOARD_HP_TIME_STAMP_SUB_MAX_COUNT;
    oldStamp += oldStampPtr->subCount;
    //新时间戳
    newStamp += newStampPtr->mainCount;
    newStamp *= BOARD_HP_TIME_STAMP_SUB_MAX_COUNT;
    newStamp += newStampPtr->subCount;
    //计算
    return newStamp - oldStamp;
}

//中断向量
void BOARD_HP_TIME_STAMP_TIMER_IRQ_HANDLE(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //回调函数
    HAL_TIM_IRQHandler(&boardHpTimeStampHandle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
