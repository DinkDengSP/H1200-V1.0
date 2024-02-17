/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-07 09:43:44 +0800
************************************************************************************************/ 
#include "ProtocolC21NetBase.h"
#include "ADT_CircularPtrList.h"
#include "tx_mutex.h"

//PE NET初始化标记
PROTOCOL_C21_NET_INIT_SET c21NetInitSet;
//串口接收缓存
static C21_NET_RECV_CACHE c21NetSocketRecvCache;

//指令ACK处理
extern void ProtocolC21NetSocketPackRecvProcessAckCmd(void);
//SOCKET 指令信息处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void ProtocolC21NetSocketPackRecvProcessCmd(C21_NET_PACKAGE* netSocketPackagePtr);

//获取数据访问权
void ProtocolC21NetSocketGetDataAccessLock(void)
{
    tx_mutex_get(&(c21NetInitSet.mutexSocketDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void ProtocolC21NetSocketGetDataAccessRelease(void)
{
    tx_mutex_put(&(c21NetInitSet.mutexSocketDataAccess));
}


//初始化接收缓存
static void ProtocolC21NetSocketRecvCacheInit(void)
{
    c21NetSocketRecvCache.lastRecvTimeStamp = 0;
    c21NetSocketRecvCache.recvDataLength = 0;
    c21NetSocketRecvCache.recvBodyLength = 0;
    UserMemSet(c21NetSocketRecvCache.recvDataBuf,0,PROTOCOL_C21_NET_SINGLE_PACK_DATA_MAX);
}

//复位接收缓存
static void ProtocolC21NetSocketRecvCacheReset(void)
{
    c21NetSocketRecvCache.lastRecvTimeStamp = 0;
    c21NetSocketRecvCache.recvDataLength = 0;
    c21NetSocketRecvCache.recvBodyLength = 0;
}

//初始化SOCKET
void ProtocolC21NetSocketInit(void)
{
    //创建互斥信号量
    if(c21NetInitSet.mutexSocketDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return ;
    }
    //创建互斥信号量
    tx_mutex_create(&(c21NetInitSet.mutexSocketDataAccess),"C21NetSocketDataAccessMutex",TX_INHERIT);
    //创建指令等待缓存链表
    c21NetInitSet.listSendCmdWaitAckPtr = ADT_CircularPtrListCreate(ProtocolC21NetSocketMemMalloc,ProtocolC21NetSocketMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //初始化接收缓存
    ProtocolC21NetSocketRecvCacheInit();
}

//检查SOCKET是否已经初始化
FlagStatus ProtocolC21NetSocketInitQuery(void)
{
    if(c21NetInitSet.mutexSocketDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return SET;
    }
    return RESET;
}

//设置C21_NET_PACKAGE为默认值
static void C21NetSocketPackageReset(C21_NET_PACKAGE* netSocketPackagePtr)
{
    netSocketPackagePtr->cmdCode = 0;
    netSocketPackagePtr->dataBodyLength = 0;
    netSocketPackagePtr->dataBodyBufferPtr = NULL;
}


//通过接收缓存创建接收信息
static C21_NET_PACKAGE* ProtocolC21NetSocketPackRecvCreatePackMsg(void)
{   
    //创建消息体
    C21_NET_PACKAGE* netSocketPackagePtr = ProtocolC21NetSocketMemMalloc(DIM_STRUCT_LENGTH_BYTE(C21_NET_PACKAGE));
    if(netSocketPackagePtr == NULL)
    {
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, ProtocolC21NetSocketMemMalloc Failed\r\n",__func__,__LINE__);
        return NULL;
    }
    //设置默认值
    C21NetSocketPackageReset(netSocketPackagePtr);
    //申请数据缓存
    if(c21NetSocketRecvCache.recvBodyLength != 0)
    {
        netSocketPackagePtr->dataBodyBufferPtr = ProtocolC21NetSocketMemMalloc(c21NetSocketRecvCache.recvBodyLength);
        if(netSocketPackagePtr->dataBodyBufferPtr == NULL)
        {
            PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, ProtocolC21NetSocketMemMalloc Buffer Failed,Length: %d\r\n",__func__,__LINE__,
                                        c21NetSocketRecvCache.recvBodyLength);
            //内存申请失败
            ProtocolC21NetSocketMemFree(netSocketPackagePtr);
            return NULL;
        }
        //拷贝数据
        UserMemCopy(netSocketPackagePtr->dataBodyBufferPtr,c21NetSocketRecvCache.recvDataBuf+PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER,
                        c21NetSocketRecvCache.recvBodyLength);
        netSocketPackagePtr->dataBodyLength = c21NetSocketRecvCache.recvBodyLength;
    }
    else
    {
        netSocketPackagePtr->dataBodyBufferPtr = NULL;
        netSocketPackagePtr->dataBodyLength = 0;
    }
    //开始解析数据
    netSocketPackagePtr->cmdCode += c21NetSocketRecvCache.recvDataBuf[3];
    netSocketPackagePtr->cmdCode <<= 8;
    netSocketPackagePtr->cmdCode += c21NetSocketRecvCache.recvDataBuf[2];
    return netSocketPackagePtr;
}


//帧切片检测
static void ProtocolC21NetSocketPackRecvTimeFrameCheck(uint32_t currentTickMs)
{
    if(c21NetSocketRecvCache.recvDataLength == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    if(c21NetSocketRecvCache.lastRecvTimeStamp == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    //当前时间大于上一帧接收时间且大于切片时间
    if(currentTickMs >= c21NetSocketRecvCache.lastRecvTimeStamp)
    {
        if(currentTickMs - c21NetSocketRecvCache.lastRecvTimeStamp > TIME_INVERVAL_MS_C21_NET_SOCKET_FRAME)
        {
            ProtocolC21NetSocketRecvCacheReset();
            return;
        }
        return;
    }
    //当前时间小于上一帧时间,时间异常,接收复位
    ProtocolC21NetSocketRecvCacheReset();
}

//接收数据第一个字节处理
static void ProtocolC21NetSocketPackRecvByteFirst(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == PROTOCOL_C21_NET_FRAME_HEAD_LEADER_A)
    {
        c21NetSocketRecvCache.recvDataBuf[0] = recvByte;
        c21NetSocketRecvCache.recvDataLength = 1;
        //接收起始,设置帧接收时间
        c21NetSocketRecvCache.lastRecvTimeStamp = currentTickMs;
        return;
    }
    else 
    {
        return;
    }
}

//接收数据第二个字节处理
static void ProtocolC21NetSocketPackRecvByteSecond(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == PROTOCOL_C21_NET_FRAME_HEAD_LEADER_B)
    {
        c21NetSocketRecvCache.recvDataBuf[1] = recvByte;
        c21NetSocketRecvCache.recvDataLength = 2;
        return;
    }
    else
    {
        //收到PROTOCOL_C21_NET_FRAME_HEAD_LEADER_A之后必然接受PROTOCOL_C21_NET_FRAME_HEAD_LEADER_B
        //否则清除
        ProtocolC21NetSocketRecvCacheReset();
        return;
    }
}


//指令头校验和检测
static PROTOCOL_C21_NET_RECV_STATE ProtocolC21NetSocketPackRecvCmdSumCheck(void)
{
    uint8_t indexUtil = 0;
    uint16_t cmdCheckSumRecv = 0;
    //刚好收集到足够的帧头,开始校验和计算匹配
    cmdCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[11];
    cmdCheckSumRecv <<= 8;
    cmdCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[10];
    //计算校验和
    uint16_t cmdCheckSumCalc = 0;
    for(indexUtil = 0; indexUtil < 10; indexUtil++)
    {
        cmdCheckSumCalc += c21NetSocketRecvCache.recvDataBuf[indexUtil];
    }
    //计算匹配
    if(cmdCheckSumCalc != cmdCheckSumRecv)
    {
        //匹配不上
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, CmdCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,
                                    cmdCheckSumRecv, cmdCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        ProtocolC21NetSocketRecvCacheReset();
        return PROTOCOL_C21_NET_RECV_STATE_ERROR;
    }
    return PROTOCOL_C21_NET_RECV_STATE_NONE;
}

//指令数据长度检验
static PROTOCOL_C21_NET_RECV_STATE ProtocolC21NetSocketPackRecvDataLengthCheck(void)
{
    //计算出需要接收的数据长度
    c21NetSocketRecvCache.recvBodyLength = 0;
    c21NetSocketRecvCache.recvBodyLength += c21NetSocketRecvCache.recvDataBuf[5];
    c21NetSocketRecvCache.recvBodyLength <<= 8;
    c21NetSocketRecvCache.recvBodyLength += c21NetSocketRecvCache.recvDataBuf[4];
    //检查长度是否正确
    if(c21NetSocketRecvCache.recvBodyLength > PROTOCOL_C21_NET_SOCKET_SINGLE_PACK_DATA_BODY_MAX)
    {
        //数据包长度最大为NET_CMD_SINGLE_PACK_LENGTH_MAX
        PROTOCOL_C21_NET_SOCKET_LOG("E:>%s,%d DataAreaLength More Then %d ,Real %d\r\n",__func__,__LINE__,
                                        PROTOCOL_C21_NET_SOCKET_SINGLE_PACK_DATA_BODY_MAX,c21NetSocketRecvCache.recvBodyLength);
        //清理接收状态,当成一包新数据接收
        ProtocolC21NetSocketRecvCacheReset();
        return PROTOCOL_C21_NET_RECV_STATE_ERROR;
    }
    return PROTOCOL_C21_NET_RECV_STATE_NONE;
}


//数据校验和校验
static PROTOCOL_C21_NET_RECV_STATE ProtocolC21NetSocketPackRecvDataSumCheck(void)
{
    uint16_t indexUtil = 0;
    uint32_t dataCheckSumRecv = 0;
    //先读取接收到的校验和
    dataCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[9];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[8];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[7];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += c21NetSocketRecvCache.recvDataBuf[6];
    //计算校验和
    uint32_t dataCheckSumCalc = 0;
    for(indexUtil = 0; indexUtil < c21NetSocketRecvCache.recvBodyLength; indexUtil++)
    {
        dataCheckSumCalc += c21NetSocketRecvCache.recvDataBuf[PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER + indexUtil];
    }
    //计算匹配
    if(dataCheckSumRecv != dataCheckSumCalc)
    {
        //匹配不上
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, DataCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,dataCheckSumRecv, dataCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        ProtocolC21NetSocketRecvCacheReset();
        return PROTOCOL_C21_NET_RECV_STATE_ERROR;
    }
    //数据匹配上了,一包数据接收完成
    return PROTOCOL_C21_NET_RECV_STATE_COMPLETE;
}

//串口接收一个字节数据的处理
static PROTOCOL_C21_NET_RECV_STATE ProtocolC21NetSocketPackRecvByteProc(uint8_t recvByte,uint32_t currentTickMs)
{
    PROTOCOL_C21_NET_RECV_STATE netRecvState = PROTOCOL_C21_NET_RECV_STATE_NONE;
    //帧超时时间检测,主要是为了防止TCPIP的数据切片导致两帧时间间隔过长
    ProtocolC21NetSocketPackRecvTimeFrameCheck(currentTickMs);
    //接受数据长度为0,当前数据必须是PROTOCOL_C21_NET_FRAME_HEAD_LEADER_A才能启动接收
    if(c21NetSocketRecvCache.recvDataLength == 0)
    {
        ProtocolC21NetSocketPackRecvByteFirst(recvByte,currentTickMs);
        return PROTOCOL_C21_NET_RECV_STATE_NONE;
    }
    //接收长度为1,当前数据必须是NET_CMD_FRAME_HEAD_LEADER_B才能启动接收
    else if(c21NetSocketRecvCache.recvDataLength == 1)
    {
        ProtocolC21NetSocketPackRecvByteSecond(recvByte,currentTickMs);
        return PROTOCOL_C21_NET_RECV_STATE_NONE;
    }
    if(c21NetSocketRecvCache.recvDataLength < PROTOCOL_C21_NET_FRAME_LENGTH_HEADER)
    {
        //还没接收到完整的指令头
        c21NetSocketRecvCache.recvDataBuf[c21NetSocketRecvCache.recvDataLength] = recvByte;
        c21NetSocketRecvCache.recvDataLength++;
        //接收到完整指令头
        if(c21NetSocketRecvCache.recvDataLength == PROTOCOL_C21_NET_FRAME_LENGTH_HEADER)
        {
            //指令头校验
            netRecvState = ProtocolC21NetSocketPackRecvCmdSumCheck();
            if(netRecvState == PROTOCOL_C21_NET_RECV_STATE_ERROR)
            {
                return netRecvState;
            }
            //数据长度校验
            netRecvState = ProtocolC21NetSocketPackRecvDataLengthCheck();
            if(netRecvState == PROTOCOL_C21_NET_RECV_STATE_ERROR)
            {
                return netRecvState;
            }
            //数据长度达标,空包,不用校验
            if(c21NetSocketRecvCache.recvBodyLength == 0)
            {
                //空包数据接收完成
                return PROTOCOL_C21_NET_RECV_STATE_COMPLETE;
            }
            return netRecvState;
        }
        return netRecvState;
    }
    else
    {
        //数据大于帧头,数据继续接收
        c21NetSocketRecvCache.recvDataBuf[c21NetSocketRecvCache.recvDataLength] = recvByte;
        c21NetSocketRecvCache.recvDataLength++;
        //检测接收到的数据长度是否等于想要接收的数据长度
        if(c21NetSocketRecvCache.recvBodyLength == c21NetSocketRecvCache.recvDataLength - PROTOCOL_C21_NET_FRAME_OFFSET_DATA_BUFFER)
        {
            //数据包校验
            netRecvState = ProtocolC21NetSocketPackRecvDataSumCheck();
        }
        return netRecvState;
    }
}

//指令ACK处理
static void ProtocolC21NetSocketPackRecvProcessAck(C21_NET_PACKAGE* netSocketPackagePtr)
{
    //指令ACK的数据长度必须是1
    if(netSocketPackagePtr->dataBodyLength != 1)
    {
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, AckCmd DataLength Must Be 1 RealLength: %d\r\n",__func__,__LINE__,netSocketPackagePtr->dataBodyLength);
        if(netSocketPackagePtr->dataBodyBufferPtr != NULL)
        {
            //释放内存
            ProtocolC21NetSocketMemFree(netSocketPackagePtr->dataBodyBufferPtr);
        }
        //释放内存
        ProtocolC21NetSocketMemFree(netSocketPackagePtr);
        return;
    }
    //查看ACK识别码
    uint8_t ackResult = netSocketPackagePtr->dataBodyBufferPtr[0];
    if(ackResult == 0)
    {
        //指令接收不正常
        PROTOCOL_C21_NET_SOCKET_LOG("E:> %s %d, AckResult Error!!\r\n",__func__,__LINE__);
        //释放内存
        ProtocolC21NetSocketMemFree(netSocketPackagePtr->dataBodyBufferPtr);
        //释放内存
        ProtocolC21NetSocketMemFree(netSocketPackagePtr);
        return;
    }
    //释放内存
    ProtocolC21NetSocketMemFree(netSocketPackagePtr->dataBodyBufferPtr);
    //释放内存
    ProtocolC21NetSocketMemFree(netSocketPackagePtr);
    //调用相应的程序处理
    ProtocolC21NetSocketPackRecvProcessAckCmd();
}


//处理接受的NET消息
void static ProtocolC21NetSocketPackRecvProcess(C21_NET_PACKAGE* netSocketPackagePtr)
{
    switch(netSocketPackagePtr->cmdCode)
    {
        case PROTOCOL_C21_NET_ACK_CMD:
            //ACK指令
            ProtocolC21NetSocketPackRecvProcessAck(netSocketPackagePtr);
            break;
        default:
            //其他指令
            ProtocolC21NetSocketPackRecvProcessCmd(netSocketPackagePtr);
            //释放内存
            if(netSocketPackagePtr->dataBodyBufferPtr != NULL)
            {
                ProtocolC21NetSocketMemFree(netSocketPackagePtr->dataBodyBufferPtr);
            }
            ProtocolC21NetSocketMemFree(netSocketPackagePtr);
            break;
    }
}

//SOCKET数据包接收
void ProtocolC21NetSocketPackRecv(NET_RECV_PACK* netRecvPackPtr,uint32_t currentTickMs)
{
    uint16_t indexUtil = 0;
    PROTOCOL_C21_NET_RECV_STATE netRecvState = PROTOCOL_C21_NET_RECV_STATE_NONE;
    C21_NET_PACKAGE* netSocketPackagePtr = NULL;
    //处理接收数据
    for(indexUtil = 0; indexUtil < netRecvPackPtr->recvDataLength; indexUtil++)
    {
        //单字节循环解析
        netRecvState = ProtocolC21NetSocketPackRecvByteProc(netRecvPackPtr->recvDatBufPtr[indexUtil],currentTickMs);
        //检查是否接收到完整的一包数据
        if(netRecvState == PROTOCOL_C21_NET_RECV_STATE_COMPLETE)
        {
            //从接收缓存创建消息报
            netSocketPackagePtr = ProtocolC21NetSocketPackRecvCreatePackMsg();
            //调用下一级处理
            if(netSocketPackagePtr != NULL)
            {
                //处理掉
                ProtocolC21NetSocketPackRecvProcess(netSocketPackagePtr);
            }
            //处理完成之后清空缓存
            ProtocolC21NetSocketRecvCacheReset();
            //处理状态设置为初始状态
            netRecvState = PROTOCOL_C21_NET_RECV_STATE_NONE;
        }
    }
}

//SOCKET内部周期性数据处理
void ProtocolC21NetRunPeriod(uint32_t currentTickMs)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    C21_NET_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t elementCount;
    listNodePtr = NULL;
    //获取访问量
    ProtocolC21NetSocketGetDataAccessLock();
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(c21NetInitSet.listSendCmdWaitAckPtr);
    if(elementCount == 0)
    {
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    //查看顶部元素
    listNodePtr = ADT_CircularPtrListPeekTop(c21NetInitSet.listSendCmdWaitAckPtr);
    if(listNodePtr == NULL)
    {
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    //找到元素了,获取数据指针
    cmdWaitPackPtr = (C21_NET_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(cmdWaitPackPtr->retryCount == 0)
    {
        if(RESET == ProtocolC21NetSocketGetConnectFlag())
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            //释放访问量
            ProtocolC21NetSocketGetDataAccessRelease();
            PROTOCOL_C21_NET_SOCKET_LOG("E>: %s,%d,ProtocolC21NetSocketGetConnectFlag. Disconect,CmdValue: 0X%04X\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->cmdValue);
            return;
        }
        //排队数据第一次发送
        errorSub = ProtocolC21NetSendCmdOnly(cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        }
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    if(currentTickMs - cmdWaitPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_C21_NET_SOCKET)
    {
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        //没到超时时间
        return;
    }
    //到了超时时间,检测重发次数
    if(cmdWaitPackPtr->retryCount >= 3)
    {
        //日志发送失败
        PROTOCOL_C21_NET_SOCKET_LOG("E>: %s,%d,ProtocolC21NetSocketSendCmdFailed.RetryMax.CmdVal: 0X%02X\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->cmdValue);
        //移除顶部元素
        ADT_CircularPtrListDeleteNodeCurrent(c21NetInitSet.listSendCmdWaitAckPtr,listNodePtr);
        //释放等待
        if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
        {
            ProtocolC21NetSocketMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
        }
        ProtocolC21NetSocketMemFree(cmdWaitPackPtr);
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
    else
    {
        if(RESET == ProtocolC21NetSocketGetConnectFlag())
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            //释放访问量
            ProtocolC21NetSocketGetDataAccessRelease();
            PROTOCOL_C21_NET_SOCKET_LOG("E>: %s,%d,ProtocolC21NetSocketGetConnectFlag. Disconect,CmdValue: 0X%04X\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->cmdValue);
            return;
        }
        //重发
        errorSub = ProtocolC21NetSendCmdOnly(cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            cmdWaitPackPtr->retryCount += 1;
            //发送成功,时间更新
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            //释放访问量
            ProtocolC21NetSocketGetDataAccessRelease();
            return;
        }
        //发送失败了,错误打印
        PROTOCOL_C21_NET_SOCKET_LOG("E>: %s,%d,ProtocolC21NetSendCmdOnly Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //超过了指令滞留的最大时间
        if(currentTickMs - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_RETRY_C21_NET_SOCKET)
        {
            //删除掉这个节点
            ADT_CircularPtrListDeleteNodeCurrent(c21NetInitSet.listSendCmdWaitAckPtr,listNodePtr);
            //释放等待
            if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
            {
                ProtocolC21NetSocketMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
            }
            ProtocolC21NetSocketMemFree(cmdWaitPackPtr);
        }
        //释放访问量
        ProtocolC21NetSocketGetDataAccessRelease();
        return;
    }
}