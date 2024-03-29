/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-29 13:14:19 +0800
************************************************************************************************/ 
#include "UartProtBase.h"

//UART SOCKET初始化标记
extern UART_PROT_INIT_SET uartProtInitSet;
//串口接收缓存
extern UART_PROT_RECV_CACHE uartProtRecvCache;

//获取数据访问权
void UartProtGetDataAccessLock(void)
{
    tx_mutex_get(&(uartProtInitSet.mutexUartProtDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void UartProtGetDataAccessRelease(void)
{
    tx_mutex_put(&(uartProtInitSet.mutexUartProtDataAccess));
}

//基础发送函数
ERROR_SUB UartProtSendDataBase(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            PROT_PACK_TYPE packType,uint32_t idCode,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t* sendDataBufferPtr = NULL;
    uint16_t sendBufferLength = 18+paramLength;
    uint16_t crc16CalcResult = 0;
    //内存申请
    sendDataBufferPtr = UartProtMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = UART_PROT_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = UART_PROT_FRAME_HEAD_LEADER_B;
    sendDataBufferPtr[2] = srcModule;
    sendDataBufferPtr[3] = targetModule;
    sendDataBufferPtr[4] = srcBoardID;
    sendDataBufferPtr[5] = targetBoardID;
    sendDataBufferPtr[6] = (uint8_t)packType;
    //ID码设置
    sendDataBufferPtr[7] = (uint8_t)(idCode>>0);
    sendDataBufferPtr[8] = (uint8_t)(idCode>>8);
    sendDataBufferPtr[9] = (uint8_t)(idCode>>16);
    sendDataBufferPtr[10] = (uint8_t)(idCode>>24);
    //数据长度
    sendDataBufferPtr[11] = (uint8_t)(paramLength>>0);
    sendDataBufferPtr[12] = (uint8_t)(paramLength>>8);
    //数据区域CRC
    crc16CalcResult = UartProtUtilCalcCRC16(paramBuffer,paramLength);
    sendDataBufferPtr[13] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[14] = (uint8_t)(crc16CalcResult>>8);
    //重发标记
    sendDataBufferPtr[15] = retryCount;
    //帧头校验码
    crc16CalcResult = UartProtUtilCalcCRC16(sendDataBufferPtr+2,14);
    sendDataBufferPtr[16] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[17] = (uint8_t)(crc16CalcResult>>8);
    //拷贝参数
    if(sendDataBufferPtr != NULL)
    {
        UserMemCopy(sendDataBufferPtr+18,paramBuffer,paramLength);
    }
    //申请互斥信号量
    UartProtGetMutexLock();
    //数据发送
    errorSub = UartProtSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    UartProtReleaseMutexLock();
    //释放内存
    UartProtMemFree(sendDataBufferPtr);
    return errorSub;
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB UartProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    return UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_CMD,cmdVal,
                                            paramBuffer,paramLength);
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB UartProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,
                                                uint16_t paramLength)
{
    return UartProtSendDataBase(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_CMD,cmdVal,paramBuffer,paramLength);
}

//SOCKET发送指令ACK包
ERROR_SUB UartProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                            PROT_ACK ackCmd)
{
    return UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_CMD,cmdVal,
                                            (uint8_t*)&ackCmd,1);
}

//SOCKET发送指令ACK包
ERROR_SUB UartProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    return UartProtSendDataBase(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_CMD,cmdVal,(uint8_t*)&ackCmd,1);
}

//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB UartProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
                                            uint8_t retryCount,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                                            ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    uint8_t* sendDataBufferPtr = NULL;
    uint16_t sendBufferLength = 26+resultBufferLen;
    uint16_t validDataLength = 8+resultBufferLen;
    uint16_t crc16CalcResult = 0;
    uint32_t utilDataTemp = 0;
    //内存申请
    sendDataBufferPtr = UartProtMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = UART_PROT_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = UART_PROT_FRAME_HEAD_LEADER_B;
    sendDataBufferPtr[2] = srcModule;
    sendDataBufferPtr[3] = targetModule;
    sendDataBufferPtr[4] = srcBoardID;
    sendDataBufferPtr[5] = targetBoardID;
    sendDataBufferPtr[6] = (uint8_t)PROT_PACK_TYPE_RESULT;
    //ID码设置
    sendDataBufferPtr[7] = (uint8_t)(cmdVal>>0);
    sendDataBufferPtr[8] = (uint8_t)(cmdVal>>8);
    sendDataBufferPtr[9] = (uint8_t)(cmdVal>>16);
    sendDataBufferPtr[10] = (uint8_t)(cmdVal>>24);
    //数据长度
    sendDataBufferPtr[11] = (uint8_t)(validDataLength>>0);
    sendDataBufferPtr[12] = (uint8_t)(validDataLength>>8);
    //重发标记
    sendDataBufferPtr[15] = retryCount;
    //结果参数设定
    //执行结果
    sendDataBufferPtr[18] = (uint8_t)resultCode;
    //错误主码
    utilDataTemp = (uint32_t)errorMain;
    sendDataBufferPtr[19] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    sendDataBufferPtr[20] = (uint8_t)(utilDataTemp&0xff);
    //错误级别
    sendDataBufferPtr[21] = (uint8_t)errorLevel;
    //错误子码
    utilDataTemp = (uint32_t)errorSub;
    uint32_t boardIdCode = UartProtGetSelfBoardID();
    boardIdCode <<= 24;
    //对错误子码进行处理附加板号
    if((utilDataTemp != 0X00000000)&&((utilDataTemp&0XFF000000) == 0X00000000))
    {
        //最大区域等于0
        utilDataTemp |= boardIdCode;
    }
    sendDataBufferPtr[22] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    sendDataBufferPtr[23] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    sendDataBufferPtr[24] = (uint8_t)(utilDataTemp&0xff);
    utilDataTemp >>= 8;
    sendDataBufferPtr[25] = (uint8_t)(utilDataTemp&0xff);
    //拷贝参数
    if(resultBufferLen != NULL)
    {
        UserMemCopy(sendDataBufferPtr+26,resultParamBuffer,resultBufferLen);
    }
    //数据区域CRC
    crc16CalcResult = UartProtUtilCalcCRC16(sendDataBufferPtr+18,resultBufferLen+8);
    sendDataBufferPtr[13] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[14] = (uint8_t)(crc16CalcResult>>8);
    //帧头校验码
    crc16CalcResult = UartProtUtilCalcCRC16(sendDataBufferPtr+2,14);
    sendDataBufferPtr[16] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[17] = (uint8_t)(crc16CalcResult>>8);
    //申请互斥信号量
    UartProtGetMutexLock();
    //数据发送
    errorSubLocal = UartProtSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    UartProtReleaseMutexLock();
    //释放内存
    UartProtMemFree(sendDataBufferPtr);
    return errorSubLocal;
}

//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB UartProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                                    PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                                    uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    return UartProtSendResultOnly(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,cmdVal,
                                                resultCode,errorMain,errorLevel,errorSub,resultParamBuffer,resultBufferLen);
}
                                            
//SOCKET发送结果ACK包
ERROR_SUB UartProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
                                           uint8_t retryCount,uint32_t cmdVal)
{
    return UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_RESULT,cmdVal,
                                            NULL,0);
}

//SOCKET发送结果ACK包
ERROR_SUB UartProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal)
{
    return UartProtSendDataBase(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_RESULT,cmdVal,NULL,0);
}

//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB UartProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID,uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_DATA_UPLOAD,uploadDataID,
                                            uploadDataBuffer,uploadDataLen);
}

//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB UartProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return UartProtSendDataBase(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_DATA_UPLOAD,uploadDataID,uploadDataBuffer,uploadDataLen);
}

//SOCKET发送数据上传ACK包
ERROR_SUB UartProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID)
{
    return UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_DATA_UPLOAD,uploadDataID,
                                            NULL,0);
}

//SOCKET发送数据上传ACK包
ERROR_SUB UartProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID)
{
    return UartProtSendDataBase(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_DATA_UPLOAD,uploadDataID,NULL,0);
}

//UART SOCKET发送日志
ERROR_SUB UartProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                        uint8_t* logBuffer,uint16_t logLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = UartProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_LOG,
                                                0X00000000,logBuffer,logLength);
    return errorSub;
}

//UART SOCKET发送日志,简易模式
ERROR_SUB UartProtSendLogSample(uint8_t* logBuffer,uint16_t logLength)
{
    return UartProtSendLog(uartProtInitSet.selfModuleNo,uartProtInitSet.selfBoardID,SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,
                                        0,logBuffer,logLength);
}

//初始化接收缓存
void UartProtRecvCacheInit(void)
{
    uartProtRecvCache.lastRecvTimeStamp = 0;
    uartProtRecvCache.recvDataLength = 0;
    uartProtRecvCache.recvBodyLength = 0;
    UserMemSet(uartProtRecvCache.recvDataBuf,0,UART_PROT_SINGLE_PACK_DATA_MAX);
}

//复位接收缓存
void UartProtRecvCacheReset(void)
{
    uartProtRecvCache.lastRecvTimeStamp = 0;
    uartProtRecvCache.recvDataLength = 0;
    uartProtRecvCache.recvBodyLength = 0;
}

//帧切片检测
static void UartProtPackRecvTimeFrameCheck(uint32_t currentTickMs)
{
    if(uartProtRecvCache.recvDataLength == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    if(uartProtRecvCache.lastRecvTimeStamp == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    //当前时间大于上一帧接收时间且大于切片时间
    if(currentTickMs >= uartProtRecvCache.lastRecvTimeStamp)
    {
        if(currentTickMs - uartProtRecvCache.lastRecvTimeStamp > TIME_INVERVAL_MS_UART_PROT_FRAME)
        {
            UartProtRecvCacheReset();
            return;
        }
        return;
    }
    //当前时间小于上一帧时间,时间异常,接收复位
    UartProtRecvCacheReset();
}

//接收数据第一个字节处理
static void UartProtPackRecvByteFirst(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == UART_PROT_FRAME_HEAD_LEADER_A)
    {
        uartProtRecvCache.recvDataBuf[0] = recvByte;
        uartProtRecvCache.recvDataLength = 1;
        //接收起始,设置帧接收时间
        uartProtRecvCache.lastRecvTimeStamp = currentTickMs;
        return;
    }
    else 
    {
        return;
    }
}

//接收数据第二个字节处理
static void UartProtPackRecvByteSecond(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == UART_PROT_FRAME_HEAD_LEADER_B)
    {
        uartProtRecvCache.recvDataBuf[1] = recvByte;
        uartProtRecvCache.recvDataLength = 2;
        return;
    }
    else
    {
        //收到UART_PROT_FRAME_HEAD_LEADER_A之后必然接受UART_PROT_FRAME_HEAD_LEADER_B
        //否则清除
        UartProtRecvCacheReset();
        return;
    }
}

//指令头校验和检测
static UART_PROT_RECV_STA UartProtPackRecvCmdSumCheck(void)
{
    uint16_t cmdCheckSumRecv = 0;
    uint16_t cmdCheckSumCalc = 0;
    //刚好收集到足够的帧头,开始校验和计算匹配
    cmdCheckSumRecv += uartProtRecvCache.recvDataBuf[17];
    cmdCheckSumRecv <<= 8;
    cmdCheckSumRecv += uartProtRecvCache.recvDataBuf[16];
    //计算校验和
    cmdCheckSumCalc = UartProtUtilCalcCRC16(uartProtRecvCache.recvDataBuf+2,14);
    //计算匹配
    if(cmdCheckSumCalc != cmdCheckSumRecv)
    {
        //匹配不上
        UART_PROT_LOG("E:> %s %d, CmdCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,cmdCheckSumRecv, cmdCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        UartProtRecvCacheReset();
        return UART_PROT_RECV_STA_ERROR;
    }
    return UART_PROT_RECV_STA_NONE;
}

//指令数据长度检验
static UART_PROT_RECV_STA UartProtPackRecvDataLengthCheck(void)
{
    //计算出需要接收的数据长度
    uartProtRecvCache.recvBodyLength = 0;
    uartProtRecvCache.recvBodyLength += uartProtRecvCache.recvDataBuf[12];
    uartProtRecvCache.recvBodyLength <<= 8;
    uartProtRecvCache.recvBodyLength += uartProtRecvCache.recvDataBuf[11];
    //检查长度是否正确
    if(uartProtRecvCache.recvBodyLength > UART_PROT_SINGLE_PACK_DATA_BODY_MAX)
    {
        //数据包长度最大为UART_CMD_SINGLE_PACK_LENGTH_MAX
        UART_PROT_LOG("E:>%s,%d DataAreaLength More Then %d ,Real %d\r\n",__func__,__LINE__,UART_PROT_SINGLE_PACK_DATA_BODY_MAX,
                        uartProtRecvCache.recvBodyLength);
        //清理接收状态,当成一包新数据接收
        UartProtRecvCacheReset();
        return UART_PROT_RECV_STA_ERROR;
    }
    return UART_PROT_RECV_STA_NONE;
}

//数据校验和校验
static UART_PROT_RECV_STA UartProtPackRecvDataSumCheck(void)
{
    uint16_t dataCheckSumRecv = 0;
    uint16_t dataCheckSumCalc = 0;
    //先读取接收到的校验和
    dataCheckSumRecv += uartProtRecvCache.recvDataBuf[14];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += uartProtRecvCache.recvDataBuf[13];
    //计算校验和
    dataCheckSumCalc = UartProtUtilCalcCRC16(uartProtRecvCache.recvDataBuf+UART_PROT_FRAME_OFFSET_DATA_BUFFER,
                                                    uartProtRecvCache.recvBodyLength);
    //计算匹配
    if(dataCheckSumRecv != dataCheckSumCalc)
    {
        //匹配不上
        UART_PROT_LOG("E:> %s %d, DataCrc16Check,Recv: 0X%04X, Calc: 0X%04X\r\n",__func__,__LINE__,dataCheckSumRecv, dataCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        UartProtRecvCacheReset();
        return UART_PROT_RECV_STA_ERROR;
    }
    //数据匹配上了,一包数据接收完成
    return UART_PROT_RECV_STA_COMPLETE;
}

//串口接收一个字节数据的处理
UART_PROT_RECV_STA UartProtPackRecvByteProc(uint8_t recvByte,uint32_t currentTickMs)
{
    UART_PROT_RECV_STA uartRecvState = UART_PROT_RECV_STA_NONE;
    //帧超时时间检测,主要是为了防止TCPIP的数据切片导致两帧时间间隔过长
    UartProtPackRecvTimeFrameCheck(currentTickMs);
    //接受数据长度为0,当前数据必须是UART_PROT_FRAME_HEAD_LEADER_A才能启动接收
    if(uartProtRecvCache.recvDataLength == 0)
    {
        UartProtPackRecvByteFirst(recvByte,currentTickMs);
        return UART_PROT_RECV_STA_NONE;
    }
    //接收长度为1,当前数据必须是UART_CMD_FRAME_HEAD_LEADER_B才能启动接收
    else if(uartProtRecvCache.recvDataLength == 1)
    {
        UartProtPackRecvByteSecond(recvByte,currentTickMs);
        return UART_PROT_RECV_STA_NONE;
    }
    if(uartProtRecvCache.recvDataLength < UART_PROT_FRAME_LENGTH_HEADER)
    {
        //还没接收到完整的指令头
        uartProtRecvCache.recvDataBuf[uartProtRecvCache.recvDataLength] = recvByte;
        uartProtRecvCache.recvDataLength++;
        //接收到完整指令头
        if(uartProtRecvCache.recvDataLength == UART_PROT_FRAME_LENGTH_HEADER)
        {
            //指令头校验
            uartRecvState = UartProtPackRecvCmdSumCheck();
            if(uartRecvState == UART_PROT_RECV_STA_ERROR)
            {
                return uartRecvState;
            }
            //数据长度校验
            uartRecvState = UartProtPackRecvDataLengthCheck();
            if(uartRecvState == UART_PROT_RECV_STA_ERROR)
            {
                return uartRecvState;
            }
            //数据长度达标,空包,不用校验
            if(uartProtRecvCache.recvBodyLength == 0)
            {
                //空包数据接收完成
                return UART_PROT_RECV_STA_COMPLETE;
            }
            return uartRecvState;
        }
        return uartRecvState;
    }
    else
    {
        //数据大于帧头,数据继续接收
        uartProtRecvCache.recvDataBuf[uartProtRecvCache.recvDataLength] = recvByte;
        uartProtRecvCache.recvDataLength++;
        //检测接收到的数据长度是否等于想要接收的数据长度
        if(uartProtRecvCache.recvBodyLength == uartProtRecvCache.recvDataLength - UART_PROT_FRAME_OFFSET_DATA_BUFFER)
        {
            //数据包校验
            uartRecvState = UartProtPackRecvDataSumCheck();
        }
        return uartRecvState;
    }
}

//设置UART_PROT_PACKAGE为默认值
static void UartProtPackageReset(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    uartProtPackagePtr->srcModuleNo = 0;
    uartProtPackagePtr->srcBoardID = 0;
    uartProtPackagePtr->targetModule = 0;
    uartProtPackagePtr->targetBoardID = 0;
    uartProtPackagePtr->packType = PROT_PACK_TYPE_RESERVE;
    uartProtPackagePtr->idCode = 0;
    uartProtPackagePtr->retryCount = 0;
    uartProtPackagePtr->dataBodyLength = 0;
    uartProtPackagePtr->dataBodyBufferPtr = NULL;
}


//通过接收缓存创建接收信息
UART_PROT_PACKAGE* UartProtPackRecvCreatePackMsg(void)
{   
    //创建消息体
    UART_PROT_PACKAGE* uartSocketPackagePtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_PACKAGE));
    if(uartSocketPackagePtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, UartProtMemMalloc Failed\r\n",__func__,__LINE__);
        return NULL;
    }
    //设置默认值
    UartProtPackageReset(uartSocketPackagePtr);
    //申请数据缓存
    if(uartProtRecvCache.recvBodyLength != 0)
    {
        uartSocketPackagePtr->dataBodyBufferPtr = UartProtMemMalloc(uartProtRecvCache.recvBodyLength);
        if(uartSocketPackagePtr->dataBodyBufferPtr == NULL)
        {
            UART_PROT_LOG("E:> %s %d, UartProtMemMalloc Buffer Failed,Length: %d\r\n",__func__,__LINE__,
                                        uartProtRecvCache.recvBodyLength);
            //内存申请失败
            UartProtMemFree(uartSocketPackagePtr);
            return NULL;
        }
        //拷贝数据
        UserMemCopy(uartSocketPackagePtr->dataBodyBufferPtr,uartProtRecvCache.recvDataBuf+18,uartProtRecvCache.recvBodyLength);
        uartSocketPackagePtr->dataBodyLength = uartProtRecvCache.recvBodyLength;
    }
    else
    {
        uartSocketPackagePtr->dataBodyBufferPtr = NULL;
        uartSocketPackagePtr->dataBodyLength = 0;
    }
    //开始解析数据
    uartSocketPackagePtr->srcModuleNo = uartProtRecvCache.recvDataBuf[2];
    uartSocketPackagePtr->targetModule = uartProtRecvCache.recvDataBuf[3];
    uartSocketPackagePtr->srcBoardID = uartProtRecvCache.recvDataBuf[4];
    uartSocketPackagePtr->targetBoardID = uartProtRecvCache.recvDataBuf[5];
    uartSocketPackagePtr->packType = (PROT_PACK_TYPE)uartProtRecvCache.recvDataBuf[6];
    uartSocketPackagePtr->idCode += uartProtRecvCache.recvDataBuf[10];
    uartSocketPackagePtr->idCode <<= 8;
    uartSocketPackagePtr->idCode += uartProtRecvCache.recvDataBuf[9];
    uartSocketPackagePtr->idCode <<= 8;
    uartSocketPackagePtr->idCode += uartProtRecvCache.recvDataBuf[8];
    uartSocketPackagePtr->idCode <<= 8;
    uartSocketPackagePtr->idCode += uartProtRecvCache.recvDataBuf[7];
    uartSocketPackagePtr->dataBodyLength = uartProtRecvCache.recvBodyLength;
    uartSocketPackagePtr->retryCount = uartProtRecvCache.recvDataBuf[15];
    return uartSocketPackagePtr;
}

//删除上传等待包的节点
static void UartProtRunPeriodSubUploadWaitAckDeleteNode(ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr,
                                                                    UART_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(uartProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        UART_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
    {
        UartProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
    }
    UartProtMemFree(uploadWaitAckPackPtr);
}

//上传数据包缓存处理
void UartProtRunPeriodSubUploadWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t elementCount;
    UART_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(uartProtInitSet.listSendUploadWaitAckCachePtr);
    if(elementCount == 0)
    {
        return;
    }
    //从链表头部获取一个元素
    listNodePtr = ADT_CircularPtrListPeekTop(uartProtInitSet.listSendUploadWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        return;
    }
    //找到元素了,获取数据指针
    uploadWaitAckPackPtr = (UART_PROT_UPLOAD_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(uploadWaitAckPackPtr->retryCount == 0)
    {
        //第一次发送
        errorSub = UartProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                                            uploadWaitAckPackPtr->retryCount,uploadWaitAckPackPtr->uploadID,
                                                            uploadWaitAckPackPtr->uploadDataBufferPtr,uploadWaitAckPackPtr->uploadDataBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            uploadWaitAckPackPtr->retryCount += 1;
            //发送成功,时间更新
            uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        }
        return;
    }
    //检查节点是否超时,没有超时,继续等待,超时,进入重发过程
    if(currentTickMs >= uploadWaitAckPackPtr->lastSendTimeTickMs)
    {
        if(currentTickMs - uploadWaitAckPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_UART_PROT)
        {
            //还没到ACK超时时间,退出
            return;
        }
        //到了超时时间,要进行重发
    }
    else
    {
        //当前时间比上次发送时间还小,绝对不对劲,删除这一个节点
        UartProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
        return;
    }
    //检测重发次数
    if(uploadWaitAckPackPtr->retryCount >= 3)
    {
        //日志发送失败
        UART_PROT_LOG("E>: %s,%d,UartProtSendUploadFailed.RetryMax.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X\r\n",
                                    __func__,__LINE__,uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                    uploadWaitAckPackPtr->uploadID);
        //重发超过限制,删除节点
        UartProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
        return;
    }
    //需要重发
    uploadWaitAckPackPtr->retryCount += 1;
    //重发
    errorSub = UartProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                                        uploadWaitAckPackPtr->retryCount,uploadWaitAckPackPtr->uploadID,
                                                        uploadWaitAckPackPtr->uploadDataBufferPtr,uploadWaitAckPackPtr->uploadDataBufferLength);
    //发送完成,时间更新
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    if(errorSub != ERROR_SUB_OK)
    {
        //发送失败了,错误打印
        UART_PROT_LOG("E>: %s,%d,UartProtSendUploadOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //日志发送失败
        UART_PROT_LOG("E>: UartProtSendUploadFailed.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                                    uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                    uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->retryCount);
    }
    return;
}

//删除上传等待包的节点
static void UartProtRunPeriodSubResultWaitAckDeleteNode(ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr,
                                                                    UART_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(uartProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        UART_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
    {
        UartProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
    }
    UartProtMemFree(resultWaitAckPackPtr);
}

//结果数据包处理
void UartProtRunPeriodSubResultWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t elementCount;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    UART_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(uartProtInitSet.listSendResultWaitAckCachePtr);
    if(elementCount == 0)
    {
        return;
    }
    //从链表头部获取一个元素
    listNodePtr = ADT_CircularPtrListPeekTop(uartProtInitSet.listSendResultWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        return;
    }
    //找到元素了,获取数据指针
    resultWaitAckPackPtr = (UART_PROT_RESULT_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(resultWaitAckPackPtr->retryCount == 0)
    {//数据还没发送过
        //第一次发送
        errorSub = UartProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->retryCount,
                                                            resultWaitAckPackPtr->cmdValue,resultWaitAckPackPtr->resultCode,resultWaitAckPackPtr->errorMainCode,
                                                            resultWaitAckPackPtr->errorLevelCode,resultWaitAckPackPtr->errorSubCode,resultWaitAckPackPtr->resultParamBufferPtr,
                                                            resultWaitAckPackPtr->resultParamBufferLength);
        if(errorSub == ERROR_SUB_OK)
        {
            resultWaitAckPackPtr->retryCount += 1;
            //发送成功,时间更新
            resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        }
        return;
    }
    //检查节点是否超时,没有超时,继续等待,超时,进入重发过程
    if(currentTickMs >= resultWaitAckPackPtr->lastSendTimeTickMs)
    {
        if(currentTickMs - resultWaitAckPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_UART_PROT)
        {
            //还没到ACK超时时间,退出
            return;
        }
        //到了超时时间,要进行重发
    }
    else
    {
        //当前时间比上次发送时间还小,绝对不对劲,删除这一个节点
        UartProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
        return;
    }
    //检测重发次数
    if(resultWaitAckPackPtr->retryCount >= 3)
    {
        //日志发送失败
        UART_PROT_LOG("E>: %s,%d,UartProtSendResultFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                    __func__,__LINE__,resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,
                                    resultWaitAckPackPtr->cmdValue);
        //重发超过限制,删除节点
        UartProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
        return;
    }
    resultWaitAckPackPtr->retryCount += 1;
    //重发
    errorSub = UartProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->retryCount,
                                                        resultWaitAckPackPtr->cmdValue,resultWaitAckPackPtr->resultCode,resultWaitAckPackPtr->errorMainCode,
                                                        resultWaitAckPackPtr->errorLevelCode,resultWaitAckPackPtr->errorSubCode,resultWaitAckPackPtr->resultParamBufferPtr,
                                                        resultWaitAckPackPtr->resultParamBufferLength);
    //发送完成,时间更新
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        UART_PROT_LOG("E>: %s,%d,UartProtSendResultOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //日志发送失败
        UART_PROT_LOG("E>: UartProtSendResultFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,RetryCount: %d\r\n",
                                    resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->cmdValue,
                                    resultWaitAckPackPtr->retryCount);
    }
    return;
}

//查找链表中等待ACK的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCmdWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    UART_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (UART_PROT_CMD_WAIT_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //首先检查状态是不是等待ACK
    if(cmdWaitPackPtr->cmdStage != PROT_CMD_STA_WAIT_ACK)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(currentTick >= cmdWaitPackPtr->lastSendTimeTickMs)
    {
        if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_UART_PROT)
        {
            return ADT_CMP_EQUAL;
        }
        return ADT_CMP_NOT_EQUAL;
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//周期性的检查指令重发
void UartProtRunPeriodSubCmdWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    UART_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(uartProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCmdWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (UART_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //该元素是没发送过的元素
        if(cmdWaitPackPtr->retryCount == 0)
        {
            //网络已连接
            errorSub = UartProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->retryCount,
                                                        cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub != ERROR_SUB_OK)
            {
                //发送成功,时间更新
                UART_PROT_LOG("E>: %s,%d,UartProtSendCmdOnlySample Failed.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%08X\r\n",
                                __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                cmdWaitPackPtr->cmdValue);
                //状态设置为重发失败
                cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
                //结束本次循环
                continue;
            }
            //更新发送时间
            cmdWaitPackPtr->retryCount += 1;
            cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            //结束本次循环
            continue;
        }
        //检测重发次数
        if(cmdWaitPackPtr->retryCount >= 3)
        {
            //状态设置为ACK超时
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_TIME_OUT;
            //日志发送失败
            UART_PROT_LOG("E>: %s,%d,UartProtSendCmdFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                        cmdWaitPackPtr->cmdValue);
            //结束本次循环
            continue;
        }
        else
        {
            cmdWaitPackPtr->retryCount += 1;
            //重发
            errorSub = UartProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->retryCount,
                                                            cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,错误打印
            UART_PROT_LOG("E>: %s,%d,UartProtSendCmdOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            UART_PROT_LOG("E>: UartProtSendCmdFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,RetryCount: %d\r\n",
                                        cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->cmdValue,
                                        cmdWaitPackPtr->retryCount);
            //状态设置为重发失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
            //结束本次循环
            continue;
        }
    }
}

//查找链表中等待结果的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCmdWaitResultListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    UART_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (UART_PROT_CMD_WAIT_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //首先检查状态是不是等待ACK
    if(cmdWaitPackPtr->cmdStage != PROT_CMD_STA_WAIT_RESULT)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(currentTick >= cmdWaitPackPtr->lastSendTimeTickMs)
    {
        if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > cmdWaitPackPtr->resultTimeOutMs)
        {
            return ADT_CMP_EQUAL;
        }
        return ADT_CMP_NOT_EQUAL;
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//周期性的检查指令超时
void UartProtRunPeriodSubCmdWaitResultPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    UART_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(uartProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCmdWaitResultListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (UART_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //超时的元素状态设置为超时
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_TIME_OUT;
        //日志超时
        UART_PROT_LOG("E>: %s,%d,UartProtSendCmdFailed.ResultTimeOut.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X,TimeOutSet\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                    cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->resultTimeOutMs);
        //结束本次循环
        continue;
    }
}




