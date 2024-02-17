/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-27 09:26:18 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-07 14:37:33 +0800
************************************************************************************************/ 
#include "BoardBarScanCR100M.h"
#include "tx_queue.h"
#include "ADT_CircularPtrQueue.h"
#include "tx_mutex.h"

/*------------------------------------------------------------------------------------------------------------------------------*/
//CR100M
//单次打开扫码枪
const uint8_t barScanCmdCR100M_OpenOnce[] = {0x02,0x2B,0x0D,0x0A};
//单次关闭扫码枪
const uint8_t barScanCmdCR100M_Close[] = {0x02,0x2D,0x0D,0x0A};
//一维码关闭重复扫描
const uint8_t barScanCmdCR100M_CloseCopy[] = {0x02,0x50,0x54,0x30,0x33,0x32,0x30,0x33,0x34,0x30,0x30,0x0D,0x0A};
//一维码开启重复扫描
const uint8_t barScanCmdCR100M_OpenCopy[] = {0x02,0x50,0x54,0x30,0x33,0x32,0x30,0x33,0x34,0x30,0x31,0x0D,0x0A};
//一维码开启扫码不同才输出
const uint8_t barScanCmdCR100M_DiffOutput[] = {0x02,0x50,0x54,0x30,0x33,0x32,0x30,0x38,0x36,0x41,0x30,0x0D,0x0A};
//一维码测试模式开启
const uint8_t barScanCmdCR100M_TestOn[] = {0x02,0x4A,0x50,0x2B,0x0D,0x0A};
//一维码测试模式关闭
const uint8_t barScanCmdCR100M_TestOff[] = {0x02,0x4A,0x50,0x2D,0x0D,0x0A};
//查看版本号
const uint8_t barScanCmdCR100M_ReadVersion[] = {0x02,0x56,0x0D,0x0A};
//设置扫码识别最小长度为三
const uint8_t barScanCmdCR100M_SetMinLength[] = {0x02,0x50,0x54,0x30,0x30,0x32,0x30,0x30,0x39,0x34,0x33,0x0D,0x0A};
//版本号信息
const uint8_t barScanCR100M_VersionCode[] = "CR100       V  1.02  24.04.2018";


/*------------------------------------------------------------------------------------------------------------------------------*/
//设备互斥信号量
static TX_MUTEX mutexBoardBarScanCR100ma;
//设备消息队列
static TX_QUEUE queueBoardBarScanCR100maRecvMsg;
//消息缓冲区
uint32_t queueBufferBoardBarScanCR100maRecvMsg[LENGTH_QUEUE_BOARD_BARSCAN_CR100MA_RECV_MSG];
//设备定时器,用于定时处理串口消息
static TX_TIMER timerBoardBarScanCR100maRecvMsg;
//当前接收数据长度
static uint16_t recvDataLengthBoardBarScanCR100ma;
//当前接受数据缓存
static uint8_t recvDataBufferBoardBarScanCR100ma[COMM_CACHE_LENGTH_BOARD_BARSCAN_CR100M];
//上次接收完成的时间戳
static uint32_t lastRecvPackTimeTickBoardBarScanCR100ma;
//存放已经扫码成功的数据
static BARSCAN_CR100M_RESULT cr100maResultArray[BOARD_BARSCAN_CR100MA_RESULT_BLOCK_COUNT];
//扫码成功的结果数量
static uint16_t cr100maResultCount = 0;

//获取设备锁
static void BoardBarScanCR100MA_GetLock(void)
{
    tx_mutex_get(&mutexBoardBarScanCR100ma,TX_WAIT_FOREVER);
}

//释放设备锁
static void BoardBarScanCR100MA_ReleaseLock(void)
{
    tx_mutex_put(&mutexBoardBarScanCR100ma);
}

//串口接收转发函数
static ErrorStatus BoardBarScanCR100MA_RecvMsgDispatch(UART_RECV_PACK* uartPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    //消息队列没有初始化
    if(queueBoardBarScanCR100maRecvMsg.tx_queue_id != TX_QUEUE_ID)
    {
        return ERROR;
    }
    //转发
    txResult = tx_queue_send(&queueBoardBarScanCR100maRecvMsg,(void*)(&uartPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("BoardBarScanCR100MA_RecvMsgDispatch tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//数据处理
static void BoardBarScanCR100MA_RecvPackProcess(uint8_t* packBufPtr,uint16_t packLength)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < packLength; indexUtil++)
    {
        //条码讯息处理
        if(0 != (recvDataLengthBoardBarScanCR100ma&0x4000))
        {
            //接收到换行
            if(packBufPtr[indexUtil] == '\n')
            {
                //接收到完整的一帧
                lastRecvPackTimeTickBoardBarScanCR100ma = tx_time_get();
                //检查结果块是否满
                if(cr100maResultCount == BOARD_BARSCAN_CR100MA_RESULT_BLOCK_COUNT)
                {
                    //队列已经满了
                    asm("nop");
                }
                else
                {
                    //队列没满,数据拷贝放到队列中,开头的0X02不要
                    cr100maResultArray[cr100maResultCount].barcodeLength = (recvDataLengthBoardBarScanCR100ma&0X0FFF);
                    cr100maResultArray[cr100maResultCount].barcodeTimeTick = lastRecvPackTimeTickBoardBarScanCR100ma;
                    UserMemCopy(cr100maResultArray[cr100maResultCount].buffer,recvDataBufferBoardBarScanCR100ma,
                                    cr100maResultArray[cr100maResultCount].barcodeLength);
                    //结果数量增加
                    cr100maResultCount += 1;
                }
                //重新接收
                recvDataLengthBoardBarScanCR100ma = 0;
            }
            else
            {
                //数据异常,回车之后必须是换行
                //重新接收
                recvDataLengthBoardBarScanCR100ma = 0;
            }
        }
        else
        {
            //正常接收
            if(packBufPtr[indexUtil] == '\r')
            {
                //回车
                recvDataLengthBoardBarScanCR100ma |= 0x4000;
                if((recvDataLengthBoardBarScanCR100ma & 0X0FFF) < LENGTH_MIN_BARCODE_CR100M)
                {
                    //单次数据最小字节限制
                    recvDataLengthBoardBarScanCR100ma = 0;
                }
                if(recvDataLengthBoardBarScanCR100ma != 0)
                {
                    if((recvDataBufferBoardBarScanCR100ma[0] == 'P')&&(recvDataBufferBoardBarScanCR100ma[1] == 'S'))
                    {
                        //过滤指令返回
                        recvDataLengthBoardBarScanCR100ma = 0;
                    }
                }
            }
            else
            {
                if((packBufPtr[indexUtil] != 0x02) && (packBufPtr[indexUtil] != '\n'))
                {
                    //0x02数据起始字符,不处理，0x0A换行符不处理
                    recvDataBufferBoardBarScanCR100ma[recvDataLengthBoardBarScanCR100ma] = packBufPtr[indexUtil];
                    recvDataLengthBoardBarScanCR100ma++;
                    if(recvDataLengthBoardBarScanCR100ma >= COMM_CACHE_LENGTH_BOARD_BARSCAN_CR100M)
                    {
                        //超过了最大缓存长度限制
                        recvDataLengthBoardBarScanCR100ma = 0;
                    };
                }
            }
        }
    }
}

//串口接收处理函数
static void BoardBarScanCR100MA_RecvMsgProcessPeriod(ULONG unusedParam)
{
    //从消息队列中获取消息
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardBarScanCR100maRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            return;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_BARSCAN_CR100MA)
        {
            //不是我想要的数据
            continue;
        }
        //数据处理
        BoardBarScanCR100MA_RecvPackProcess(queueMsgPtr->uartRecvDataBuffer,queueMsgPtr->uartRecvDataLength);
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_BARSCAN_CR100MA(queueMsgPtr);
    }while(1);
}


//清除接收缓存
static void BoardBarScanCR100MA_ClearRecvCache(void)
{
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    //定时器停止
    tx_timer_deactivate(&timerBoardBarScanCR100maRecvMsg);
    //清空消息队列
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardBarScanCR100maRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            break;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_BARSCAN_CR100MA)
        {
            //不是我想要的数据
            continue;
        }
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_BARSCAN_CR100MA(queueMsgPtr);
    }while(1);
    //清除内部缓存
    recvDataLengthBoardBarScanCR100ma = 0;
    UserMemClear(recvDataBufferBoardBarScanCR100ma,COMM_CACHE_LENGTH_BOARD_BARSCAN_CR100M);
    //清除结果
    cr100maResultCount = 0;
    //清除时间戳
    lastRecvPackTimeTickBoardBarScanCR100ma = 0;
}

//停止数据接收
static void BoardBarScanCR100MA_StopRecv(void)
{
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    //定时器停止
    tx_timer_deactivate(&timerBoardBarScanCR100maRecvMsg);
    //清空消息队列
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardBarScanCR100maRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            break;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_BARSCAN_CR100MA)
        {
            //不是我想要的数据
            continue;
        }
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_BARSCAN_CR100MA(queueMsgPtr);
    }while(1);
}

//端口初始化
void BoardBarScanCR100MA_PortInit(void)
{
    //初始化相关资源
    if(mutexBoardBarScanCR100ma.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,直接返回
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexBoardBarScanCR100ma,"mutexBoardBarScanCR100ma",TX_INHERIT);
    //创建消息队列
    tx_queue_create(&queueBoardBarScanCR100maRecvMsg,"queueBoardBarScanCR100maRecvMsg",1,(void*)queueBufferBoardBarScanCR100maRecvMsg,
                        sizeof(queueBufferBoardBarScanCR100maRecvMsg)/sizeof(uint8_t));
    //创建定时器任务,创建的时候不使能
    tx_timer_create(&timerBoardBarScanCR100maRecvMsg,"timerBoardBarScanCR100maRecvMsg",BoardBarScanCR100MA_RecvMsgProcessPeriod,0,
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_BARSCAN_CR100MA_RECV_MSG),
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_BARSCAN_CR100MA_RECV_MSG),
                        TX_NO_ACTIVATE);
    //串口资源初始化
    COMM_INIT_BOARD_BARSCAN_CR100MA(BOARD_BARSCAN_CR100MA_BAUD,BOARD_BARSCAN_CR100MA_LENGTH,BOARD_BARSCAN_CR100MA_STOP_BIT,
                                        BOARD_BARSCAN_CR100MA_CHECK_MODE,BOARD_BARSCAN_CR100MA_HARD_CONTROL);
    //回调函数注册
    COMM_CALLBACL_REGISTER_BOARD_BARSCAN_CR100MA(BoardBarScanCR100MA_RecvMsgDispatch,NULL,NULL);
    //接收数据缓存清空
    BoardBarScanCR100MA_ClearRecvCache();
    //延时一段时间
    CoreDelayMs(100);
}

//发送指令,不等待返回
static void BoardBarScanCR100MA_SendCommandNoReturn(uint8_t* cmdBuf,uint16_t cmdLength)
{
    //清空缓存
    BoardBarScanCR100MA_ClearRecvCache();
    //首先发送指令
    COMM_SEND_CMD_BUF_BOARD_BARSCAN_CR100MA(cmdBuf,cmdLength);
}

//发送指令,等待返回
static void BoardBarScanCR100MA_SendCommand(uint8_t* cmdBuf,uint16_t cmdLength)
{
    //清空缓存
    BoardBarScanCR100MA_ClearRecvCache();
    //启动定时器
    tx_timer_activate(&timerBoardBarScanCR100maRecvMsg);
    //首先发送指令
    COMM_SEND_CMD_BUF_BOARD_BARSCAN_CR100MA(cmdBuf,cmdLength);
}

//条码枪默认配置
ERROR_SUB BoardBarScanCR100MA_ConfigDefault(void)
{
    //关闭单次扫码
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_Close,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_Close));
    //延时一段时间
    CoreDelayMs(100);
    //关闭重复
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_CloseCopy,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_CloseCopy));
    //延时一段时间
    CoreDelayMs(100);
    //开启扫码不同才输出
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_DiffOutput,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_DiffOutput));
    //延时一段时间
    CoreDelayMs(100);
    //关闭测试模式
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_TestOff,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_TestOff));
    //延时一段时间
    CoreDelayMs(100);
    //设置扫码枪识别最小长度为3
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_SetMinLength,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_SetMinLength));
    //延时一段时间
    CoreDelayMs(100);
    return BoardBarScanCR100MA_CheckVersion();
}

//检查条码枪版本号
ERROR_SUB BoardBarScanCR100MA_CheckVersion(void)
{
    uint8_t retryCount = 0;
    uint32_t startTimeStamp;
    uint16_t utilIndex = 0;
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    do
    {
        //发送指令,读取版本号
        BoardBarScanCR100MA_SendCommand((uint8_t*)barScanCmdCR100M_ReadVersion,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_ReadVersion));
        //等待收到完整一帧
        startTimeStamp = tx_time_get();
        //等待接收
        while (tx_time_get() - startTimeStamp < 500)
        {
            if(cr100maResultCount > 0)
            {
                break;
            }
            else
            {
                CoreDelayMs(10);
            }
        }
        //检查接收
        if(cr100maResultCount > 0)
        {
            break;
        }
        //没收到,计数增加
        retryCount++;
    } while (retryCount < 3);
    //检查是否收到返回
    if(cr100maResultCount == 0)
    {
        //清空缓存
        BoardBarScanCR100MA_ClearRecvCache();
        //释放设备锁
        BoardBarScanCR100MA_ReleaseLock();
        return ERROR_SUB_BOARD_CR100MA_CHECK_VERSION_NO_RESP;
    }
    //收到返回,检查结果
    for(utilIndex = 0; utilIndex < VERSION_CHECK_MATCH_CODE_LENGTH;utilIndex++)
    {
        if(barScanCR100M_VersionCode[utilIndex] != cr100maResultArray[0].buffer[utilIndex])
        {
            //清空缓存
            BoardBarScanCR100MA_ClearRecvCache();
            //释放设备锁
            BoardBarScanCR100MA_ReleaseLock();
            return ERROR_SUB_BOARD_CR100MA_CHECK_VERSION_NOT_MATCH;
        }
    }
    //清空缓存
    BoardBarScanCR100MA_ClearRecvCache();
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//打开条码枪,单次扫描
ERROR_SUB BoardBarScanCR100MA_OpenSingle(void)
{
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //发送指令
    BoardBarScanCR100MA_SendCommand((uint8_t*)barScanCmdCR100M_OpenOnce,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_OpenOnce));
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//打开条码枪,测试模式
ERROR_SUB BoardBarScanCR100MA_OpenTestMode(void)
{
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //发送指令
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_TestOn,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_TestOn));
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//打开条码枪,多次扫码
ERROR_SUB BoardBarScanCR100MA_OpenMulitple(void)
{
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //发送指令
    BoardBarScanCR100MA_SendCommand((uint8_t*)barScanCmdCR100M_OpenCopy,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_OpenCopy));
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//关闭条码枪
ERROR_SUB BoardBarScanCR100MA_Close(void)
{
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //关闭单次扫码
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_Close,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_Close));
    //延时一段时间
    CoreDelayMs(100);
    //关闭重复
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_CloseCopy,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_CloseCopy));
    //延时一段时间
    CoreDelayMs(100);
    //关闭测试模式
    BoardBarScanCR100MA_SendCommandNoReturn((uint8_t*)barScanCmdCR100M_TestOff,DIM_ARRAY_ELEMENT_COUNT(barScanCmdCR100M_TestOff));
    //延时一段时间
    CoreDelayMs(100);
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//清除条码枪缓冲区
ERROR_SUB BoardBarScanCR100MA_ClearHistory(void)
{
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //清除缓冲区
    BoardBarScanCR100MA_ClearRecvCache();
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return ERROR_SUB_OK;
}

//获取条码个数
uint16_t BoardBarScanCR100MA_GetBarCodeResultCount(void)
{
    return cr100maResultCount;
}

//获取上次条码结果,如果没有结果,返回空指针
BARSCAN_CR100M_RESULT* BoardBarScanCR100MA_GetLastResult(uint16_t* resultCount)
{
    BARSCAN_CR100M_RESULT* resultArrayPtr = NULL;
    //首先获取设备锁
    BoardBarScanCR100MA_GetLock();
    //停止数据接收
    BoardBarScanCR100MA_StopRecv();
    //返回结果
    *resultCount = cr100maResultCount;
    if(cr100maResultCount == 0)
    {
        //释放设备锁
        BoardBarScanCR100MA_ReleaseLock();
        return NULL;
    }
    //申请内存
    resultArrayPtr = MEM_MALLOC_BOARD_BARSCAN_CR100MA(cr100maResultCount*DIM_STRUCT_LENGTH_BYTE(BARSCAN_CR100M_RESULT));
    //数据拷贝
    UserMemCopy((void*)resultArrayPtr,(void*)cr100maResultArray,cr100maResultCount*DIM_STRUCT_LENGTH_BYTE(BARSCAN_CR100M_RESULT));
    //数据清除
    BoardBarScanCR100MA_ClearRecvCache();
    //释放设备锁
    BoardBarScanCR100MA_ReleaseLock();
    return resultArrayPtr;
}

//释放扫码结果
void BoardBarScanCR100MA_ReleaseMem(BARSCAN_CR100M_RESULT* barscanResultPtr)
{
    if(barscanResultPtr == NULL)
    {
        return;
    }
    MEM_FREE_BOARD_BARSCAN_CR100MA(barscanResultPtr);
}


