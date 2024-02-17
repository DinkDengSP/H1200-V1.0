/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-18 17:09:59 +0800
************************************************************************************************/ 
#include "UartProtBase.h"

//UART SOCKET初始化标记
extern UART_PROT_INIT_SET uartProtInitSet;
//串口接收缓存
extern UART_PROT_RECV_CACHE uartProtRecvCache;

//获取数据访问权
extern void UartProtGetDataAccessLock(void);
//释放数据访问权
extern void UartProtGetDataAccessRelease(void);

//释放UART接收包
static void UartProtPackRecvFree(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    if(uartProtPackagePtr->dataBodyBufferPtr != NULL)
    {
        UartProtMemFree(uartProtPackagePtr->dataBodyBufferPtr);
    }
    UartProtMemFree(uartProtPackagePtr);
}

//日志包处理
static void UartProtPackRecvProcessLogMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //日志包缓存长度必须大于0,否则没有意义
    if(uartProtPackagePtr->dataBodyLength == 0)
    {
        UART_PROT_LOG("E:> %s %d, LogPackage DataLength Must More Then 0\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //转换日志包
    UART_PROT_LOG_PACK_RECV* logPackPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_LOG_PACK_RECV));
    if(logPackPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, logPackPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //内存直接赋值,不拷贝了
    logPackPtr->logBodyBufferPtr = uartProtPackagePtr->dataBodyBufferPtr;
    //数据拷贝
    logPackPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    logPackPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    logPackPtr->targetModule = uartProtPackagePtr->targetModule;
    logPackPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    logPackPtr->logBodyLength = uartProtPackagePtr->dataBodyLength;
    //释放内存
    UartProtMemFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((logPackPtr->targetModule == uartProtInitSet.selfModuleNo)&&(logPackPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //自己处理
        UartProtLogMsgSelfProcess(logPackPtr);
    }
    else
    {
        //透传处理
        UartProtLogMsgForward(logPackPtr);
    }
    //释放内存
    if(logPackPtr->logBodyBufferPtr != NULL)
    {
        UartProtMemFree(logPackPtr->logBodyBufferPtr);
    }
    UartProtMemFree(logPackPtr);
}

//指令包处理
static void UartProtPackRecvProcessCmdMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //创建指令包
    UART_PROT_CMD_PACK_RECV* cmdPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_CMD_PACK_RECV));
    if(cmdPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, cmdPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    if(uartProtPackagePtr->dataBodyLength != 0)
    {
        cmdPackRecvPtr->dataBodyLength = uartProtPackagePtr->dataBodyLength;
        cmdPackRecvPtr->dataBodyBufferPtr = uartProtPackagePtr->dataBodyBufferPtr;
    }
    else
    {
        cmdPackRecvPtr->dataBodyLength = 0;
        cmdPackRecvPtr->dataBodyBufferPtr = NULL;
    }
    //数据拷贝
    cmdPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    cmdPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    cmdPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    cmdPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    cmdPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    cmdPackRecvPtr->cmdCode = uartProtPackagePtr->idCode;
    //释放内存
    UartProtMemFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((cmdPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(cmdPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //自己处理
        UartProtCmdMsgSelfProcess(cmdPackRecvPtr);
    }
    else
    {
        //透传处理
        UartProtCmdMsgForward(cmdPackRecvPtr);
    }
    //释放内存
    if(cmdPackRecvPtr->dataBodyBufferPtr != NULL)
    {
        UartProtMemFree(cmdPackRecvPtr->dataBodyBufferPtr);
    }
    UartProtMemFree(cmdPackRecvPtr);
}

//指令ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void UartProtPackRecvProcessAckCmd(UART_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr);
//指令ACK处理
static void UartProtPackRecvProcessAckCmdMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //指令ACK的数据长度必须是1
    if(uartProtPackagePtr->dataBodyLength != 1)
    {
        UART_PROT_LOG("E:> %s %d, AckCmd DataLength Must Be 1 RealLength: %d\r\n",__func__,__LINE__,uartProtPackagePtr->dataBodyLength);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //创建指令ACK包
    UART_PROT_ACK_CMD_PACK_RECV* ackCmdPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_ACK_CMD_PACK_RECV));
    if(ackCmdPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, ackCmdPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //数据拷贝
    ackCmdPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    ackCmdPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    ackCmdPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    ackCmdPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    ackCmdPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    ackCmdPackRecvPtr->ackCmdCode = uartProtPackagePtr->idCode;
    //ACK结果
    ackCmdPackRecvPtr->ackCode = (PROT_ACK)uartProtPackagePtr->dataBodyBufferPtr[0];
    //释放内存
    UartProtPackRecvFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((ackCmdPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(ackCmdPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //获取数据锁
        UartProtGetDataAccessLock();
        //自己处理
        UartProtPackRecvProcessAckCmd(ackCmdPackRecvPtr);
        //释放数据锁
        UartProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        UartProtAckCmdForward(ackCmdPackRecvPtr);
    }
    //释放内存
    UartProtMemFree(ackCmdPackRecvPtr);
}


//收到的结果包的处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void UartProtPackRecvProcessResult(UART_PROT_RESULT_PACK_RECV* resultPackPtr);

//结果包处理
static void UartProtPackRecvProcessResultMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //指令RESULT的数据长度必须是1
    if(uartProtPackagePtr->dataBodyLength < 8)
    {
        UART_PROT_LOG("E:> %s %d, Result DataLength Must MoreThen 8 RealLength: %d\r\n",__func__,__LINE__,uartProtPackagePtr->dataBodyLength);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    UART_PROT_RESULT_PACK_RECV* resultPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_RESULT_PACK_RECV));
    if(resultPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, resultPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //申请缓存
    resultPackRecvPtr->resultParamBufferLength = uartProtPackagePtr->dataBodyLength - 8;
    if(resultPackRecvPtr->resultParamBufferLength != 0)
    {
        //申请数据缓存
        resultPackRecvPtr->resultParamBufferPtr = UartProtMemMalloc(resultPackRecvPtr->resultParamBufferLength);
        if(resultPackRecvPtr->resultParamBufferPtr == NULL)
        {
            UART_PROT_LOG("E:> %s %d, resultPackRecvPtr Buffer MemMalloc Failed!\r\n",__func__,__LINE__);
            //释放内存
            UartProtMemFree(resultPackRecvPtr);
            UartProtPackRecvFree(uartProtPackagePtr);
            return;
        }
        //数据拷贝
        UserMemCopy(resultPackRecvPtr->resultParamBufferPtr,uartProtPackagePtr->dataBodyBufferPtr+8,
                                resultPackRecvPtr->resultParamBufferLength);
    }
    else
    {
        resultPackRecvPtr->resultParamBufferPtr = NULL;
    }
    //数据拷贝
    resultPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    resultPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    resultPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    resultPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    resultPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    resultPackRecvPtr->cmdCode = uartProtPackagePtr->idCode;
    //解析结果参数
    resultPackRecvPtr->resultCode = (PROT_RESULT)(uartProtPackagePtr->dataBodyBufferPtr[0]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[2];
    utilDataTemp <<= 8;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[1];
    resultPackRecvPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    resultPackRecvPtr->errorLevelCode = (ERROR_LEVEL)(uartProtPackagePtr->dataBodyBufferPtr[3]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[7];
    utilDataTemp <<= 8;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[6];
    utilDataTemp <<= 8;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[5];
    utilDataTemp <<= 8;
    utilDataTemp += uartProtPackagePtr->dataBodyBufferPtr[4];
    resultPackRecvPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    //释放内存
    UartProtPackRecvFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((resultPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(resultPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //获取数据锁
        UartProtGetDataAccessLock();
        //自己处理
        UartProtPackRecvProcessResult(resultPackRecvPtr);
        //释放数据锁
        UartProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        UartProtResultMsgForward(resultPackRecvPtr);
    }
    //释放内存
    if(resultPackRecvPtr->resultParamBufferPtr != NULL)
    {
        UartProtMemFree(resultPackRecvPtr->resultParamBufferPtr);
    }
    UartProtMemFree(resultPackRecvPtr);
}

//结果包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void UartProtPackRecvProcessAckResult(UART_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr);

//结果包ACK处理
static void UartProtPackRecvProcessAckResultMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //结果ACK的数据长度必须为0
    if(uartProtPackagePtr->dataBodyLength != 0)
    {
        UART_PROT_LOG("E:> %s %d, AckResult DataLength Must Be 0 RealLength: %d\r\n",__func__,__LINE__,uartProtPackagePtr->dataBodyLength);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //创建结果ACK包
    UART_PROT_ACK_RESULT_PACK_RECV* ackResultPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_ACK_RESULT_PACK_RECV));
    if(ackResultPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, ackResultPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //数据拷贝
    ackResultPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    ackResultPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    ackResultPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    ackResultPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    ackResultPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    ackResultPackRecvPtr->ackCmdCode = uartProtPackagePtr->idCode;
    //释放内存
    UartProtPackRecvFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((ackResultPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(ackResultPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //获取数据锁
        UartProtGetDataAccessLock();
        //自己处理
        UartProtPackRecvProcessAckResult(ackResultPackRecvPtr);
        //释放数据锁
        UartProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        UartProtAckResultForward(ackResultPackRecvPtr);
    }
    //释放内存
    UartProtMemFree(ackResultPackRecvPtr);
}

//数据上传包处理
static void UartProtPackRecvProcessDataUploadMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //创建上传包
    UART_PROT_UPLOAD_PACK_RECV* uploadPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_UPLOAD_PACK_RECV));
    if(uploadPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, uploadPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    if(uartProtPackagePtr->dataBodyLength != 0)
    {
        uploadPackRecvPtr->paramBufferLength = uartProtPackagePtr->dataBodyLength;
        uploadPackRecvPtr->paramBufferPtr = uartProtPackagePtr->dataBodyBufferPtr;
    }
    else
    {
        uploadPackRecvPtr->paramBufferLength = 0;
        uploadPackRecvPtr->paramBufferPtr = NULL;
    }
    //数据拷贝
    uploadPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    uploadPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    uploadPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    uploadPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    uploadPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    uploadPackRecvPtr->uploadID = uartProtPackagePtr->idCode;
    //释放内存
    UartProtMemFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((uploadPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(uploadPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //自己处理
        UartProtUploadMsgSelfProcess(uploadPackRecvPtr);
    }
    else
    {
        //透传处理
        UartProtUploadMsgForward(uploadPackRecvPtr);
    }
    //释放内存
    if(uploadPackRecvPtr->paramBufferPtr != NULL)
    {
        UartProtMemFree(uploadPackRecvPtr->paramBufferPtr);
    }
    UartProtMemFree(uploadPackRecvPtr);
}

//数据上传包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
extern void UartProtPackRecvProcessAckDataUpload(UART_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr);

//数据上传包ACK处理
static void UartProtPackRecvProcessAckDataUploadMain(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    //数据上传ACK的数据长度必须为0
    if(uartProtPackagePtr->dataBodyLength != 0)
    {
        UART_PROT_LOG("E:> %s %d, AckUpload DataLength Must Be 0 RealLength: %d\r\n",__func__,__LINE__,uartProtPackagePtr->dataBodyLength);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //创建数据上传ACK包
    UART_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackRecvPtr = UartProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(UART_PROT_ACK_UPLOAD_PACK_RECV));
    if(ackUploadPackRecvPtr == NULL)
    {
        UART_PROT_LOG("E:> %s %d, ackUploadPackRecvPtr MemMalloc Failed!\r\n",__func__,__LINE__);
        //释放内存
        UartProtPackRecvFree(uartProtPackagePtr);
        return;
    }
    //数据拷贝
    ackUploadPackRecvPtr->srcModuleNo = uartProtPackagePtr->srcModuleNo;
    ackUploadPackRecvPtr->srcBoardID = uartProtPackagePtr->srcBoardID;
    ackUploadPackRecvPtr->targetModule = uartProtPackagePtr->targetModule;
    ackUploadPackRecvPtr->targetBoardID = uartProtPackagePtr->targetBoardID;
    ackUploadPackRecvPtr->retryCount = uartProtPackagePtr->retryCount;
    ackUploadPackRecvPtr->uploadID = uartProtPackagePtr->idCode;
    //释放内存
    UartProtPackRecvFree(uartProtPackagePtr);
    //调用相应的程序处理
    if((ackUploadPackRecvPtr->targetModule == uartProtInitSet.selfModuleNo)&&(ackUploadPackRecvPtr->targetBoardID == uartProtInitSet.selfBoardID))
    {
        //获取数据锁
        UartProtGetDataAccessLock();
        //自己处理
        UartProtPackRecvProcessAckDataUpload(ackUploadPackRecvPtr);
        //释放数据锁
        UartProtGetDataAccessRelease();
    }
    else
    {
        //透传处理
        UartProtAckUploadForward(ackUploadPackRecvPtr);
    }
    //释放内存
    UartProtMemFree(ackUploadPackRecvPtr);
}

//处理接受的UART消息
void UartProtPackRecvProcess(UART_PROT_PACKAGE* uartProtPackagePtr)
{
    switch(uartProtPackagePtr->packType)
    {
        case PROT_PACK_TYPE_LOG:
            UartProtPackRecvProcessLogMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_CMD:
            UartProtPackRecvProcessCmdMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_CMD:
            UartProtPackRecvProcessAckCmdMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_RESULT:
            UartProtPackRecvProcessResultMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_RESULT:
            UartProtPackRecvProcessAckResultMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_DATA_UPLOAD:
            UartProtPackRecvProcessDataUploadMain(uartProtPackagePtr);
            break;
        case PROT_PACK_TYPE_ACK_DATA_UPLOAD:
            UartProtPackRecvProcessAckDataUploadMain(uartProtPackagePtr);
            break;
        default:
            //不支持的消息类型,报错,释放内存
            UART_PROT_LOG("E:> %s %d, UnSupport PackType: %d\r\n",__func__,__LINE__,uartProtPackagePtr->packType);
            if(uartProtPackagePtr->dataBodyBufferPtr != NULL)
            {
                UartProtMemFree(uartProtPackagePtr->dataBodyBufferPtr);
            }
            UartProtMemFree(uartProtPackagePtr);
            break;
    }
}







