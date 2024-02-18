/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 10:50:25 +0800
************************************************************************************************/ 
#include "MCU_CAN1.h"
#include "MCU_PIN.h"
#include "MCU_LogShow.h"
#include "ADT_CircularPtrQueue.h"
#include "tx_mutex.h"
#include "tx_semaphore.h"
#include "MCU_Mem.h"


//导入CAN数据段时时序配置表
extern const MCU_CAN_BPS_SETTING MCU_CAN_BaudArray[];

//CAN1操作句柄
static CAN_HandleTypeDef CAN1_Handler;
//CAN1过滤器句柄
static CAN_FilterTypeDef CAN1_FilterConfig;
//CAN1接收器句柄
static CAN_RxHeaderTypeDef CAN1_RxHeader;
//CAN模块讯息
static MCU_CAN_INFO can1Info;
//信号量,用于控制发送逻辑
static TX_SEMAPHORE semaphoreMCU_CAN1;
//CAN1空闲队列
static ADT_CIRCULAR_PTR_QUEUE_T can1CircularPtrIdleQueuePtr;
//CAN接收缓存
static CAN_RECV_PACK can1RecvPackIdleBuffer[MCU_CAN1_MSG_FIFO_LENGTH];
//CAN1接收空闲队列缓存
static uint8_t can1RecvBufIdleCircle[MCU_CAN1_MSG_FIFO_LENGTH][MCU_CAN_SINGLE_PACK_DATA_MAX];
//CAN1接收本地缓存
static uint8_t can1RecvBufLocal[MCU_CAN_SINGLE_PACK_DATA_MAX];
//互斥信号量,外界对于CAN总线调用的独占
static TX_MUTEX mutexMCU_CAN1;   
//CAN接收回调函数指针
static MCU_CAN_RecvPackProcFunc can1RecvPackCallBackFuncPtr = NULL;

//导入外部过滤器配置
//过滤器配置导入
#if(MCU_CAN1_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
extern const CAN_STD_FILTER can1StdFilterArray[];
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetStdFilterElementCount(void);
#endif

#if(MCU_CAN1_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
//CAN1扩展帧过滤器
extern const CAN_EXTEND_FILTER can1ExtendFilterArray[];
//供给外部调用的,过滤元素表长度
uint16_t MCU_Can1GetExtendFilterElementCount(void);
#endif

/*--------------------------------回调函数申明-------------------------------------*/
//发送邮箱0完成回调
void HAL_CAN1_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
//发送邮箱1完成回调
void HAL_CAN1_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
//发送邮箱2完成回调
void HAL_CAN1_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
//发送邮箱0取消回调
void HAL_CAN1_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan);
//发送邮箱1取消回调
void HAL_CAN1_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan);
//发送邮箱2取消回调
void HAL_CAN1_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan);
//接收FIFO0消息回调
void HAL_CAN1_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
//接收FIFO0满回调
void HAL_CAN1_RxFifo0FullCallback(CAN_HandleTypeDef *hcan);
//接收FIFO1消息回调
void HAL_CAN1_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
//接收FIFO1满回调
void HAL_CAN1_RxFifo1FullCallback(CAN_HandleTypeDef *hcan);
//休眠回调
void HAL_CAN1_SleepCallback(CAN_HandleTypeDef *hcan);
//唤醒回调
void HAL_CAN1_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan);
//错误回调
void HAL_CAN1_ErrorCallback(CAN_HandleTypeDef *hcan);

//清除状态
void MCU_Can1InfoClear(void)
{
    can1Info.errorCode = 0;
    can1Info.state = HAL_CAN_STATE_RESET;
    can1Info.reserve = 0;
}

/*---------------------------------------实现函数-------------------------------------*/
//硬件端口初始化
void MCU_CAN1_MspInit(void)
{
    //使能CAN时钟
    __HAL_RCC_CAN1_CLK_ENABLE();
    //CAN TX
    MCU_PinInit(MCU_CAN1_TX_PIN,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF9_CAN1);
    //CAN RX
    MCU_PinInit(MCU_CAN1_RX_PIN,MCU_PIN_MODE_AF_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,GPIO_AF9_CAN1);
    //清除info
    MCU_Can1InfoClear();
}

//硬件端口复位
void MCU_CAN1_MspDeInit(void)
{
    //CAN TX
    MCU_PinDeInit(MCU_CAN1_TX_PIN);
    //CAN RX
    MCU_PinDeInit(MCU_CAN1_RX_PIN);
    //关闭模块中断
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
}

//获取CAN互斥锁
void MCU_CAN1_GetLock(void)
{
    tx_mutex_get(&mutexMCU_CAN1,TX_WAIT_FOREVER);
}

//释放CAN1互斥锁
void MCU_CAN1_ReleaseLock(void)
{
    tx_mutex_put(&mutexMCU_CAN1);
}

/*---------------------------------CAN经典模式-------------------------------------*/
//初始化CAN总线
void MCU_CAN1_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr)
{
    if(mutexMCU_CAN1.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,不需要反复初始化
        return;
    }
    HAL_StatusTypeDef halState = HAL_OK;
    //创建互斥信号量
    tx_mutex_create(&mutexMCU_CAN1,"mutexMCU_CAN1",TX_INHERIT);
    //创建信号量,并将其初始化为发送缓存个数
    tx_semaphore_create(&semaphoreMCU_CAN1,"semaphoreMCU_CAN1",MCU_CAN_TRANS_FIFO_NBR_COUNT);
    //设置模块为CAN1
    CAN1_Handler.Instance = CAN1;
    //CAN1时钟使能
    __HAL_RCC_CAN1_CLK_ENABLE();
    //硬件复位
    MCU_CAN1_MspDeInit();
    //设备复位
    halState = HAL_CAN_DeInit(&CAN1_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
        while (1);
    }
    //中断回调函数设定
    can1RecvPackCallBackFuncPtr = recvCallBackPtr;
    //链接接收缓存
    uint16_t indexUtil = 0;
    //初始化空闲队列
    can1CircularPtrIdleQueuePtr = ADT_CircularPtrQueueCreate(MCU_CAN1_MSG_FIFO_LENGTH,MCU_MemMallocSramIn,MCU_MemFreeSramIn,
                                                                CoreLockIntBasePri,CoreUnlockIntBasePri,MCU_RTT_SendStringInt);
    //为空闲队列补充结构体
    for(indexUtil = 0; indexUtil < MCU_CAN1_MSG_FIFO_LENGTH; indexUtil++)
    {
        //清空接收缓存
        UserMemClear((uint8_t*)can1RecvBufIdleCircle[indexUtil],MCU_CAN_SINGLE_PACK_DATA_MAX);
        //接收对象参数初始化
        can1RecvPackIdleBuffer[indexUtil].idType = CAN_ID_TYPE_STD;
        can1RecvPackIdleBuffer[indexUtil].packSrc = CAN_PACK_SRC_1;
        can1RecvPackIdleBuffer[indexUtil].recvCmdID = 0X0000;
        can1RecvPackIdleBuffer[indexUtil].recvDataLength = 0;
        can1RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = NULL;
        //接收对象和缓存链接
        can1RecvPackIdleBuffer[indexUtil].recvDataBufferPtr = (uint8_t*)(can1RecvBufIdleCircle[indexUtil]);
        //接收队列和接收对象链接
        ADT_CircularPtrQueueWrite(can1CircularPtrIdleQueuePtr,(void*)(&(can1RecvPackIdleBuffer[indexUtil])));
    }
    //选择CAN 的工作模式，可配置为正常模式，内部回环测试模式，外部回环测试模式等
    CAN1_Handler.Init.Mode = CAN_MODE_NORMAL;
    //是否使用自动重传功能，使用自动重传功能时，会一直发送报文直到成功为止
    CAN1_Handler.Init.AutoRetransmission = MCU_CAN1_AUTO_RETRANS;
    //接收到溢出数据的时候怎么处理
    CAN1_Handler.Init.ReceiveFifoLocked = MCU_CAN1_RX_FIFO_OVERRUN_IGNORE_NEW;
    //时间触发模式
    CAN1_Handler.Init.TimeTriggeredMode = DISABLE;
    //自动离线管理
    CAN1_Handler.Init.AutoBusOff = MCU_CAN1_AUTO_BUS_OFF_MANAGE;
    //自动硬件唤醒
    CAN1_Handler.Init.AutoWakeUp = MCU_CAN1_AUTO_WAKE_UP_MANAGE;
    //发送优先模式
    CAN1_Handler.Init.TransmitFifoPriority = MCU_CAN1_TRANS_FIFO_ORDER_FIRST;
    //时钟分频因子，控制时间片Tq 的时间长度
    CAN1_Handler.Init.Prescaler = MCU_CAN_BaudArray[MCU_CAN1_BAUD_USER].brpDiv; 
    //配置SJW 的极限长度，即CAN 重新同步是单次可增加或缩短的最大长度
    CAN1_Handler.Init.SyncJumpWidth = MCU_CAN_BaudArray[MCU_CAN1_BAUD_USER].tqWithSJW;
    //配置CAN 时序中的BS1 段的长度，是PTS 段和PBS1 段的时间长度时间长度之和
    CAN1_Handler.Init.TimeSeg1 = MCU_CAN_BaudArray[MCU_CAN1_BAUD_USER].tqWithBS1;
    //配置CAN 位时序中的 BS2 段的长度
    CAN1_Handler.Init.TimeSeg2 = MCU_CAN_BaudArray[MCU_CAN1_BAUD_USER].tqWithBS2;
    //初始化之前进行底层初始化
    MCU_CAN1_MspInit();
    //模块初始化
    halState = HAL_CAN_Init(&CAN1_Handler);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
        while(1);
    }
    //注册回调函数
    CAN1_Handler.TxMailbox0CompleteCallback = HAL_CAN1_TxMailbox0CompleteCallback;
    CAN1_Handler.TxMailbox1CompleteCallback = HAL_CAN1_TxMailbox1CompleteCallback;
    CAN1_Handler.TxMailbox2CompleteCallback = HAL_CAN1_TxMailbox2CompleteCallback;
    CAN1_Handler.TxMailbox0AbortCallback = HAL_CAN1_TxMailbox0AbortCallback;
    CAN1_Handler.TxMailbox1AbortCallback = HAL_CAN1_TxMailbox1AbortCallback;
    CAN1_Handler.TxMailbox2AbortCallback = HAL_CAN1_TxMailbox2AbortCallback;
    CAN1_Handler.RxFifo0MsgPendingCallback = HAL_CAN1_RxFifo0MsgPendingCallback;
    CAN1_Handler.RxFifo0FullCallback = HAL_CAN1_RxFifo0FullCallback;
    CAN1_Handler.RxFifo1MsgPendingCallback = HAL_CAN1_RxFifo1MsgPendingCallback;
    CAN1_Handler.RxFifo1FullCallback = HAL_CAN1_RxFifo1FullCallback;
    CAN1_Handler.SleepCallback = HAL_CAN1_SleepCallback;
    CAN1_Handler.WakeUpFromRxMsgCallback = HAL_CAN1_WakeUpFromRxMsgCallback;
    CAN1_Handler.ErrorCallback = HAL_CAN1_ErrorCallback;
    //过滤设定,标准ID的过滤器个数
    uint16_t filterStdNbrCount = 0;
    uint16_t filterExtendNbrCount = 1;
    //计算程序中设定的过滤器数量
#if(MCU_CAN1_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
    filterStdNbrCount = MCU_Can1GetStdFilterElementCount();
#endif
#if(MCU_CAN1_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
    filterExtendNbrCount += MCU_Can1GetExtendFilterElementCount();
#endif
    //检查过滤器总数
    if(MCU_CAN1_FILTER_COUNT_SET < (filterExtendNbrCount + filterStdNbrCount))
    {
        MCU_RTT_SendString("MCU_CAN1 Filter Count Max Than System!!\r\n");
        while(1);
    }
    //过滤器共用配置
    CAN1_FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN1_FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN1_FilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    //CAN2过滤器的起始序号
    CAN1_FilterConfig.SlaveStartFilterBank = MCU_CAN1_FILTER_COUNT_SET;
    //开始设置过滤器,标准帧过滤器配置
#if(MCU_CAN1_STD_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
    //配置接收过滤器
    for(indexUtil = 0;indexUtil < filterStdNbrCount;indexUtil++)
    {
        //过滤器的类型是标准帧
        CAN1_FilterConfig.FilterBank = indexUtil;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        CAN1_FilterConfig.FilterFIFOAssignment = can1StdFilterArray[indexUtil].fifoNumber;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        //ID
        CAN1_FilterConfig.FilterIdLow = MCU_CAN_CALC_STD_ID_LOW(can1StdFilterArray[indexUtil].filterID);
        CAN1_FilterConfig.FilterIdHigh = MCU_CAN_CALC_STD_ID_HIGH(can1StdFilterArray[indexUtil].filterID);
        //匹配掩码
        CAN1_FilterConfig.FilterMaskIdLow = MCU_CAN_CALC_STD_MASK_LOW(can1StdFilterArray[indexUtil].filterMask);
        CAN1_FilterConfig.FilterMaskIdHigh = MCU_CAN_CALC_STD_MASK_HIGH(can1StdFilterArray[indexUtil].filterMask);
        //过滤器设定 
        halState = HAL_CAN_ConfigFilter(&CAN1_Handler, &CAN1_FilterConfig);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            while(1);
        }
    }
#endif
    //至少配置一个扩展帧
    CAN1_FilterConfig.FilterBank = filterStdNbrCount;
    //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
    CAN1_FilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
    CAN1_FilterConfig.FilterIdLow = MCU_CAN_CALC_EXT_ID_LOW(extFilterVal);
    CAN1_FilterConfig.FilterIdHigh = MCU_CAN_CALC_EXT_ID_HIGH(extFilterVal);
    //匹配掩码
    CAN1_FilterConfig.FilterMaskIdLow = MCU_CAN_CALC_EXT_MASK_LOW(extFilterMask);
    CAN1_FilterConfig.FilterMaskIdHigh = MCU_CAN_CALC_EXT_MASK_HIGH(extFilterMask);
    //过滤器设定 
    halState = HAL_CAN_ConfigFilter(&CAN1_Handler, &CAN1_FilterConfig);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
        while(1);
    }
    //开始设置过滤器,扩展帧过滤器配置
#if(MCU_CAN1_EXT_FILTER_NBR_EXIST == CONFIG_FUNC_ENABLE)
    //配置接收过滤器
    for(indexUtil = 0;(indexUtil < filterExtendNbrCount - 1);indexUtil++)
    {
        //过滤器的类型是扩展帧
        CAN1_FilterConfig.FilterBank = indexUtil + filterStdNbrCount + 1;
        //本成员主要用于设置经过筛选后的数据存储到哪一个接受FIFO
        CAN1_FilterConfig.FilterFIFOAssignment = can1ExtendFilterArray[indexUtil].fifoNumber;
        //本成员用于存储要筛选的 ID。根据不同筛选方式，ID 的值不同
        //ID
        CAN1_FilterConfig.FilterIdLow = MCU_CAN_CALC_EXT_ID_LOW(can1ExtendFilterArray[indexUtil].filterID);
        CAN1_FilterConfig.FilterIdHigh = MCU_CAN_CALC_EXT_ID_HIGH(can1ExtendFilterArray[indexUtil].filterID);
        //匹配掩码
        CAN1_FilterConfig.FilterMaskIdLow = MCU_CAN_CALC_EXT_MASK_LOW(can1ExtendFilterArray[indexUtil].filterMask);
        CAN1_FilterConfig.FilterMaskIdHigh = MCU_CAN_CALC_EXT_MASK_HIGH(can1ExtendFilterArray[indexUtil].filterMask);
        //过滤器设定 
        halState = HAL_CAN_ConfigFilter(&CAN1_Handler, &CAN1_FilterConfig);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            while(1);
        }
    }
#endif
    //中断优先级设置
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, CAN1_TX_PREE_PRI, CAN1_TX_SUB_PRI);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, CAN1_RX0_PREE_PRI, CAN1_RX0_SUB_PRI);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, CAN1_RX1_PREE_PRI, CAN1_RX1_SUB_PRI);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, CAN1_SEC_PREE_PRI, CAN1_SEC_SUB_PRI);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    //使能CAN接收中断
    //RX FIFO0
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO0_FULL);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO0_OVERRUN);
    //RX FIFO1
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO1_MSG_PENDING);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO1_FULL);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO1_OVERRUN);
    //TX MAILBOX
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_TX_MAILBOX_EMPTY);
    //ERROR
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_ERROR_WARNING);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_ERROR_PASSIVE);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_BUSOFF);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_LAST_ERROR_CODE);
    HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_ERROR);
    //启动CAN模块
    HAL_CAN_Start(&CAN1_Handler);
}

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr)
{
    ADT_CircularPtrQueueWrite(can1CircularPtrIdleQueuePtr,recvDataPtr);
}

//等待总线使能
static ERROR_SUB MCU_CAN1_SendMsgWaitBusActive(uint32_t maxWaitTimeMs)
{
    uint32_t utilCount = 0;
    uint32_t statusReg = 0;
    //等待非同步
    do
    {
        statusReg = READ_REG(CAN1_Handler.Instance->ESR);
        statusReg = (statusReg & CAN_ESR_BOFF);
        statusReg >>= CAN_ESR_BOFF_Pos;
        if(statusReg == 0X01)
        {
            utilCount++;
            CoreDelayMinTick();
            //同步中
            continue;
        }
        else
        {
            break;
        }
    }while(utilCount < maxWaitTimeMs);
    if(utilCount >= maxWaitTimeMs)
    {
        return ERROR_SUB_MCU_CAN1_TX_BUS_OFF_WAIT_TIME_OUT;
    }
    return ERROR_SUB_OK;
}

//等待空闲邮箱不是0
static ERROR_SUB MCU_CAN1_SendMsgWaitFreeMailBox(uint32_t maxWaitTimeMs)
{
    uint32_t freeMailBoxCount = 0;
    uint32_t utilCount = 0;
    //等待非同步
    do
    {
        freeMailBoxCount = HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler);
        if(freeMailBoxCount == 0)
        {
            utilCount++;
            CoreDelayMinTick();
            continue;
        }
        else
        {
            break;
        }
    }while(utilCount < maxWaitTimeMs);
    if(utilCount >= maxWaitTimeMs)
    {
        return ERROR_SUB_MCU_CAN1_TX_WAIT_FREE_MAIL_BOX;
    }
    return ERROR_SUB_OK;
}

//CAN总线发送数据
ERROR_SUB MCU_CAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_TxHeaderTypeDef TxHeader;
    HAL_StatusTypeDef halState = HAL_OK;
    uint32_t freeMailBoxCount = 0;
    uint32_t sendUseMailBoxNo = 0;
    //配置发送结构
    if(idType == CAN_ID_TYPE_STD)
    {
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = id;
    }
    else
    {
        TxHeader.IDE = CAN_ID_EXT;
        TxHeader.ExtId = id;
    }
    //帧类型
    TxHeader.RTR = CAN_RTR_DATA;
    if(dataLen > 8)
    {
        MCU_LogPrintf("%s,%d,SendDataLength: %d!!\r\n",__func__,__LINE__,dataLen);
        //FIFO里面有空间,但是就是加不进去.
        return ERROR_SUB_MCU_CAN1_TX_LENGTH;
    }
    //数据长度
    TxHeader.DLC = dataLen;
    //申请互斥信号量
    MCU_CAN1_GetLock();
    //等待总线在线
    errorSub = MCU_CAN1_SendMsgWaitBusActive(MCU_CAN1_WAIT_BUS_OFF_TIME_MS);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        MCU_CAN1_ReleaseLock();
        return errorSub;
    }
    //获取当前缓冲区数量,根据缓冲量决定等待时间
    freeMailBoxCount = HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler);
    //申请信号量,超时时间为单帧发送时间乘以发送缓存数量
    uint32_t txResult = TX_SUCCESS;
    if(freeMailBoxCount == 0)
    {
        txResult = tx_semaphore_get(&semaphoreMCU_CAN1,MCU_CAN1_WAIT_FIFO_SINGLE_PACK_MS);
    }
    else
    {
        txResult = tx_semaphore_get(&semaphoreMCU_CAN1,TX_NO_WAIT);
    }
    if(txResult != TX_SUCCESS)
    {
        //邮箱已经满了,全部清空
        if(0 == HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler))
        {
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX0);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX1);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX2);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
            //然后添加信号量到满状态
            for(uint8_t indexUtil = 0; indexUtil < MCU_CAN_TRANS_FIFO_NBR_COUNT;indexUtil++)
            {
                tx_semaphore_ceiling_put(&semaphoreMCU_CAN1,MCU_CAN_TRANS_FIFO_NBR_COUNT);
            }
            //释放互斥信号量
            MCU_CAN1_ReleaseLock();
            return ERROR_SUB_MCU_CAN1_TX_FIFO_FULL;
        }
        //释放掉所有的带发送数据
        if(0 != HAL_CAN_IsTxMessagePending(&CAN1_Handler,CAN_TX_MAILBOX0))
        {
            //释放发送空间 TX MAILBOX0
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX0);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
        }
        if(0 != HAL_CAN_IsTxMessagePending(&CAN1_Handler,CAN_TX_MAILBOX1))
        {
            //释放发送空间 TX MAILBOX0
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX1);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
        }
        if(0 != HAL_CAN_IsTxMessagePending(&CAN1_Handler,CAN_TX_MAILBOX2))
        {
            //释放发送空间 TX MAILBOX0
            halState = HAL_CAN_AbortTxRequest(&CAN1_Handler,CAN_TX_MAILBOX2);
            if(halState != HAL_OK)
            {
                MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
            }
        }
        //然后添加信号量到满状态
        for(uint8_t indexUtil = 0; indexUtil < MCU_CAN_TRANS_FIFO_NBR_COUNT;indexUtil++)
        {
            tx_semaphore_ceiling_put(&semaphoreMCU_CAN1,MCU_CAN_TRANS_FIFO_NBR_COUNT);
        }
        //释放互斥信号量
        MCU_CAN1_ReleaseLock();
        //FIFO里面有空间,但是就是加不进去.
        return ERROR_SUB_MCU_CAN1_TX_FAIL;
    }
    //申请到信号量之后还是要查询一下
    errorSub = MCU_CAN1_SendMsgWaitFreeMailBox(MCU_CAN1_WAIT_FIFO_SINGLE_PACK_MS);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放互斥信号量
        MCU_CAN1_ReleaseLock();
        return errorSub;
    }
    //发出数据
    halState = HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, dataBuf,&sendUseMailBoxNo);
    if(halState != HAL_OK)
    {
        //发送失败,添加信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_CAN1,MCU_CAN_TRANS_FIFO_NBR_COUNT);
        //释放互斥信号量
        MCU_CAN1_ReleaseLock();
        //数据添加到缓冲区错误
        MCU_LogShowHalState(halState,CAN1_Handler.ErrorCode);
        return ERROR_SUB_MCU_CAN1_TX_FAIL;
    }
    //释放互斥信号量
    MCU_CAN1_ReleaseLock();
    return ERROR_SUB_OK;
}

//获取CAN1的各项信息
MCU_CAN_INFO* MCU_CAN1_GetInfo(void)
{
    return &(can1Info);
}


/*----------------------------------CAN中断函数--------------------------------*/
//中断服务函数
void CAN1_TX_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_CAN_IRQHandler(&CAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void CAN1_RX0_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_CAN_IRQHandler(&CAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void CAN1_RX1_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_CAN_IRQHandler(&CAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

void CAN1_SCE_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用HAL库中断
    HAL_CAN_IRQHandler(&CAN1_Handler);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}


/*----------------------------------CAN中断回调处理函数-------------------------------------*/
//发送完成回调
void MCU_CAN1_TxCompleteCallBack(uint32_t BufferIndexes)
{
    //发送完成,回传信号量,通知进行下一次传输
    tx_semaphore_ceiling_put(&semaphoreMCU_CAN1,MCU_CAN_TRANS_FIFO_NBR_COUNT);
}

//FIFO0接收回调
void MCU_CAN1_RxFifo0CallBack(uint32_t RxFifo0ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo0ITs & CAN_IT_RX_FIFO0_OVERRUN) != RESET)
    {
        //CAN缓冲区数据丢失中断
        MCU_LogSendStringInt("MCU_CAN1_RxFifo0CallBack CAN_IT_RX_FIFO0_OVERRUN\r\n");
        //还是打开中断
        HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO0_OVERRUN);
    }
    else if((RxFifo0ITs & CAN_IT_RX_FIFO0_FULL) != RESET)
    {
        //CAN缓冲区满中断
        MCU_LogSendStringInt("MCU_CAN1_RxFifo0CallBack CAN_IT_RX_FIFO0_FULL\r\n");
        //还是打开中断
        HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO0_FULL);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo0ITs & CAN_IT_RX_FIFO0_MSG_PENDING) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO0, &CAN1_RxHeader, can1RecvBufLocal);
            //准备转发,只处理数据帧
            if(CAN1_RxHeader.RTR != CAN_RTR_DATA)
            {
                continue;
            }
            if(can1RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN1的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(can1CircularPtrIdleQueuePtr);
            if(recvPackPtr == NULL)
            {
                MCU_LogSendStringInt("MCU_CAN1_RxFifo0CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_1;
            if(CAN1_RxHeader.IDE == CAN_ID_STD)
            {
                recvPackPtr->idType = CAN_ID_TYPE_STD;
                recvPackPtr->recvCmdID = (uint32_t)CAN1_RxHeader.StdId;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_TYPE_EXT;
                recvPackPtr->recvCmdID = (uint32_t)CAN1_RxHeader.ExtId;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = CAN1_RxHeader.DLC;
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can1RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can1RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(can1CircularPtrIdleQueuePtr,(void*)recvPackPtr);
            }
        }while(0 != HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler,CAN_RX_FIFO0));
        //再次打开接收中断
        HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING);
    }
}

//FIFO1接收回调
void MCU_CAN1_RxFifo1CallBack(uint32_t RxFifo1ITs)
{
    CAN_RECV_PACK* recvPackPtr = NULL;
    ErrorStatus callBackFuncResult = SUCCESS;
    if((RxFifo1ITs & CAN_IT_RX_FIFO1_OVERRUN) != RESET)
    {
        //CAN缓冲区数据丢失中断
        MCU_LogSendStringInt("MCU_CAN1_RxFifo1CallBack CAN_IT_RX_FIFO1_OVERRUN\r\n");
        //还是打开中断
        HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO1_OVERRUN);
    }
    else if((RxFifo1ITs & CAN_IT_RX_FIFO1_FULL) != RESET)
    {
        //CAN缓冲区满中断
        MCU_LogSendStringInt("MCU_CAN1_RxFifo1CallBack CAN_IT_RX_FIFO1_FULL\r\n");
        //还是打开中断
        HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_RX_FIFO1_FULL);
    }
    //收到新的CAN数据,读取数据
    if ((RxFifo1ITs & CAN_IT_RX_FIFO1_MSG_PENDING) != RESET)
    {
        //循环读取数据,直到缓冲区数据为0
        do
        {
            //接收数据
            HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO1, &CAN1_RxHeader, can1RecvBufLocal);
            //准备转发,只处理数据帧
            if(CAN1_RxHeader.RTR != CAN_RTR_DATA)
            {
                continue;
            }
            if(can1RecvPackCallBackFuncPtr == NULL)
            {
                //没设置接收回调函数
                continue;
            }
            //从CAN1的空闲队列取出一个数据指针
            recvPackPtr = (CAN_RECV_PACK*)ADT_CircularPtrQueueGet(can1CircularPtrIdleQueuePtr);
            if(recvPackPtr == NULL)
            {
                MCU_LogSendStringInt("MCU_CAN1_RxFifo1CallBack IdleQueue No Element To Get\r\n");
                //本次循环结束
                continue;
            }
            //拷贝数据
            recvPackPtr->packSrc = CAN_PACK_SRC_1;
            if(CAN1_RxHeader.IDE == CAN_ID_STD)
            {
                recvPackPtr->idType = CAN_ID_TYPE_STD;
                recvPackPtr->recvCmdID = (uint32_t)CAN1_RxHeader.StdId;
            }
            else
            {
                recvPackPtr->idType = CAN_ID_TYPE_EXT;
                recvPackPtr->recvCmdID = (uint32_t)CAN1_RxHeader.ExtId;
            }
            //标准长度转换为数值长度
            recvPackPtr->recvDataLength = CAN1_RxHeader.DLC;
            for(uint8_t indexUtil = 0; indexUtil < recvPackPtr->recvDataLength; indexUtil++)
            {
                recvPackPtr->recvDataBufferPtr[indexUtil] = can1RecvBufLocal[indexUtil];
            }
            //转发到处理函数
            callBackFuncResult = can1RecvPackCallBackFuncPtr(recvPackPtr);
            if(callBackFuncResult == ERROR)
            {
                //转发失败,放回去
                ADT_CircularPtrQueueWrite(can1CircularPtrIdleQueuePtr,(void*)recvPackPtr);
            }
        }while(0 != HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler,CAN_RX_FIFO1));
        //再次打开接收中断
        HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING);
    }
}


//错误代码,ST的库写的真是粑粑一样
#define   MCU_CAN_ERROR_NONE      0x00000000U    /*!< No error             */
#define   MCU_CAN_ERROR_EWG       0x00000001U    /*!< EWG error            */
#define   MCU_CAN_ERROR_EPV       0x00000002U    /*!< EPV error            */
#define   MCU_CAN_ERROR_BOF       0x00000004U    /*!< BOF error            */
#define   MCU_CAN_ERROR_STF       0x00000008U    /*!< Stuff error          */
#define   MCU_CAN_ERROR_FOR       0x00000010U    /*!< Form error           */
#define   MCU_CAN_ERROR_ACK       0x00000020U    /*!< Acknowledgment error */
#define   MCU_CAN_ERROR_BR        0x00000040U    /*!< Bit recessive        */
#define   MCU_CAN_ERROR_BD        0x00000080U    /*!< LEC dominant         */
#define   MCU_CAN_ERROR_CRC       0x00000100U    /*!< LEC transfer error   */
#define   MCU_CAN_ERROR_FOV0      0x00000200U    /*!< FIFO0 overrun error  */
#define   MCU_CAN_ERROR_FOV1      0x00000400U    /*!< FIFO1 overrun error  */
#define   MCU_CAN_ERROR_TXFAIL    0x00000800U    /*!< Transmit failure     */
//错误回调
void MCU_CAN1_ErrorCallBack(void)
{
    can1Info.errorCode = CAN1_Handler.ErrorCode;
    can1Info.state = CAN1_Handler.State;
    if((can1Info.errorCode & MCU_CAN_ERROR_FOV0) != RESET)
    {
        //溢出回调
        MCU_CAN1_RxFifo0CallBack(CAN_IT_RX_FIFO0_OVERRUN);
    }
    if((can1Info.errorCode & MCU_CAN_ERROR_FOV1) != RESET)
    {
        //溢出回调
        MCU_CAN1_RxFifo1CallBack(CAN_IT_RX_FIFO1_OVERRUN);
    }
    if((can1Info.errorCode & MCU_CAN_ERROR_TXFAIL) != RESET)
    {
        //错误打印
        MCU_LogSendStringInt("MCU_CAN1_ErrorCallBack HAL_CAN_ERROR_TXFAIL\r\n");
        //错误中断使能
        HAL_CAN_ActivateNotification(&CAN1_Handler,CAN_IT_TX_MAILBOX_EMPTY);
        //发送信号量
        tx_semaphore_ceiling_put(&semaphoreMCU_CAN1,MCU_CAN_TRANS_FIFO_NBR_COUNT);
    }
    if((can1Info.errorCode & MCU_CAN_ERROR_BOF) != RESET)
    {
        //总线关闭回调,错误打印
        MCU_LogSendStringInt("MCU_CAN1_ErrorCallBack MCU_CAN_ERROR_BOF\r\n");
    }
    else if((can1Info.errorCode & MCU_CAN_ERROR_ACK) != RESET)
    {
        //总线ACK回调,错误打印
        MCU_LogSendStringInt("MCU_CAN1_ErrorCallBack MCU_CAN_ERROR_ACK\r\n");
    }
    else
    {
        MCU_LogSendStringInt("MCU_CAN1_ErrorCallBack!!!\r\n");
    }
    //复位CAN模块错误
    HAL_CAN_ResetError(&CAN1_Handler);
}

/*----------------------------------CAN中断回调函数----------------------------------------*/
//发送邮箱0完成回调
void HAL_CAN1_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_TxCompleteCallBack(CAN_TX_MAILBOX0);
    }
}

//发送邮箱1完成回调
void HAL_CAN1_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_TxCompleteCallBack(CAN_TX_MAILBOX1);
    }
}

//发送邮箱2完成回调
void HAL_CAN1_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_TxCompleteCallBack(CAN_TX_MAILBOX2);
    }
}

//发送邮箱0取消回调
void HAL_CAN1_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        asm("nop");
    }
}

//发送邮箱1取消回调
void HAL_CAN1_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        asm("nop");
    }
}

//发送邮箱2取消回调
void HAL_CAN1_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        asm("nop");
    }
}

//接收FIFO0消息回调
void HAL_CAN1_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_RxFifo0CallBack(CAN_IT_RX_FIFO0_MSG_PENDING);
    }
}

//接收FIFO0满回调
void HAL_CAN1_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_RxFifo0CallBack(CAN_IT_RX_FIFO0_FULL|CAN_IT_RX_FIFO0_MSG_PENDING);
    }
}

//接收FIFO1消息回调
void HAL_CAN1_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_RxFifo1CallBack(CAN_IT_RX_FIFO1_MSG_PENDING);
    }
}

//接收FIFO1满回调
void HAL_CAN1_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_RxFifo1CallBack(CAN_IT_RX_FIFO1_FULL|CAN_IT_RX_FIFO1_MSG_PENDING);
    }
}

//休眠回调
void HAL_CAN1_SleepCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        asm("nop");
    }
}

//唤醒回调
void HAL_CAN1_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        asm("nop");
    }
}

//错误回调
void HAL_CAN1_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    if(hcan->Instance == CAN1)
    {
        MCU_CAN1_ErrorCallBack();
    }
}















