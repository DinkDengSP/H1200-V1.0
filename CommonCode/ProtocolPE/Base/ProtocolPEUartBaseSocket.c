/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 17:01:20 +0800
************************************************************************************************/ 
#include "ProtocolPEUartBase.h"
#include "ADT_CircularPtrList.h"
#include "tx_mutex.h"

//PE UART初始化标记
PROTOCOL_PE_UART_INIT_SET peUartInitSet;
//串口接收缓存
static PE_UART_RECV_CACHE peUartSocketRecvCache;

//指令ACK处理
extern void ProtocolPEUartSocketPackRecvProcessAckCmd(void);
//SOCKET 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void ProtocolPEUartSocketPackRecvProcessCmd(PE_UART_PACKAGE* uartSocketPackagePtr);

//获取数据访问权
void ProtocolPEUartSocketGetDataAccessLock(void)
{
    tx_mutex_get(&(peUartInitSet.mutexSocketDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void ProtocolPEUartSocketGetDataAccessRelease(void)
{
    tx_mutex_put(&(peUartInitSet.mutexSocketDataAccess));
}


//初始化接收缓存
static void ProtocolPEUartSocketRecvCacheInit(void)
{
    peUartSocketRecvCache.lastRecvTimeStamp = 0;
    peUartSocketRecvCache.recvDataLength = 0;
    peUartSocketRecvCache.recvBodyLength = 0;
    UserMemSet(peUartSocketRecvCache.recvDataBuf,0,PROTOCOL_PE_UART_SINGLE_PACK_DATA_MAX);
}

//复位接收缓存
static void ProtocolPEUartSocketRecvCacheReset(void)
{
    peUartSocketRecvCache.lastRecvTimeStamp = 0;
    peUartSocketRecvCache.recvDataLength = 0;
    peUartSocketRecvCache.recvBodyLength = 0;
}

//初始化SOCKET
void ProtocolPEUartSocketInit(void)
{
    //创建互斥信号量
    if(peUartInitSet.mutexSocketDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return ;
    }
    //创建互斥信号量
    tx_mutex_create(&(peUartInitSet.mutexSocketDataAccess),"PEUartSocketDataAccessMutex",TX_INHERIT);
    //创建指令等待缓存链表
    peUartInitSet.listSendCmdWaitAckPtr = ADT_CircularPtrListCreate(ProtocolPEUartSocketMemMalloc,ProtocolPEUartSocketMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //初始化接收缓存
    ProtocolPEUartSocketRecvCacheInit();
}

//检查SOCKET是否已经初始化
FlagStatus ProtocolPEUartSocketInitQuery(void)
{
    if(peUartInitSet.mutexSocketDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return SET;
    }
    return RESET;
}

//设置PE_UART_PACKAGE为默认值
static void PEUartSocketPackageReset(PE_UART_PACKAGE* uartSocketPackagePtr)
{
    uartSocketPackagePtr->cmdCode = 0;
    uartSocketPackagePtr->dataBodyLength = 0;
    uartSocketPackagePtr->dataBodyBufferPtr = NULL;
}


//通过接收缓存创建接收信息
static PE_UART_PACKAGE* ProtocolPEUartSocketPackRecvCreatePackMsg(void)
{   
    //创建消息体
    PE_UART_PACKAGE* uartSocketPackagePtr = ProtocolPEUartSocketMemMalloc(DIM_STRUCT_LENGTH_BYTE(PE_UART_PACKAGE));
    if(uartSocketPackagePtr == NULL)
    {
        PROTOCOL_PE_UART_SOCKET_LOG("E:> %s %d, ProtocolPEUartSocketMemMalloc Failed\r\n",__func__,__LINE__);
        return NULL;
    }
    //设置默认值
    PEUartSocketPackageReset(uartSocketPackagePtr);
    //申请数据缓存
    if(peUartSocketRecvCache.recvBodyLength != 0)
    {
        uartSocketPackagePtr->dataBodyBufferPtr = ProtocolPEUartSocketMemMalloc(peUartSocketRecvCache.recvBodyLength);
        if(uartSocketPackagePtr->dataBodyBufferPtr == NULL)
        {
            PROTOCOL_PE_UART_SOCKET_LOG("E:> %s %d, ProtocolPEUartSocketMemMalloc Buffer Failed,Length: %d\r\n",__func__,__LINE__,
                                        peUartSocketRecvCache.recvBodyLength);
            //内存申请失败
            ProtocolPEUartSocketMemFree(uartSocketPackagePtr);
            return NULL;
        }
        //拷贝数据
        UserMemCopy(uartSocketPackagePtr->dataBodyBufferPtr,peUartSocketRecvCache.recvDataBuf+PROTOCOL_PE_UART_FRAME_OFFSET_DATA_BUFFER,
                        peUartSocketRecvCache.recvBodyLength);
        uartSocketPackagePtr->dataBodyLength = peUartSocketRecvCache.recvBodyLength;
    }
    else
    {
        uartSocketPackagePtr->dataBodyBufferPtr = NULL;
        uartSocketPackagePtr->dataBodyLength = 0;
    }
    //开始解析数据
    uartSocketPackagePtr->cmdCode += peUartSocketRecvCache.recvDataBuf[2];
    uartSocketPackagePtr->cmdCode <<= 8;
    uartSocketPackagePtr->cmdCode += peUartSocketRecvCache.recvDataBuf[3];
    return uartSocketPackagePtr;
}


//帧切片检测
static void ProtocolPEUartSocketPackRecvTimeFrameCheck(uint32_t currentTickMs)
{
    if(peUartSocketRecvCache.recvDataLength == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    if(peUartSocketRecvCache.lastRecvTimeStamp == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    //当前时间大于上一帧接收时间且大于切片时间
    if(currentTickMs >= peUartSocketRecvCache.lastRecvTimeStamp)
    {
        if(currentTickMs - peUartSocketRecvCache.lastRecvTimeStamp > TIME_INVERVAL_MS_PE_UART_SOCKET_FRAME)
        {
            ProtocolPEUartSocketRecvCacheReset();
            return;
        }
        return;
    }
    //当前时间小于上一帧时间,时间异常,接收复位
    ProtocolPEUartSocketRecvCacheReset();
}

//接收数据第一个字节处理
static void ProtocolPEUartSocketPackRecvByteFirst(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == PROTOCOL_PE_UART_FRAME_HEAD_LEADER_A)
    {
        peUartSocketRecvCache.recvDataBuf[0] = recvByte;
        peUartSocketRecvCache.recvDataLength = 1;
        //接收起始,设置帧接收时间
        peUartSocketRecvCache.lastRecvTimeStamp = currentTickMs;
        return;
    }
    else 
    {
        return;
    }
}

//接收数据第二个字节处理
static void ProtocolPEUartSocketPackRecvByteSecond(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == PROTOCOL_PE_UART_FRAME_HEAD_LEADER_B)
    {
        peUartSocketRecvCache.recvDataBuf[1] = recvByte;
        peUartSocketRecvCache.recvDataLength = 2;
        return;
    }
    else
    {
        //收到PROTOCOL_PE_UART_FRAME_HEAD_LEADER_A之后必然接受PROTOCOL_PE_UART_FRAME_HEAD_LEADER_B
        //否则清除
        ProtocolPEUartSocketRecvCacheReset();
        return;
    }
}


//指令头校验和检测
static PROTOCOL_PE_UART_RECV_STATE ProtocolPEUartSocketPackRecvCmdSumCheck(void)
{
    uint8_t indexUtil = 0;
    uint16_t cmdCheckSumRecv = 0;
    //刚好收集到足够的帧头,开始校验和计算匹配
    cmdCheckSumRecv += peUartSocketRecvCache.recvDataBuf[10];
    cmdCheckSumRecv <<= 8;
    cmdCheckSumRecv += peUartSocketRecvCache.recvDataBuf[11];
    //计算校验和
    uint16_t cmdCheckSumCalc = 0;
    for(indexUtil = 0; indexUtil < 10; indexUtil++)
    {
        cmdCheckSumCalc += peUartSocketRecvCache.recvDataBuf[indexUtil];
    }
    //计算匹配
    if(cmdCheckSumCalc != cmdCheckSumRecv)
    {
        //匹配不上
        PROTOCOL_PE_UART_SOCKET_LOG("E:> %s %d, CmdCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,
                                    cmdCheckSumRecv, cmdCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        ProtocolPEUartSocketRecvCacheReset();
        return PROTOCOL_PE_UART_RECV_STATE_ERROR;
    }
    return PROTOCOL_PE_UART_RECV_STATE_NONE;
}

//指令数据长度检验
static PROTOCOL_PE_UART_RECV_STATE ProtocolPEUartSocketPackRecvDataLengthCheck(void)
{
    //计算出需要接收的数据长度
    peUartSocketRecvCache.recvBodyLength = 0;
    peUartSocketRecvCache.recvBodyLength += peUartSocketRecvCache.recvDataBuf[4];
    peUartSocketRecvCache.recvBodyLength <<= 8;
    peUartSocketRecvCache.recvBodyLength += peUartSocketRecvCache.recvDataBuf[5];
    //检查长度是否正确
    if(peUartSocketRecvCache.recvBodyLength > PROTOCOL_PE_UART_SOCKET_SINGLE_PACK_DATA_BODY_MAX)
    {
        //数据包长度最大为UART_CMD_SINGLE_PACK_LENGTH_MAX
        PROTOCOL_PE_UART_SOCKET_LOG("E:>%s,%d DataAreaLength More Then %d ,Real %d\r\n",__func__,__LINE__,
                                        PROTOCOL_PE_UART_SOCKET_SINGLE_PACK_DATA_BODY_MAX,peUartSocketRecvCache.recvBodyLength);
        //清理接收状态,当成一包新数据接收
        ProtocolPEUartSocketRecvCacheReset();
        return PROTOCOL_PE_UART_RECV_STATE_ERROR;
    }
    return PROTOCOL_PE_UART_RECV_STATE_NONE;
}


//数据校验和校验
static PROTOCOL_PE_UART_RECV_STATE ProtocolPEUartSocketPackRecvDataSumCheck(void)
{
    uint16_t indexUtil = 0;
    uint32_t dataCheckSumRecv = 0;
    //先读取接收到的校验和
    dataCheckSumRecv += peUartSocketRecvCache.recvDataBuf[6];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += peUartSocketRecvCache.recvDataBuf[7];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += peUartSocketRecvCache.recvDataBuf[8];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += peUartSocketRecvCache.recvDataBuf[9];
    //计算校验和
    uint32_t dataCheckSumCalc = 0;
    for(indexUtil = 0; indexUtil < peUartSocketRecvCache.recvBodyLength; indexUtil++)
    {
        dataCheckSumCalc += peUartSocketRecvCache.recvDataBuf[PROTOCOL_PE_UART_FRAME_OFFSET_DATA_BUFFER + indexUtil];
    }
    //计算匹配
    if(dataCheckSumRecv != dataCheckSumCalc)
    {
        //匹配不上
        PROTOCOL_PE_UART_SOCKET_LOG("E:> %s %d, DataCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,dataCheckSumRecv, dataCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        ProtocolPEUartSocketRecvCacheReset();
        return PROTOCOL_PE_UART_RECV_STATE_ERROR;
    }
    //数据匹配上了,一包数据接收完成
    return PROTOCOL_PE_UART_RECV_STATE_COMPLETE;
}

//串口接收一个字节数据的处理
static PROTOCOL_PE_UART_RECV_STATE ProtocolPEUartSocketPackRecvByteProc(uint8_t recvByte,uint32_t currentTickMs)
{
    PROTOCOL_PE_UART_RECV_STATE uartRecvState = PROTOCOL_PE_UART_RECV_STATE_NONE;
    //帧超时时间检测,主要是为了防止TCPIP的数据切片导致两帧时间间隔过长
    ProtocolPEUartSocketPackRecvTimeFrameCheck(currentTickMs);
    //接受数据长度为0,当前数据必须是PROTOCOL_PE_UART_FRAME_HEAD_LEADER_A才能启动接收
    if(peUartSocketRecvCache.recvDataLength == 0)
    {
        ProtocolPEUartSocketPackRecvByteFirst(recvByte,currentTickMs);
        return PROTOCOL_PE_UART_RECV_STATE_NONE;
    }
    //接收长度为1,当前数据必须是UART_CMD_FRAME_HEAD_LEADER_B才能启动接收
    else if(peUartSocketRecvCache.recvDataLength == 1)
    {
        ProtocolPEUartSocketPackRecvByteSecond(recvByte,currentTickMs);
        return PROTOCOL_PE_UART_RECV_STATE_NONE;
    }
    if(peUartSocketRecvCache.recvDataLength < PROTOCOL_PE_UART_FRAME_LENGTH_HEADER)
    {
        //还没接收到完整的指令头
        peUartSocketRecvCache.recvDataBuf[peUartSocketRecvCache.recvDataLength] = recvByte;
        peUartSocketRecvCache.recvDataLength++;
        //接收到完整指令头
        if(peUartSocketRecvCache.recvDataLength == PROTOCOL_PE_UART_FRAME_LENGTH_HEADER)
        {
            //指令头校验
            uartRecvState = ProtocolPEUartSocketPackRecvCmdSumCheck();
            if(uartRecvState == PROTOCOL_PE_UART_RECV_STATE_ERROR)
            {
                return uartRecvState;
            }
            //数据长度校验
            uartRecvState = ProtocolPEUartSocketPackRecvDataLengthCheck();
            if(uartRecvState == PROTOCOL_PE_UART_RECV_STATE_ERROR)
            {
                return uartRecvState;
            }
            //数据长度达标,空包,不用校验
            if(peUartSocketRecvCache.recvBodyLength == 0)
            {
                //空包数据接收完成
                return PROTOCOL_PE_UART_RECV_STATE_COMPLETE;
            }
            return uartRecvState;
        }
        return uartRecvState;
    }
    else
    {
        //数据大于帧头,数据继续接收
        peUartSocketRecvCache.recvDataBuf[peUartSocketRecvCache.recvDataLength] = recvByte;
        peUartSocketRecvCache.recvDataLength++;
        //检测接收到的数据长度是否等于想要接收的数据长度
        if(peUartSocketRecvCache.recvBodyLength == peUartSocketRecvCache.recvDataLength - PROTOCOL_PE_UART_FRAME_OFFSET_DATA_BUFFER)
        {
            //数据包校验
            uartRecvState = ProtocolPEUartSocketPackRecvDataSumCheck();
        }
        return uartRecvState;
    }
}

//指令ACK处理
static void ProtocolPEUartSocketPackRecvProcessAck(PE_UART_PACKAGE* uartSocketPackagePtr)
{
    //指令ACK的数据长度必须是1
    if(uartSocketPackagePtr->dataBodyLength != 0)
    {
        PROTOCOL_PE_UART_SOCKET_LOG("E:> %s %d, AckCmd DataLength Must Be 0 RealLength: %d\r\n",__func__,__LINE__,uartSocketPackagePtr->dataBodyLength);
        if(uartSocketPackagePtr->dataBodyBufferPtr != NULL)
        {
            //释放内存
            ProtocolPEUartSocketMemFree(uartSocketPackagePtr->dataBodyBufferPtr);
        }
        //释放内存
        ProtocolPEUartSocketMemFree(uartSocketPackagePtr);
        return;
    }
    //释放内存
    ProtocolPEUartSocketMemFree(uartSocketPackagePtr);
    //调用相应的程序处理
    ProtocolPEUartSocketPackRecvProcessAckCmd();
}


//处理接受的UART消息
void static ProtocolPEUartSocketPackRecvProcess(PE_UART_PACKAGE* uartSocketPackagePtr)
{
    switch(uartSocketPackagePtr->cmdCode)
    {
        case PROTOCOL_PE_UART_ACK_CMD:
            //ACK指令
            ProtocolPEUartSocketPackRecvProcessAck(uartSocketPackagePtr);
            break;
        default:
            //其他指令
            ProtocolPEUartSocketPackRecvProcessCmd(uartSocketPackagePtr);
            //释放内存
            if(uartSocketPackagePtr->dataBodyBufferPtr != NULL)
            {
                ProtocolPEUartSocketMemFree(uartSocketPackagePtr->dataBodyBufferPtr);
            }
            ProtocolPEUartSocketMemFree(uartSocketPackagePtr);
            break;
    }
}

//SOCKET数据包接收
void ProtocolPEUartSocketPackRecv(UART_RECV_PACK* uartRecvPackPtr,uint32_t currentTickMs)
{
    uint16_t indexUtil = 0;
    PROTOCOL_PE_UART_RECV_STATE uartRecvState = PROTOCOL_PE_UART_RECV_STATE_NONE;
    PE_UART_PACKAGE* uartSocketPackagePtr = NULL;
    //处理接收数据
    for(indexUtil = 0; indexUtil < uartRecvPackPtr->recvDataLength; indexUtil++)
    {
        //单字节循环解析
        uartRecvState = ProtocolPEUartSocketPackRecvByteProc(uartRecvPackPtr->recvDataBufferPtr[indexUtil],currentTickMs);
        //检查是否接收到完整的一包数据
        if(uartRecvState == PROTOCOL_PE_UART_RECV_STATE_COMPLETE)
        {
            //从接收缓存创建消息报
            uartSocketPackagePtr = ProtocolPEUartSocketPackRecvCreatePackMsg();
            //调用下一级处理
            if(uartSocketPackagePtr != NULL)
            {
                //处理掉
                ProtocolPEUartSocketPackRecvProcess(uartSocketPackagePtr);
            }
            //处理完成之后清空缓存
            ProtocolPEUartSocketRecvCacheReset();
            //处理状态设置为初始状态
            uartRecvState = PROTOCOL_PE_UART_RECV_STATE_NONE;
        }
    }
}

//SOCKET1内部周期性数据处理
void ProtocolPEUartRunPeriod(uint32_t currentTickMs)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PE_UART_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t elementCount;
    listNodePtr = NULL;
    //获取访问量
    ProtocolPEUartSocketGetDataAccessLock();
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(peUartInitSet.listSendCmdWaitAckPtr);
    if(elementCount == 0)
    {
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    //查看顶部元素
    listNodePtr = ADT_CircularPtrListPeekTop(peUartInitSet.listSendCmdWaitAckPtr);
    if(listNodePtr == NULL)
    {
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    //找到元素了,获取数据指针
    cmdWaitPackPtr = (PE_UART_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(cmdWaitPackPtr->retryCount == 0)
    {
        //排队数据第一次发送
        errorSub = ProtocolPEUartSendCmdOnly(cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        }
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    if(currentTickMs - cmdWaitPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_PE_UART_SOCKET)
    {
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        //没到超时时间
        return;
    }
    //到了超时时间,检测重发次数
    if(cmdWaitPackPtr->retryCount >= 3)
    {
        //日志发送失败
        PROTOCOL_PE_UART_SOCKET_LOG("E>: %s,%d,ProtocolPEUartSocketSendCmdFailed.RetryMax.CmdVal: 0X%02X\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->cmdValue);
        //移除顶部元素
        ADT_CircularPtrListDeleteNodeCurrent(peUartInitSet.listSendCmdWaitAckPtr,listNodePtr);
        //释放等待
        if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
        {
            ProtocolPEUartSocketMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
        }
        ProtocolPEUartSocketMemFree(cmdWaitPackPtr);
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
    else
    {
        //重发
        errorSub = ProtocolPEUartSendCmdOnly(cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            //释放访问量
            ProtocolPEUartSocketGetDataAccessRelease();
            return;
        }
        //发送失败了,错误打印
        PROTOCOL_PE_UART_SOCKET_LOG("E>: %s,%d,ProtocolPEUartSendCmdOnly Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //超过了指令滞留的最大时间
        if(currentTickMs - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_RETRY_PE_UART_SOCKET)
        {
            //删除掉这个节点
            ADT_CircularPtrListDeleteNodeCurrent(peUartInitSet.listSendCmdWaitAckPtr,listNodePtr);
            //释放等待
            if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
            {
                ProtocolPEUartSocketMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
            }
            ProtocolPEUartSocketMemFree(cmdWaitPackPtr);
        }
        //释放访问量
        ProtocolPEUartSocketGetDataAccessRelease();
        return;
    }
}