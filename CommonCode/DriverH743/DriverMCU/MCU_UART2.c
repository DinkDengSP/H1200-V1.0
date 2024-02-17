/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 13:56:28 +0800
************************************************************************************************/ 
#include "MCU_UART2.h"
#include "MCU_RTT.h"
#include "ADT_CircularByteQueue.h"
#include "ADT_CircularPtrQueue.h"
#include "MCU_PIN.h"
#include "MCU_Mem.h"
#include "DriverConfigUART.h"

//UART句柄
static UART_HandleTypeDef UART2_Handler;  
//串口互斥信号量
static TX_MUTEX mutexMCU_UART2;
//串口回调函数
static MCU_UART_CALLBACK uart2CallBackSet = {0};

/*-------------------------------------串口发送相关的数据定义----------------------------------*/
//发送数据的相关定义
#if(MCU_UART2_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//用于printf的缓冲区,只要定义了发送功能就必须存在
static uint8_t blockBufferWithUart2Printf[LENGTH_UART2_BUFFER_WITH_FORMAT] = {0};
#endif
//中断和DMA发送都需要的数据块
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//循环队列
static ADT_CIRCULAR_BYTE_QUEUE_T circularByteQueueUart2SendPtr = NULL;
//传输正在进行标志
static MCU_UART_TRANS_STATE mcuUart2TransState = MCU_UART_TRANS_IDLE;
#endif
//中断发送,需要使用的数据块
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart2SendCurrent[LENGTH_UART2_BUFFER_WITH_BLOCK] = {0};
#pragma pack (pop)
#endif
//DMA发送,需要使用的数据块
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
//发送DMA句柄
static DMA_HandleTypeDef  UART2_TxDMA_Handler;
#pragma pack(push) //保存对齐状态 
#pragma pack(4)
//用于当前发送的数据块
static uint8_t blockBufferWithUart2SendCurrent[LENGTH_UART2_BUFFER_WITH_BLOCK]@".d2_sram2_dma" = {0};
#pragma pack (pop)
#endif


/*-------------------------------------串口接收相关的数据定义----------------------------------*/
//中断和DMA接收都需要的数据块
#if((MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//循环队列,保存数据指针
static ADT_CIRCULAR_PTR_QUEUE_T circularPtrQueueUart2RecvIdleBlockPtr = NULL;
//接收缓存
static UART_RECV_PACK uart2RecvPackArray[COUNT_UART2_RECV_BLOCK];
//接收缓存
static uint8_t blockBufferWithUart2Recv[COUNT_UART2_RECV_BLOCK][LENGTH_UART2_RECV_SINGLE_BLOCK] = {0};
#endif
//中断接收需要的数据块
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
//当前接收对象
static uint8_t currentBufferWithUart2Recv[LENGTH_UART2_RECV_SINGLE_BLOCK] = {0};
#endif
//DMA接收需要的数据块
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
//接收DMA句柄
static DMA_HandleTypeDef  UART2_RxDMA_Handler;
//当前接收对象
static uint8_t currentBufferWithUart2Recv[LENGTH_UART2_RECV_SINGLE_BLOCK]@".d2_sram2_dma" = {0};
#endif  


//MSP复位
static void MCU_UART2_MspInit(void)
{
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX))
    MCU_PinInit(UART2_PIN_TX,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF7_USART2);
#endif
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX))
    MCU_PinInit(UART2_PIN_RX,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF7_USART2);
#endif
}

//MSP初始化
void MCU_UART2_MspDeInit(void)
{
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX))
    MCU_PinDeInit(UART2_PIN_TX);
#endif
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX))
    MCU_PinDeInit(UART2_PIN_RX);
#endif
    //关闭中断
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    //清除中断悬挂
    HAL_NVIC_ClearPendingIRQ(USART2_IRQn);
}

//获取互斥锁
void MCU_UART2_GetLock(void)
{
    //获取信号量
    tx_mutex_get(&mutexMCU_UART2,TX_WAIT_FOREVER);
}

//释放互斥锁
void MCU_UART2_ReleaseLock(void)
{
    //释放信号量
    tx_mutex_put(&mutexMCU_UART2);
}

//串口回调函数,接收完成中断
static void MCU_UART2_RxCpltCallBack(UART_HandleTypeDef* uartHandlerPtr);
//串口回调函数,发送完成中断
static void MCU_UART2_TxCpltCallBack(UART_HandleTypeDef* uartHandlerPtr);
//串口回调函数,错误中断
static void MCU_UART2_ErrorCallBack(UART_HandleTypeDef* uartHandlerPtr);
//串口回调函数,接收事件中断
static void MCU_UART2_RxEventCallBack(UART_HandleTypeDef* uartHandlerPtr,uint16_t dataLength);

//串口基础初始化
static void MCU_UART2_BaseInit(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl)
{
    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
    //首先关闭串口时钟,才能配置串口
    __HAL_RCC_USART2_CLK_DISABLE();
    //选择时钟端口
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    //APB2
    RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    //打开串口时钟,然后复位串口模块然后解除串口模块
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();
    //UART 初始化设置
    //USART端口选择,串口外设基地址
    UART2_Handler.Instance = USART2;
    //底层复位
    MCU_UART2_MspDeInit();
    //初始化UART
    while(HAL_OK != HAL_UART_DeInit(&UART2_Handler));
    //创建串口互斥信号量
    tx_mutex_create(&mutexMCU_UART2,"mutexMCU_UART2",TX_INHERIT);
    //波特率			    
    UART2_Handler.Init.BaudRate = baud;	
    //字长		    
    UART2_Handler.Init.WordLength = length; 
    //停止位  
    UART2_Handler.Init.StopBits = stopBit;	
    //奇偶校验位    
    UART2_Handler.Init.Parity = checkMode;
    //硬件流控		    
    UART2_Handler.Init.HwFlowCtl = hardWareControl;
    //收发模式   
    UART2_Handler.Init.Mode = MCU_UART2_TRANS_MODE;	
    //过采样设置,8或者16,8位速度更快,16位对波特率的兼容性更好
    UART2_Handler.Init.OverSampling = MCU_UART2_OVERSAMPLING_MODE;
    //采样值的位数,是否打开一个采样位方法,打开的话,将禁止噪声检测标记
    UART2_Handler.Init.OneBitSampling = MCU_UART2_ONE_BIT_SAMPLE;
    //关闭FIFO模式
    UART2_Handler.FifoMode = UART_FIFOMODE_DISABLE;
    //高级配置,其中可以设置端口反转和电平反转,根据需要设计自己的高级特性相或
    uint32_t advFeatureInit = UART_ADVFEATURE_NO_INIT;
    //是否关闭ORE过载标记
#if(MCU_UART2_DISABLE_ORE != 0)
    advFeatureInit |= UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
#endif
    UART2_Handler.AdvancedInit.AdvFeatureInit = advFeatureInit;
    //底层初始化
    MCU_UART2_MspInit();
    //根据配置初始化串口
    while(HAL_OK != HAL_UART_Init(&UART2_Handler));	  
    //清除TC发送完成标志 
    SET_BIT(USART2->ICR, USART_ICR_TCCF);
    //清除RXNE接收FIFO,清空发送之后需要延时一会,等待清空之后再打开空闲中断标记,否则一打开中断
    //就会收到空闲终端
    SET_BIT(USART2->RQR, USART_RQR_RXFRQ);
    //等待清除完成
    CoreDelayMinTick();
//接收模式或者发送模式但凡有一个使用中断,就要配置中断并打开中断
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
    (MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //打开中断,串口优先级设置		
    HAL_NVIC_SetPriority(USART2_IRQn,MCU_UART2_PREE_PRI,MCU_UART2_SUB_PRI);
    //使能USART2中断通道
    HAL_NVIC_EnableIRQ(USART2_IRQn);
#endif
}

//串口发送辅助数据初始化
void MCU_UART2_TransDataInit(void)
{
    asm("nop");
#if(MCU_UART2_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
    //清零打印缓存
    UserMemClear((void*)blockBufferWithUart2Printf,LENGTH_UART2_BUFFER_WITH_FORMAT);
#endif
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    //数据结构初始化
    circularByteQueueUart2SendPtr = ADT_CircularByteQueueCreate(LENGTH_UART2_BUFFER_WITH_SEND,MCU_MemMallocAxiSram,MCU_MemFreeAxiSram,
                                                                CoreLockIntBasePri,CoreUnlockIntBasePri,MCU_RTT_SendStringInt);
    //状态设置
    mcuUart2TransState = MCU_UART_TRANS_IDLE;
    //清零正在发送块数据
    UserMemClear((void*)blockBufferWithUart2SendCurrent,LENGTH_UART2_BUFFER_WITH_BLOCK);
#endif
}

//串口发送DMA初始化
void MCU_UART2_TransDMA_Init(void)
{
    asm("nop");
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART2_DMA_TX_CLK_ENABLE();  
    //DMA配置参数设定
    UART2_TxDMA_Handler.Instance = MCU_UART2_DMA_TX_INSTANCE;
    //参数配置
    /*DMA数据流请求*/	
    UART2_TxDMA_Handler.Init.Request = DMA_REQUEST_USART2_TX; 
    /*方向：从内存到外设*/		
    UART2_TxDMA_Handler.Init.Direction= DMA_MEMORY_TO_PERIPH;	
    /*外设地址不增*/	    
    UART2_TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART2_TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART2_TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART2_TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART2_TxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART2_TxDMA_Handler.Init.Priority = MCU_UART2_DMA_TX_PRIORITY;      
    /*禁用FIFO*/
    UART2_TxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART2_TxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART2_TxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART2_TxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;   
    //DMA链接
    __HAL_LINKDMA(&UART2_Handler, hdmatx, UART2_TxDMA_Handler);
    //复位DMA
    HAL_DMA_DeInit(&UART2_TxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART2_TxDMA_Handler); 
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART2_DMA_TX_IRQ,MCU_UART2_TX_DMA_PREE_PRI,MCU_UART2_TX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART2_DMA_TX_IRQ);
#endif
}

//串口接收辅助数据初始化
void MCU_UART2_RecvDataInit(void)
{
#if((MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //初始化接收缓存
    UserMemClear((void*)blockBufferWithUart2Recv,COUNT_UART2_RECV_BLOCK*LENGTH_UART2_RECV_SINGLE_BLOCK);
    //初始化接收队列
    circularPtrQueueUart2RecvIdleBlockPtr = ADT_CircularPtrQueueCreate(COUNT_UART2_RECV_BLOCK,MCU_MemMallocAxiSram,MCU_MemFreeAxiSram,
                                                                        CoreLockIntGlobal,CoreUnlockIntGlobal,MCU_RTT_SendStringInt);
    //填充接收队列
    uint16_t indexUtil = 0;
    //节点赋值
    for(indexUtil = 0;indexUtil < COUNT_UART2_RECV_BLOCK;indexUtil++)
    {
        uart2RecvPackArray[indexUtil].packSrc = UART_PACK_SRC_2;
        uart2RecvPackArray[indexUtil].recvDataBufferPtr = (uint8_t*)(blockBufferWithUart2Recv[indexUtil]);
        uart2RecvPackArray[indexUtil].recvDataLength = 0;
    }
    //队列导入
    for(indexUtil = 0;indexUtil < COUNT_UART2_RECV_BLOCK;indexUtil++)
    {
        ADT_CircularPtrQueueWrite(circularPtrQueueUart2RecvIdleBlockPtr,(void*)(&(uart2RecvPackArray[indexUtil])));
    }
    //清除当前接收队列
    UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
    asm("nop");
}

//串口接收DMA初始化
void MCU_UART2_RecvDMA_Init(void)
{
    asm("nop");
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //DMA时钟使能	
    MCU_UART2_DMA_RX_CLK_ENABLE();  
    //DMA配置参数设定
    UART2_RxDMA_Handler.Instance = MCU_UART2_DMA_RX_INSTANCE;
    //参数配置
    /*DMA接收数据流请求*/	
    UART2_RxDMA_Handler.Init.Request = DMA_REQUEST_USART2_RX; 
    /*方向：从外设到内存*/		
    UART2_RxDMA_Handler.Init.Direction= DMA_PERIPH_TO_MEMORY;	
    /*外设地址不增*/	    
    UART2_RxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE; 
    /*内存地址自增*/
    UART2_RxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;	
    /*外设数据单位*/	
    UART2_RxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    /*内存数据单位 8bit*/
    UART2_RxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	
    /*DMA模式：单次模式*/
    UART2_RxDMA_Handler.Init.Mode = DMA_NORMAL;	 
    /*优先级*/	
    UART2_RxDMA_Handler.Init.Priority = MCU_UART2_DMA_RX_PRIORITY;      
    /*禁用FIFO*/
    UART2_RxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        
    UART2_RxDMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    
    /*存储器突发传输 1个节拍*/
    UART2_RxDMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;    
    /*外设突发传输 1个节拍*/
    UART2_RxDMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
    //DMA链接
    __HAL_LINKDMA(&UART2_Handler, hdmarx, UART2_RxDMA_Handler);   
    //复位DMA
    HAL_DMA_DeInit(&UART2_RxDMA_Handler); 
    //配置DMA
    HAL_DMA_Init(&UART2_RxDMA_Handler); 
    //DMA优先级配置
    HAL_NVIC_SetPriority(MCU_UART2_DMA_RX_IRQ,MCU_UART2_RX_DMA_PREE_PRI,MCU_UART2_RX_DMA_SUB_PRI);
    //打开DMA通道中断
    HAL_NVIC_EnableIRQ(MCU_UART2_DMA_RX_IRQ);
#endif
}

//开始接收
static void MCU_UART2_StartRecv(void)
{
    asm("nop");
#if((MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    //清除空闲中断标志
    __HAL_UART_CLEAR_IDLEFLAG(&UART2_Handler);  
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler, (uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
}

//串口初始化
void MCU_UART2_Init(uint32_t baud, MCU_UART_LENGTH length, MCU_UART_STOPBIT stopBit,MCU_UART_CHECK_MODE checkMode, MCU_UART_HARD_CONTROL hardWareControl)
{
     //基础串口初始化
    MCU_UART2_BaseInit(baud,length,stopBit,checkMode,hardWareControl);
    //相关数据初始化
    MCU_UART2_TransDataInit();
    //DMA初始化
    MCU_UART2_TransDMA_Init();
    //相关数据初始化
    MCU_UART2_RecvDataInit();
    //DMA初始化
    MCU_UART2_RecvDMA_Init();
    //开始接收
    MCU_UART2_StartRecv();
    //中断回调关联
    UART2_Handler.TxCpltCallback = MCU_UART2_TxCpltCallBack;
    UART2_Handler.RxCpltCallback = MCU_UART2_RxCpltCallBack;
    UART2_Handler.ErrorCallback = MCU_UART2_ErrorCallBack;
    UART2_Handler.RxEventCallback = MCU_UART2_RxEventCallBack;
    //清空回调函数
    uart2CallBackSet.recvIntCallBack = NULL;
    uart2CallBackSet.sendDataOverCallBack = NULL;
    uart2CallBackSet.sendDataStartCallBack = NULL;
}

//配置串口回调函数
void MCU_UART2_ConfigCallBack(MCU_UartRecvIntProcFunc recvCallBack,MCU_UartSendDataStartProcFunc sendStartCallBack,
                                MCU_UartSendDataOverProcFunc sendOverCallBack)
{
    uart2CallBackSet.recvIntCallBack = recvCallBack;
    uart2CallBackSet.sendDataStartCallBack = sendStartCallBack;
    uart2CallBackSet.sendDataOverCallBack = sendOverCallBack;
}

//查询模式下串口发送
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
void MCU_UART2_SendBufferPool(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //获取信号量
    MCU_UART2_GetLock();
    //发送前回调,针对485优化
    if(uart2CallBackSet.sendDataStartCallBack != NULL)
    {
        uart2CallBackSet.sendDataStartCallBack();
    }
    //阻塞发送
    halState = HAL_UART_Transmit(&UART2_Handler,bufferStartPtr,sendLength,MCU_UART2_WAIT_TRANS_MAX(sendLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART2_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
    //发送后回调,针对485回调
    if(uart2CallBackSet.sendDataOverCallBack != NULL)
    {
        uart2CallBackSet.sendDataOverCallBack();
    }
    //释放信号量
    MCU_UART2_ReleaseLock();
}
#endif

//中断和DMA发送都需要的数据块
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
//将缓存写入发送队列
static ErrorStatus MCU_UART2_WriteBufferToQueue(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    HAL_StatusTypeDef halState = HAL_OK;
    uint16_t currentBufferHadSendLength = 0;
    uint16_t currentBufferWaitSendLength = sendLength;
    uint16_t realWriteDataCount = 0;
    //获取等待本次发送数据全部写入缓冲区的最长等待时间,极限情况是缓冲区里面数据是满的,那么计算这个
    //时间就是单字节发送时间乘以(缓冲区长度+本次发送数据长度)
    int32_t waitBufferToSendMaxTimeMs = MCU_UART2_WAIT_TRANS_MAX(LENGTH_UART2_BUFFER_WITH_SEND+sendLength);
    //开始等待发送时间
    int32_t sendStartTimeStamp = tx_time_get();
    do
    {
        realWriteDataCount = 0;
        if(ADT_CircularByteQueueGetCurrentEmptySize(circularByteQueueUart2SendPtr) > 0)
        {
            //写入循环队列
            ADT_CircularByteQueueWriteBuffer(circularByteQueueUart2SendPtr,(bufferStartPtr+currentBufferHadSendLength),currentBufferWaitSendLength,&realWriteDataCount);
            currentBufferHadSendLength += realWriteDataCount;
            currentBufferWaitSendLength -= realWriteDataCount;
        }
        if(mcuUart2TransState == MCU_UART_TRANS_IDLE)
        {
            //从循环队列读取指定长度数据
            ADT_CircularByteQueueReadBuffer(circularByteQueueUart2SendPtr,blockBufferWithUart2SendCurrent,LENGTH_UART2_BUFFER_WITH_BLOCK,&realWriteDataCount);
            //数据长度大于0
            if(realWriteDataCount > 0)
            {
                //发送前回调,针对485优化
                if(uart2CallBackSet.sendDataStartCallBack != NULL)
                {
                    uart2CallBackSet.sendDataStartCallBack();
                }
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
                //触发中断数据发送
                halState = HAL_UART_Transmit_IT(&UART2_Handler,(uint8_t*)blockBufferWithUart2SendCurrent,realWriteDataCount);
#else
                //触发DMA数据发送
                halState = HAL_UART_Transmit_DMA(&UART2_Handler,(uint8_t*)blockBufferWithUart2SendCurrent,realWriteDataCount);
#endif
                if(halState != HAL_OK)
                {
                    //发送失败
                    HAL_UART_AbortTransmit(&UART2_Handler);
                    mcuUart2TransState = MCU_UART_TRANS_IDLE;
                    //清空数据
                    ADT_CircularByteQueueClear(circularByteQueueUart2SendPtr);
                    //发送后回调,针对485回调
                    if(uart2CallBackSet.sendDataOverCallBack != NULL)
                    {
                        uart2CallBackSet.sendDataOverCallBack();
                    }
                    //加一个RTT打印
                    MCU_RTT_Printf("MCU_UART2_WriteBufferToQueue HalError: 0X%02X\r\n",halState);
                    return ERROR;
                }
                //标识正在发送
                mcuUart2TransState = MCU_UART_TRANS_ING;
            }
        }
        if(currentBufferHadSendLength != sendLength)       
        {
            CoreDelayMinTick();
        }   
        if(tx_time_get() - sendStartTimeStamp > waitBufferToSendMaxTimeMs)
        {
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART2 SendBuffer TimeOut!!!\r\n");
            //发送失败
            HAL_UART_AbortTransmit(&UART2_Handler);
            //状态设置
            mcuUart2TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(circularByteQueueUart2SendPtr);
            //发送后回调,针对485优化
            if(uart2CallBackSet.sendDataOverCallBack != NULL)
            {
                uart2CallBackSet.sendDataOverCallBack();
            }
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return ERROR;
        }                    
    }while(currentBufferHadSendLength != sendLength);
    return SUCCESS;
}

//等待缓存中的数据发送完成
void MCU_UART2_WaitQueueSendOver(uint16_t sendLength)
{
    int32_t waitSendOverMaxMs = 2*MCU_UART2_WAIT_TRANS_MAX(sendLength);
    //开始等待时间
    int32_t sendStartTimeStamp = tx_time_get();
    //等待发送完成,也就是发送队列为空
    while(mcuUart2TransState != MCU_UART_TRANS_IDLE)
    {
        CoreDelayMinTick();
        if(tx_time_get() - sendStartTimeStamp > waitSendOverMaxMs)
        {
            //发送失败
            HAL_UART_AbortTransmit(&UART2_Handler);
            //状态设置
            mcuUart2TransState = MCU_UART_TRANS_IDLE;
            //清空数据
            ADT_CircularByteQueueClear(circularByteQueueUart2SendPtr);
            //发送后回调,针对485优化
            if(uart2CallBackSet.sendDataOverCallBack != NULL)
            {
                uart2CallBackSet.sendDataOverCallBack();
            }
            //加一个RTT打印
            MCU_RTT_SendString("MCU UART2 WaitComplete TimeOut!!!\r\n");
            //注意,这个时候可以选择复位串口,因为串口极有可能出问题了
            return;
        }
    }
    return;
}

void MCU_UART2_SendBufferIntDMA(uint8_t* bufferStartPtr,uint16_t sendLength)
{
    if(sendLength == 0)
        return;
    //申请互斥信号量
    MCU_UART2_GetLock();
    //写入缓存
    if(ERROR == MCU_UART2_WriteBufferToQueue(bufferStartPtr,sendLength))
    {
        //写入缓存超时
        MCU_UART2_ReleaseLock();
        //缓存写入失败
        return;
    }
//是否异步发送
#if(UART2_TRANS_FUNC_SYNC == 1) 
    //等待发送完成
    MCU_UART2_WaitQueueSendOver(sendLength);
#endif
    //释放互斥信号量
    MCU_UART2_ReleaseLock();
    return;
}

#endif


//串口发送数组
void MCU_UART2_SendBuffer(uint8_t* bufferStartPtr,uint16_t sendLength)
{
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
    //没有开启发送
    asm("nop");
#elif(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_POOL)
    //阻塞发送
    MCU_UART2_SendBufferPool(bufferStartPtr,sendLength);
#else
    //中断和DMA发送
    MCU_UART2_SendBufferIntDMA(bufferStartPtr,sendLength);
#endif
}

//串口发送字符串,常规模式下发送
void MCU_UART2_SendString(uint8_t* stringStartPtr)
{
    int stringLength = strlen((char const *)stringStartPtr);
    if (stringLength <= 0)
    {
        return;
    }
    MCU_UART2_SendBuffer(stringStartPtr, (uint16_t)stringLength);
}

//串口发送字符串,中断中发送
void MCU_UART2_SendStringInt(uint8_t* stringStartPtr)
{
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX))
    uint16_t strLength = strlen((char const *)stringStartPtr);
    HAL_StatusTypeDef halState = HAL_OK;
    if (strLength <= 0)
    {
        return;
    }
    //发送前回调,针对485优化
    if(uart2CallBackSet.sendDataStartCallBack != NULL)
    {
        uart2CallBackSet.sendDataStartCallBack();
    }
    //阻塞发送
    halState = HAL_UART_Transmit(&UART2_Handler,stringStartPtr,strLength,MCU_UART2_WAIT_TRANS_MAX(strLength));
    if(halState != HAL_OK)
    {
        //错误,操作终止
        HAL_UART_Abort(&UART2_Handler);
        //日志打印
        MCU_RTT_Printf("%s,HalState: 0X%02X\r\n",halState);
    }
    //发送后回调,针对485优化
    if(uart2CallBackSet.sendDataOverCallBack != NULL)
    {
        uart2CallBackSet.sendDataOverCallBack();
    }
#else
    asm("nop");
#endif
}

//串口接收到的数据释放
void MCU_UART2_RecvDatRelease(UART_RECV_PACK* uartRecvPackPtr)
{
    asm("nop");
#if((MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    if(uartRecvPackPtr == NULL)
    {
        return;
    }
    uartRecvPackPtr->recvDataLength = 0;
    //稳妥起见应该检查指针是否包含在接收块中,这里为了省事就没做了
    UserMemClear((void*)(uartRecvPackPtr->recvDataBufferPtr),LENGTH_UART2_RECV_SINGLE_BLOCK);
    //数据指针写回
    ADT_CircularPtrQueueWrite(circularPtrQueueUart2RecvIdleBlockPtr,uartRecvPackPtr);
#endif
}

//串口打印,带格式化
void MCU_UART2_Printf(const char *format, ...)
{
    //是否打开发送
#if((MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)||(MCU_UART2_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX))
    //转换
    va_list arg;
    int rv;
    //申请互斥信号量
    MCU_UART2_GetLock();
    //锁定线程
    uint32_t oldState = CoreLockSchedule();
    //启动可变参数解析
    va_start(arg, format);
    //将数据转换成带打印缓存
    rv = vsnprintf((char *)blockBufferWithUart2Printf, LENGTH_UART2_BUFFER_WITH_FORMAT, format, arg);
    va_end(arg);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    //将数据发送出去
    if ((rv > 0) && (rv <= LENGTH_UART2_BUFFER_WITH_FORMAT))
    {
        //RV大于0代表转换成功,且RV在指定范围内部LENGTH_UART2_BUFFER_WITH_FORMAT
        MCU_UART2_SendBuffer(blockBufferWithUart2Printf, (uint16_t)rv);
    }
    //释放互斥信号量
    MCU_UART2_ReleaseLock();
    //查看结果,相应的报警
    if ((rv < 0)||(rv > LENGTH_UART2_BUFFER_WITH_FORMAT))
    {
        //加一个RTT打印
        MCU_RTT_Printf("MCU UART2 Sprintf Failed,rv: %d!!!\r\n",rv);
    }
    return;
#else
    return;
#endif
}

//串口回调函数,接收完成中断
void MCU_UART2_RxCpltCallBack(UART_HandleTypeDef* uartHandlerPtr)
{
    asm("nop");
#if((MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
    if(uart2CallBackSet.recvIntCallBack == NULL)
    {
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler, (uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif   
        return;
    }
    //从队列中拿出一个指针做接收转发
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(circularPtrQueueUart2RecvIdleBlockPtr);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler, (uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif 
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_2;
        recvDispatchPtr->recvDataLength = LENGTH_UART2_RECV_SINGLE_BLOCK;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
        //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler, (uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif   
        //数据转发
        if(ERROR == uart2CallBackSet.recvIntCallBack(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART2_RecvDatRelease(recvDispatchPtr);
        }
    }
#endif
}

//串口回调函数,发送完成中断
void MCU_UART2_TxCpltCallBack(UART_HandleTypeDef* uartHandlerPtr)
{
    asm("nop");
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA))
    uint16_t transPackLength = 0;
    HAL_StatusTypeDef halState = HAL_OK;
    //状态不是发送中
    if(mcuUart2TransState == MCU_UART_TRANS_IDLE)
    {
        //发送后回调,针对485优化
        if(uart2CallBackSet.sendDataOverCallBack != NULL)
        {
            uart2CallBackSet.sendDataOverCallBack();
        }
        return;
    }
    //队列现在没数据
    if(ADT_CircularByteQueueGetCurrentSize(circularByteQueueUart2SendPtr) == 0)
    {
        //设置发送结束
        mcuUart2TransState = MCU_UART_TRANS_IDLE; 
        //发送后回调,针对485优化
        if(uart2CallBackSet.sendDataOverCallBack != NULL)
        {
            uart2CallBackSet.sendDataOverCallBack();
        }   
        return;
    }
    //从队列中取数据
    ADT_CircularByteQueueReadBuffer(circularByteQueueUart2SendPtr,blockBufferWithUart2SendCurrent,LENGTH_UART2_BUFFER_WITH_BLOCK,&transPackLength);
    //缓冲区里面有数据
    if(transPackLength == 0)
    {
        //设置发送结束
        mcuUart2TransState = MCU_UART_TRANS_IDLE;   
        //发送后回调,针对485优化
        if(uart2CallBackSet.sendDataOverCallBack != NULL)
        {
            uart2CallBackSet.sendDataOverCallBack();
        } 
        return;
    }
    //发送前回调,针对485优化
    if(uart2CallBackSet.sendDataStartCallBack != NULL)
    {
        uart2CallBackSet.sendDataStartCallBack();
    }
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)
    //中断数据发送
    halState = HAL_UART_Transmit_IT(&UART2_Handler,(uint8_t *)blockBufferWithUart2SendCurrent,transPackLength);
#else
    //DMA数据发送
    halState = HAL_UART_Transmit_DMA(&UART2_Handler,(uint8_t*)blockBufferWithUart2SendCurrent,transPackLength);
#endif
    if(halState != HAL_OK)
    {
        //发送失败
        HAL_UART_AbortTransmit(&UART2_Handler);
        mcuUart2TransState = MCU_UART_TRANS_IDLE;
        MCU_RTT_SendStringInt("MCU_UART2_TxCpltCallback SendData HalError:\r\n");
        //发送后回调,针对485优化
        if(uart2CallBackSet.sendDataOverCallBack != NULL)
        {
            uart2CallBackSet.sendDataOverCallBack();
        } 
        return;
    }
#endif
}

//串口回调函数,错误中断
void MCU_UART2_ErrorCallBack(UART_HandleTypeDef* uartHandlerPtr)
{
    //关闭接收
    ATOMIC_CLEAR_BIT(UART2_Handler.Instance->CR1, USART_CR1_RE);
    //诊断中断来源并打印到RTT中
    if ((UART2_Handler.ErrorCode & HAL_UART_ERROR_ORE) != RESET)
    {
        //接收溢出中断
        __HAL_UART_CLEAR_OREFLAG(&UART2_Handler);   
        MCU_RTT_SendStringInt("MCU_UART2_ErrorCallback HAL_UART_ERROR_ORE\r\n");
    }
    if ((UART2_Handler.ErrorCode & HAL_UART_ERROR_FE) != RESET)
    {
        //帧错误中断
        __HAL_UART_CLEAR_FEFLAG(&UART2_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART2_ErrorCallback HAL_UART_ERROR_FE\r\n");
    }
    if ((UART2_Handler.ErrorCode & HAL_UART_ERROR_PE) != RESET)
    {
        //奇偶校验中断
        __HAL_UART_CLEAR_PEFLAG(&UART2_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART2_ErrorCallback HAL_UART_ERROR_PE\r\n");
    }
    if ((UART2_Handler.ErrorCode & HAL_UART_ERROR_NE) != RESET)
    {
        //奇偶校验中断
        __HAL_UART_CLEAR_NEFLAG(&UART2_Handler);
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART2_ErrorCallback HAL_UART_ERROR_NE\r\n");
    }
    if ((UART2_Handler.ErrorCode & HAL_UART_ERROR_DMA) != RESET)
    {
        //错误打印
        MCU_RTT_SendStringInt("MCU_UART2_ErrorCallback HAL_UART_ERROR_DMA\r\n");
    }
    //重新启动接收
    HAL_UART_AbortReceive(&UART2_Handler);
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
    HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler, (uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
    //开始DMA接收中断
    HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
#endif
    //重新使能接收
    ATOMIC_SET_BIT(UART2_Handler.Instance->CR1, USART_CR1_RE);
}

//串口回调函数,接收事件中断
void MCU_UART2_RxEventCallBack(UART_HandleTypeDef* uartHandlerPtr,uint16_t dataLength)
{
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_INT)
    if((dataLength == 0)||(uart2CallBackSet.recvIntCallBack == NULL))
    {
        //清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
        return;
    }
    //从队列中拿出一个空闲块
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(&circularPtrQueueUart2RecvIdleBlockPtr);
    if(recvDispatchPtr == NULL)
    {
        //显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
        //清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_2;
        recvDispatchPtr->recvDataLength = dataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart2Recv,dataLength);
        //清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //打开空闲接收
        HAL_UARTEx_ReceiveToIdle_IT(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
        //数据转发
        if(ERROR == uart2CallBackSet.recvIntCallBack(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART2_RecvDatRelease(recvDispatchPtr);
        }
    }
    return;
#endif
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA) 
    if(UART2_Handler.RxState != HAL_UART_STATE_READY)
    {
        //DMA 半传输中断,DMA的状态时BUSY,不管,只处理串口空闲中断
        return;
    }
    //DMA模式接收  
    if((dataLength == 0)||(uart2CallBackSet.recvIntCallBack == NULL))
    {
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
        return;
    }
    UART_RECV_PACK* recvDispatchPtr = ADT_CircularPtrQueueGet(circularPtrQueueUart2RecvIdleBlockPtr);
    if(recvDispatchPtr == NULL)
    {
        //没有配置转发函数,显示日志
        MCU_RTT_SendStringInt("ADT_CircularPtrQueueGet PTR NULL,Discard Recv\r\n");
        //没有转发函数,清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
    }
    else
    {
        //配置接收参数
        recvDispatchPtr->packSrc = UART_PACK_SRC_2;
        recvDispatchPtr->recvDataLength = dataLength;
        //数据拷贝
        UserMemCopy((void*)(recvDispatchPtr->recvDataBufferPtr),(void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //清空接收
        UserMemClear((void*)currentBufferWithUart2Recv,LENGTH_UART2_RECV_SINGLE_BLOCK);
        //开始DMA接收中断
        HAL_UARTEx_ReceiveToIdle_DMA(&UART2_Handler,(uint8_t *)(currentBufferWithUart2Recv), LENGTH_UART2_RECV_SINGLE_BLOCK);
        //数据转发
        if(ERROR == uart2CallBackSet.recvIntCallBack(recvDispatchPtr))
        {
            //转发失败,将数据放回去
            MCU_UART2_RecvDatRelease(recvDispatchPtr);
        }
    }
    return;
#endif
}


//但凡收发存在一个中断,就定义中断
#if((MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_INT)||(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)||\
(MCU_UART2_RX_FUNC_USER==MCU_UART_RX_FUNC_INT)||(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA))
//串口中断
void USART2_IRQHandler(void) //串口1中断服务程序
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_UART_IRQHandler(&UART2_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//发送DMA中断
#if(MCU_UART2_TX_FUNC_USER == MCU_UART_TX_FUNC_DMA)
void MCU_UART2_DMA_TX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART2_TxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif

//接收DMA中断
#if(MCU_UART2_RX_FUNC_USER == MCU_UART_RX_FUNC_DMA)
void MCU_UART2_DMA_RX_IRQ_HANDLER(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL处理
    HAL_DMA_IRQHandler(&UART2_RxDMA_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
#endif











