/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 15:34:02 +0800
************************************************************************************************/ 
#include "MCU_Timer2Encoder.h"
#include "MCU_Pin.h"
#include "DriverConfigTimer.h"
#include "MCU_LogShow.h"

//定时器对象
static TIM_HandleTypeDef timer2EncoderHandler;
//定时器周期中断函数回调
void MCU_Timer2_EncoderPeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr); 
//溢出计数
static int32_t circleCount = 0;

//底层复位
static void MCU_Timer2_MspDeInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM2_CLK_ENABLE();
    //模块复位
    __HAL_RCC_TIM2_FORCE_RESET();
    //取消复位
    __HAL_RCC_TIM2_RELEASE_RESET();
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
    //IO禁用
    MCU_PinDeInit(PIN_TI1_MCU_TIMER2_ENCODER);
    MCU_PinDeInit(PIN_TI2_MCU_TIMER2_ENCODER);
#ifdef PIN_Z_SIGNAL_MCU_TIMER2_ENCODER
    MCU_PinDeInit(PIN_Z_SIGNAL_MCU_TIMER2_ENCODER);
#endif
}

//底层初始化
static void MCU_Timer2EncoderMspInit(void)
{
    //使能定时器时钟
    __HAL_RCC_TIM2_CLK_ENABLE();
    //端口初始化
    //TI1
    MCU_PinInit(PIN_TI1_MCU_TIMER2_ENCODER,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF1_TIM2);
    //TI2
    MCU_PinInit(PIN_TI2_MCU_TIMER2_ENCODER,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF1_TIM2);
#ifdef PIN_Z_SIGNAL_MCU_TIMER2_ENCODER
    //ZSIGNAL
    MCU_PinInit(PIN_Z_SIGNAL_MCU_TIMER2_ENCODER,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF1_TIM2);
#endif
}

//编码器初始化
void MCU_Timer2EncoderInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //如果设置过回调函数,说明已经初始化了
    if(timer2EncoderHandler.PeriodElapsedCallback == MCU_Timer2_EncoderPeriodElapsedCallback)
    {
        return;
    }
    TIM_Encoder_InitTypeDef sEncoderConfig = {0};
    //初始化
    timer2EncoderHandler.Instance = TIM2;
    //定时器回调函数复位
    TIM_ResetCallback(&timer2EncoderHandler);
    //设置定时器
    timer2EncoderHandler.Init.Prescaler = PRESCALER_MCU_TIMER2_ENCODER - 1;
    timer2EncoderHandler.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer2EncoderHandler.Init.Period = PERIOD_MCU_TIMER2_ENCODER;
    timer2EncoderHandler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    timer2EncoderHandler.Init.RepetitionCounter=0;
    timer2EncoderHandler.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
    //定时器底层复位
    MCU_Timer2_MspDeInit();
    //定时器复位
    halState = HAL_TIM_Encoder_DeInit(&timer2EncoderHandler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer2EncoderHandler.State);
        while(1);
    }
    //配置定时器编码器接口
#if(MODE_MCU_TIMER2_ENCODER == MODE_MCU_TIMER_ENCODER_TI1)
    sEncoderConfig.EncoderMode  = TIM_ENCODERMODE_TI1;  
#elif(MODE_MCU_TIMER2_ENCODER == MODE_MCU_TIMER_ENCODER_TI2)
    sEncoderConfig.EncoderMode  = TIM_ENCODERMODE_TI2;  
#elif(MODE_MCU_TIMER2_ENCODER == MODE_MCU_TIMER_ENCODER_TI12)
    sEncoderConfig.EncoderMode  = TIM_ENCODERMODE_TI12;  
#endif
    //TI1
    sEncoderConfig.IC1Polarity  = ICPOLARITY_TI1_MCU_TIMER2_ENCODER;   
    sEncoderConfig.IC1Selection = LINK_TI_MCU_TIMER2_ENCODER;  
    sEncoderConfig.IC1Prescaler = ICPCS_TI_MCU_TIMER2_ENCODER; 
    sEncoderConfig.IC1Filter    = FILTER_TI_MCU_TIMER2_ENCODER;
    //TI2
    sEncoderConfig.IC2Polarity  = ICPOLARITY_TI2_MCU_TIMER2_ENCODER;   
    sEncoderConfig.IC2Selection = LINK_TI_MCU_TIMER2_ENCODER;  
    sEncoderConfig.IC2Prescaler = ICPCS_TI_MCU_TIMER2_ENCODER; 
    sEncoderConfig.IC2Filter    = FILTER_TI_MCU_TIMER2_ENCODER;
    //编码器底层初始化
    MCU_Timer2EncoderMspInit();
    //编码器初始化
    halState = HAL_TIM_Encoder_Init(&timer2EncoderHandler, &sEncoderConfig);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer2EncoderHandler.State);
        while(1);
    }
    //关联溢出函数
    timer2EncoderHandler.PeriodElapsedCallback = MCU_Timer2_EncoderPeriodElapsedCallback;
    //清除更新中断标志位
    __HAL_TIM_CLEAR_IT(&timer2EncoderHandler, TIM_IT_UPDATE);  
    //仅允许计数器溢出才产生更新中断
    __HAL_TIM_URS_ENABLE(&timer2EncoderHandler);   
    //使能更新中断            
    __HAL_TIM_ENABLE_IT(&timer2EncoderHandler,TIM_IT_UPDATE);
    //停止计数
    halState = HAL_TIM_Encoder_Stop(&timer2EncoderHandler,TIM_CHANNEL_ALL);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer2EncoderHandler.State);
        while(1);
    }
    //设置初始计数值
    circleCount = 0;
    __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,INIT_COUNT_MCU_TIMER2_ENCODER);
    //配置中断优先级并打开中断
    HAL_NVIC_SetPriority(TIM2_IRQn, ENCODER_TIMER2_PREE_PRI_INT, ENCODER_TIMER2_SUB_PRI_INT);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    //启动计数
    MCU_Timer2EncoderStart();
}

//编码器开始
void MCU_Timer2EncoderStart(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //清除更新中断标志位
    __HAL_TIM_CLEAR_IT(&timer2EncoderHandler, TIM_IT_UPDATE);  
    //仅允许计数器溢出才产生更新中断
    __HAL_TIM_URS_ENABLE(&timer2EncoderHandler);   
    //使能更新中断            
    __HAL_TIM_ENABLE_IT(&timer2EncoderHandler,TIM_IT_UPDATE);
    //开始计数
    halState = HAL_TIM_Encoder_Start(&timer2EncoderHandler,TIM_CHANNEL_ALL);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer2EncoderHandler.State);
        while(1);
    }
    //打开中断
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

//编码器停止
void MCU_Timer2EncoderStop(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //停止计数
    halState = HAL_TIM_Encoder_Stop(&timer2EncoderHandler,TIM_CHANNEL_ALL);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,timer2EncoderHandler.State);
        while(1);
    }
    //清除更新中断标志位
    __HAL_TIM_CLEAR_IT(&timer2EncoderHandler, TIM_IT_UPDATE);
    //禁止更新中断            
    __HAL_TIM_DISABLE_IT(&timer2EncoderHandler,TIM_IT_UPDATE);
    //关闭定时器中断
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
}

//复位编码器信号为0
void MCU_Timer2EncoderResetCount(void)
{
    //设置初始计数值
    circleCount = 0;
    __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,INIT_COUNT_MCU_TIMER2_ENCODER);
}

//设置编码器当前计数值
void MCU_Timer2EncoderSetCountCurrent(int32_t setCount)
{
    //加锁定
    uint32_t lastIntState;
    lastIntState = CoreLockIntGlobal();
    if(setCount >= 0)
    {
        if(setCount <= INIT_COUNT_MCU_TIMER2_ENCODER)
        {
            circleCount = 0;
            __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,INIT_COUNT_MCU_TIMER2_ENCODER+setCount);
        }
        else
        {
            setCount = setCount - INIT_COUNT_MCU_TIMER2_ENCODER;
            circleCount = setCount/PERIOD_MCU_TIMER2_ENCODER;
            circleCount += 1;
            setCount = setCount%PERIOD_MCU_TIMER2_ENCODER;
            __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,setCount);
        }
    }
    else
    {
        setCount = 0 - setCount;
        if(setCount <= INIT_COUNT_MCU_TIMER2_ENCODER)
        {
            circleCount = 0;
            __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,INIT_COUNT_MCU_TIMER2_ENCODER-setCount);
        }
        else
        {
            setCount = setCount - INIT_COUNT_MCU_TIMER2_ENCODER;
            circleCount = setCount/PERIOD_MCU_TIMER2_ENCODER;
            circleCount += 1;
            circleCount = 0 - circleCount;
            setCount = setCount%PERIOD_MCU_TIMER2_ENCODER;
            __HAL_TIM_SET_COUNTER(&timer2EncoderHandler,PERIOD_MCU_TIMER2_ENCODER - setCount);
        }
    }
    //解锁
    CoreUnlockIntGlobal(lastIntState);
}


//获取编码器当前位置
int32_t MCU_Timer2EncoderGetCurrentCount(void)
{
    //当前坐标
    int32_t currentCount = (int32_t)(__HAL_TIM_GET_COUNTER(&timer2EncoderHandler));
    //计算矫正
    int32_t realCount = currentCount + PERIOD_MCU_TIMER2_ENCODER*((int32_t)circleCount);
    //去除初始值
    realCount -= INIT_COUNT_MCU_TIMER2_ENCODER;
//如果打开了四倍采样,那么要对四倍采样进行处理
#if(MODE_MCU_TIMER2_ENCODER == MODE_MCU_TIMER_ENCODER_TI12)
    realCount /= 4;
#endif
    return realCount;
}

//定时器周期中断函数回调
void MCU_Timer2_EncoderPeriodElapsedCallback(TIM_HandleTypeDef* timHandlePtr)
{
    if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&timer2EncoderHandler))
    {
        circleCount -= 1;
    }
    else
    {
        circleCount += 1;
    }
}

//定时器中断
void TIM2_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用定时器中断处理函数
    HAL_TIM_IRQHandler(&timer2EncoderHandler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}









