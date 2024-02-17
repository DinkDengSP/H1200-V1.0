/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 19:51:00 +0800
************************************************************************************************/ 
#include "MCU_Timer6.h"
#include "DriverConfigTimer.h"

//基础定时器模式,带中断
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_BASE)
//定时器配置
static MCU_TIMER_BASE16_CONFIG tim6BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer6BaseHandler;
//定时器周期中断函数回调
void MCU_Timer6_PeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr);


//底层复位
void MCU_Timer6_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM6_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM6_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
}

//底层初始化
void MCU_Timer6_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //根据是否配置终端回调函数决定是否打开中断
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //设置中断优先级,基础模式下,中断响应级别不用那么高
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, MCU_TIMER6_BASE_PREE_PRI, MCU_TIMER6_BASE_SUB_PRI);
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    //调试冻结
    __HAL_DBGMCU_FREEZE_TIM6();
}

//基础模式打开定时器溢出中断,中断之后自动更新到0
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax,MCU_TimerBaseCallBack callBackFuncPtr)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0}; 
    //选择定时器6
    timer6BaseHandler.Instance = TIM6;   
    //定时器回调函数复位
    TIM_ResetCallback(&timer6BaseHandler);
    //定时器6底层复位
    MCU_Timer6_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer6BaseHandler);
    //预分频器
    timer6BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer6BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer6BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer6BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    tim6BaseConfig.callBackFuncPtr = callBackFuncPtr;
    //初始化定时器底层
    MCU_Timer6_MspInit();
    //初始化定时器
    HAL_TIM_Base_Init(&timer6BaseHandler);      
    //选择内部时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timer6BaseHandler, &sClockSourceConfig);   
    //只有TIM1/2/3/4/5/6/7/8/15可以配置为主模式,连接到其他定时器,TIM12可以输出触发信号,但是没有连接到内部的定时器
    //TIM13/14/16/17没有输出触发信号,不能作为主模式
    //基础定时模式,更新之后重新归零,不对外部进行触发
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET; 
    // 禁用主/从模式
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timer6BaseHandler, &sMasterConfig);    
    //回调函数关联
    timer6BaseHandler.PeriodElapsedCallback = MCU_Timer6_PeriodElapsedCallback; 
    //首先关闭定时器中断
    MCU_Timer6Stop();
}

//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    //定时器先停止
    MCU_Timer6Stop();
    //设置新的参数
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer6BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer6BaseHandler);
    }
}

//定时器启动
void MCU_Timer6Start(void)
{
    //定时器先停止
    MCU_Timer6Stop();
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,tim6BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,tim6BaseConfig.countUpValue);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //打开中断
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
        //启动定时器,带中断
        HAL_TIM_Base_Start_IT(&timer6BaseHandler);
    }
    else
    {
        //关闭定时器中断
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        //启动定时器,不带中断
        HAL_TIM_Base_Start(&timer6BaseHandler);
    }
}

//定时器停止
void MCU_Timer6Stop(void)
{
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        //关闭定时器
        HAL_TIM_Base_Stop_IT(&timer6BaseHandler); 
    }
    else
    {
        //启动定时器,不带中断
        HAL_TIM_Base_Stop(&timer6BaseHandler);
    }
}

//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer6BaseHandler);
}

//定时器中断
void TIM6_DAC_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用定时器中断处理函数
    HAL_TIM_IRQHandler(&timer6BaseHandler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//定时器周期中断函数回调
void MCU_Timer6_PeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr)
{
    //调用回调函数
    if(tim6BaseConfig.callBackFuncPtr != NULL)
    {
        tim6BaseConfig.callBackFuncPtr();
    }
#if(MCU_TIMER6_INT_MSH_SHOW == CONFIG_FUNC_ENABLE)
    LowLevelSendStringInt("MCU_Timer6_PeriodElapsedCallback\r\n");
#endif
}

#endif


//DAC模式,只是输出,不带中断
#if(MCU_TIMER6_MODE_USER == MCU_TIMER6_MODE_DAC)
//定时器配置
MCU_TIMER_BASE16_CONFIG tim6BaseConfig = {0};
//定时器对象
static TIM_HandleTypeDef timer6BaseHandler;
//底层复位
void MCU_Timer6_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM6_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM6_RELEASE_RESET();
}

//底层初始化
void MCU_Timer6_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM6_CLK_ENABLE();
}

//DAC模式溢出不中断,但是会触发DAC的更新,形成波形
void MCU_Timer6Init(uint16_t freqDivParam,uint16_t countMax)
{
    TIM_ClockConfigTypeDef  sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig      = {0}; 
    //选择定时器6
    timer6BaseHandler.Instance = TIM6;  
    //定时器回调函数复位
    TIM_ResetCallback(&timer6BaseHandler); 
    //定时器6底层复位
    MCU_Timer6_MspDeInit();       
    //定时器复位
    HAL_TIM_Base_DeInit(&timer6BaseHandler);
    //预分频器
    timer6BaseHandler.Init.Prescaler   = freqDivParam;
    // 计数模式:向上计数
    timer6BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer6BaseHandler.Init.Period      = countMax;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer6BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //参数记录
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    tim6BaseConfig.callBackFuncPtr = NULL;
    //初始化定时器底层
    MCU_Timer6_MspInit();
    //初始化定时器
    HAL_TIM_Base_Init(&timer6BaseHandler);     
    //只有TIM1/2/3/4/5/6/7/8/12/15可以选择多个时钟源   
    //选择内部时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&timer6BaseHandler, &sClockSourceConfig);   
    //只有TIM1/2/3/4/5/6/7/8/15可以配置为主模式,连接到其他定时器,TIM12可以输出触发信号,但是没有连接到内部的定时器
    //TIM13/14/16/17没有输出触发信号,不能作为主模式
    //DAC触发模式,更新事件触发输出
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE; 
    // 禁用主/从模式
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&timer6BaseHandler, &sMasterConfig);     
    //首先关闭定时器中断
    MCU_Timer6Stop();
}


//定时器重新启动
void MCU_Timer6ReStart(uint16_t freqDivParam,uint16_t countMax)
{
    tim6BaseConfig.freqDivParam = freqDivParam;
    tim6BaseConfig.countUpValue = countMax;
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
    //设置新的分频系数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,freqDivParam);
    //设置新的重装载值
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,countMax);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer6BaseHandler);
}

//定时器启动
void MCU_Timer6Start(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
    //设置计数值为0
    __HAL_TIM_SET_COUNTER(&timer6BaseHandler,0);
    //设置参数为内部保存参数
    __HAL_TIM_SET_PRESCALER(&timer6BaseHandler,tim6BaseConfig.freqDivParam);
    __HAL_TIM_SET_AUTORELOAD(&timer6BaseHandler,tim6BaseConfig.countUpValue);
    //启动定时器,不带中断
    HAL_TIM_Base_Start(&timer6BaseHandler);
}

//定时器停止
void MCU_Timer6Stop(void)
{
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer6BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer6BaseHandler);
}

//获取定时器当前值
uint16_t MCU_Timer6GetCurrentCount(void)
{
    return __HAL_TIM_GET_COUNTER(&timer6BaseHandler);
}


#endif





