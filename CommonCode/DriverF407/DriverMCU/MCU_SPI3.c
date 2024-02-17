/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 16:01:55 +0800
************************************************************************************************/ 
#include "MCU_SPI3.h"
#include "MCU_Pin.h"
#include "MCU_LogShow.h"
#include "tx_mutex.h"
#include "DriverConfigMCU.h"

//保存配置
static SPI23_CONFIG spi3Config;
//互斥量
static TX_MUTEX mutexMCU_SPI3;
//SPI3操作句柄
static SPI_HandleTypeDef SPI3_Handler = {0};


//SPI底层复位
static void MCU_SPI3_MspDeInit(void)
{
    //IO口复位
    MCU_PinDeInit(MCU_SPI3_PIN_SCK);
    MCU_PinDeInit(MCU_SPI3_PIN_MISO);
    MCU_PinDeInit(MCU_SPI3_PIN_MOSI);
    //模块复位
    __HAL_RCC_SPI3_FORCE_RESET();
    __HAL_RCC_SPI3_RELEASE_RESET();
    //关闭时钟
    __HAL_RCC_SPI3_CLK_DISABLE();
}

//SPI底层初始化
static void MCU_SPI3_MspInit(void)
{
    //模块时钟使能
    __HAL_RCC_SPI3_CLK_ENABLE();
    //IO口初始化
    //SCK
    MCU_PinInit(MCU_SPI3_PIN_SCK,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF6_SPI3);
    //MISO
    MCU_PinInit(MCU_SPI3_PIN_MISO,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF6_SPI3);
    //MOSI
    MCU_PinInit(MCU_SPI3_PIN_MOSI,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,GPIO_AF6_SPI3);
}

//锁定SPI3
void MCU_SPI3_GetLock(void)
{
    tx_mutex_get(&mutexMCU_SPI3,TX_WAIT_FOREVER);
}

//解锁SPI
void MCU_SPI3_ReleaseLock(void)
{
    tx_mutex_put(&mutexMCU_SPI3);
}

//MCU SPI基础初始化
static void MCU_SPI3_BaseConfig(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    HAL_StatusTypeDef halState;
    //参数记录
    spi3Config.baudRate = speed;
    spi3Config.cpolConfig = cpol;
    spi3Config.cphaConfig = cpha;
    //设置SPI3
    SPI3_Handler.Instance = SPI3;
    //端口重新初始化
    MCU_SPI3_MspDeInit();
    //模块重新复位
    do
    {
        halState = HAL_SPI_DeInit(&SPI3_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,SPI3_Handler.ErrorCode);
        }
    }while(halState != HAL_OK);
    //设置SPI通讯参数
    SPI3_Handler.Init.Mode 			    = SPI_MODE_MASTER;
	SPI3_Handler.Init.Direction         = SPI_DIRECTION_2LINES;
    SPI3_Handler.Init.DataSize          = SPI_DATASIZE_8BIT;
    SPI3_Handler.Init.CLKPolarity       = cpol;
	SPI3_Handler.Init.CLKPhase          = cpha;
    SPI3_Handler.Init.NSS               = SPI_NSS_SOFT;
    SPI3_Handler.Init.BaudRatePrescaler = speed;
	SPI3_Handler.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SPI3_Handler.Init.TIMode            = SPI_TIMODE_DISABLE;
	SPI3_Handler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SPI3_Handler.Init.CRCPolynomial     = 7;
    //端口重新初始化
    MCU_SPI3_MspInit();
    //初始化
	do
    {
        halState = HAL_SPI_Init(&SPI3_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,SPI3_Handler.ErrorCode);
        }
    }while(halState != HAL_OK);
}

//SPI3设定传输速度
void MCU_SPI3_SetSpeed(MCU_SPI_23_CLK speed)
{
    //获取锁
    MCU_SPI3_GetLock();
    //关闭SPI
    __HAL_SPI_DISABLE(&SPI3_Handler);            
    //位30-28清零，用来设置波特率
    SPI3_Handler.Instance->CR1&=0XFFC7;  
    //设置SPI速度   
    SPI3_Handler.Instance->CR1|=speed;
    //使能SPI
    __HAL_SPI_ENABLE(&SPI3_Handler); 
    //释放锁
    MCU_SPI3_ReleaseLock();
}


/*------------------------------------使用查询进行SPI通讯---------------------------------------------*/
#if(MCU_SPI3_MODE_USER == MCU_SPI_MODE_POOL)
//SPI3初始化
void MCU_SPI3_Init(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    if(mutexMCU_SPI3.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化了
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_SPI3,"mutexMCU_SPI3",TX_INHERIT);
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
}

//SPI3 通讯参数重新设置
void MCU_SPI3_ResetParam(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //没有变化不用改
    if((spi3Config.baudRate == speed)&&(spi3Config.cpolConfig == cpol)&&
        (spi3Config.cphaConfig == cpha))
    {
        return;
    }
    //获取锁
    MCU_SPI3_GetLock();
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
    //释放锁
    MCU_SPI3_ReleaseLock();
}

//SPI3读写数据
ERROR_SUB MCU_SPI3_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength)
{
    HAL_StatusTypeDef halState;
    //获取锁
    MCU_SPI3_GetLock();
    if((writeDataPtr != SPI_PTR_NULL)&&(readDataPtr != SPI_PTR_NULL))
    {
        //同时收发
        halState = HAL_SPI_TransmitReceive(&SPI3_Handler, (uint8_t*)writeDataPtr, (uint8_t *)readDataPtr, dataLength, 
                                            TIME_OUT_MS_SPI_SINGLE_BYTE*dataLength);
    }
    else if(writeDataPtr != SPI_PTR_NULL)
    {
        //只发不收
        halState = HAL_SPI_Transmit(&SPI3_Handler, (uint8_t*)writeDataPtr, dataLength, 
                                            TIME_OUT_MS_SPI_SINGLE_BYTE*dataLength);
    }
    else if(readDataPtr != SPI_PTR_NULL)
    {
        //只收不发
        halState = HAL_SPI_Receive(&SPI3_Handler, (uint8_t*)readDataPtr, dataLength, 
                                            TIME_OUT_MS_SPI_SINGLE_BYTE*dataLength);
    }
    else
    {
        //释放锁
        MCU_SPI3_ReleaseLock();
        return ERROR_SUB_OK;
    }
    if(halState != HAL_OK)
    {
        //释放锁
        MCU_SPI3_ReleaseLock();
        //显示错误
        MCU_LogShowHalState(halState,SPI3_Handler.ErrorCode);
        return ERROR_SUB_MCU_SPI3_TRANS_TIME_OUT;
    }
    //释放锁
    MCU_SPI3_ReleaseLock();
    return ERROR_SUB_OK;
}

#endif


/*------------------------------------使用中断进行SPI通讯---------------------------------------------*/
#if(MCU_SPI3_MODE_USER == MCU_SPI_MODE_INT)
//传输标记,中断模式下使用
static SPI_TRANS_STATE transState;
//传输标记,标记是否取消传输完成
static uint8_t spi3AbortTransFlag = 0;

//取消传输标记
void MCU_SPI3_AbortCpltCallback_IT(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送取消完成标记
    spi3AbortTransFlag = 1;
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_AbortCpltCallback_IT\r\n");
#endif
}

//收发完成中断回调函数
void MCU_SPI3_TxRxCpltCallback_Int(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送信号量标记
    transState = SPI_TRANS_STATE_END;
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_TxRxCpltCallback_Int\r\n");
#endif
}

//发送中断回调函数
void MCU_SPI3_TxCpltCallback_Int(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送信号量标记
    transState = SPI_TRANS_STATE_END;
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_TxCpltCallback_Int\r\n");
#endif
}

//接收中断回调函数
void MCU_SPI3_RxCpltCallback_Int(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送信号量标记
    transState = SPI_TRANS_STATE_END;
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_RxCpltCallback_Int\r\n");
#endif
}

//错误回调函数
void MCU_SPI3_ErrorCallback_Int(SPI_HandleTypeDef* spiHandlerPtr)
{
    //中断打印数据
    transState = SPI_TRANS_STATE_ERROR;
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_ErrorCallback_Int\r\n");
#endif
}

//清除取消传输标记
static void MCU_SPI3_ClearAbortTransFlag(void)
{
    spi3AbortTransFlag = 0;
}

//等待传输取消完成标记
static ERROR_SUB MCU_SPI3_WaitAbortTransComplete(uint32_t timeOutMs)
{
    int32_t startWaitTimeStamp = tx_time_get();
    do
    {
        if(spi3AbortTransFlag == 0)
        {
            CoreDelayMinTick();
        }
        else
        {
            return ERROR_SUB_OK;
        }
    }while(tx_time_get() - startWaitTimeStamp <= timeOutMs);
    return ERROR_SUB_MCU_SPI3_WAIT_ABORT_COMPLETE;
}

//SPI3初始化
void MCU_SPI3_Init(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    if(mutexMCU_SPI3.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化了
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_SPI3,"mutexMCU_SPI3",TX_INHERIT);
    //先关闭中断
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
    //SPI中断关联
    SPI3_Handler.TxCpltCallback = MCU_SPI3_TxCpltCallback_Int;
    SPI3_Handler.RxCpltCallback = MCU_SPI3_RxCpltCallback_Int;
    SPI3_Handler.TxRxCpltCallback = MCU_SPI3_TxRxCpltCallback_Int;
    SPI3_Handler.ErrorCallback = MCU_SPI3_ErrorCallback_Int;
    SPI3_Handler.AbortCpltCallback = MCU_SPI3_AbortCpltCallback_IT;
    //传输状态设置为空闲状态
    transState = SPI_TRANS_STATE_IDLE;
    //配置完成,配置相关的中断
    HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_PREE_PRI_INT, SPI3_SUB_PRI_INT);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);

}

//SPI3 通讯参数重新设置
void MCU_SPI3_ResetParam(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //没有变化不用改
    if((spi3Config.baudRate == speed)&&(spi3Config.cpolConfig == cpol)&&
        (spi3Config.cphaConfig == cpha))
    {
        return;
    }
    //获取锁
    MCU_SPI3_GetLock();
    //先关闭中断
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
    //SPI中断关联
    SPI3_Handler.TxCpltCallback = MCU_SPI3_TxCpltCallback_Int;
    SPI3_Handler.RxCpltCallback = MCU_SPI3_RxCpltCallback_Int;
    SPI3_Handler.TxRxCpltCallback = MCU_SPI3_TxRxCpltCallback_Int;
    SPI3_Handler.ErrorCallback = MCU_SPI3_ErrorCallback_Int;
    SPI3_Handler.AbortCpltCallback = MCU_SPI3_AbortCpltCallback_IT;
    transState = SPI_TRANS_STATE_IDLE;
    //配置完成,配置相关的中断
    HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_PREE_PRI_INT, SPI3_SUB_PRI_INT);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);
    //释放锁
    MCU_SPI3_ReleaseLock();
}

//SPI取消传输
ERROR_SUB MCU_SPI3_AbortTrans(uint32_t timeOutMs)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先清除取消传输标记
    MCU_SPI3_ClearAbortTransFlag();
    //发送取消传输指令
    HAL_SPI_Abort_IT(&SPI3_Handler);
    //等待取消传输状态匹配
    errorSubCode = MCU_SPI3_WaitAbortTransComplete(timeOutMs);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
    }
    //返回结果
    return errorSubCode;
}

//SPI3读写数据
ERROR_SUB MCU_SPI3_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength)
{
    HAL_StatusTypeDef halState;
    //状态设置为启动
    transState = SPI_TRANS_STATE_START;
    //获取锁
    MCU_SPI3_GetLock();
    //选择传输模式
    if((writeDataPtr != SPI_PTR_NULL)&&(readDataPtr != SPI_PTR_NULL))
    {
        //同时收发
        halState = HAL_SPI_TransmitReceive_IT(&SPI3_Handler, (uint8_t*)writeDataPtr, (uint8_t *)readDataPtr, dataLength);
    }
    else if(writeDataPtr != SPI_PTR_NULL)
    {
        //只发不收
        halState = HAL_SPI_Transmit_IT(&SPI3_Handler, (uint8_t*)writeDataPtr, dataLength);
    }
    else if(readDataPtr != SPI_PTR_NULL)
    {
        //只收不发
        halState = HAL_SPI_Receive_IT(&SPI3_Handler, (uint8_t*)readDataPtr, dataLength);
    }
    else
    {
        //释放锁
        MCU_SPI3_ReleaseLock();
        return ERROR_SUB_OK;
    }
    if(halState != HAL_OK)
    {
        transState = SPI_TRANS_STATE_IDLE;
        //释放锁
        MCU_SPI3_ReleaseLock();
        //错误日志打印
        MCU_LogShowHalState(halState,SPI3_Handler.ErrorCode);
        return ERROR_SUB_MCU_SPI3_TRANS_IT_START;
    }
    //开始等待时间
    int32_t transStartTimeStamp = tx_time_get();
    //等待报错或者超时
    do
    {
        if(transState == SPI_TRANS_STATE_START)
        {
#if(MCU_SPI3_INT_ENABLE_SCHEDULE == CONFIG_FUNC_ENABLE)
            CoreDelayMinTick();
#else
            asm("nop");
#endif
        }
        else
        {
            break;
        }
    }while(tx_time_get() - transStartTimeStamp <= MS_TO_OS_TICK((TIME_OUT_MS_SPI_SINGLE_BYTE*dataLength)));
    if(transState == SPI_TRANS_STATE_START)
    {
        //取消传输
        MCU_SPI3_AbortTrans(MCU_SPI3_ABORT_WAIT_TIME_MAX);
        //超时失败
        transState = SPI_TRANS_STATE_IDLE;
        //释放锁
        MCU_SPI3_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_TIME_OUT);
        return ERROR_SUB_MCU_SPI3_TRANS_TIME_OUT;
    }
    else if(transState == SPI_TRANS_STATE_END)
    {
        transState = SPI_TRANS_STATE_IDLE;
        //释放锁
        MCU_SPI3_ReleaseLock();
        //正常结束
        return ERROR_SUB_OK;
    }
    else if(transState == SPI_TRANS_STATE_ERROR)
    {
        //取消传输
        MCU_SPI3_AbortTrans(MCU_SPI3_ABORT_WAIT_TIME_MAX);
        //异常失败
        transState = SPI_TRANS_STATE_IDLE;
        //释放锁
        MCU_SPI3_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_IT_ERR);
        return ERROR_SUB_MCU_SPI3_TRANS_IT_ERR;
    }
    //莫名其妙的错误,取消传输
    MCU_SPI3_AbortTrans(MCU_SPI3_ABORT_WAIT_TIME_MAX);
    //居然是莫名其妙的其他状态
    transState = SPI_TRANS_STATE_IDLE;
    //释放锁
    MCU_SPI3_ReleaseLock();
    //错误日志打印
    MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_IT_ERR);
    return ERROR_SUB_MCU_SPI3_TRANS_IT_ERR;
}

//非查询模式,就需要中断
void SPI3_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_SPI_IRQHandler(&SPI3_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif



/*------------------------------------使用DMA进行SPI通讯---------------------------------------------*/
#if(MCU_SPI3_MODE_USER == MCU_SPI_MODE_DMA)
//DMA操作句柄
static DMA_HandleTypeDef SPI3_DMA_TX_Handler;
static DMA_HandleTypeDef SPI3_DMA_RX_Handler;
//DMA通讯事件标志组
static TX_EVENT_FLAGS_GROUP eventFlagsGroupSPI3;
//表示指令已发送
#define EVENT_FLAG_SPI3_TX_COMPLETE             (0X00000001)
//表示指令已接收
#define EVENT_FLAG_SPI3_RX_COMPLETE             (0X00000002)
//表示指令收发完成
#define EVENT_FLAG_SPI3_TXRX_COMPLETE           (0X00000004)
//表示指令已报错
#define EVENT_FLAG_SPI3_ERROR_COMPLETE          (0X00000008)
//表示指令已终止
#define EVENT_FLAG_SPI3_ABORT_COMPLETE          (0X00000010)
//需要与DMA共享的内存都设置成D2_SRAM1_DMA
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于DMA发送数据快
static uint8_t bufferWithSpi3Send[LENGTH_BYTES_SPI3_DMA_BUF]@".sramin" = {0};
//用于DMA接收数据块
static uint8_t bufferWithSpi3Recv[LENGTH_BYTES_SPI3_DMA_BUF]@".sramin" = {0};
#pragma pack (pop)


//取消传输标记
void MCU_SPI3_AbortCpltCallback_DMA(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送取消完成标记
    tx_event_flags_set(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_ABORT_COMPLETE,TX_OR);
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_AbortCpltCallback_DMA\r\n");
#endif
}

//收发完成中断回调函数
void MCU_SPI3_TxRxCpltCallback_DMA(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送接收传输完成标记
    tx_event_flags_set(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_TXRX_COMPLETE,TX_OR);
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_TxRxCpltCallback_DMA\r\n");
#endif
}

//发送中断回调函数
void MCU_SPI3_TxCpltCallback_DMA(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送传输完成标记
    tx_event_flags_set(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_TX_COMPLETE,TX_OR);
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_TxCpltCallback_DMA\r\n");
#endif
}

//接收中断回调函数
void MCU_SPI3_RxCpltCallback_DMA(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送接收完成标记
    tx_event_flags_set(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_RX_COMPLETE,TX_OR);
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_RxCpltCallback_DMA\r\n");
#endif
}

//错误回调函数
void MCU_SPI3_ErrorCallback_DMA(SPI_HandleTypeDef* spiHandlerPtr)
{
    //发送错误产生标记
    tx_event_flags_set(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_ERROR_COMPLETE,TX_OR);
#if(MCU_SPI3_ENABLE_INT_SHOW == CONFIG_FUNC_ENABLE)
    MCU_LogSendStringInt("MCU_SPI3_ErrorCallback_DMA\r\n");
#endif
}

//SPI3清除取消完成标记
static void MCU_SPI3_ClearAbortCompleteFlag(void)
{
    uint32_t eventFlagsActualFlag = 0;
    while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_ABORT_COMPLETE,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            TX_NO_WAIT));
}

//SPI3等待取消完成完成标记
static ERROR_SUB MCU_SPI3_WaitAbortCompleteFlag(uint32_t timeOutMs)
{
    int32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    txResult = tx_event_flags_get(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_ABORT_COMPLETE,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,
                                            MS_TO_OS_TICK(timeOutMs));
    if(txResult == TX_SUCCESS)
    {
        return ERROR_SUB_OK;
    }
    return ERROR_SUB_MCU_SPI3_WAIT_ABORT_COMPLETE;
}

static void MCU_SPI3_DMA_Init(void)
{
    //使能时钟
    SPI3_DMA_TX_CLK_ENABLE();
    //发送DMA
    SPI3_DMA_TX_Handler.Instance                 = SPI3_DMA_TX_INSTANCE;
    SPI3_DMA_TX_Handler.Init.Channel             = SPI3_DMA_TX_CHANNEL;
    //内存到外设
    SPI3_DMA_TX_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    SPI3_DMA_TX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    SPI3_DMA_TX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SPI3_DMA_TX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;	// DMA_MBURST_INC4; 
    SPI3_DMA_TX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;	// DMA_PBURST_INC4; 
    SPI3_DMA_TX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    SPI3_DMA_TX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    SPI3_DMA_TX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    SPI3_DMA_TX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    SPI3_DMA_TX_Handler.Init.Mode                = DMA_NORMAL;
    SPI3_DMA_TX_Handler.Init.Priority            = SPI3_DMA_TX_PRIORITY;
    HAL_DMA_Init(&SPI3_DMA_TX_Handler);
    __HAL_LINKDMA(&SPI3_Handler, hdmatx, SPI3_DMA_TX_Handler);	
    //发送DMA中断设定
    HAL_NVIC_SetPriority(SPI3_DMA_TX_IRQ, SPI3_DMA_TX_PREE_PRI, SPI3_DMA_TX_SUB_PRI);
	HAL_NVIC_EnableIRQ(SPI3_DMA_TX_IRQ);

    //使能时钟
    SPI3_DMA_RX_CLK_ENABLE();
    //接收DMA设定
    SPI3_DMA_RX_Handler.Instance                 = SPI3_DMA_RX_INSTANCE;
    SPI3_DMA_RX_Handler.Init.Channel             = SPI3_DMA_RX_CHANNEL;
    //外设到内存
    SPI3_DMA_RX_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    SPI3_DMA_RX_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    SPI3_DMA_RX_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SPI3_DMA_RX_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;
    SPI3_DMA_RX_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    SPI3_DMA_RX_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    SPI3_DMA_RX_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    SPI3_DMA_RX_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    SPI3_DMA_RX_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    SPI3_DMA_RX_Handler.Init.Mode                = DMA_NORMAL;
    SPI3_DMA_RX_Handler.Init.Priority            = SPI3_DMA_RX_PRIORITY;
    HAL_DMA_Init(&SPI3_DMA_RX_Handler);    
    __HAL_LINKDMA(&SPI3_Handler, hdmarx, SPI3_DMA_RX_Handler);	
    //接收DMA中断设定
    HAL_NVIC_SetPriority(SPI3_DMA_RX_IRQ, SPI3_DMA_RX_PREE_PRI, SPI3_DMA_RX_SUB_PRI);
	HAL_NVIC_EnableIRQ(SPI3_DMA_RX_IRQ);
}

//SPI3初始化
void MCU_SPI3_Init(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    if(mutexMCU_SPI3.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化了
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_SPI3,"mutexMCU_SPI3",TX_INHERIT);
    //创建事件标志组
    tx_event_flags_create(&eventFlagsGroupSPI3,"eventFlagsGroupSPI3");
    //先关闭中断
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
    //SPI中断关联
    SPI3_Handler.TxCpltCallback = MCU_SPI3_TxCpltCallback_DMA;
    SPI3_Handler.RxCpltCallback = MCU_SPI3_RxCpltCallback_DMA;
    SPI3_Handler.TxRxCpltCallback = MCU_SPI3_TxRxCpltCallback_DMA;
    SPI3_Handler.ErrorCallback = MCU_SPI3_ErrorCallback_DMA;
    SPI3_Handler.AbortCpltCallback = MCU_SPI3_AbortCpltCallback_DMA;
    //配置完成,配置相关的中断
    HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_PREE_PRI_DMA, SPI3_SUB_PRI_DMA);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);
    //DMA初始化
    MCU_SPI3_DMA_Init();
}

//SPI3 通讯参数重新设置
void MCU_SPI3_ResetParam(MCU_SPI_23_CLK speed,MCU_SPI_CPOL cpol,MCU_SPI_CPHA cpha)
{
    //没有变化不用改
    if((spi3Config.baudRate == speed)&&(spi3Config.cpolConfig == cpol)&&
        (spi3Config.cphaConfig == cpha))
    {
        return;
    }
    //获取锁
    MCU_SPI3_GetLock();
    //先关闭中断
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
    HAL_NVIC_DisableIRQ(SPI3_DMA_TX_IRQ);
    HAL_NVIC_DisableIRQ(SPI3_DMA_RX_IRQ);
    //SPI基础配置初始化
    MCU_SPI3_BaseConfig(speed,cpol,cpha);
    //SPI中断关联
    SPI3_Handler.TxCpltCallback = MCU_SPI3_TxCpltCallback_DMA;
    SPI3_Handler.RxCpltCallback = MCU_SPI3_RxCpltCallback_DMA;
    SPI3_Handler.TxRxCpltCallback = MCU_SPI3_TxRxCpltCallback_DMA;
    SPI3_Handler.ErrorCallback = MCU_SPI3_ErrorCallback_DMA;
    SPI3_Handler.AbortCpltCallback = MCU_SPI3_AbortCpltCallback_DMA;
    //配置完成,配置相关的中断
    HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_PREE_PRI_DMA, SPI3_SUB_PRI_DMA);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);
    //DMA初始化
    MCU_SPI3_DMA_Init();
    //释放锁
    MCU_SPI3_ReleaseLock();
}


//SPI取消传输
ERROR_SUB MCU_SPI3_AbortTrans(uint32_t timeOutMs)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先清除取消传输标记
    MCU_SPI3_ClearAbortCompleteFlag();
    //发送取消传输指令
    HAL_SPI_Abort_IT(&SPI3_Handler);
    //等待取消传输状态匹配
    errorSubCode = MCU_SPI3_WaitAbortCompleteFlag(timeOutMs);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
    }
    //返回结果
    return errorSubCode;
}

//SPI3读写数据
ERROR_SUB MCU_SPI3_ReadWriteBuffer(uint8_t* writeDataPtr,uint8_t* readDataPtr,uint16_t dataLength)
{
    HAL_StatusTypeDef halState;
    int32_t txResult = TX_SUCCESS;
    uint32_t eventFlagsActualFlag = 0;
    //辅助标签
    uint16_t indexUtil = 0;
    //本次单包能发送的数据量
    uint16_t singlePackLength = 0;
    //当前读写指针
    uint16_t packPtrOffset = 0;
    if((writeDataPtr == SPI_PTR_NULL)&&(readDataPtr == SPI_PTR_NULL))
    {
        return ERROR_SUB_OK;
    }
    //一共需要发多少包
    uint16_t sendPackCountMax = dataLength/LENGTH_BYTES_SPI3_DMA_BUF;
    if(dataLength%LENGTH_BYTES_SPI3_DMA_BUF != 0)
    {
        sendPackCountMax += 1;
    }
    //没有发送数据的时候,清空发送数据
    if(writeDataPtr == SPI_PTR_NULL)
    {
        UserMemSet(bufferWithSpi3Send,0,LENGTH_BYTES_SPI3_DMA_BUF);
    }
    //获取锁
    MCU_SPI3_GetLock();
    for(indexUtil = 0; indexUtil < sendPackCountMax;indexUtil++)
    {
        //计算本次需要发送的数据
        if(((indexUtil+1)*LENGTH_BYTES_SPI3_DMA_BUF) > dataLength)
        {
            //本次发不完
            singlePackLength = dataLength - (indexUtil*LENGTH_BYTES_SPI3_DMA_BUF);
        }
        else
        {
            //本次发的完
            singlePackLength = LENGTH_BYTES_SPI3_DMA_BUF;
        }
        //数据包位置偏移
        packPtrOffset = indexUtil*LENGTH_BYTES_SPI3_DMA_BUF;
        if(writeDataPtr != SPI_PTR_NULL)
        {
            //发送数据拷贝
            UserMemCopy(bufferWithSpi3Send,writeDataPtr+packPtrOffset,singlePackLength);
        }
        if(readDataPtr != SPI_PTR_NULL)
        {
            //接收数据清空
            UserMemSet(bufferWithSpi3Recv,0,LENGTH_BYTES_SPI3_DMA_BUF);
        }
        //清空信号量标记
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_TXRX_COMPLETE,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //清空错误标记
        while(TX_SUCCESS == tx_event_flags_get(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_ERROR_COMPLETE,TX_AND_CLEAR,(ULONG*)&eventFlagsActualFlag,TX_NO_WAIT));
        //DMA数据发送,同时收发
        halState = HAL_SPI_TransmitReceive_DMA(&SPI3_Handler, (uint8_t*)bufferWithSpi3Send, (uint8_t *)bufferWithSpi3Recv, 
                                                    singlePackLength);
        if(halState != HAL_OK)
        {
            //释放锁
            MCU_SPI3_ReleaseLock();
            //错误日志打印
            MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_DMA_START);
            return ERROR_SUB_MCU_SPI3_TRANS_DMA_START;
        }
        //开始等待
        txResult = tx_event_flags_get(&eventFlagsGroupSPI3,EVENT_FLAG_SPI3_TXRX_COMPLETE|EVENT_FLAG_SPI3_ERROR_COMPLETE,TX_OR_CLEAR,
                                        (ULONG*)&eventFlagsActualFlag,MS_TO_OS_TICK(TIME_OUT_MS_SPI_SINGLE_BYTE*singlePackLength));
        if(txResult != TX_SUCCESS)
        {
            //传输超时,中断传输
            MCU_SPI3_AbortTrans(MCU_SPI3_ABORT_WAIT_TIME_MAX);
            //释放锁
            MCU_SPI3_ReleaseLock();
            //错误日志打印
            MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_TIME_OUT);
            return ERROR_SUB_MCU_SPI3_TRANS_TIME_OUT;
        }
        //完成,查看是否传输异常
        if(eventFlagsActualFlag&EVENT_FLAG_SPI3_ERROR_COMPLETE)
        {
            //发生错误
            MCU_SPI3_AbortTrans(MCU_SPI3_ABORT_WAIT_TIME_MAX);
            //释放锁
            MCU_SPI3_ReleaseLock();
            //错误日志打印
            MCU_LogShowErrorSubCode(ERROR_SUB_MCU_SPI3_TRANS_DMA_ERR);
            return ERROR_SUB_MCU_SPI3_TRANS_DMA_ERR;
        }
        //传输成功,数据拷贝
        if(readDataPtr != SPI_PTR_NULL)
        {
            //完成之后进行接收数据拷贝
            UserMemCopy(readDataPtr+packPtrOffset,bufferWithSpi3Recv,singlePackLength);
        }
    }
    //释放锁
    MCU_SPI3_ReleaseLock();
    return ERROR_SUB_OK;
}


//非查询模式,就需要中断
void SPI3_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_SPI_IRQHandler(&SPI3_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//发送DMA
void SPI3_DMA_TX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&SPI3_DMA_TX_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
//接收DMA
void SPI3_DMA_RX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&SPI3_DMA_RX_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

#endif











