/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-27 14:29:47 +0800
************************************************************************************************/ 
#include "MCU_Timer9_PWM.h"
#include "MCU_Pin.h"
#include "DriverConfigTimer.h"
#include "MCU_LogShow.h"

//定时器对象
static TIM_HandleTypeDef timer9BaseHandler = {0};
//通道1配置对象
static TIM_OC_InitTypeDef timer9PwmOutChannel1Config = {0};	
//回调函数
static MCU_TimerBaseCallBack timer9Ch1UpdataCallbackFuncPtr = NULL;


//底层复位
void MCU_Timer9_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM9_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM9_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM9_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
}

//底层初始化
void MCU_Timer9_MspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM9_CLK_ENABLE();
    //设置中断优先级,基础模式下,中断响应级别不用那么高
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, MCU_TIMER9_UP_BASE_PREE_PRI, MCU_TIMER9_UP_BASE_SUB_PRI);
    //打开中断
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
    //调试冻结
    __HAL_DBGMCU_FREEZE_TIM9();
}

//定时器PWM输出初始化
void MCU_Timer9Ch1_Init(MCU_TimerBaseCallBack callBackFuncPtr)
{
    HAL_StatusTypeDef halState = HAL_OK;
    TIM_ClockConfigTypeDef  timClkSrcConfig = {0};
    TIM_MasterConfigTypeDef masterTrigConfig      = {0}; 
    timer9Ch1UpdataCallbackFuncPtr = callBackFuncPtr;
    //选择定时器9
    timer9BaseHandler.Instance = TIM9;   
    //定时器9底层复位
    MCU_Timer9_MspDeInit();     
    //定时器回调函数复位
    TIM_ResetCallback(&timer9BaseHandler);      
    //定时器复位
    HAL_TIM_Base_DeInit(&timer9BaseHandler);
    //预分频器
    timer9BaseHandler.Init.Prescaler   = (MCU_TIMER9_DIV -1);
    // 计数模式:向上计数
    timer9BaseHandler.Init.CounterMode = TIM_COUNTERMODE_UP; 
    // 计数周期 
    timer9BaseHandler.Init.Period      = (MCU_TIMER9_FREQ_CALC_BASE/MCU_TIMER9_FREQ_MIN);;  
    //自动重装载缓冲,打开之后对计数值的设定需要下次发生溢出事件之后才会生效
    timer9BaseHandler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    //重复匹配次数,这个只有1和8存在,就不使用了
    timer9BaseHandler.Init.RepetitionCounter = 0;
    //初始化定时器底层
    MCU_Timer9_MspInit();
    //初始化定时器
    halState = HAL_TIM_PWM_Init(&timer9BaseHandler);    
    if(halState != HAL_OK)
    {
        //失败报错
        MCU_LogShowHalState(halState,timer9BaseHandler.State);
        while(1);
    }  
    //选择内部时钟源
    timClkSrcConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    halState = HAL_TIM_ConfigClockSource(&timer9BaseHandler, &timClkSrcConfig);  
    if(halState != HAL_OK)
    {
        //失败报错
        MCU_LogShowHalState(halState,timer9BaseHandler.State);
        while(1);
    }  
    //没有触发
    masterTrigConfig.MasterOutputTrigger = TIM_TRGO_RESET; 
    //禁用主/从模式
    masterTrigConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    halState = HAL_TIMEx_MasterConfigSynchronization(&timer9BaseHandler, &masterTrigConfig);    
    if(halState != HAL_OK)
    {
        //失败报错
        MCU_LogShowHalState(halState,timer9BaseHandler.State);
        while(1);
    } 
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer9BaseHandler,TIM_IT_UPDATE);
    //关闭定时器,不带中断
    HAL_TIM_Base_Stop(&timer9BaseHandler);
    //初始化通道1
    //PWM模式
    timer9PwmOutChannel1Config.OCMode       = TIM_OCMODE_PWM2;
    //触发输出记性
	timer9PwmOutChannel1Config.OCPolarity   = TIM_OCNPOLARITY_LOW;
    //空闲状态下的引脚状态
    timer9PwmOutChannel1Config.OCIdleState  = TIM_OCNIDLESTATE_RESET;
    //触发快速输出模式
	timer9PwmOutChannel1Config.OCFastMode   = TIM_OCFAST_DISABLE;
    //互补N通道触发输出极性
	timer9PwmOutChannel1Config.OCNPolarity  = TIM_OCNPOLARITY_LOW;
    //互补N通道空闲状态下引脚状态
	timer9PwmOutChannel1Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	//占空比
	timer9PwmOutChannel1Config.Pulse = ((MCU_TIMER9_FREQ_CALC_BASE/MCU_TIMER9_FREQ_MIN)/2); ;
	//配置通道
    halState = HAL_TIM_PWM_ConfigChannel(&timer9BaseHandler, &timer9PwmOutChannel1Config, TIM_CHANNEL_1);
	if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer9BaseHandler.State);
        while(1);
    }
    //关闭中断
    __HAL_TIM_DISABLE_IT(&timer9BaseHandler,TIM_IT_CC1);
    //关闭CCR预加载
    __HAL_TIM_DISABLE_OCxPRELOAD(&timer9BaseHandler,TIM_CHANNEL_1);
    //停止输出
    TIM_CCxChannelCmd(timer9BaseHandler.Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
    //IO初始化
    MCU_PinInit(MCU_PIN_TIMER9_CH1_PWM,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF3_TIM9);
}

//设置脉冲频率
void MCU_Timer9Ch1_SetFreq(uint32_t freq)
{
    //限制频率最大最小值
    if(freq > MCU_TIMER9_FREQ_MAX)freq = MCU_TIMER9_FREQ_MAX;
    else if(freq < MCU_TIMER9_FREQ_MIN)freq = MCU_TIMER9_FREQ_MIN;
    //设置频率占空比
    TIM9->ARR = (uint16_t)(MCU_TIMER9_FREQ_CALC_BASE/freq);
	TIM9->CCR1 = (uint16_t)((MCU_TIMER9_FREQ_CALC_BASE/freq)/2);
}

//设置转速
void MCU_Timer9Ch1_SetRpm(float rpm)
{
    float freq = rpm;
    //转换RPS,除以60秒
    freq /= 60.0;
    //转换PPS,程序一圈200脉冲
    freq *= 200.0;
    //乘以细分数
    freq *= MCU_TIMER9_MOTOR_DRIVER_DIV;
    //写入频率
    MCU_Timer9Ch1_SetFreq((uint32_t)freq);
}

//启动定时器输出
void MCU_Timer9Ch1_StartOutput(void)
{
    if(timer9Ch1UpdataCallbackFuncPtr != NULL)
    {
        //打开周期计数中断
        __HAL_TIM_ENABLE_IT(&timer9BaseHandler,TIM_IT_UPDATE);
    }
    //启动定时器
    HAL_TIM_PWM_Start(&timer9BaseHandler,TIM_CHANNEL_1);
}

//关闭定时器输出
void MCU_Timer9Ch1_StopOutput(void)
{
    //关闭周期计数中断
    __HAL_TIM_DISABLE_IT(&timer9BaseHandler,TIM_IT_UPDATE);
    //定时器停止
    HAL_TIM_PWM_Stop(&timer9BaseHandler,TIM_CHANNEL_1);
    //清除中断标记
    __HAL_TIM_CLEAR_IT(&timer9BaseHandler,TIM_IT_UPDATE);
    //计数清零
    __HAL_TIM_SET_COUNTER(&timer9BaseHandler,0);
}



//定时器更新中断
void TIM1_BRK_TIM9_IRQHandler(void)
{
    //更新标记
    if (__HAL_TIM_GET_FLAG(&timer9BaseHandler, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&timer9BaseHandler, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&timer9BaseHandler, TIM_IT_UPDATE);
            if(timer9Ch1UpdataCallbackFuncPtr != NULL)
            {
                //调用回调函数
                timer9Ch1UpdataCallbackFuncPtr();
            }
        }
    }
    //通道1
    if (__HAL_TIM_GET_FLAG(&timer9BaseHandler, TIM_FLAG_CC1) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&timer9BaseHandler, TIM_IT_CC1) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&timer9BaseHandler, TIM_IT_CC1);
        }
    }
}







