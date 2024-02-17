/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#include "MCU_Timer7.h"

//基础定时器模式,带中断
#if(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_BASE)
//定时器配置
static MCU_TIMER_BASE16_CONFIG tim7BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer7BaseHandler;
//定时器周期中断函数回调
void MCU_Timer7_PeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr);

//底层复位
void MCU_Timer7_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM7_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM7_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM7_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
}

//底层初始化
void MCU_Timer7_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM7_CLK_ENABLE();
    //根据是否配置终端回调函数决定是否打开中断
    if(tim7BaseConfig.callBackFuncPtr != NULL)
    {
        //设置中断优先级,基础模式下,中断响应级别不用那么高
        HAL_NVIC_SetPriority(TIM7_IRQn, MCU_TIMER7_BASE_PREE_PRI, MCU_TIMER7_BASE_SUB_PRI);
        //打开中断
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
    }
    //调试冻结
    __HAL_DBGMCU_FREEZE_TIM7();
}

//基础定时器模式,用于计时,溢出中断并自动回到0重新开始计时
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0}; 
    //选择定时器7
    timer7BaseHandler.Instance = TIM7;
    //定时器回调函数复位
    TIM_ResetCallback(&timer7BaseHandler);  
    //定时器7底层复位
    MCU_Timer7_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer7BaseHandler);
    //预分频器
    timer7BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer7BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer7BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer7BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim7BaseConfig.freqDivParam = freqDivParam;
    tim7BaseConfig.countUpValue = countMax;
    tim7BaseConfig.callBackFuncPtr = callBackFuncPtr;
    //初始化定时器底层
    MCU_Timer7_MspInit();
    //初始化定时器
    HAL_TIM_Base_Init(&timer7BaseHandler);      
    //选择内部时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timer7BaseHandler, &sClockSourceConfig);   
    //只有TIM1/2/3/4/5/6/7/8/15可以配置为主模式,连接到其他定时器,TIM12可以输出触发信号,但是没有连接到内部的定时器
    //TIM13/14/16/17没有输出触发信号,不能作为主模式
    //基础定时模式,更新之后重新归零,不对外部进行触发
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; 
    // 禁用主/从模式
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timer7BaseHandler, &sMasterConfig);  
    //回调函数关联
    timer7BaseHandler.PeriodElapsedCallback = MCU_Timer7_PeriodElapsedCallback;    
    //首先关闭定时器中断
    MCU_Timer7Stop();
}

//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    //定时器先停止
    MCU_Timer7Stop();
    //设置新的参数
    tim7BaseConfig.freqDivParam = freqDivParam;
    tim7BaseConfig.countUpValue = countMax;
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer7BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer7BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer7BaseHandler,0);
    if(tim7BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer7BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer7BaseHandler);
    }
}

//定时器启动
void MCU_Timer7Start(void)
{
    //定时器先停止
    MCU_Timer7Stop();
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer7BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer7BaseHandler,tim7BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer7BaseHandler,tim7BaseConfig.countUpValue);
    if(tim7BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM7_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer7BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM7_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer7BaseHandler);
    }
}

//定时器停止
void MCU_Timer7Stop(void)
{
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
    if(tim7BaseConfig.callBackFuncPtr != NULL)
    {
        //关闭定时器
        HAL_TIM_Base_Stop_IT(&timer7BaseHandler); 
    }
    else
    {
        //启动定时器,不带中断
        HAL_TIM_Base_Stop(&timer7BaseHandler);
    }
}

//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer7BaseHandler);
}

//定时器中断
void TIM7_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用定时器中断处理函数
    HAL_TIM_IRQHandler(&timer7BaseHandler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//定时器周期中断函数回调
void MCU_Timer7_PeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr)
{
    //调用回调函数
    if(tim7BaseConfig.callBackFuncPtr != NULL)
    {
        tim7BaseConfig.callBackFuncPtr();
    }
#if(MCU_TIMER7_INT_MSH_SHOW == CONFIG_FUNC_ENABLE)
    LowLevelSendStringInt("MCU_Timer7_PeriodElapsedCallback\r\n");
#endif
}

#endif


//DAC模式,只是输出,不带中断
#if(MCU_TIMER7_MODE_USER == MCU_TIMER7_MODE_DAC)
//定时器配置
static MCU_TIMER_BASE16_CONFIG tim7BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer7BaseHandler;
//底层复位
void MCU_Timer7_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM7_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM7_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM7_RELEASE_RESET();
}


//底层初始化
void MCU_Timer7_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM7_CLK_ENABLE();
}



//DAC模式,溢出不中断,触发DAC更新,然后回到0
void MCU_Timer7Init(uint16_t freqDivParam,uint16_t countMax)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0}; 
    //选择定时器7
    timer7BaseHandler.Instance = TIM7;   
    //定时器回调函数复位
    TIM_ResetCallback(&timer7BaseHandler);
    //定时器7底层复位
    MCU_Timer7_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer7BaseHandler);
    //预分频器
    timer7BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer7BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer7BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer7BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim7BaseConfig.freqDivParam = freqDivParam;
    tim7BaseConfig.countUpValue = countMax;
    tim7BaseConfig.callBackFuncPtr = NULL;
    //初始化定时器底层
    MCU_Timer7_MspInit();
    //初始化定时器
    HAL_TIM_Base_Init(&timer7BaseHandler);     
    //只有TIM1/2/3/4/5/6/7/8/12/15可以选择多个时钟源   
    //选择内部时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timer7BaseHandler, &sClockSourceConfig);   
    //只有TIM1/2/3/4/5/6/7/8/15可以配置为主模式,连接到其他定时器,TIM12可以输出触发信号,但是没有连接到内部的定时器
    //TIM13/14/16/17没有输出触发信号,不能作为主模式
    //DAC触发模式,更新事件触发输出
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE; 
    // 禁用主/从模式
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timer7BaseHandler, &sMasterConfig);     
    //首先关闭定时器中断
    MCU_Timer7Stop();
}


//定时器重新启动
void MCU_Timer7ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    tim7BaseConfig.freqDivParam = freqDivParam;
    tim7BaseConfig.countUpValue = countMax;
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer7BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer7BaseHandler);
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer7BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer7BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer7BaseHandler,0);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer7BaseHandler);
}

//定时器启动
void MCU_Timer7Start(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer7BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer7BaseHandler);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer7BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer7BaseHandler,tim7BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer7BaseHandler,tim7BaseConfig.countUpValue);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer7BaseHandler);
}

//定时器停止
void MCU_Timer7Stop(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer7BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer7BaseHandler);
}

//获取定时器当前值
uint16_t MCU_Timer7GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer7BaseHandler);
}


#endif










