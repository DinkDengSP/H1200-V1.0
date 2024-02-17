/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-18 17:09:59 +0800
************************************************************************************************/ 
#include "NetProtBase.h"

//NET SOCKET初始化标记
extern NET_PROT_INIT_SET netProtInitSet;
//网口接收缓存
extern NET_PROT_RECV_CACHE netProtRecvCache;

//获取数据访问权
extern void NetProtGetDataAccessLock(void);
//释放数据访问权
extern void NetProtGetDataAccessRelease(void);

//释放NET接收包
static void NetProtPackRecvFree(NET_PROT_PACKAGE* netProtPackagePtr)
{
    if(netProtPackagePtr->dataBodyBufferPtr != NULL)
    {
        NetProtMemFree(netProtPackagePtr->dataBodyBufferPtr);
    }
    NetProtMemFree(netProtPackagePtr);
}

//日志包处理
static void NetProtPackRecvProcessLogMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //日志包缓存长度必须大于0,否则没有意义
    if(netProtPackagePtr->dataBodyLength == 0)
    {
        NET_PROT_LOG("E:> %s %d, LogPackage DataLength Must More Then 0\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //转换日志包
    NET_PROT_LOG_PACK_RECV* logPackPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_LOG_PACK_RECV));
    if(logPackPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, logPackPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //内存直接赋值,不拷贝了
    logPackPtr->logBodyBufferPtr = netProtPackagePtr->dataBodyBufferPtr;
    //数据拷贝
    logPackPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    logPackPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    logPackPtr->targetModule = netProtPackagePtr->targetModule;
    logPackPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    logPackPtr->logBodyLength = netProtPackagePtr->dataBodyLength;
    //释放内存
    NetProtMemFree(netProtPackagePtr);
    //调用相应的程序处理
    if((logPackPtr->targetModule == netProtInitSet.selfModuleNo)&&(logPackPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //自己处理
        NetProtLogMsgSelfProcess(logPackPtr);
    }
    else
    {
        //透传处理
        NetProtLogMsgForward(logPackPtr);
    }
    //释放内存
    if(logPackPtr->logBodyBufferPtr != NULL)
    {
        NetProtMemFree(logPackPtr->logBodyBufferPtr);
    }
    NetProtMemFree(logPackPtr);
}

//指令包处理
static void NetProtPackRecvProcessCmdMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //创建指令包
    NET_PROT_CMD_PACK_RECV* cmdPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_CMD_PACK_RECV));
    if(cmdPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, cmdPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    if(netProtPackagePtr->dataBodyLength != 0)
    {
        cmdPackRecvPtr->dataBodyLength = netProtPackagePtr->dataBodyLength;
        cmdPackRecvPtr->dataBodyBufferPtr = netProtPackagePtr->dataBodyBufferPtr;
    }
    else
    {
        cmdPackRecvPtr->dataBodyLength = 0;
        cmdPackRecvPtr->dataBodyBufferPtr = NULL;
    }
    //数据拷贝
    cmdPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    cmdPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    cmdPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    cmdPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    cmdPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    cmdPackRecvPtr->cmdCode = netProtPackagePtr->idCode;
    //释放内存
    NetProtMemFree(netProtPackagePtr);
    //调用相应的程序处理
    if((cmdPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(cmdPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //自己处理
        NetProtCmdMsgSelfProcess(cmdPackRecvPtr);
    }
    else
    {
        //透传处理
        NetProtCmdMsgForward(cmdPackRecvPtr);
    }
    //释放内存
    if(cmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        NetProtMemFree(cmdPackRecvPtr->dataBodyBufferPtr);
    }
    NetProtMemFree(cmdPackRecvPtr);
}

//指令ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void NetProtPackRecvProcessAckCmd(NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr);
//指令ACK处理
static void NetProtPackRecvProcessAckCmdMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //指令ACK的数据长度必须是1
    if(netProtPackagePtr->dataBodyLength != 1)
    {
        NET_PROT_LOG("E:> %s %d, AckCmd DataLength Must Be 1 RealLength: %d\r\n",__func__,__LINE__,netProtPackagePtr->dataBodyLength);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //创建指令ACK包
    NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_ACK_CMD_PACK_RECV));
    if(ackCmdPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, ackCmdPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //数据拷贝
    ackCmdPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    ackCmdPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    ackCmdPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    ackCmdPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    ackCmdPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    ackCmdPackRecvPtr->ackCmdCode = netProtPackagePtr->idCode;
    //ACK结果
    ackCmdPackRecvPtr->ackCode = (PROT_ACK)netProtPackagePtr->dataBodyBufferPtr[0];
    //释放内存
    NetProtPackRecvFree(netProtPackagePtr);
    //调用相应的程序处理
    if((ackCmdPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(ackCmdPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //获取数据锁
        NetProtGetDataAccessLock();
        //自己处理
        NetProtPackRecvProcessAckCmd(ackCmdPackRecvPtr);
        //释放数据锁
        NetProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        NetProtAckCmdForward(ackCmdPackRecvPtr);
    }
    //释放内存
    NetProtMemFree(ackCmdPackRecvPtr);
}


//收到的结果包的处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void NetProtPackRecvProcessResult(NET_PROT_RESULT_PACK_RECV* resultPackPtr);

//结果包处理
static void NetProtPackRecvProcessResultMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //指令RESULT的数据长度必须是1
    if(netProtPackagePtr->dataBodyLength < 8)
    {
        NET_PROT_LOG("E:> %s %d, Result DataLength Must MoreThen 8 RealLength: %d\r\n",__func__,__LINE__,netProtPackagePtr->dataBodyLength);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    NET_PROT_RESULT_PACK_RECV* resultPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_RESULT_PACK_RECV));
    if(resultPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, resultPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //申请缓存
    resultPackRecvPtr->resultParamBufferLength = netProtPackagePtr->dataBodyLength - 8;
    if(resultPackRecvPtr->resultParamBufferLength != 0)
    {
        //申请数据缓存
        resultPackRecvPtr->resultParamBufferPtr = NetProtMemMalloc(resultPackRecvPtr->resultParamBufferLength);
        if(resultPackRecvPtr->resultParamBufferPtr == NULL)
        {
            NET_PROT_LOG("E:> %s %d, resultPackRecvPtr Buffer MemMalloc Failed!\r\n",__func__,__LINE__);
            //释放内存
            NetProtMemFree(resultPackRecvPtr);
            NetProtPackRecvFree(netProtPackagePtr);
            return;
        }
        //数据拷贝
        UserMemCopy(resultPackRecvPtr->resultParamBufferPtr,netProtPackagePtr->dataBodyBufferPtr+8,
                                resultPackRecvPtr->resultParamBufferLength);
    }
    else
    {
        resultPackRecvPtr->resultParamBufferPtr = NULL;
    }
    //数据拷贝
    resultPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    resultPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    resultPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    resultPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    resultPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    resultPackRecvPtr->cmdCode = netProtPackagePtr->idCode;
    //解析结果参数
    resultPackRecvPtr->resultCode = (PROT_RESULT)(netProtPackagePtr->dataBodyBufferPtr[0]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[2];
    utilDataTemp <<= 8;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[1];
    resultPackRecvPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    resultPackRecvPtr->errorLevelCode = (ERROR_LEVEL)(netProtPackagePtr->dataBodyBufferPtr[3]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[7];
    utilDataTemp <<= 8;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[6];
    utilDataTemp <<= 8;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[5];
    utilDataTemp <<= 8;
    utilDataTemp += netProtPackagePtr->dataBodyBufferPtr[4];
    resultPackRecvPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    //释放内存
    NetProtPackRecvFree(netProtPackagePtr);
    //调用相应的程序处理
    if((resultPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(resultPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //获取数据锁
        NetProtGetDataAccessLock();
        //自己处理
        NetProtPackRecvProcessResult(resultPackRecvPtr);
        //释放数据锁
        NetProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        NetProtResultMsgForward(resultPackRecvPtr);
    }
    //释放内存
    if(resultPackRecvPtr->resultParamBufferPtr != NULL)
    {
        NetProtMemFree(resultPackRecvPtr->resultParamBufferPtr);
    }
    NetProtMemFree(resultPackRecvPtr);
}

//结果包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void NetProtPackRecvProcessAckResult(NET_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr);

//结果包ACK处理
static void NetProtPackRecvProcessAckResultMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //结果ACK的数据长度必须为0
    if(netProtPackagePtr->dataBodyLength != 0)
    {
        NET_PROT_LOG("E:> %s %d, AckResult DataLength Must Be 0 RealLength: %d\r\n",__func__,__LINE__,netProtPackagePtr->dataBodyLength);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //创建结果ACK包
    NET_PROT_ACK_RESULT_PACK_RECV* ackResultPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_ACK_RESULT_PACK_RECV));
    if(ackResultPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, ackResultPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //数据拷贝
    ackResultPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    ackResultPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    ackResultPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    ackResultPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    ackResultPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    ackResultPackRecvPtr->ackCmdCode = netProtPackagePtr->idCode;
    //释放内存
    NetProtPackRecvFree(netProtPackagePtr);
    //调用相应的程序处理
    if((ackResultPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(ackResultPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //获取数据锁
        NetProtGetDataAccessLock();
        //自己处理
        NetProtPackRecvProcessAckResult(ackResultPackRecvPtr);
        //释放数据锁
        NetProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        NetProtAckResultForward(ackResultPackRecvPtr);
    }
    //释放内存
    NetProtMemFree(ackResultPackRecvPtr);
}

//数据上传包处理
static void NetProtPackRecvProcessDataUploadMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //创建上传包
    NET_PROT_UPLOAD_PACK_RECV* uploadPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_UPLOAD_PACK_RECV));
    if(uploadPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, uploadPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    if(netProtPackagePtr->dataBodyLength != 0)
    {
        uploadPackRecvPtr->paramBufferLength = netProtPackagePtr->dataBodyLength;
        uploadPackRecvPtr->paramBufferPtr = netProtPackagePtr->dataBodyBufferPtr;
    }
    else
    {
        uploadPackRecvPtr->paramBufferLength = 0;
        uploadPackRecvPtr->paramBufferPtr = NULL;
    }
    //数据拷贝
    uploadPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    uploadPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    uploadPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    uploadPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    uploadPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    uploadPackRecvPtr->uploadID = netProtPackagePtr->idCode;
    //释放内存
    NetProtMemFree(netProtPackagePtr);
    //调用相应的程序处理
    if((uploadPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(uploadPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //自己处理
        NetProtUploadMsgSelfProcess(uploadPackRecvPtr);
    }
    else
    {
        //透传处理
        NetProtUploadMsgForward(uploadPackRecvPtr);
    }
    //释放内存
    if(uploadPackRecvPtr->paramBufferPtr != NULL)
    {
        NetProtMemFree(uploadPackRecvPtr->paramBufferPtr);
    }
    NetProtMemFree(uploadPackRecvPtr);
}

//数据上传包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void NetProtPackRecvProcessAckDataUpload(NET_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr);

//数据上传包ACK处理
static void NetProtPackRecvProcessAckDataUploadMain(NET_PROT_PACKAGE* netProtPackagePtr)
{
    //数据上传ACK的数据长度必须为0
    if(netProtPackagePtr->dataBodyLength != 0)
    {
        NET_PROT_LOG("E:> %s %d, AckUpload DataLength Must Be 0 RealLength: %d\r\n",__func__,__LINE__,netProtPackagePtr->dataBodyLength);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //创建数据上传ACK包
    NET_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackRecvPtr = NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_ACK_UPLOAD_PACK_RECV));
    if(ackUploadPackRecvPtr == NULL)
    {
        NET_PROT_LOG("E:> %s %d, ackUploadPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        NetProtPackRecvFree(netProtPackagePtr);
        return;
    }
    //数据拷贝
    ackUploadPackRecvPtr->srcModuleNo = netProtPackagePtr->srcModuleNo;
    ackUploadPackRecvPtr->srcBoardID = netProtPackagePtr->srcBoardID;
    ackUploadPackRecvPtr->targetModule = netProtPackagePtr->targetModule;
    ackUploadPackRecvPtr->targetBoardID = netProtPackagePtr->targetBoardID;
    ackUploadPackRecvPtr->retryCount = netProtPackagePtr->retryCount;
    ackUploadPackRecvPtr->uploadID = netProtPackagePtr->idCode;
    //释放内存
    NetProtPackRecvFree(netProtPackagePtr);
    //调用相应的程序处理
    if((ackUploadPackRecvPtr->targetModule == netProtInitSet.selfModuleNo)&&(ackUploadPackRecvPtr->targetBoardID == netProtInitSet.selfBoardID))
    {
        //获取数据锁
        NetProtGetDataAccessLock();
        //自己处理
        NetProtPackRecvProcessAckDataUpload(ackUploadPackRecvPtr);
        //释放数据锁
        NetProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        NetProtAckUploadForward(ackUploadPackRecvPtr);
    }
    //释放内存
    NetProtMemFree(ackUploadPackRecvPtr);
}

//处理接受的NET消息
void NetProtPackRecvProcess(NET_PROT_PACKAGE* netProtPackagePtr)
{
    switch(netProtPackagePtr->packType)
    {
        case PROT_PACK_TYPE_LOG:
            NetProtPackRecvProcessLogMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_CMD:
            NetProtPackRecvProcessCmdMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_CMD:
            NetProtPackRecvProcessAckCmdMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_RESULT:
            NetProtPackRecvProcessResultMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_RESULT:
            NetProtPackRecvProcessAckResultMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_DATA_UPLOAD:
            NetProtPackRecvProcessDataUploadMain(netProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_DATA_UPLOAD:
            NetProtPackRecvProcessAckDataUploadMain(netProtPackagePtr);
            break;
        default:
            //不支持的消息类型,报错,释放内存
            NET_PROT_LOG("E:> %s %d, UnSupport PackType: %d\r\n",__func__,__LINE__,netProtPackagePtr->packType);
            if(netProtPackagePtr->dataBodyBufferPtr != NULL)
            {
                NetProtMemFree(netProtPackagePtr->dataBodyBufferPtr);
            }
            NetProtMemFree(netProtPackagePtr);
            break;
    }
}







