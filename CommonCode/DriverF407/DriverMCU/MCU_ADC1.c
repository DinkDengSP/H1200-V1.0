/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-21 09:13:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-21 14:21:38 +0800
************************************************************************************************/ 
#include "MCU_ADC1.h"
#include "tx_mutex.h"
#include "MCU_LogShow.h"

//ADC转换器句柄
ADC_HandleTypeDef ADC1_Handle = {0};
//ADC配置句柄
ADC_ChannelConfTypeDef ADC1_ChannelConfig = {0};
//互斥信号量
static TX_MUTEX mutexMcuAdc1;

//获取互斥信号量
static void MCU_ADC1_GetLock(void)
{
    tx_mutex_get(&mutexMcuAdc1,TX_WAIT_FOREVER);
}

//释放互斥信号量
static void MCU_ADC1_ReleaseLock(void)
{
    tx_mutex_put(&mutexMcuAdc1);
}

/*-------------------------------------ADC查询模式-----------------------------------------*/
#if(MODE_ADC1_USER == MODE_ADC1_POOL)
//初始化
void MCU_ADC1_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    if(mutexMcuAdc1.tx_mutex_id != TX_MUTEX_ID)
    {
        tx_mutex_create(&mutexMcuAdc1,"mutexMcuAdc1",TX_INHERIT);
    }
    //首先关闭ADC时钟
    __HAL_RCC_ADC1_CLK_DISABLE();	
    //打开ADC时钟
    __HAL_RCC_ADC1_CLK_ENABLE();	
    //ADC模块强制复位
    __HAL_RCC_ADC_FORCE_RESET();
    //ADC模块释放复位
    __HAL_RCC_ADC_RELEASE_RESET();			      
    //ADC复位
	ADC1_Handle.Instance = ADC1;
    halState = HAL_ADC_DeInit(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    return;
}

//ADC转换终止
static void MCU_ADC1_Stop(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //ADC采样停止
    halState = HAL_ADC_Stop(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
}

//ADC转换开始
static void MCU_ADC1_Start(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //ADC采样启动
    halState = HAL_ADC_Start(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
}

//ADC通道配置
void MCU_ADC1_ConfigChannel(MCU_ADC_CHANNEL_CONFIG* channelConfigs,uint8_t configCount)
{
    HAL_StatusTypeDef halState = HAL_OK;	   
    uint8_t indexUtil = 0;   
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //ADC句柄设置
	ADC1_Handle.Instance=ADC1;
    //ADC终止
    MCU_ADC1_Stop();
    //ADC模块复位
    halState = HAL_ADC_DeInit(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    //时钟分频
    ADC1_Handle.Init.ClockPrescaler = CLK_DIV_MCU_ADC1;
    //ADC分辨率
    ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    //对齐模式,查询模式下,统一设置成间断模式,驱动一次转换一次
	ADC1_Handle.Init.DataAlign = RESULT_ALIGN_MODE_MCU_ADC1; 
    //禁止外部边沿触发
    ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;            
	//扫描模式设定,转换通道数大于1的时候,才能开启连续扫描模式,否则不能开启
    if(configCount > 1)
    {
        ADC1_Handle.Init.ScanConvMode = ENABLE;      
    }
    else
    {
        ADC1_Handle.Init.ScanConvMode = DISABLE;    
    }             
	//连续转换设定
    ADC1_Handle.Init.ContinuousConvMode = DISABLE; 
    ADC1_Handle.Init.NbrOfConversion = 0;     
    //间断转换设定
    ADC1_Handle.Init.DiscontinuousConvMode = ENABLE;
    ADC1_Handle.Init.NbrOfDiscConversion = configCount;         
	//触发模式
	ADC1_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;       
    //关闭连续转换请求
    ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
    //转换完成标志
    ADC1_Handle.Init.EOCSelection          = ENABLE;
	//初始化
	halState = HAL_ADC_Init(&ADC1_Handle); 
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    //通道配置
    for(indexUtil = 0; indexUtil < configCount;indexUtil++)
    {
        //端口初始化
        if(MCU_PIN_NULL != channelConfigs[indexUtil].adcChannelPin)
        {
            MCU_PinDeInit(channelConfigs[indexUtil].adcChannelPin);
            MCU_PinInit(channelConfigs[indexUtil].adcChannelPin,MCU_PIN_MODE_ANALOG,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,0);
        }
        //ADC通道号码
        ADC1_ChannelConfig.Channel      = channelConfigs[indexUtil].channel;
        //ADC转换序号
        ADC1_ChannelConfig.Rank         = channelConfigs[indexUtil].rankSet;
        //采样周期	
        ADC1_ChannelConfig.SamplingTime = channelConfigs[indexUtil].sampleTime ;
        ADC1_ChannelConfig.Offset = 0;
        //配置 ADC 通道转换
        halState = HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_ChannelConfig);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
            while(1);
        }
    }
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
}

//ADC转换
void MCU_ADC1_ConvertOnce(uint16_t* dataBuffer,uint8_t rankCount)
{
    uint8_t indexUtil = 0;
    //等待转换完成
    HAL_StatusTypeDef halState = HAL_OK;
    //获取互斥信号量
    MCU_ADC1_GetLock();
    for(indexUtil = 0;indexUtil < rankCount;indexUtil++)
    {
        //启动转换
        MCU_ADC1_Start();
        //等待结果
        halState = HAL_ADC_PollForConversion(&ADC1_Handle,TIME_OUT_MS_ADC1_CONVERT);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
            while(1);
        }
        //读取结果
        dataBuffer[indexUtil] = HAL_ADC_GetValue(&ADC1_Handle);
    }
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
}


//获取ADC模块温度值
float MCU_ADC1_GetChipTemp(uint16_t sampleCount)
{
    uint16_t convertIndexUtil = 0;
    uint16_t convertResultSingle = 0;
    uint32_t convertResultSum = 0;
    double temperatureResult = 0.0;
    //温度转换配置
    MCU_ADC_CHANNEL_CONFIG tempConvConfig;
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //停止转换
    MCU_ADC1_Stop();
    //配置温度转换
    tempConvConfig.adcChannelPin = MCU_PIN_NULL;
    tempConvConfig.rankSet = MCU_ADC_REGULAR_RANK_1;
    tempConvConfig.channel = MCU_ADC_CHANNEL_TEMPSENSOR;
    tempConvConfig.sampleTime = MCU_ADC_SAMPLE_480CYCLES;
    MCU_ADC1_ConfigChannel(&tempConvConfig,1);
    for(convertIndexUtil = 0;convertIndexUtil < sampleCount;convertIndexUtil++)
    {
        //等待转换数据
        MCU_ADC1_ConvertOnce(&convertResultSingle,1);
        //累加
        convertResultSum += convertResultSingle;
    }
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
    //计算平均值
    convertResultSingle = convertResultSum/sampleCount;
    //电压值
    temperatureResult = (double)convertResultSingle*(3.3/4096);	
    //转换为温度值 	
	temperatureResult = (temperatureResult-0.76)/0.0025 + 25; 
	return (float)temperatureResult;
}

#endif

/*-------------------------------------ADC_DMA模式-----------------------------------------*/
#if(MODE_ADC1_USER == MODE_ADC1_DMA)
#include "UserMemManage.h"
//DMA句柄
static DMA_HandleTypeDef ADC1_DMA_Handler = {0};
//DMA缓冲区
static uint16_t* adcConvertBufferPtr;
//采样数据长度
static uint16_t sampleDataLength;
//采样完成标记
static FlagStatus convertOverState;

//ADC初始化
void MCU_ADC1_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    if(mutexMcuAdc1.tx_mutex_id != TX_MUTEX_ID)
    {
        tx_mutex_create(&mutexMcuAdc1,"mutexMcuAdc1",TX_INHERIT);
    }
    //首先关闭ADC时钟
    __HAL_RCC_ADC1_CLK_DISABLE();	
    //打开ADC时钟
    __HAL_RCC_ADC1_CLK_ENABLE();	
    //ADC模块强制复位
    __HAL_RCC_ADC_FORCE_RESET();
    //ADC模块释放复位
    __HAL_RCC_ADC_RELEASE_RESET();			      
    //ADC复位
	ADC1_Handle.Instance=ADC1;
    halState = HAL_ADC_DeInit(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    //打开NVIC并且设置中断优先级
    HAL_NVIC_SetPriority(ADC_IRQn, ADC1_SUB_PRI, ADC1_SUB_PRI);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    //打开DMA时钟
    MCU_ADC1_DMA_CLK_ENABLE();
    //DMA复位
    ADC1_DMA_Handler.Instance = 	MCU_ADC1_DMA_INSTANCE;
    halState = HAL_DMA_DeInit(&ADC1_DMA_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_DMA_Handler.State);
        while(1);
    }
    //打开DMA中断并且设置中断优先级
    HAL_NVIC_SetPriority(MCU_ADC1_DMA_IRQ, ADC1_DMA_PREE_PRI, ADC1_DMA_SUB_PRI);
    HAL_NVIC_EnableIRQ(MCU_ADC1_DMA_IRQ);
    //初始化内存
    adcConvertBufferPtr = NULL;
    sampleDataLength = 0;
    convertOverState = RESET;
    return;
}

//ADC转换终止
static void MCU_ADC1_Stop(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //ADC采样停止
    halState = HAL_ADC_Stop_DMA(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
}

//ADC转换开始
static void MCU_ADC1_Start(uint16_t* recvDataBufferPtr,uint16_t dataLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //ADC采样启动
    halState = HAL_ADC_Start_DMA(&ADC1_Handle,(uint32_t*)recvDataBufferPtr,dataLength);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
}

//ADC通道配置,设置采样点数
void MCU_ADC1_ConfigChannel(MCU_ADC_CHANNEL_CONFIG* channelConfigs,uint8_t rankCount)
{
    HAL_StatusTypeDef halState = HAL_OK;	
    uint8_t rankIndexUtil = 0;      
    //ADC句柄设置
	ADC1_Handle.Instance=ADC1;
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //ADC终止
    MCU_ADC1_Stop();
    //ADC模块复位
    halState = HAL_ADC_DeInit(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    //DMA模块复位
    halState = HAL_DMA_DeInit(&ADC1_DMA_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_DMA_Handler.State);
        while(1);
    }
    //时钟分频
    ADC1_Handle.Init.ClockPrescaler = CLK_DIV_MCU_ADC1;
    //ADC分辨率
    ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    //对齐模式,查询模式下,统一设置成间断模式,驱动一次转换一次
	ADC1_Handle.Init.DataAlign = RESULT_ALIGN_MODE_MCU_ADC1; 
    //禁止外部边沿触发
    ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;            
	//扫描模式设定,转换通道数大于1的时候,才能开启连续扫描模式,否则不能开启
    if(rankCount > 1)
    {
        ADC1_Handle.Init.ScanConvMode = ENABLE;      
    }
    else
    {
        ADC1_Handle.Init.ScanConvMode = DISABLE;    
    }   
    ADC1_Handle.Init.ContinuousConvMode = ENABLE;      
	//扫描几个通道
	ADC1_Handle.Init.NbrOfConversion = rankCount;
    //间断转换设定
    ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
    //不连续采样通道数设定
    ADC1_Handle.Init.NbrOfDiscConversion = 0;              
	//触发模式
	ADC1_Handle.Init.ExternalTrigConv = EXTERN_TRIG_MODE_MCU_ADC1;    
    //关闭连续转换请求
    ADC1_Handle.Init.DMAContinuousRequests = ENABLE;
    //转换完成标志
    ADC1_Handle.Init.EOCSelection          = DISABLE;   
	//初始化
	halState = HAL_ADC_Init(&ADC1_Handle); 
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
        while(1);
    }
    //通道配置
    for(rankIndexUtil = 0; rankIndexUtil < rankCount;rankIndexUtil++)
    {
        //端口初始化
        if(MCU_PIN_NULL != channelConfigs[rankIndexUtil].adcChannelPin)
        {
            MCU_PinDeInit(channelConfigs[rankIndexUtil].adcChannelPin);
            MCU_PinInit(channelConfigs[rankIndexUtil].adcChannelPin,MCU_PIN_MODE_ANALOG,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,0);
        }
        //ADC通道号码
        ADC1_ChannelConfig.Channel      = channelConfigs[rankIndexUtil].channel;
        //ADC转换序号
        ADC1_ChannelConfig.Rank         = channelConfigs[rankIndexUtil].rankSet;
        //采样周期	
        ADC1_ChannelConfig.SamplingTime = channelConfigs[rankIndexUtil].sampleTime ;
        //配置 ADC 通道转换
        halState = HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_ChannelConfig);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,ADC1_Handle.ErrorCode);
            while(1);
        }
    }
    
    //DMA初始化
    ADC1_DMA_Handler.Instance       = MCU_ADC1_DMA_INSTANCE;//实体
    ADC1_DMA_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;//存储器到外设
    ADC1_DMA_Handler.Init.Channel   = MCU_ADC1_DMA_CHANNEL;//通道
    ADC1_DMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;//外设非增量模式
    ADC1_DMA_Handler.Init.MemInc    = DMA_MINC_ENABLE;//存储器增量模式 
    ADC1_DMA_Handler.Init.PeriphDataAlignment   =DMA_PDATAALIGN_HALFWORD;//外设数据长度:16位
    ADC1_DMA_Handler.Init.MemDataAlignment      =DMA_MDATAALIGN_HALFWORD;//存储器数据长度:16位
    ADC1_DMA_Handler.Init.Mode      = DMA_NORMAL;//外设普通模式
    ADC1_DMA_Handler.Init.Priority  = MCU_ADC1_DMA_PRIORITY;//中等优先级
    ADC1_DMA_Handler.Init.FIFOMode  = DMA_FIFOMODE_DISABLE;
    ADC1_DMA_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    ADC1_DMA_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	// DMA_MBURST_INC4; 
    ADC1_DMA_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	// DMA_PBURST_INC4; 
    //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
    HAL_DMA_Init(&ADC1_DMA_Handler); 
    //链接外设
    __HAL_LINKDMA(&ADC1_Handle,DMA_Handle,ADC1_DMA_Handler);
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
}


//启动转换
void MCU_ADC1_ConvertStart(uint8_t rankCount,uint16_t exceptSampleCount)
{
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //生成数据缓冲
    if(adcConvertBufferPtr != NULL)
    {
        //释放缓存
        UserMemFree(MEM_REGION_MCU_ADC1,adcConvertBufferPtr);
        adcConvertBufferPtr = NULL;
    }
    //创建缓存
    adcConvertBufferPtr = UserMemMallocWhileSuccess(MEM_REGION_MCU_ADC1,exceptSampleCount*rankCount*sizeof(uint16_t));
    //设置期望点数
    convertOverState = RESET;
    sampleDataLength = exceptSampleCount*rankCount;
    //ADC采样启动
    MCU_ADC1_Start(adcConvertBufferPtr,sampleDataLength);
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
}


//检查转换是否完成
FlagStatus MCU_ADC1_CheckConvertIsOver(void)
{
    return convertOverState;
}


//获取转换数据并且清除内存储存的转换历史
void MCU_ADC1_GetConvertResultClearHistory(uint16_t** dataBufferPtrPtr,uint16_t* dataLength)
{
    if(adcConvertBufferPtr == NULL)
    {
        *dataLength = 0;
        *dataBufferPtrPtr = NULL;
    }
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //获取元素数量
    *dataLength = sampleDataLength;
    //申请内存
    *dataBufferPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MCU_ADC1,(*dataLength)*sizeof(uint16_t));
    //数据读取
    UserMemCopy((void*)*dataBufferPtrPtr,(void*)adcConvertBufferPtr,sampleDataLength*sizeof(uint16_t));
    //释放内部存储
    UserMemFree(MEM_REGION_MCU_ADC1,adcConvertBufferPtr);
    adcConvertBufferPtr = NULL;
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
}

//释放读取数据
void MCU_ADC1_ReleaseConvertResult(uint16_t* dataBufferPtrPtr)
{
    if(dataBufferPtrPtr == NULL)
    {
        return;
    }
    UserMemFree(MEM_REGION_MCU_ADC1,dataBufferPtrPtr);
}


//获取ADC模块温度值
float MCU_ADC1_GetChipTemp(uint16_t sampleCount)
{
    uint16_t convertIndexUtil = 0;
    uint16_t* convertResultArrayPtr = 0;
    uint32_t convertResultSum = 0;
    double temperatureResult = 0.0;
    //温度转换配置
    MCU_ADC_CHANNEL_CONFIG tempConvConfig;
    //获取互斥信号量
    MCU_ADC1_GetLock();
    //停止转换
    MCU_ADC1_Stop();
    //配置温度转换
    tempConvConfig.adcChannelPin = MCU_PIN_NULL;
    tempConvConfig.rankSet = MCU_ADC_REGULAR_RANK_1;
    tempConvConfig.channel = MCU_ADC_CHANNEL_TEMPSENSOR;
    tempConvConfig.sampleTime = MCU_ADC_SAMPLE_480CYCLES;
    MCU_ADC1_ConfigChannel(&tempConvConfig,1);
    //开始转换
    MCU_ADC1_ConvertStart(1,sampleCount);
    //等待转换完成
    while(RESET == MCU_ADC1_CheckConvertIsOver())
    {
        CoreDelayMinTick();
    }
    //读取数据
    MCU_ADC1_GetConvertResultClearHistory(&convertResultArrayPtr,&sampleCount);
    if(sampleCount == 0)
    {
        //释放互斥信号量
        MCU_ADC1_ReleaseLock();
        return 0.0;
    }
    //计算和
    for(convertIndexUtil = 0;convertIndexUtil < sampleCount;convertIndexUtil++)
    {
        //累加
        convertResultSum += convertResultArrayPtr[convertIndexUtil];
    }
    //处理完成,释放指针
    MCU_ADC1_ReleaseConvertResult(convertResultArrayPtr);
    //释放互斥信号量
    MCU_ADC1_ReleaseLock();
    //计算平均值
    convertResultSum = convertResultSum/sampleCount;
    //电压值
    temperatureResult = (double)convertResultSum*(3.3/4096);	
    //转换为温度值 	
	temperatureResult = (temperatureResult-0.76)/0.0025 + 25; 	
	return (float)temperatureResult;
}

/*-------------------------------回调函数--------------------------------------*/
//转换完成回调
void MCU_ADC1_ConvCpltCallbackDMA(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //采样完成,停止采样
    halState = HAL_ADC_Stop_DMA(&ADC1_Handle);
    if(halState != HAL_OK)
    {
        MCU_LogSendStringInt("MCU_ADC1_ConvCpltCallbackDMA HAL_ADC_Stop_DMA Failed!!\r\n");
    }
    //设置标记
    convertOverState = SET;
#if(ADC1_INT_MSG_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_ADC1_ConvCpltCallbackDMA\r\n");
#endif
}

//转换半完成回调
void MCU_ADC1_ConvHalfCpltCallbackDMA(void)
{
#if(ADC1_INT_MSG_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_ADC1_ConvHalfCpltCallbackDMA\r\n");
#endif
}

//溢出回调
void MCU_ADC1_LevelOutOfWindowCallbackDMA(void)
{
#if(ADC1_ERR_INT_MSG_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_ADC1_LevelOutOfWindowCallbackDMA\r\n");
#endif
}

//错误回调
void MCU_ADC1_ErrorCallbackDMA(void)
{
#if(ADC1_ERR_INT_MSG_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_ADC1_ErrorCallbackDMA\r\n");
#endif
}

//注入转换完成回调
void MCU_ADC1_InjectedConvCpltCallbackDMA(void)
{
#if(ADC1_INT_MSG_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_ADC1_InjectedConvCpltCallbackDMA\r\n");
#endif
}

//ADC中断回调函数
void ADC_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_ADC_IRQHandler(&ADC1_Handle);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//DMA中断回调
void MCU_ADC1_DMA_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&ADC1_DMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
/*------------------------------------------------------------------------*/

#endif





