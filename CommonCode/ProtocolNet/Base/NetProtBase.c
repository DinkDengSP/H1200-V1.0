/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-02 14:05:11 +0800
************************************************************************************************/ 
#include "NetProtBase.h"

//NET SOCKET初始化标记
extern NET_PROT_INIT_SET netProtInitSet;
//网口接收缓存
extern NET_PROT_RECV_CACHE netProtRecvCache;

//获取数据访问权
void NetProtGetDataAccessLock(void)
{
    tx_mutex_get(&(netProtInitSet.mutexNetProtDataAccess),TX_WAIT_FOREVER);
}
//释放数据访问权
void NetProtGetDataAccessRelease(void)
{
    tx_mutex_put(&(netProtInitSet.mutexNetProtDataAccess));
}

//基础发送函数
ERROR_SUB NetProtSendDataBase(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            PROT_PACK_TYPE packType,uint32_t idCode,uint8_t* paramBuffer,uint16_t paramLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t* sendDataBufferPtr = NULL;
    uint16_t sendBufferLength = 18+paramLength;
    uint16_t crc16CalcResult = 0;
    //内存申请
    sendDataBufferPtr = NetProtMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = NET_PROT_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = NET_PROT_FRAME_HEAD_LEADER_B;
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
    crc16CalcResult = NetProtUtilCalcCRC16(paramBuffer,paramLength);
    sendDataBufferPtr[13] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[14] = (uint8_t)(crc16CalcResult>>8);
    //重发标记
    sendDataBufferPtr[15] = retryCount;
    //帧头校验码
    crc16CalcResult = NetProtUtilCalcCRC16(sendDataBufferPtr+2,14);
    sendDataBufferPtr[16] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[17] = (uint8_t)(crc16CalcResult>>8);
    //拷贝参数
    if(sendDataBufferPtr != NULL)
    {
        UserMemCopy(sendDataBufferPtr+18,paramBuffer,paramLength);
    }
    //申请互斥信号量
    NetProtGetMutexLock();
    //数据发送
    errorSub = NetProtSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    NetProtReleaseMutexLock();
    //释放内存
    NetProtMemFree(sendDataBufferPtr);
    return errorSub;
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB NetProtSendCmdOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t cmdVal,uint8_t* paramBuffer,uint16_t paramLength)
{
    return NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_CMD,cmdVal,
                                            paramBuffer,paramLength);
}

//SOCKET发送指令包,仅发送不做数据存储
ERROR_SUB NetProtSendCmdOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,uint8_t* paramBuffer,
                                                uint16_t paramLength)
{
    return NetProtSendDataBase(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_CMD,cmdVal,paramBuffer,paramLength);
}

//SOCKET发送指令ACK包
ERROR_SUB NetProtSendAckCmd(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                            PROT_ACK ackCmd)
{
    return NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_CMD,cmdVal,
                                            (uint8_t*)&ackCmd,1);
}

//SOCKET发送指令ACK包
ERROR_SUB NetProtSendAckCmdSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    return NetProtSendDataBase(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_CMD,cmdVal,(uint8_t*)&ackCmd,1);
}

//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB NetProtSendResultOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
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
    sendDataBufferPtr = NetProtMemMallocWhileSuccess(sendBufferLength);
    //内存参数设置
    sendDataBufferPtr[0] = NET_PROT_FRAME_HEAD_LEADER_A;
    sendDataBufferPtr[1] = NET_PROT_FRAME_HEAD_LEADER_B;
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
    uint32_t boardIdCode = NetProtGetSelfBoardID();
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
    crc16CalcResult = NetProtUtilCalcCRC16(sendDataBufferPtr+18,resultBufferLen+8);
    sendDataBufferPtr[13] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[14] = (uint8_t)(crc16CalcResult>>8);
    //帧头校验码
    crc16CalcResult = NetProtUtilCalcCRC16(sendDataBufferPtr+2,14);
    sendDataBufferPtr[16] = (uint8_t)(crc16CalcResult>>0);
    sendDataBufferPtr[17] = (uint8_t)(crc16CalcResult>>8);
    //申请互斥信号量
    NetProtGetMutexLock();
    //数据发送
    errorSubLocal = NetProtSendFrame(sendBufferLength,sendDataBufferPtr);
    //释放互斥信号量
    NetProtReleaseMutexLock();
    //释放内存
    NetProtMemFree(sendDataBufferPtr);
    return errorSubLocal;
}

//SOCKET发送结果包,仅发送不做数据存储
ERROR_SUB NetProtSendResultOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal,
                                                    PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                                    uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    //NET_PROT_LOG("NetProtSendResultOnlySample,Cmd: 0X%08X,TimeTick: %d\r\n",cmdVal,tx_time_get());
    return NetProtSendResultOnly(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,cmdVal,
                                                resultCode,errorMain,errorLevel,errorSub,resultParamBuffer,resultBufferLen);
}
                                            
//SOCKET发送结果ACK包
ERROR_SUB NetProtSendAckResult(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,
                                           uint8_t retryCount,uint32_t cmdVal)
{
    return NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_RESULT,cmdVal,
                                            NULL,0);
}

//SOCKET发送结果ACK包
ERROR_SUB NetProtSendAckResultSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t cmdVal)
{
    return NetProtSendDataBase(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_RESULT,cmdVal,NULL,0);
}

//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB NetProtSendUploadOnly(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID,uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_DATA_UPLOAD,uploadDataID,
                                            uploadDataBuffer,uploadDataLen);
}

//SOCKET发送数据上传包,仅发送不做数据存储
ERROR_SUB NetProtSendUploadOnlySample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID,
                                                    uint8_t* uploadDataBuffer,uint16_t uploadDataLen)
{
    return NetProtSendDataBase(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_DATA_UPLOAD,uploadDataID,uploadDataBuffer,uploadDataLen);
}

//SOCKET发送数据上传ACK包
ERROR_SUB NetProtSendAckUpload(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                            uint32_t uploadDataID)
{
    return NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_ACK_DATA_UPLOAD,uploadDataID,
                                            NULL,0);
}

//SOCKET发送数据上传ACK包
ERROR_SUB NetProtSendAckUploadSample(uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,uint32_t uploadDataID)
{
    return NetProtSendDataBase(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,targetModule,targetBoardID,retryCount,
                                            PROT_PACK_TYPE_ACK_DATA_UPLOAD,uploadDataID,NULL,0);
}

//NET SOCKET发送日志
ERROR_SUB NetProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t retryCount,
                                        uint8_t* logBuffer,uint16_t logLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = NetProtSendDataBase(srcModule,srcBoardID,targetModule,targetBoardID,retryCount,PROT_PACK_TYPE_LOG,
                                                0X00000000,logBuffer,logLength);
    return errorSub;
}

//NET SOCKET发送日志,简易模式
ERROR_SUB NetProtSendLogSample(uint8_t* logBuffer,uint16_t logLength)
{
    return NetProtSendLog(netProtInitSet.selfModuleNo,netProtInitSet.selfBoardID,SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,
                                        0,logBuffer,logLength);
}

//初始化接收缓存
void NetProtRecvCacheInit(void)
{
    netProtRecvCache.lastRecvTimeStamp = 0;
    netProtRecvCache.recvDataLength = 0;
    netProtRecvCache.recvBodyLength = 0;
    UserMemSet(netProtRecvCache.recvDataBuf,0,NET_PROT_SINGLE_PACK_DATA_MAX);
}

//复位接收缓存
void NetProtRecvCacheReset(void)
{
    netProtRecvCache.lastRecvTimeStamp = 0;
    netProtRecvCache.recvDataLength = 0;
    netProtRecvCache.recvBodyLength = 0;
}

//帧切片检测
static void NetProtPackRecvTimeFrameCheck(uint32_t currentTickMs)
{
    if(netProtRecvCache.recvDataLength == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    if(netProtRecvCache.lastRecvTimeStamp == 0)
    {
        //没有数据接收,不需要检测
        return;
    }
    //当前时间大于上一帧接收时间且大于切片时间
    if(currentTickMs >= netProtRecvCache.lastRecvTimeStamp)
    {
        if(currentTickMs - netProtRecvCache.lastRecvTimeStamp > TIME_INVERVAL_MS_NET_PROT_FRAME)
        {
            NetProtRecvCacheReset();
            return;
        }
        return;
    }
    //当前时间小于上一帧时间,时间异常,接收复位
    NetProtRecvCacheReset();
}

//接收数据第一个字节处理
static void NetProtPackRecvByteFirst(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == NET_PROT_FRAME_HEAD_LEADER_A)
    {
        netProtRecvCache.recvDataBuf[0] = recvByte;
        netProtRecvCache.recvDataLength = 1;
        //接收起始,设置帧接收时间
        netProtRecvCache.lastRecvTimeStamp = currentTickMs;
        return;
    }
    else 
    {
        return;
    }
}

//接收数据第二个字节处理
static void NetProtPackRecvByteSecond(uint8_t recvByte,uint32_t currentTickMs)
{
    if(recvByte == NET_PROT_FRAME_HEAD_LEADER_B)
    {
        netProtRecvCache.recvDataBuf[1] = recvByte;
        netProtRecvCache.recvDataLength = 2;
        return;
    }
    else
    {
        //收到NET_PROT_FRAME_HEAD_LEADER_A之后必然接受NET_PROT_FRAME_HEAD_LEADER_B
        //否则清除
        NetProtRecvCacheReset();
        return;
    }
}

//指令头校验和检测
static NET_PROT_RECV_STA NetProtPackRecvCmdSumCheck(void)
{
    uint16_t cmdCheckSumRecv = 0;
    uint16_t cmdCheckSumCalc = 0;
    //刚好收集到足够的帧头,开始校验和计算匹配
    cmdCheckSumRecv += netProtRecvCache.recvDataBuf[17];
    cmdCheckSumRecv <<= 8;
    cmdCheckSumRecv += netProtRecvCache.recvDataBuf[16];
    //计算校验和
    cmdCheckSumCalc = NetProtUtilCalcCRC16(netProtRecvCache.recvDataBuf+2,14);
    //计算匹配
    if(cmdCheckSumCalc != cmdCheckSumRecv)
    {
        //匹配不上
        NET_PROT_LOG("E:> %s %d, CmdCrc16Check,Recv: 0X%08X, Calc: 0X%08X\r\n",__func__,__LINE__,cmdCheckSumRecv, cmdCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        NetProtRecvCacheReset();
        return NET_PROT_RECV_STA_ERROR;
    }
    return NET_PROT_RECV_STA_NONE;
}

//指令数据长度检验
static NET_PROT_RECV_STA NetProtPackRecvDataLengthCheck(void)
{
    //计算出需要接收的数据长度
    netProtRecvCache.recvBodyLength = 0;
    netProtRecvCache.recvBodyLength += netProtRecvCache.recvDataBuf[12];
    netProtRecvCache.recvBodyLength <<= 8;
    netProtRecvCache.recvBodyLength += netProtRecvCache.recvDataBuf[11];
    //检查长度是否正确
    if(netProtRecvCache.recvBodyLength > NET_PROT_SINGLE_PACK_DATA_BODY_MAX)
    {
        //数据包长度最大为NET_CMD_SINGLE_PACK_LENGTH_MAX
        NET_PROT_LOG("E:>%s,%d DataAreaLength More Then %d ,Real %d\r\n",__func__,__LINE__,NET_PROT_SINGLE_PACK_DATA_BODY_MAX,
                        netProtRecvCache.recvBodyLength);
        //清理接收状态,当成一包新数据接收
        NetProtRecvCacheReset();
        return NET_PROT_RECV_STA_ERROR;
    }
    return NET_PROT_RECV_STA_NONE;
}

//数据校验和校验
static NET_PROT_RECV_STA NetProtPackRecvDataSumCheck(void)
{
    uint16_t dataCheckSumRecv = 0;
    uint16_t dataCheckSumCalc = 0;
    //先读取接收到的校验和
    dataCheckSumRecv += netProtRecvCache.recvDataBuf[14];
    dataCheckSumRecv <<= 8;
    dataCheckSumRecv += netProtRecvCache.recvDataBuf[13];
    //计算校验和
    dataCheckSumCalc = NetProtUtilCalcCRC16(netProtRecvCache.recvDataBuf+NET_PROT_FRAME_OFFSET_DATA_BUFFER,
                                                    netProtRecvCache.recvBodyLength);
    //计算匹配
    if(dataCheckSumRecv != dataCheckSumCalc)
    {
        //匹配不上
        NET_PROT_LOG("E:> %s %d, DataCrc16Check,Recv: 0X%04X, Calc: 0X%04X\r\n",__func__,__LINE__,dataCheckSumRecv, dataCheckSumCalc);
        //清理接收状态,当成一包新数据接收
        NetProtRecvCacheReset();
        return NET_PROT_RECV_STA_ERROR;
    }
    //数据匹配上了,一包数据接收完成
    return NET_PROT_RECV_STA_COMPLETE;
}

//网口接收一个字节数据的处理
NET_PROT_RECV_STA NetProtPackRecvByteProc(uint8_t recvByte,uint32_t currentTickMs)
{
    NET_PROT_RECV_STA netRecvState = NET_PROT_RECV_STA_NONE;
    //帧超时时间检测,主要是为了防止TCPIP的数据切片导致两帧时间间隔过长
    NetProtPackRecvTimeFrameCheck(currentTickMs);
    //接受数据长度为0,当前数据必须是NET_PROT_FRAME_HEAD_LEADER_A才能启动接收
    if(netProtRecvCache.recvDataLength == 0)
    {
        NetProtPackRecvByteFirst(recvByte,currentTickMs);
        return NET_PROT_RECV_STA_NONE;
    }
    //接收长度为1,当前数据必须是NET_CMD_FRAME_HEAD_LEADER_B才能启动接收
    else if(netProtRecvCache.recvDataLength == 1)
    {
        NetProtPackRecvByteSecond(recvByte,currentTickMs);
        return NET_PROT_RECV_STA_NONE;
    }
    if(netProtRecvCache.recvDataLength < NET_PROT_FRAME_LENGTH_HEADER)
    {
        //还没接收到完整的指令头
        netProtRecvCache.recvDataBuf[netProtRecvCache.recvDataLength] = recvByte;
        netProtRecvCache.recvDataLength++;
        //接收到完整指令头
        if(netProtRecvCache.recvDataLength == NET_PROT_FRAME_LENGTH_HEADER)
        {
            //指令头校验
            netRecvState = NetProtPackRecvCmdSumCheck();
            if(netRecvState == NET_PROT_RECV_STA_ERROR)
            {
                return netRecvState;
            }
            //数据长度校验
            netRecvState = NetProtPackRecvDataLengthCheck();
            if(netRecvState == NET_PROT_RECV_STA_ERROR)
            {
                return netRecvState;
            }
            //数据长度达标,空包,不用校验
            if(netProtRecvCache.recvBodyLength == 0)
            {
                //空包数据接收完成
                return NET_PROT_RECV_STA_COMPLETE;
            }
            return netRecvState;
        }
        return netRecvState;
    }
    else
    {
        //数据大于帧头,数据继续接收
        netProtRecvCache.recvDataBuf[netProtRecvCache.recvDataLength] = recvByte;
        netProtRecvCache.recvDataLength++;
        //检测接收到的数据长度是否等于想要接收的数据长度
        if(netProtRecvCache.recvBodyLength == netProtRecvCache.recvDataLength - NET_PROT_FRAME_OFFSET_DATA_BUFFER)
        {
            //数据包校验
            netRecvState = NetProtPackRecvDataSumCheck();
        }
        return netRecvState;
    }
}

//设置NET_PROT_PACKAGE为默认值
static void NetProtPackageReset(NET_PROT_PACKAGE* netProtPackagePtr)
{
    netProtPackagePtr->srcModuleNo = 0;
    netProtPackagePtr->srcBoardID = 0;
    netProtPackagePtr->targetModule = 0;
    netProtPackagePtr->targetBoardID = 0;
    netProtPackagePtr->packType = PROT_PACK_TYPE_RESERVE;
    netProtPackagePtr->idCode = 0;
    netProtPackagePtr->retryCount = 0;
    netProtPackagePtr->dataBodyLength = 0;
    netProtPackagePtr->dataBodyBufferPtr = NULL;
}


//通过接收缓存创建接收信息
NET_PROT_PACKAGE* NetProtPackRecvCreatePackMsg(void)
{   
    //创建消息体
    NET_PROT_PACKAGE* netSocketPackagePtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_PACKAGE));
    if(netSocketPackagePtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, NetProtMemMalloc Failed\r\n",__func__,__LINE__);
        return NULL;
    }
    //设置默认值
    NetProtPackageReset(netSocketPackagePtr);
    //申请数据缓存
    if(netProtRecvCache.recvBodyLength != 0)
    {
        netSocketPackagePtr->dataBodyBufferPtr = NetProtMemMalloc(netProtRecvCache.recvBodyLength);
        if(netSocketPackagePtr->dataBodyBufferPtr == NULL)
        {
            NET_PROT_LOG("E:> %s %d, NetProtMemMalloc Buffer Failed,Length: %d\r\n",__func__,__LINE__,
                                        netProtRecvCache.recvBodyLength);
            //内存申请失败
            NetProtMemFree(netSocketPackagePtr);
            return NULL;
        }
        //拷贝数据
        UserMemCopy(netSocketPackagePtr->dataBodyBufferPtr,netProtRecvCache.recvDataBuf+18,netProtRecvCache.recvBodyLength);
        netSocketPackagePtr->dataBodyLength = netProtRecvCache.recvBodyLength;
    }
    else
    {
        netSocketPackagePtr->dataBodyBufferPtr = NULL;
        netSocketPackagePtr->dataBodyLength = 0;
    }
    //开始解析数据
    netSocketPackagePtr->srcModuleNo = netProtRecvCache.recvDataBuf[2];
    netSocketPackagePtr->targetModule = netProtRecvCache.recvDataBuf[3];
    netSocketPackagePtr->srcBoardID = netProtRecvCache.recvDataBuf[4];
    netSocketPackagePtr->targetBoardID = netProtRecvCache.recvDataBuf[5];
    netSocketPackagePtr->packType = (PROT_PACK_TYPE)netProtRecvCache.recvDataBuf[6];
    netSocketPackagePtr->idCode += netProtRecvCache.recvDataBuf[10];
    netSocketPackagePtr->idCode <<= 8;
    netSocketPackagePtr->idCode += netProtRecvCache.recvDataBuf[9];
    netSocketPackagePtr->idCode <<= 8;
    netSocketPackagePtr->idCode += netProtRecvCache.recvDataBuf[8];
    netSocketPackagePtr->idCode <<= 8;
    netSocketPackagePtr->idCode += netProtRecvCache.recvDataBuf[7];
    netSocketPackagePtr->dataBodyLength = netProtRecvCache.recvBodyLength;
    netSocketPackagePtr->retryCount = netProtRecvCache.recvDataBuf[15];
    return netSocketPackagePtr;
}

//删除上传等待包的节点
static void NetProtRunPeriodSubUploadWaitAckDeleteNode(ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr,
                                                                    NET_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
    {
        NetProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
    }
    NetProtMemFree(uploadWaitAckPackPtr);
}

//上传数据包缓存处理
void NetProtRunPeriodSubUploadWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t elementCount;
    NET_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(netProtInitSet.listSendUploadWaitAckCachePtr);
    if(elementCount == 0)
    {
        return;
    }
    //从链表头部获取一个元素
    listNodePtr = ADT_CircularPtrListPeekTop(netProtInitSet.listSendUploadWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        return;
    }
    //找到元素了,获取数据指针
    uploadWaitAckPackPtr = (NET_PROT_UPLOAD_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(uploadWaitAckPackPtr->retryCount == 0)
    {
        if(RESET == NetProtGetConnectFlag())
        {
            if(uploadWaitAckPackPtr->lastSendTimeTickMs == 0)
            {
                //网络没连接,时间更新
                uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                return;
            }
            else
            {
                //时间更新了
                if(currentTickMs - uploadWaitAckPackPtr->lastSendTimeTickMs >= TIME_OUT_MS_MAX_WAIT_NET_PROT_CONNECT)
                {
                    //到达了最大的等待SOCKET连接时间,删除这个节点
                    NetProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
                }
                return;
            }
        }
        //第一次发送
        errorSub = NetProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
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
        if(currentTickMs - uploadWaitAckPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_NET_PROT)
        {
            //还没到ACK超时时间,退出
            return;
        }
        //到了超时时间,要进行重发,检测SOCKET状态是否连接,已连接就能重发,未连接继续等待
        if(RESET == NetProtGetConnectFlag())
        {
            //网络未连接,检测重发次数,如果已经重发两次,那么现在就可以删除了
            if(uploadWaitAckPackPtr->retryCount >= 3)
            {
                //日志发送失败
                NET_PROT_LOG("E>: %s,%d,NetProtSendUploadFailed.RetryMax.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X\r\n",
                                            __func__,__LINE__,uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                            uploadWaitAckPackPtr->uploadID);
                //重发超过限制,删除节点
                NetProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
                return;
            }
            //网络未连接,检测超时时间是否大于系统设置的上限
            if(currentTickMs - uploadWaitAckPackPtr->lastSendTimeTickMs >= TIME_OUT_MS_MAX_WAIT_NET_PROT_CONNECT)
            {
                //到达了最大的等待SOCKET连接时间,删除这个节点
                NetProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
            }
            return;
        }
    }
    else
    {
        //当前时间比上次发送时间还小,绝对不对劲,删除这一个节点
        NetProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
        return;
    }
    //检测重发次数
    if(uploadWaitAckPackPtr->retryCount >= 3)
    {
        //日志发送失败
        NET_PROT_LOG("E>: %s,%d,NetProtSendUploadFailed.RetryMax.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X\r\n",
                                    __func__,__LINE__,uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                    uploadWaitAckPackPtr->uploadID);
        //重发超过限制,删除节点
        NetProtRunPeriodSubUploadWaitAckDeleteNode(listNodePtr,uploadWaitAckPackPtr);
        return;
    }
    //需要重发
    uploadWaitAckPackPtr->retryCount += 1;
    //重发
    errorSub = NetProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                                        uploadWaitAckPackPtr->retryCount,uploadWaitAckPackPtr->uploadID,
                                                        uploadWaitAckPackPtr->uploadDataBufferPtr,uploadWaitAckPackPtr->uploadDataBufferLength);
    //发送完成,时间更新
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    if(errorSub != ERROR_SUB_OK)
    {
        //发送失败了,错误打印
        NET_PROT_LOG("E>: %s,%d,NetProtSendUploadOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //日志发送失败
        NET_PROT_LOG("E>: NetProtSendUploadFailed.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                                    uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                    uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->retryCount);
    }
    return;
}

//删除上传等待包的节点
static void NetProtRunPeriodSubResultWaitAckDeleteNode(ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr,
                                                                    NET_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
    {
        NetProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
    }
    NetProtMemFree(resultWaitAckPackPtr);
}

//结果数据包处理
void NetProtRunPeriodSubResultWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t elementCount;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    NET_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    //首先查询是否存在元素
    elementCount = ADT_CircularPtrListGetNodeCount(netProtInitSet.listSendResultWaitAckCachePtr);
    if(elementCount == 0)
    {
        return;
    }
    //从链表头部获取一个元素
    listNodePtr = ADT_CircularPtrListPeekTop(netProtInitSet.listSendResultWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        return;
    }
    //找到元素了,获取数据指针
    resultWaitAckPackPtr = (NET_PROT_RESULT_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //该元素是没发送过的元素
    if(resultWaitAckPackPtr->retryCount == 0)
    {//数据还没发送过
        if(RESET == NetProtGetConnectFlag())
        {
            //网络没链接
            if(resultWaitAckPackPtr->lastSendTimeTickMs == 0)
            {
                //网络没连接,时间更新
                resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                return;
            }
            else
            {
                //时间更新了
                if(currentTickMs - resultWaitAckPackPtr->lastSendTimeTickMs >= TIME_OUT_MS_MAX_WAIT_NET_PROT_CONNECT)
                {
                    //到达了最大的等待SOCKET连接时间,删除这个节点
                    NetProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
                }
                return;
            }
        }
        //第一次发送
        errorSub = NetProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->retryCount,
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
        if(currentTickMs - resultWaitAckPackPtr->lastSendTimeTickMs < TIME_OUT_MS_ACK_NET_PROT)
        {
            //还没到ACK超时时间,退出
            return;
        }
        //到了超时时间,要进行重发,检测SOCKET状态是否连接,已连接就能重发,未连接继续等待
        if(RESET == NetProtGetConnectFlag())
        {
            //网络未连接,检测重发次数,如果已经重发三次,那么现在就可以删除了
            if(resultWaitAckPackPtr->retryCount >= 3)
            {
                //日志发送失败
                NET_PROT_LOG("E>: %s,%d,NetProtSendResultFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%08X\r\n",
                                            __func__,__LINE__,resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,
                                            resultWaitAckPackPtr->cmdValue);
                //重发超过限制,删除节点
                NetProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
                return;
            }
            //网络未连接,检测超时时间是否大于系统设置的上限
            if(currentTickMs - resultWaitAckPackPtr->lastSendTimeTickMs >= TIME_OUT_MS_MAX_WAIT_NET_PROT_CONNECT)
            {
                //到达了最大的等待SOCKET连接时间,删除这个节点
                NetProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
            }
            return;
        }
    }
    else
    {
        //当前时间比上次发送时间还小,绝对不对劲,删除这一个节点
        NetProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
        return;
    }
    //检测重发次数
    if(resultWaitAckPackPtr->retryCount >= 3)
    {
        //日志发送失败
        NET_PROT_LOG("E>: %s,%d,NetProtSendResultFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                    __func__,__LINE__,resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,
                                    resultWaitAckPackPtr->cmdValue);
        //重发超过限制,删除节点
        NetProtRunPeriodSubResultWaitAckDeleteNode(listNodePtr,resultWaitAckPackPtr);
        return;
    }
    resultWaitAckPackPtr->retryCount += 1;
    //重发
    errorSub = NetProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->retryCount,
                                                        resultWaitAckPackPtr->cmdValue,resultWaitAckPackPtr->resultCode,resultWaitAckPackPtr->errorMainCode,
                                                        resultWaitAckPackPtr->errorLevelCode,resultWaitAckPackPtr->errorSubCode,resultWaitAckPackPtr->resultParamBufferPtr,
                                                        resultWaitAckPackPtr->resultParamBufferLength);
    //发送完成,时间更新
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    if(errorSub != ERROR_SUB_OK)
    {
        //错误打印
        NET_PROT_LOG("E>: %s,%d,NetProtSendResultOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //日志发送失败
        NET_PROT_LOG("E>: NetProtSendResultFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,RetryCount: %d\r\n",
                                    resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->cmdValue,
                                    resultWaitAckPackPtr->retryCount);
    }
    return;
}

//查找链表中等待ACK的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCmdWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //首先检查状态是不是等待ACK
    if(cmdWaitPackPtr->cmdStage != PROT_CMD_STA_WAIT_ACK)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(currentTick >= cmdWaitPackPtr->lastSendTimeTickMs)
    {
        if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_NET_PROT)
        {
            return ADT_CMP_EQUAL;
        }
        return ADT_CMP_NOT_EQUAL;
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//周期性的检查指令重发
void NetProtRunPeriodSubCmdWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCmdWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //该元素是没发送过的元素
        if(cmdWaitPackPtr->retryCount == 0)
        {
            if(RESET == NetProtGetConnectFlag())
            {
                //网络未连接
                //状态设置为重发失败
                cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
                //结束本次循环
                continue;
            }
            //网络已连接
            errorSub = NetProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->retryCount,
                                                        cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub != ERROR_SUB_OK)
            {
                //发送成功,时间更新
                NET_PROT_LOG("E>: %s,%d,NetProtSendCmdOnlySample Failed.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%08X\r\n",
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
            NET_PROT_LOG("E>: %s,%d,NetProtSendCmdFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                        cmdWaitPackPtr->cmdValue);
            //结束本次循环
            continue;
        }
        else
        {
            //网络未连接
            if(RESET == NetProtGetConnectFlag())
            {
                //状态设置为重发失败
                cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
                //结束本次循环
                continue;
            }
            cmdWaitPackPtr->retryCount += 1;
            //重发
            errorSub = NetProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->retryCount,
                                                            cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,错误打印
            NET_PROT_LOG("E>: %s,%d,NetProtSendCmdOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            NET_PROT_LOG("E>: NetProtSendCmdFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,RetryCount: %d\r\n",
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
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
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
void NetProtRunPeriodSubCmdWaitResultPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCmdWaitResultListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //超时的元素状态设置为超时
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_TIME_OUT;
        //日志超时
        NET_PROT_LOG("E>: %s,%d,NetProtSendCmdFailed.ResultTimeOut.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X,TimeOutSet\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                    cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->resultTimeOutMs);
        //结束本次循环
        continue;
    }
}




