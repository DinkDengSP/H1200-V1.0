/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 17:05:43 +0800
************************************************************************************************/ 
#include "CanProtBase.h"


//CAN1初始化标记
extern CAN_PROT_INIT_SET can1ProtInitSet;
//CAN2初始化标记
extern CAN_PROT_INIT_SET can2ProtInitSet;

//获取数据访问权
extern void Can1ProtGetDataAccessLock(void);
//释放数据访问权
extern void Can1ProtGetDataAccessRelease(void);
//获取数据访问权
extern void Can2ProtGetDataAccessLock(void);
//释放数据访问权
extern void Can2ProtGetDataAccessRelease(void);
//比较日志缓存中的元素是否相等,用于节点查找
extern ADT_CMP_RESULT ADT_RecvCacheListElementCmp(void* cmpSrc,void* cmpDst);

/*******************************************CAN1_PROT*************************************************************/
//指令包的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can1ProtAckCmdMsgPackProc(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr);
//收到的结果包的处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can1ProtResultMsgPackProc(CAN_PROT_RESULT_PACK* resultPackPtr);
//结果上传的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can1ProtAckResultMsgPackProc(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr);
//主动上报的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can1ProtAckUploadMsgPackProc(CAN_PROT_ACK_UPLOAD_PACK* uploadPackPtr);

//日志数据包短包处理
static void Can1ProtRecvProcLogFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubLogFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //生成一个日志包
    CAN_PROT_LOG_PACK* logPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_LOG_PACK));
    if(logPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_LOG_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //生成日志缓存数据
    logPackPtr->logDataBufferPtr = Can1ProtPortMemMalloc(validDataByte);
    if(logPackPtr->logDataBufferPtr == NULL)
    {
        //释放之前申请的内存
        Can1ProtPortMemFree(logPackPtr);
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc logDataBufferPtr Failed\r\n",__func__,__LINE__);
        return;
    }
    //拷贝日志数据
    UserMemCopy(logPackPtr->logDataBufferPtr,canRecvPackPtr->recvDataBufferPtr+1,validDataByte);
    //其余元素拷贝
    logPackPtr->srcBoardID = protCanID->srcBoardID;
    logPackPtr->srcModuleNo = protCanID->srcModuleNo;
    logPackPtr->targetBoardID = protCanID->targetBoardID;
    logPackPtr->targetModuleNo = protCanID->targetModuleNo;
    logPackPtr->singalChannel = protCanID->singalChannel;
    logPackPtr->logDataBufferLength = validDataByte;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((logPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (logPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can1ProtLogMsgForward(logPackPtr);
    }
    else
    {
        //是自己的数据
        Can1ProtLogMsgSelfProc(logPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(logPackPtr->logDataBufferPtr);
    Can1ProtPortMemFree(logPackPtr);
}


//从接收ID中创建消息头
static void Can1ProtRecvProcSetCacheHeader(CAN_PROT_RECV_PACK_CACHE_HEADER* recvPackCacheHeaderPtr,CAN_PROT_ID* protCanID)
{
    //填充数据
    recvPackCacheHeaderPtr->srcBoardID = protCanID->srcBoardID;
    recvPackCacheHeaderPtr->srcModuleNo = protCanID->srcModuleNo;
    recvPackCacheHeaderPtr->targetBoardID = protCanID->targetBoardID;
    recvPackCacheHeaderPtr->targetModuleNo = protCanID->targetModuleNo;
    recvPackCacheHeaderPtr->singalChannel = protCanID->singalChannel;
}

//从开始帧中解析信息
static void Can1ProtRecvProcSetFrameStartMsg(CAN_PROT_MSG_FRAME_START* startFrameMsgPtr,CAN_RECV_PACK* canRecvPackPtr)
{
    //预期接收的缓存区总字节长度
    startFrameMsgPtr->exceptPackAllByteLength = 0;
    startFrameMsgPtr->exceptPackAllByteLength += canRecvPackPtr->recvDataBufferPtr[1];
    startFrameMsgPtr->exceptPackAllByteLength <<= 8;
    startFrameMsgPtr->exceptPackAllByteLength += canRecvPackPtr->recvDataBufferPtr[0];
    //预期接收的单包有效数据长度
    startFrameMsgPtr->exceptSinglePackValidByteCount = canRecvPackPtr->recvDataBufferPtr[2];
    //预期接收的最后一包的有效数据长度
    startFrameMsgPtr->exceptLastPackValidByteCount = canRecvPackPtr->recvDataBufferPtr[3];
    //预期接收的数据包总数
    startFrameMsgPtr->exceptPackCountAll = 0;
    startFrameMsgPtr->exceptPackCountAll += canRecvPackPtr->recvDataBufferPtr[5];
    startFrameMsgPtr->exceptPackCountAll <<= 8;
    startFrameMsgPtr->exceptPackCountAll += canRecvPackPtr->recvDataBufferPtr[4];
}

static ADT_CIRCULAR_PTR_LIST_T Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE cacheNo)
{
    if(cacheNo == CAN_PROT_RECV_CACHE_LOG)
    {
        return can1ProtInitSet.listRecvLogMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_CMD)
    {
        return can1ProtInitSet.listRecvCmdMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_RESULT)
    {
        return can1ProtInitSet.listRecvResultMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_UPLOAD)
    {
        return can1ProtInitSet.listRecvUploadMsgCachePtr;
    }
    else
    {
        return NULL;
    }
}

//从接收缓存链表中清除指定的信息
static ErrorStatus Can1ProtRecvProcClearNodeByCacheHeader(uint8_t* funcNamePtr,CAN_PROT_RECV_CACHE cacheNo,
                                                                        CAN_PROT_RECV_PACK_CACHE_HEADER* cacheHeaderPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can1ProtRecvProcGetCacheList(cacheNo);
    if(cacheListPtr == NULL)return ERROR;
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T msgRecvCacheNodePtr = ADT_CircularPtrListFindNodeWithData(cacheListPtr,(void*)cacheHeaderPtr,
                                                                                            ADT_RecvCacheListElementCmp);
    //检查是否存在相应的节点
    if(msgRecvCacheNodePtr != NULL)
    {
        //释放节点中的元素
        recvMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(msgRecvCacheNodePtr);
        if(recvMsgCachePtr != NULL)
        {
            //释放缓存
            if(recvMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can1ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
            }
            Can1ProtPortMemFree(recvMsgCachePtr);
            recvMsgCachePtr = NULL;
        }
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(cacheListPtr,msgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",funcNamePtr);
            return ERROR;
        }
        msgRecvCacheNodePtr = NULL;
    }
    return SUCCESS;
}

//创建新的缓存节点
static CAN_PROT_RECV_PACK_CACHE* Can1ProtRecvProcCreateNewCacheNode(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs,
                                                                                        CAN_PROT_MSG_FRAME_START* startFrameMsgPtr)
{
    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr = NULL;
    recvMsgCachePtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RECV_PACK_CACHE));
    if(recvMsgCachePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s, recvMsgCachePtr MemMalloc Failed\r\n",__func__);
        return NULL;
    }
    //初始化数据
    recvMsgCachePtr->targetBoardID = protCanID->targetBoardID;
    recvMsgCachePtr->targetModuleNo = protCanID->targetModuleNo;
    recvMsgCachePtr->srcBoardID = protCanID->srcBoardID;
    recvMsgCachePtr->srcModuleNo = protCanID->srcModuleNo;
    recvMsgCachePtr->singalChannel = protCanID->singalChannel;
    recvMsgCachePtr->longPackStage = CAN_PROT_LONG_PACK_STA_WAIT_DATA;
    recvMsgCachePtr->lastFrameRecvTimeStamp = currentTickMs;
    recvMsgCachePtr->exceptPackAllByteLength = startFrameMsgPtr->exceptPackAllByteLength;
    recvMsgCachePtr->exceptSinglePackValidByteCount = startFrameMsgPtr->exceptSinglePackValidByteCount;
    recvMsgCachePtr->exceptLastPackValidByteCount = startFrameMsgPtr->exceptLastPackValidByteCount;
    recvMsgCachePtr->exceptPackCountAll = startFrameMsgPtr->exceptPackCountAll;
    recvMsgCachePtr->retryCount = protCanID->retrySendCount;
    recvMsgCachePtr->currentPackAllByteLength = 0;
    recvMsgCachePtr->currentPackCountAll = 0;
    //申请缓冲区
    recvMsgCachePtr->recvDataBufferPtr = Can1ProtPortMemMalloc(recvMsgCachePtr->exceptPackAllByteLength);
    if(recvMsgCachePtr->recvDataBufferPtr == NULL)
    {
        CAN_PROT_LOG("E>: %s, recvMsgCachePtr->recvDataBufferPtr MemMalloc Failed\r\n",__func__);
        //释放内存
        Can1ProtPortMemFree(recvMsgCachePtr);
        return NULL;
    }
    return recvMsgCachePtr;
}

//将新的缓存数据压入链表
static ErrorStatus Can1ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE node,CAN_PROT_RECV_PACK_CACHE* recvPackCachePtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can1ProtRecvProcGetCacheList(node);
    if(cacheListPtr == NULL)return ERROR;
    //将缓存信息压入链表
    adtResult = ADT_CircularPtrListInsertNodeEnd(cacheListPtr,recvPackCachePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //插入节点失败,释放内存
        if(recvPackCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvPackCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvPackCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//长包起始帧检测
static ErrorStatus Can1ProtRecvProcFrameStartCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength != 6)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongStartFrame DataLength Need 6,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//日志开始包
void Can1ProtRecvProcLogFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can1ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can1ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_LOG,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvLogMsgCachePtr = Can1ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvLogMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can1ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_LOG,recvLogMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can1ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//检查数据包长度是否正确
static ErrorStatus Can1ProtRecvProcFrameDataCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength <= 2)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongDataFrame DataLength Need More Then 2,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//获取指定的元素节点
static ADT_CIRCULAR_PTR_LIST_NODE_T Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE no,
                                                                        CAN_PROT_RECV_PACK_CACHE_HEADER* cacheHeaderPtr)
{
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can1ProtRecvProcGetCacheList(no);
    if(cacheListPtr == NULL)
    {
        return NULL; 
    }  
    return ADT_CircularPtrListFindNodeWithData(cacheListPtr,(void*)cacheHeaderPtr,ADT_RecvCacheListElementCmp);
}

//获取当前包序号
static uint16_t Can1ProtRecvProcDataFrameGetCurrentIndex(CAN_RECV_PACK* canRecvPackPtr)
{
    uint16_t currentPackIndex = 0;
    currentPackIndex += canRecvPackPtr->recvDataBufferPtr[1];
    currentPackIndex <<= 8;
    currentPackIndex += canRecvPackPtr->recvDataBufferPtr[0];
    return currentPackIndex;
}

//检查包序号和包长度是否正确
static ErrorStatus Can1ProtRecvProcDataFrameCheckIndexAndLength(uint8_t* funcName,uint16_t currentPackIndex,uint16_t lastPackIndex,
                                                                CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,CAN_RECV_PACK* canRecvPackPtr)
{
    //检查数据长度是否正确
    if(currentPackIndex < lastPackIndex)
    {
        //包长度检测
        if(canRecvPackPtr->recvDataLength < recvMsgCachePtr->exceptSinglePackValidByteCount)
        {
            CAN_PROT_LOG("E>: %s, Not Last Pack,LengthNotEnough,Need: %d,Real: %d\r\n",funcName,
                            recvMsgCachePtr->exceptSinglePackValidByteCount,canRecvPackPtr->recvDataLength);
            return ERROR;
        }
    }
    else
    {
        //包长度检测
        if(canRecvPackPtr->recvDataLength < recvMsgCachePtr->exceptLastPackValidByteCount)
        {
            CAN_PROT_LOG("E>: %s, Last Pack,LengthNotEnough,Need: %d,Real: %d\r\n",funcName,
                            recvMsgCachePtr->exceptLastPackValidByteCount,canRecvPackPtr->recvDataLength);
            return ERROR;
        }
    }
    //检查包序号是否匹配
    if(currentPackIndex != recvMsgCachePtr->currentPackCountAll)
    {
        CAN_PROT_LOG("E>: %s, PackIndex Not Match,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->currentPackCountAll,
                                        currentPackIndex);
        return ERROR;
    }
    return SUCCESS;
}

//数据拷贝
static ErrorStatus Can1ProtRecvProcDataFrameCopyData(uint8_t* funcName,uint16_t currentPackIndex,uint16_t lastPackIndex,
                                                                    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,CAN_RECV_PACK* canRecvPackPtr)
{   
    uint16_t recvByteCountAll = 0;
    //数据拷贝
    if(currentPackIndex < lastPackIndex)
    {
        //-2是去除两个index占用的位置
        recvByteCountAll = recvMsgCachePtr->currentPackAllByteLength + recvMsgCachePtr->exceptSinglePackValidByteCount -2;
        //不是最后一包,检查长度是否会溢出
        if(recvByteCountAll > recvMsgCachePtr->exceptPackAllByteLength)
        {
            //将会导致数据溢出,不要了
            CAN_PROT_LOG("E>: %s, RecvByteCountOverFlow,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvByteCountAll);
            return ERROR;
        }
        //不会导致数据溢出,拷贝数据
        UserMemCopy(recvMsgCachePtr->recvDataBufferPtr + recvMsgCachePtr->currentPackAllByteLength,
                        canRecvPackPtr->recvDataBufferPtr+2,recvMsgCachePtr->exceptSinglePackValidByteCount -2);
        //接收数据数量增加
        recvMsgCachePtr->currentPackAllByteLength = recvByteCountAll;
        //接收包数量增加
        recvMsgCachePtr->currentPackCountAll += 1;
    }
    else
    {
        //最后一包
        recvByteCountAll = recvMsgCachePtr->currentPackAllByteLength + recvMsgCachePtr->exceptLastPackValidByteCount -2;
        //是最后一包,检查长度是否会溢出
        if(recvByteCountAll > recvMsgCachePtr->exceptPackAllByteLength)
        {
            //将会导致数据溢出,不要了
            CAN_PROT_LOG("E>: %s, RecvByteCountOverFlow,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvByteCountAll);
            return ERROR;
        }
        //不会导致数据溢出,拷贝数据
        UserMemCopy(recvMsgCachePtr->recvDataBufferPtr + recvMsgCachePtr->currentPackAllByteLength,
                        canRecvPackPtr->recvDataBufferPtr+2,recvMsgCachePtr->exceptLastPackValidByteCount -2);
        //接收数据数量增加
        recvMsgCachePtr->currentPackAllByteLength = recvByteCountAll;
        //接收包数量增加
        recvMsgCachePtr->currentPackCountAll += 1;
    }
    return SUCCESS;
}

//数据标签更新
static ErrorStatus Can1ProtRecvProcDataFrameLabelReflush(uint8_t* funcName,CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,uint32_t currentTickMs)
{
    //更新数据接收时间
    recvMsgCachePtr->lastFrameRecvTimeStamp = currentTickMs;
    //包号与数据长度都匹配之后,等待结果帧
    if((recvMsgCachePtr->currentPackAllByteLength == recvMsgCachePtr->exceptPackAllByteLength)&&
        (recvMsgCachePtr->exceptPackCountAll == recvMsgCachePtr->currentPackCountAll))
    {
        recvMsgCachePtr->longPackStage = CAN_PROT_LONG_PACK_STA_WAIT_END;
        return SUCCESS;
    }
    //包号和数据长度有一个匹配有一个不匹配,数据传输异常了
    if((recvMsgCachePtr->currentPackAllByteLength == recvMsgCachePtr->exceptPackAllByteLength)||
        (recvMsgCachePtr->exceptPackCountAll == recvMsgCachePtr->currentPackCountAll))
    {
        CAN_PROT_LOG("E>: %s, RecvByteCount:%d,ExceptByteCount: %d,RecvPackCount: %d,ExceptPackCount: %d\r\n",funcName,
                                        recvMsgCachePtr->currentPackAllByteLength,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvMsgCachePtr->currentPackCountAll,recvMsgCachePtr->exceptPackCountAll);
        //释放掉这一包
        if(recvMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvMsgCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//日志数据包
void Can1ProtRecvProcLogFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T logMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_LOG,
                                                                                            (void*)&recvPackCacheHeader);
    if(logMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        return;
    }
    //获取存储的节点指针
    recvLogMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(logMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvLogMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        return;
    }
    uint16_t currentPackIndex = Can1ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvLogMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can1ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,
                                                                    recvLogMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can1ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvLogMsgCachePtr,
                                                        canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can1ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvLogMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvLogMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_LOG);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_LOG);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    logMsgRecvCacheNodePtr = NULL;
    return;
}

//检查数据包长度是否正确
static ErrorStatus Can1ProtRecvProcFrameEndCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength != 2)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongEndFrame DataLength Need 2,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//CRC16检查
static ErrorStatus Can1ProtRecvProcFrameEndCrc16Check(uint8_t* funcName,CAN_RECV_PACK* canRecvPackPtr,
                                                        CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr)
{
    uint16_t crc16CheckRecv = 0;
    crc16CheckRecv += canRecvPackPtr->recvDataBufferPtr[1];
    crc16CheckRecv <<= 8;
    crc16CheckRecv += canRecvPackPtr->recvDataBufferPtr[0];
    //计算CRC16
    uint16_t crc16CheckCalc = 0;
    crc16CheckCalc = CanProtPortCalcCRC16(recvMsgCachePtr->recvDataBufferPtr,recvMsgCachePtr->currentPackAllByteLength);
    //检查CRC是否匹配
    if(crc16CheckRecv != crc16CheckCalc)
    {
        CAN_PROT_LOG("E>: %s, CRC ResultNotMatch,Recv: 0X%08X,Calc: 0X%08X\r\n",funcName,crc16CheckRecv,crc16CheckCalc);
        //释放掉这一包
        if(recvMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvMsgCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//日志校验包
void Can1ProtRecvProcLogFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_LOG);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_LOG);
        return;
    }
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T logMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_LOG,
                                                                                            (void*)&recvPackCacheHeader);
    if(logMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        return;
    }
    //获取存储的节点指针
    recvLogMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(logMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvLogMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //释放掉这一包
        if(recvLogMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvLogMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvLogMsgCachePtr);
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    if(ERROR == Can1ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvLogMsgCachePtr))
    {
        //CRC匹配失败
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    CAN_PROT_LOG_PACK* logPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_LOG_PACK));
    if(logPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_LOG_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvLogMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvLogMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvLogMsgCachePtr);
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    //数据拷贝
    logPackPtr->targetBoardID = recvLogMsgCachePtr->targetBoardID;
    logPackPtr->targetModuleNo = recvLogMsgCachePtr->targetModuleNo;
    logPackPtr->srcBoardID = recvLogMsgCachePtr->srcBoardID;
    logPackPtr->srcModuleNo = recvLogMsgCachePtr->srcModuleNo;
    logPackPtr->singalChannel = recvLogMsgCachePtr->singalChannel;
    logPackPtr->logDataBufferLength = recvLogMsgCachePtr->currentPackAllByteLength;
    //数据不用拷贝,直接赋值即可
    logPackPtr->logDataBufferPtr = recvLogMsgCachePtr->recvDataBufferPtr;
    //释放数据
    Can1ProtPortMemFree(recvLogMsgCachePtr);
    recvLogMsgCachePtr = NULL;
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    logMsgRecvCacheNodePtr = NULL;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((logPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (logPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can1ProtLogMsgForward(logPackPtr);
    }
    else
    {
        //是自己的数据
        Can1ProtLogMsgSelfProc(logPackPtr);
    }
    //处理完成,释放内存
    if(logPackPtr->logDataBufferPtr != NULL)
    {
        Can1ProtPortMemFree(logPackPtr->logDataBufferPtr);
    }
    Can1ProtPortMemFree(logPackPtr);
    return;
}

//日志数据包
void Can1ProtRecvProcLogFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //日志数据包短包处理
        Can1ProtRecvProcLogFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //日志数据包长包起始包处理
        Can1ProtRecvProcLogFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //日志数据包长包数据包处理
        Can1ProtRecvProcLogFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //日志数据包长包结束包处理
        Can1ProtRecvProcLogFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//指令数据包短包处理
static void Can1ProtRecvProcCmdFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查指令最小长度,最小3字节
    if(validDataByte < 3)
    {
        //报错,有效数据长度小于3
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 3,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个指令包
    CAN_PROT_CMD_PACK* canCmdPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_PACK));
    if(canCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canCmdPackPtr->paramBufferLength = validDataByte - 3;
    //根据参数长度进行缓存数据拷贝
    if(canCmdPackPtr->paramBufferLength == 0)
    {
        canCmdPackPtr->paramBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canCmdPackPtr->paramBufferPtr = Can1ProtPortMemMalloc(canCmdPackPtr->paramBufferLength);
        if(canCmdPackPtr->paramBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can1ProtPortMemFree(canCmdPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc cmdDataBufferPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canCmdPackPtr->paramBufferPtr,canRecvPackPtr->recvDataBufferPtr+4,canCmdPackPtr->paramBufferLength);
    }
    //其余元素拷贝
    canCmdPackPtr->srcBoardID = protCanID->srcBoardID;
    canCmdPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canCmdPackPtr->targetBoardID = protCanID->targetBoardID;
    canCmdPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canCmdPackPtr->singalChannel = protCanID->singalChannel;
    canCmdPackPtr->retryCount = protCanID->retrySendCount;
    //解析cmd
    canCmdPackPtr->cmdValue = 0;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[3];
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[2];
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[1];
    if((canCmdPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canCmdPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的指令包
        Can1ProtCmdMsgForward(canCmdPackPtr);
    }
    else
    {
        //是自己的指令包
        Can1ProtCmdMsgSelfProc(canCmdPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(canCmdPackPtr->paramBufferPtr);
    Can1ProtPortMemFree(canCmdPackPtr);
}

//指令开始包
void Can1ProtRecvProcCmdFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can1ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can1ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_CMD,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvCmdMsgCachePtr = Can1ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvCmdMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can1ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_CMD,recvCmdMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can1ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//指令数据包
void Can1ProtRecvProcCmdFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T cmdMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_CMD,
                                                                                            (void*)&recvPackCacheHeader);
    if(cmdMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvCmdMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(cmdMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvCmdMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can1ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvCmdMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can1ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,
                                                                recvCmdMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can1ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvCmdMsgCachePtr,
                                                    canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can1ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvCmdMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvCmdMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_CMD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_CMD);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    cmdMsgRecvCacheNodePtr = NULL;
    return;
}

//指令校验包
void Can1ProtRecvProcCmdFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T cmdMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_CMD,
                                                                                            (void*)&recvPackCacheHeader);
    if(cmdMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvCmdMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(cmdMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvCmdMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_CMD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_CMD);
        return;
    }
    if(ERROR == Can1ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvCmdMsgCachePtr))
    {
        //CRC匹配失败
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    CAN_PROT_CMD_PACK* canCmdPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_PACK));
    if(canCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvCmdMsgCachePtr);
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        return;
    }
    //数据拷贝
    canCmdPackPtr->targetBoardID = recvCmdMsgCachePtr->targetBoardID;
    canCmdPackPtr->targetModuleNo = recvCmdMsgCachePtr->targetModuleNo;
    canCmdPackPtr->srcBoardID = recvCmdMsgCachePtr->srcBoardID;
    canCmdPackPtr->srcModuleNo = recvCmdMsgCachePtr->srcModuleNo;
    canCmdPackPtr->singalChannel = recvCmdMsgCachePtr->singalChannel;
    canCmdPackPtr->retryCount = recvCmdMsgCachePtr->retryCount;
    canCmdPackPtr->paramBufferLength = recvCmdMsgCachePtr->currentPackAllByteLength - 3;
    //计算指令码
    canCmdPackPtr->cmdValue = 0;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[2]);
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[1]);
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[0]);
    //申请内存空间存放指令参数
    canCmdPackPtr->paramBufferPtr = Can1ProtPortMemMalloc(canCmdPackPtr->paramBufferLength);
    if(canCmdPackPtr->paramBufferPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvCmdMsgCachePtr);
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        //释放包体
        Can1ProtPortMemFree(canCmdPackPtr);
        return;
    }
    //拷贝数据
    UserMemCopy(canCmdPackPtr->paramBufferPtr,recvCmdMsgCachePtr->recvDataBufferPtr+3,canCmdPackPtr->paramBufferLength);
    //释放数据
    if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can1ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
    }
    Can1ProtPortMemFree(recvCmdMsgCachePtr);
    recvCmdMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
    }
    cmdMsgRecvCacheNodePtr = NULL;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((canCmdPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canCmdPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can1ProtCmdMsgForward(canCmdPackPtr);
    }
    else
    {
        //是自己的数据
        Can1ProtCmdMsgSelfProc(canCmdPackPtr);
    }
    //处理完成,释放内存
    if(canCmdPackPtr->paramBufferPtr != NULL)
    {
        Can1ProtPortMemFree(canCmdPackPtr->paramBufferPtr);
    }
    Can1ProtPortMemFree(canCmdPackPtr);
    return;
}

//指令包
void Can1ProtRecvProcCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //指令数据包短包处理
        Can1ProtRecvProcCmdFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //指令数据包长包起始包处理
        Can1ProtRecvProcCmdFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //指令数据包长包数据包处理
        Can1ProtRecvProcCmdFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //指令数据包长包结束包处理
        Can1ProtRecvProcCmdFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}


//指令ACK数据包
void Can1ProtRecvProcAckCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //指令ACK数据包必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckCmd Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //发送给自己的ACK,从系统协议栈指令队列中找出相应的指令并处理
    //检查包长度,1字节有效数据标志,3字节cmd,一字节ACK结果
    if(canRecvPackPtr->recvDataLength != 5)
    {
        CAN_PROT_LOG("E>: %s,AckCmd Frame Length Error,Need 5 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为4
    if(canRecvPackPtr->recvDataBufferPtr[0] != 4)
    {
        CAN_PROT_LOG("E>: %s,AckCmd Frame Byte0 Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK数据包
    CAN_PROT_ACK_CMD_PACK* canAckCmdPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_CMD_PACK));
    if(canAckCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_ACK_CMD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckCmdPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckCmdPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckCmdPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckCmdPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckCmdPackPtr->singalChannel = protCanID->singalChannel;
    canAckCmdPackPtr->retryCount = protCanID->retrySendCount;
    //获取指令码
    canAckCmdPackPtr->ackCmdVal = 0;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[3];
    canAckCmdPackPtr->ackCmdVal <<= 8;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[2];
    canAckCmdPackPtr->ackCmdVal <<= 8;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[1];
    //获取ACK代码
    canAckCmdPackPtr->ackCode = (PROT_ACK)(canRecvPackPtr->recvDataBufferPtr[4]);
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckCmdPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canAckCmdPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //转发
        Can1ProtAckCmdForward(canAckCmdPackPtr);
    }
    else
    {
        //自身处理
        Can1ProtAckCmdMsgPackProc(canAckCmdPackPtr);
    }
    //释放内存
    Can1ProtPortMemFree(canAckCmdPackPtr);
}

//结果数据包短包处理
static void Can1ProtRecvProcResultFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查结果最小长度,最小11
    if(validDataByte < 11)
    {
        //报错,有效数据长度小于3
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 11,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个结果包
    CAN_PROT_RESULT_PACK* canResultPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_PACK));
    if(canResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_RESULT_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canResultPackPtr->resultParamBufferLength = validDataByte - 11;
    //根据参数长度进行缓存数据拷贝
    if(canResultPackPtr->resultParamBufferLength == 0)
    {
        canResultPackPtr->resultParamBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canResultPackPtr->resultParamBufferPtr = Can1ProtPortMemMalloc(canResultPackPtr->resultParamBufferLength);
        if(canResultPackPtr->resultParamBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can1ProtPortMemFree(canResultPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc canResultPackPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canResultPackPtr->resultParamBufferPtr,canRecvPackPtr->recvDataBufferPtr+12,
                            canResultPackPtr->resultParamBufferLength);
    }
    //其余元素拷贝
    canResultPackPtr->srcBoardID = protCanID->srcBoardID;
    canResultPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canResultPackPtr->targetBoardID = protCanID->targetBoardID;
    canResultPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canResultPackPtr->singalChannel = protCanID->singalChannel;
    canResultPackPtr->retryCount = protCanID->retrySendCount;
    //解析cmd
    canResultPackPtr->cmdValue = 0;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[3];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[2];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[1];
    //解析结果码
    canResultPackPtr->resultCode = (PROT_RESULT)(canRecvPackPtr->recvDataBufferPtr[4]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[6];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[5];
    canResultPackPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    canResultPackPtr->errorLevelCode = (ERROR_LEVEL)(canRecvPackPtr->recvDataBufferPtr[7]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[11];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[10];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[9];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[8];
    canResultPackPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    if((canResultPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canResultPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can1ProtResultMsgForward(canResultPackPtr);
    }
    else
    {
        //是自己的结果包
        Can1ProtResultMsgPackProc(canResultPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(canResultPackPtr->resultParamBufferPtr);
    Can1ProtPortMemFree(canResultPackPtr);
}

//结果数据包长包起始包处理
static void Can1ProtRecvProcResultFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can1ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can1ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_RESULT,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvResultMsgCachePtr = Can1ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvResultMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can1ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_RESULT,recvResultMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can1ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//结果数据包长包数据包处理
static void Can1ProtRecvProcResultFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T resultMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_RESULT,
                                                                                                (void*)&recvPackCacheHeader);
    if(resultMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvResultMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(resultMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvResultMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can1ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvResultMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can1ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,
                                                                recvResultMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can1ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvResultMsgCachePtr,
                                                    canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can1ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvResultMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvResultMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_RESULT);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_RESULT);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    resultMsgRecvCacheNodePtr = NULL;
    return;
}

//结果数据包长包结束包处理
static void Can1ProtRecvProcResultFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T resultMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_RESULT,
                                                                                                (void*)&recvPackCacheHeader);
    if(resultMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvResultMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(resultMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvResultMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_RESULT);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_RESULT);
        return;
    }
    if(ERROR == Can1ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvResultMsgCachePtr))
    {
        //CRC匹配失败
        recvResultMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        resultMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    //生成一个结果包
    CAN_PROT_RESULT_PACK* canResultPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_PACK));
    if(canResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_RESULT_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvResultMsgCachePtr);
        recvResultMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        resultMsgRecvCacheNodePtr = NULL;
        return;
    }
    canResultPackPtr->resultParamBufferLength = recvResultMsgCachePtr->currentPackAllByteLength - 11;
    //根据参数长度进行缓存数据拷贝
    if(canResultPackPtr->resultParamBufferLength == 0)
    {
        canResultPackPtr->resultParamBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canResultPackPtr->resultParamBufferPtr = Can1ProtPortMemMalloc(canResultPackPtr->resultParamBufferLength);
        if(canResultPackPtr->resultParamBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can1ProtPortMemFree(canResultPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc canResultPackPtr resultParamBufferPtr Failed\r\n",__func__,__LINE__);
            //释放掉这一包
            if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can1ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
            }
            Can1ProtPortMemFree(recvResultMsgCachePtr);
            recvResultMsgCachePtr = NULL;
            //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
                return;
            }
            resultMsgRecvCacheNodePtr = NULL;
            return;
        }
        //数据拷贝
        UserMemCopy(canResultPackPtr->resultParamBufferPtr,recvResultMsgCachePtr->recvDataBufferPtr+11,
                        canResultPackPtr->resultParamBufferLength);
    }
    //其余元素拷贝
    canResultPackPtr->srcBoardID = recvResultMsgCachePtr->srcBoardID;
    canResultPackPtr->srcModuleNo = recvResultMsgCachePtr->srcModuleNo;
    canResultPackPtr->targetBoardID = recvResultMsgCachePtr->targetBoardID;
    canResultPackPtr->targetModuleNo = recvResultMsgCachePtr->targetModuleNo;
    canResultPackPtr->singalChannel = recvResultMsgCachePtr->singalChannel;
    canResultPackPtr->retryCount = recvResultMsgCachePtr->retryCount;
    //解析cmd
    canResultPackPtr->cmdValue = 0;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[2];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[1];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[0];
    //解析结果码
    canResultPackPtr->resultCode = (PROT_RESULT)(recvResultMsgCachePtr->recvDataBufferPtr[3]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[5];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[4];
    canResultPackPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    canResultPackPtr->errorLevelCode = (ERROR_LEVEL)(recvResultMsgCachePtr->recvDataBufferPtr[6]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[10];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[9];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[8];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[7];
    canResultPackPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    //拷贝处理数据完成,清空数据
    if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can1ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
    }
    Can1ProtPortMemFree(recvResultMsgCachePtr);
    recvResultMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
    }
    resultMsgRecvCacheNodePtr = NULL;
    //数据处理
    if((canResultPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canResultPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can1ProtResultMsgForward(canResultPackPtr);
    }
    else
    {
        //是自己的结果包
        Can1ProtResultMsgPackProc(canResultPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(canResultPackPtr->resultParamBufferPtr);
    Can1ProtPortMemFree(canResultPackPtr);
    return;
}

//结果数据包
void Can1ProtRecvProcResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //结果数据包短包处理
        Can1ProtRecvProcResultFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //结果数据包长包起始包处理
        Can1ProtRecvProcResultFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //结果数据包长包数据包处理
        Can1ProtRecvProcResultFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //结果数据包长包结束包处理
        Can1ProtRecvProcResultFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//结果ACK数据包
void Can1ProtRecvProcAckResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //结果ACK数据包必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckResult Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //发送给自己的ACK,从系统协议栈指令队列中找出相应的指令并处理
    //检查包长度,1字节有效数据标志,3字节cmd
    if(canRecvPackPtr->recvDataLength != 4)
    {
        CAN_PROT_LOG("E>: %s,AckResult Frame Length Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为3
    if(canRecvPackPtr->recvDataBufferPtr[0] != 3)
    {
        CAN_PROT_LOG("E>: %s,AckResult Frame Byte0 Error,Need 3 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK结果包
    CAN_PROT_ACK_RESULT_PACK* canAckResultPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_RESULT_PACK));
    if(canAckResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_ACK_RESULT_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckResultPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckResultPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckResultPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckResultPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckResultPackPtr->singalChannel = protCanID->singalChannel;
    canAckResultPackPtr->retryCount = protCanID->retrySendCount;
    //获取指令码
    canAckResultPackPtr->ackCmdVal = 0;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[3];
    canAckResultPackPtr->ackCmdVal <<= 8;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[2];
    canAckResultPackPtr->ackCmdVal <<= 8;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[1];
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckResultPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canAckResultPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //转发
        Can1ProtAckResultForward(canAckResultPackPtr);
    }
    else
    {
        //自身处理
        Can1ProtAckResultMsgPackProc(canAckResultPackPtr);
    }
    //释放内存
    Can1ProtPortMemFree(canAckResultPackPtr);
}

//主动上传数据包短包处理
static void Can1ProtRecvProcDataUploadFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查结果最小长度,最小4
    if(validDataByte < 4)
    {
        //报错,有效数据长度小于4
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 4,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个上报
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_PACK));
    if(canUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canUploadPackPtr->uploadDataBufferLength = validDataByte - 4;
    //根据参数长度进行缓存数据拷贝
    if(canUploadPackPtr->uploadDataBufferLength == 0)
    {
        canUploadPackPtr->uploadDataBufferLength = NULL;
    }
    else
    {
        //生成日志缓存数据
        canUploadPackPtr->uploadDataBufferPtr = Can1ProtPortMemMalloc(canUploadPackPtr->uploadDataBufferLength);
        if(canUploadPackPtr->uploadDataBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can1ProtPortMemFree(canUploadPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc canUploadPackPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canUploadPackPtr->uploadDataBufferPtr,canRecvPackPtr->recvDataBufferPtr+5,
                        canUploadPackPtr->uploadDataBufferLength);
    }
    //其余元素拷贝
    canUploadPackPtr->srcBoardID = protCanID->srcBoardID;
    canUploadPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canUploadPackPtr->targetBoardID = protCanID->targetBoardID;
    canUploadPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canUploadPackPtr->singalChannel = protCanID->singalChannel;
    canUploadPackPtr->retryCount = protCanID->retrySendCount;
    //解析ID
    canUploadPackPtr->uploadID = 0;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[4];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[3];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[2];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[1];
    if((canUploadPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canUploadPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的指令包
        Can1ProtUploadMsgForward(canUploadPackPtr);
    }
    else
    {
        //是自己的指令包
        Can1ProtUploadMsgSelfProc(canUploadPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(canUploadPackPtr->uploadDataBufferPtr);
    Can1ProtPortMemFree(canUploadPackPtr);
}

//主动上传数据包长包起始包处理
static void Can1ProtRecvProcDataUploadFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can1ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can1ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_UPLOAD,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvUploadMsgCachePtr = Can1ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvUploadMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can1ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_UPLOAD,recvUploadMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can1ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//主动上传数据包长包数据包处理
static void Can1ProtRecvProcDataUploadFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T uploadMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_UPLOAD,
                                                                                                (void*)&recvPackCacheHeader);
    if(uploadMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvUploadMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(uploadMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvUploadMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can1ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvUploadMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can1ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,
                                                                recvUploadMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can1ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvUploadMsgCachePtr,
                                                        canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can1ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvUploadMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvUploadMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_UPLOAD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_UPLOAD);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    uploadMsgRecvCacheNodePtr = NULL;
    return;
}

//主动上传数据包长包结束包处理
static void Can1ProtRecvProcDataUploadFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can1ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can1ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T uploadMsgRecvCacheNodePtr = Can1ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_UPLOAD,
                                                                                                (void*)&recvPackCacheHeader);
    if(uploadMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvUploadMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(uploadMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvUploadMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can1ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_UPLOAD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can1ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_UPLOAD);
        return;
    }
    if(ERROR == Can1ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvUploadMsgCachePtr))
    {
        //CRC匹配失败
        recvUploadMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        uploadMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    //生成一个结果包
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_PACK));
    if(canUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(recvUploadMsgCachePtr);
        recvUploadMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        uploadMsgRecvCacheNodePtr = NULL;
        return;
    }
    canUploadPackPtr->uploadDataBufferLength = recvUploadMsgCachePtr->currentPackAllByteLength - 4;
    //根据参数长度进行缓存数据拷贝
    if(canUploadPackPtr->uploadDataBufferLength == 0)
    {
        canUploadPackPtr->uploadDataBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canUploadPackPtr->uploadDataBufferPtr = Can1ProtPortMemMalloc(canUploadPackPtr->uploadDataBufferLength);
        if(canUploadPackPtr->uploadDataBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can1ProtPortMemFree(canUploadPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc canUploadPackPtr resultParamBufferPtr Failed\r\n",__func__,__LINE__);
            //释放掉这一包
            if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can1ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
            }
            Can1ProtPortMemFree(recvUploadMsgCachePtr);
            recvUploadMsgCachePtr = NULL;
            //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
                return;
            }
            uploadMsgRecvCacheNodePtr = NULL;
            return;
        }
        //数据拷贝
        UserMemCopy(canUploadPackPtr->uploadDataBufferPtr,recvUploadMsgCachePtr->recvDataBufferPtr+4,
                        canUploadPackPtr->uploadDataBufferLength);
    }
    //其余元素拷贝
    canUploadPackPtr->srcBoardID = recvUploadMsgCachePtr->srcBoardID;
    canUploadPackPtr->srcModuleNo = recvUploadMsgCachePtr->srcModuleNo;
    canUploadPackPtr->targetBoardID = recvUploadMsgCachePtr->targetBoardID;
    canUploadPackPtr->targetModuleNo = recvUploadMsgCachePtr->targetModuleNo;
    canUploadPackPtr->singalChannel = recvUploadMsgCachePtr->singalChannel;
    canUploadPackPtr->retryCount = recvUploadMsgCachePtr->retryCount;
    //解析upload ID
    canUploadPackPtr->uploadID = 0;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[3];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[2];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[1];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[0];
    //拷贝处理数据完成,清空数据
    if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can1ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
    }
    Can1ProtPortMemFree(recvUploadMsgCachePtr);
    recvUploadMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
    }
    uploadMsgRecvCacheNodePtr = NULL;
    //数据处理
    if((canUploadPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canUploadPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can1ProtUploadMsgForward(canUploadPackPtr);
    }
    else
    {
        //是自己的结果包
        Can1ProtUploadMsgSelfProc(canUploadPackPtr);
    }
    //处理完成,释放内存
    Can1ProtPortMemFree(canUploadPackPtr->uploadDataBufferPtr);
    Can1ProtPortMemFree(canUploadPackPtr);
    return;
}

//主动上传数据包
void Can1ProtRecvProcDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //主动上传数据包短包处理
        Can1ProtRecvProcDataUploadFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //主动上传数据包长包起始包处理
        Can1ProtRecvProcDataUploadFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //主动上传数据包长包数据包处理
        Can1ProtRecvProcDataUploadFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //主动上传数据包长包结束包处理
        Can1ProtRecvProcDataUploadFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//上传数据包的ACK
void Can1ProtRecvProcAckDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //上传数据包的ACK必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckDataUpload Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //检查包长度,1字节有效数据标志,4字节upload id
    if(canRecvPackPtr->recvDataLength != 5)
    {
        CAN_PROT_LOG("E>: %s,AckDataUpload Frame Length Error,Need 5 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为4
    if(canRecvPackPtr->recvDataBufferPtr[0] != 4)
    {
        CAN_PROT_LOG("E>: %s,AckDataUpload Frame Byte0 Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK数据包
    CAN_PROT_ACK_UPLOAD_PACK* canAckUploadPackPtr = Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_UPLOAD_PACK));
    if(canAckUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can1ProtPortMemMalloc CAN_PROT_ACK_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckUploadPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckUploadPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckUploadPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckUploadPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckUploadPackPtr->singalChannel = protCanID->singalChannel;
    canAckUploadPackPtr->retryCount = protCanID->retrySendCount;
    //获取上报ID
    canAckUploadPackPtr->uploadID = 0;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[4];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[3];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[2];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[1];
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckUploadPackPtr->targetModuleNo != can1ProtInitSet.selfModuleNo)||
        (canAckUploadPackPtr->targetBoardID != can1ProtInitSet.selfBoardID))
    {
        //转发
        Can1ProtAckUploadForward(canAckUploadPackPtr);
    }
    else
    {
        //自身处理
        Can1ProtAckUploadMsgPackProc(canAckUploadPackPtr);
    }
    //释放内存
    Can1ProtPortMemFree(canAckUploadPackPtr);
}



















/*******************************************CAN2_PROT*************************************************************/
//指令包的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can2ProtAckCmdMsgPackProc(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr);
//收到的结果包的处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can2ProtResultMsgPackProc(CAN_PROT_RESULT_PACK* resultPackPtr);
//结果上传的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can2ProtAckResultMsgPackProc(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr);
//主动上报的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
extern void Can2ProtAckUploadMsgPackProc(CAN_PROT_ACK_UPLOAD_PACK* uploadPackPtr);

//日志数据包短包处理
static void Can2ProtRecvProcLogFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubLogFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //生成一个日志包
    CAN_PROT_LOG_PACK* logPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_LOG_PACK));
    if(logPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_LOG_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //生成日志缓存数据
    logPackPtr->logDataBufferPtr = Can2ProtPortMemMalloc(validDataByte);
    if(logPackPtr->logDataBufferPtr == NULL)
    {
        //释放之前申请的内存
        Can2ProtPortMemFree(logPackPtr);
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc logDataBufferPtr Failed\r\n",__func__,__LINE__);
        return;
    }
    //拷贝日志数据
    UserMemCopy(logPackPtr->logDataBufferPtr,canRecvPackPtr->recvDataBufferPtr+1,validDataByte);
    //其余元素拷贝
    logPackPtr->srcBoardID = protCanID->srcBoardID;
    logPackPtr->srcModuleNo = protCanID->srcModuleNo;
    logPackPtr->targetBoardID = protCanID->targetBoardID;
    logPackPtr->targetModuleNo = protCanID->targetModuleNo;
    logPackPtr->singalChannel = protCanID->singalChannel;
    logPackPtr->logDataBufferLength = validDataByte;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((logPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (logPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can2ProtLogMsgForward(logPackPtr);
    }
    else
    {
        //是自己的数据
        Can2ProtLogMsgSelfProc(logPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(logPackPtr->logDataBufferPtr);
    Can2ProtPortMemFree(logPackPtr);
}


//从接收ID中创建消息头
static void Can2ProtRecvProcSetCacheHeader(CAN_PROT_RECV_PACK_CACHE_HEADER* recvPackCacheHeaderPtr,CAN_PROT_ID* protCanID)
{
    //填充数据
    recvPackCacheHeaderPtr->srcBoardID = protCanID->srcBoardID;
    recvPackCacheHeaderPtr->srcModuleNo = protCanID->srcModuleNo;
    recvPackCacheHeaderPtr->targetBoardID = protCanID->targetBoardID;
    recvPackCacheHeaderPtr->targetModuleNo = protCanID->targetModuleNo;
    recvPackCacheHeaderPtr->singalChannel = protCanID->singalChannel;
}

//从开始帧中解析信息
static void Can2ProtRecvProcSetFrameStartMsg(CAN_PROT_MSG_FRAME_START* startFrameMsgPtr,CAN_RECV_PACK* canRecvPackPtr)
{
    //预期接收的缓存区总字节长度
    startFrameMsgPtr->exceptPackAllByteLength = 0;
    startFrameMsgPtr->exceptPackAllByteLength += canRecvPackPtr->recvDataBufferPtr[1];
    startFrameMsgPtr->exceptPackAllByteLength <<= 8;
    startFrameMsgPtr->exceptPackAllByteLength += canRecvPackPtr->recvDataBufferPtr[0];
    //预期接收的单包有效数据长度
    startFrameMsgPtr->exceptSinglePackValidByteCount = canRecvPackPtr->recvDataBufferPtr[2];
    //预期接收的最后一包的有效数据长度
    startFrameMsgPtr->exceptLastPackValidByteCount = canRecvPackPtr->recvDataBufferPtr[3];
    //预期接收的数据包总数
    startFrameMsgPtr->exceptPackCountAll = 0;
    startFrameMsgPtr->exceptPackCountAll += canRecvPackPtr->recvDataBufferPtr[5];
    startFrameMsgPtr->exceptPackCountAll <<= 8;
    startFrameMsgPtr->exceptPackCountAll += canRecvPackPtr->recvDataBufferPtr[4];
}

static ADT_CIRCULAR_PTR_LIST_T Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE cacheNo)
{
    if(cacheNo == CAN_PROT_RECV_CACHE_LOG)
    {
        return can2ProtInitSet.listRecvLogMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_CMD)
    {
        return can2ProtInitSet.listRecvCmdMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_RESULT)
    {
        return can2ProtInitSet.listRecvResultMsgCachePtr;
    }
    else if(cacheNo == CAN_PROT_RECV_CACHE_UPLOAD)
    {
        return can2ProtInitSet.listRecvUploadMsgCachePtr;
    }
    else
    {
        return NULL;
    }
}

//从接收缓存链表中清除指定的信息
static ErrorStatus Can2ProtRecvProcClearNodeByCacheHeader(uint8_t* funcNamePtr,CAN_PROT_RECV_CACHE cacheNo,
                                                                        CAN_PROT_RECV_PACK_CACHE_HEADER* cacheHeaderPtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can2ProtRecvProcGetCacheList(cacheNo);
    if(cacheListPtr == NULL)return ERROR;
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T msgRecvCacheNodePtr = ADT_CircularPtrListFindNodeWithData(cacheListPtr,(void*)cacheHeaderPtr,ADT_RecvCacheListElementCmp);
    //检查是否存在相应的节点
    if(msgRecvCacheNodePtr != NULL)
    {
        //释放节点中的元素
        recvMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(msgRecvCacheNodePtr);
        if(recvMsgCachePtr != NULL)
        {
            //释放缓存
            if(recvMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can2ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
            }
            Can2ProtPortMemFree(recvMsgCachePtr);
            recvMsgCachePtr = NULL;
        }
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(cacheListPtr,msgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",funcNamePtr);
            return ERROR;
        }
        msgRecvCacheNodePtr = NULL;
        //一包没收完收到起始包,本身就是异常的一种
        CAN_PROT_LOG("E>: %s, longStartFrame RecvAgain!!\r\n",funcNamePtr);
    }
    return SUCCESS;
}

//创建新的缓存节点
static CAN_PROT_RECV_PACK_CACHE* Can2ProtRecvProcCreateNewCacheNode(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs,
                                                                                        CAN_PROT_MSG_FRAME_START* startFrameMsgPtr)
{
    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr = NULL;
    recvMsgCachePtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RECV_PACK_CACHE));
    if(startFrameMsgPtr == NULL)
    {
        CAN_PROT_LOG("E>: %s, recvMsgCachePtr MemMalloc Failed\r\n",__func__);
        return NULL;
    }
    //初始化数据
    recvMsgCachePtr->targetBoardID = protCanID->targetBoardID;
    recvMsgCachePtr->targetModuleNo = protCanID->targetModuleNo;
    recvMsgCachePtr->srcBoardID = protCanID->srcBoardID;
    recvMsgCachePtr->srcModuleNo = protCanID->srcModuleNo;
    recvMsgCachePtr->singalChannel = protCanID->singalChannel;
    recvMsgCachePtr->longPackStage = CAN_PROT_LONG_PACK_STA_WAIT_DATA;
    recvMsgCachePtr->lastFrameRecvTimeStamp = currentTickMs;
    recvMsgCachePtr->exceptPackAllByteLength = startFrameMsgPtr->exceptPackAllByteLength;
    recvMsgCachePtr->exceptSinglePackValidByteCount = startFrameMsgPtr->exceptSinglePackValidByteCount;
    recvMsgCachePtr->exceptLastPackValidByteCount = startFrameMsgPtr->exceptLastPackValidByteCount;
    recvMsgCachePtr->exceptPackCountAll = startFrameMsgPtr->exceptPackCountAll;
    recvMsgCachePtr->retryCount = protCanID->retrySendCount;
    recvMsgCachePtr->currentPackAllByteLength = 0;
    recvMsgCachePtr->currentPackCountAll = 0;
    //申请缓冲区
    recvMsgCachePtr->recvDataBufferPtr = Can2ProtPortMemMalloc(recvMsgCachePtr->exceptPackAllByteLength);
    if(recvMsgCachePtr->recvDataBufferPtr == NULL)
    {
        CAN_PROT_LOG("E>: %s, recvMsgCachePtr->recvDataBufferPtr MemMalloc Failed\r\n",__func__);
        //释放内存
        Can2ProtPortMemFree(recvMsgCachePtr);
        return NULL;
    }
    return recvMsgCachePtr;
}

//将新的缓存数据压入链表
static ErrorStatus Can2ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE node,CAN_PROT_RECV_PACK_CACHE* recvPackCachePtr)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can2ProtRecvProcGetCacheList(node);
    if(cacheListPtr == NULL)return ERROR;
    //将缓存信息压入链表
    adtResult = ADT_CircularPtrListInsertNodeEnd(cacheListPtr,recvPackCachePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //插入节点失败,释放内存
        if(recvPackCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvPackCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvPackCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//长包起始帧检测
static ErrorStatus Can2ProtRecvProcFrameStartCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength != 6)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongStartFrame DataLength Need 6,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//日志开始包
void Can2ProtRecvProcLogFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can2ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can2ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_LOG,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvLogMsgCachePtr = Can2ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvLogMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can2ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_LOG,recvLogMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can2ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//检查数据包长度是否正确
static ErrorStatus Can2ProtRecvProcFrameDataCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength <= 2)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongDataFrame DataLength Need More Then 2,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//获取指定的元素节点
static ADT_CIRCULAR_PTR_LIST_NODE_T Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE no,CAN_PROT_RECV_PACK_CACHE_HEADER* cacheHeaderPtr)
{
    ADT_CIRCULAR_PTR_LIST_T cacheListPtr = Can2ProtRecvProcGetCacheList(no);
    if(cacheListPtr == NULL)
    {
        return NULL; 
    } 
    return ADT_CircularPtrListFindNodeWithData(cacheListPtr,(void*)cacheHeaderPtr,ADT_RecvCacheListElementCmp);
}

//获取当前包序号
static uint16_t Can2ProtRecvProcDataFrameGetCurrentIndex(CAN_RECV_PACK* canRecvPackPtr)
{
    uint16_t currentPackIndex = 0;
    currentPackIndex += canRecvPackPtr->recvDataBufferPtr[1];
    currentPackIndex <<= 8;
    currentPackIndex += canRecvPackPtr->recvDataBufferPtr[0];
    return currentPackIndex;
}

//检查包序号和包长度是否正确
static ErrorStatus Can2ProtRecvProcDataFrameCheckIndexAndLength(uint8_t* funcName,uint16_t currentPackIndex,uint16_t lastPackIndex,
                                                                                CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,CAN_RECV_PACK* canRecvPackPtr)
{
    //检查数据长度是否正确
    if(currentPackIndex < lastPackIndex)
    {
        //包长度检测
        if(canRecvPackPtr->recvDataLength < recvMsgCachePtr->exceptSinglePackValidByteCount)
        {
            CAN_PROT_LOG("E>: %s, Not Last Pack,LengthNotEnough,Need: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptSinglePackValidByteCount,
                                        canRecvPackPtr->recvDataLength);
            return ERROR;
        }
    }
    else
    {
        //包长度检测
        if(canRecvPackPtr->recvDataLength < recvMsgCachePtr->exceptLastPackValidByteCount)
        {
            CAN_PROT_LOG("E>: %s, Last Pack,LengthNotEnough,Need: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptLastPackValidByteCount,
                                        canRecvPackPtr->recvDataLength);
            return ERROR;
        }
    }
    //检查包序号是否匹配
    if(currentPackIndex != recvMsgCachePtr->currentPackCountAll)
    {
        CAN_PROT_LOG("E>: %s, PackIndex Not Match,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->currentPackCountAll,
                                        currentPackIndex);
        return ERROR;
    }
    return SUCCESS;
}

//数据拷贝
static ErrorStatus Can2ProtRecvProcDataFrameCopyData(uint8_t* funcName,uint16_t currentPackIndex,uint16_t lastPackIndex,
                                                                    CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,CAN_RECV_PACK* canRecvPackPtr)
{   
    uint16_t recvByteCountAll = 0;
    //数据拷贝
    if(currentPackIndex < lastPackIndex)
    {
        //-2是去除两个index占用的位置
        recvByteCountAll = recvMsgCachePtr->currentPackAllByteLength + recvMsgCachePtr->exceptSinglePackValidByteCount -2;
        //不是最后一包,检查长度是否会溢出
        if(recvByteCountAll > recvMsgCachePtr->exceptPackAllByteLength)
        {
            //将会导致数据溢出,不要了
            CAN_PROT_LOG("E>: %s, RecvByteCountOverFlow,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvByteCountAll);
            return ERROR;
        }
        //不会导致数据溢出,拷贝数据
        UserMemCopy(recvMsgCachePtr->recvDataBufferPtr + recvMsgCachePtr->currentPackAllByteLength,canRecvPackPtr->recvDataBufferPtr+2,
                        recvMsgCachePtr->exceptSinglePackValidByteCount -2);
        //接收数据数量增加
        recvMsgCachePtr->currentPackAllByteLength = recvByteCountAll;
        //接收包数量增加
        recvMsgCachePtr->currentPackCountAll += 1;
    }
    else
    {
        //最后一包
        recvByteCountAll = recvMsgCachePtr->currentPackAllByteLength + recvMsgCachePtr->exceptLastPackValidByteCount -2;
        //是最后一包,检查长度是否会溢出
        if(recvByteCountAll > recvMsgCachePtr->exceptPackAllByteLength)
        {
            //将会导致数据溢出,不要了
            CAN_PROT_LOG("E>: %s, RecvByteCountOverFlow,Except: %d,Real: %d\r\n",funcName,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvByteCountAll);
            return ERROR;
        }
        //不会导致数据溢出,拷贝数据
        UserMemCopy(recvMsgCachePtr->recvDataBufferPtr + recvMsgCachePtr->currentPackAllByteLength,canRecvPackPtr->recvDataBufferPtr+2,
                        recvMsgCachePtr->exceptLastPackValidByteCount -2);
        //接收数据数量增加
        recvMsgCachePtr->currentPackAllByteLength = recvByteCountAll;
        //接收包数量增加
        recvMsgCachePtr->currentPackCountAll += 1;
    }
    return SUCCESS;
}

//数据标签更新
static ErrorStatus Can2ProtRecvProcDataFrameLabelReflush(uint8_t* funcName,CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr,uint32_t currentTickMs)
{
    //更新数据接收时间
    recvMsgCachePtr->lastFrameRecvTimeStamp = currentTickMs;
    //包号与数据长度都匹配之后,等待结果帧
    if((recvMsgCachePtr->currentPackAllByteLength == recvMsgCachePtr->exceptPackAllByteLength)&&
        (recvMsgCachePtr->exceptPackCountAll == recvMsgCachePtr->currentPackCountAll))
    {
        recvMsgCachePtr->longPackStage = CAN_PROT_LONG_PACK_STA_WAIT_END;
        return SUCCESS;
    }
    //包号和数据长度有一个匹配有一个不匹配,数据传输异常了
    if((recvMsgCachePtr->currentPackAllByteLength == recvMsgCachePtr->exceptPackAllByteLength)||
        (recvMsgCachePtr->exceptPackCountAll == recvMsgCachePtr->currentPackCountAll))
    {
        CAN_PROT_LOG("E>: %s, RecvByteCount:%d,ExceptByteCount: %d,RecvPackCount: %d,ExceptPackCount: %d\r\n",funcName,
                                        recvMsgCachePtr->currentPackAllByteLength,recvMsgCachePtr->exceptPackAllByteLength,
                                        recvMsgCachePtr->currentPackCountAll,recvMsgCachePtr->exceptPackCountAll);
        //释放掉这一包
        if(recvMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvMsgCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//日志数据包
void Can2ProtRecvProcLogFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T logMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_LOG,(void*)&recvPackCacheHeader);
    if(logMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        return;
    }
    //获取存储的节点指针
    recvLogMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(logMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvLogMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        return;
    }
    uint16_t currentPackIndex = Can2ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvLogMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can2ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvLogMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can2ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvLogMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can2ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvLogMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvLogMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_LOG);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_LOG);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    logMsgRecvCacheNodePtr = NULL;
    return;
}

//检查数据包长度是否正确
static ErrorStatus Can2ProtRecvProcFrameEndCheck(CAN_RECV_PACK* canRecvPackPtr)
{
    if(canRecvPackPtr->recvDataLength != 2)
    {
        //长包起始包6个字节
        CAN_PROT_LOG("E>: %s, LongEndFrame DataLength Need 2,PackAllByte: %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return ERROR;
    }
    return SUCCESS;
}

//CRC16检查
static ErrorStatus Can2ProtRecvProcFrameEndCrc16Check(uint8_t* funcName,CAN_RECV_PACK* canRecvPackPtr,CAN_PROT_RECV_PACK_CACHE* recvMsgCachePtr)
{
    uint16_t crc16CheckRecv = 0;
    crc16CheckRecv += canRecvPackPtr->recvDataBufferPtr[1];
    crc16CheckRecv <<= 8;
    crc16CheckRecv += canRecvPackPtr->recvDataBufferPtr[0];
    //计算CRC16
    uint16_t crc16CheckCalc = 0;
    crc16CheckCalc = CanProtPortCalcCRC16(recvMsgCachePtr->recvDataBufferPtr,recvMsgCachePtr->currentPackAllByteLength);
    //检查CRC是否匹配
    if(crc16CheckRecv != crc16CheckCalc)
    {
        CAN_PROT_LOG("E>: %s, CRC ResultNotMatch,Recv: 0X%08X,Calc: 0X%08X\r\n",funcName,crc16CheckRecv,crc16CheckCalc);
        //释放掉这一包
        if(recvMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvMsgCachePtr);
        return ERROR;
    }
    return SUCCESS;
}

//日志校验包
void Can2ProtRecvProcLogFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvLogMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_LOG);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_LOG);
        return;
    }
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T logMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_LOG,
                                                                                            (void*)&recvPackCacheHeader);
    if(logMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        return;
    }
    //获取存储的节点指针
    recvLogMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(logMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvLogMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //释放掉这一包
        if(recvLogMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvLogMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvLogMsgCachePtr);
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    if(ERROR == Can2ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvLogMsgCachePtr))
    {
        //CRC匹配失败
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    CAN_PROT_LOG_PACK* logPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_LOG_PACK));
    if(logPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_LOG_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvLogMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvLogMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvLogMsgCachePtr);
        recvLogMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        logMsgRecvCacheNodePtr = NULL;
        return;
    }
    //数据拷贝
    logPackPtr->targetBoardID = recvLogMsgCachePtr->targetBoardID;
    logPackPtr->targetModuleNo = recvLogMsgCachePtr->targetModuleNo;
    logPackPtr->srcBoardID = recvLogMsgCachePtr->srcBoardID;
    logPackPtr->srcModuleNo = recvLogMsgCachePtr->srcModuleNo;
    logPackPtr->singalChannel = recvLogMsgCachePtr->singalChannel;
    logPackPtr->logDataBufferLength = recvLogMsgCachePtr->currentPackAllByteLength;
    //数据不用拷贝,直接赋值即可
    logPackPtr->logDataBufferPtr = recvLogMsgCachePtr->recvDataBufferPtr;
    //释放数据
    Can2ProtPortMemFree(recvLogMsgCachePtr);
    recvLogMsgCachePtr = NULL;
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,logMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    logMsgRecvCacheNodePtr = NULL;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((logPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (logPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can2ProtLogMsgForward(logPackPtr);
    }
    else
    {
        //是自己的数据
        Can2ProtLogMsgSelfProc(logPackPtr);
    }
    //处理完成,释放内存
    if(logPackPtr->logDataBufferPtr != NULL)
    {
        Can2ProtPortMemFree(logPackPtr->logDataBufferPtr);
    }
    Can2ProtPortMemFree(logPackPtr);
    return;
}

//日志数据包
void Can2ProtRecvProcLogFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //日志数据包短包处理
        Can2ProtRecvProcLogFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //日志数据包长包起始包处理
        Can2ProtRecvProcLogFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //日志数据包长包数据包处理
        Can2ProtRecvProcLogFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //日志数据包长包结束包处理
        Can2ProtRecvProcLogFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//指令数据包短包处理
static void Can2ProtRecvProcCmdFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查指令最小长度,最小3字节
    if(validDataByte < 3)
    {
        //报错,有效数据长度小于3
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 3,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个指令包
    CAN_PROT_CMD_PACK* canCmdPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_PACK));
    if(canCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canCmdPackPtr->paramBufferLength = validDataByte - 3;
    //根据参数长度进行缓存数据拷贝
    if(canCmdPackPtr->paramBufferLength == 0)
    {
        canCmdPackPtr->paramBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canCmdPackPtr->paramBufferPtr = Can2ProtPortMemMalloc(canCmdPackPtr->paramBufferLength);
        if(canCmdPackPtr->paramBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can2ProtPortMemFree(canCmdPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc cmdDataBufferPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canCmdPackPtr->paramBufferPtr,canRecvPackPtr->recvDataBufferPtr+4,canCmdPackPtr->paramBufferLength);
    }
    //其余元素拷贝
    canCmdPackPtr->srcBoardID = protCanID->srcBoardID;
    canCmdPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canCmdPackPtr->targetBoardID = protCanID->targetBoardID;
    canCmdPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canCmdPackPtr->singalChannel = protCanID->singalChannel;
    canCmdPackPtr->retryCount = protCanID->retrySendCount;
    //解析cmd
    canCmdPackPtr->cmdValue = 0;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[3];
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[2];
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[1];
    if((canCmdPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canCmdPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的指令包
        Can2ProtCmdMsgForward(canCmdPackPtr);
    }
    else
    {
        //是自己的指令包
        Can2ProtCmdMsgSelfProc(canCmdPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(canCmdPackPtr->paramBufferPtr);
    Can2ProtPortMemFree(canCmdPackPtr);
}

//指令开始包
void Can2ProtRecvProcCmdFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can2ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can2ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_CMD,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvCmdMsgCachePtr = Can2ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvCmdMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can2ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_CMD,recvCmdMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can2ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//指令数据包
void Can2ProtRecvProcCmdFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T cmdMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_CMD,(void*)&recvPackCacheHeader);
    if(cmdMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvCmdMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(cmdMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvCmdMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can2ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvCmdMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can2ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvCmdMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can2ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvCmdMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can2ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvCmdMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvCmdMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_CMD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_CMD);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    cmdMsgRecvCacheNodePtr = NULL;
    return;
}

//指令校验包
void Can2ProtRecvProcCmdFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvCmdMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T cmdMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_CMD,(void*)&recvPackCacheHeader);
    if(cmdMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvCmdMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(cmdMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvCmdMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_CMD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_CMD);
        return;
    }
    if(ERROR == Can2ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvCmdMsgCachePtr))
    {
        //CRC匹配失败
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    CAN_PROT_CMD_PACK* canCmdPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_PACK));
    if(canCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvCmdMsgCachePtr);
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        return;
    }
    //数据拷贝
    canCmdPackPtr->targetBoardID = recvCmdMsgCachePtr->targetBoardID;
    canCmdPackPtr->targetModuleNo = recvCmdMsgCachePtr->targetModuleNo;
    canCmdPackPtr->srcBoardID = recvCmdMsgCachePtr->srcBoardID;
    canCmdPackPtr->srcModuleNo = recvCmdMsgCachePtr->srcModuleNo;
    canCmdPackPtr->singalChannel = recvCmdMsgCachePtr->singalChannel;
    canCmdPackPtr->retryCount = recvCmdMsgCachePtr->retryCount;
    canCmdPackPtr->paramBufferLength = recvCmdMsgCachePtr->currentPackAllByteLength - 3;
    //计算指令码
    canCmdPackPtr->cmdValue = 0;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[2]);
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[1]);
    canCmdPackPtr->cmdValue <<= 8;
    canCmdPackPtr->cmdValue += (recvCmdMsgCachePtr->recvDataBufferPtr[0]);
    //申请内存空间存放指令参数
    canCmdPackPtr->paramBufferPtr = Can2ProtPortMemMalloc(canCmdPackPtr->paramBufferLength);
    if(canCmdPackPtr->paramBufferPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_CMD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvCmdMsgCachePtr);
        recvCmdMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        cmdMsgRecvCacheNodePtr = NULL;
        //释放包体
        Can2ProtPortMemFree(canCmdPackPtr);
        return;
    }
    //拷贝数据
    UserMemCopy(canCmdPackPtr->paramBufferPtr,recvCmdMsgCachePtr->recvDataBufferPtr+3,canCmdPackPtr->paramBufferLength);
    //释放数据
    if(recvCmdMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can2ProtPortMemFree(recvCmdMsgCachePtr->recvDataBufferPtr);
    }
    Can2ProtPortMemFree(recvCmdMsgCachePtr);
    recvCmdMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,cmdMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    cmdMsgRecvCacheNodePtr = NULL;
    //是自己的数据,调用自己的数据处理,不是自己的数据,调用转发数据处理
    if((canCmdPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||(canCmdPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的数据
        Can2ProtCmdMsgForward(canCmdPackPtr);
    }
    else
    {
        //是自己的数据
        Can2ProtCmdMsgSelfProc(canCmdPackPtr);
    }
    //处理完成,释放内存
    if(canCmdPackPtr->paramBufferPtr != NULL)
    {
        Can2ProtPortMemFree(canCmdPackPtr->paramBufferPtr);
    }
    Can2ProtPortMemFree(canCmdPackPtr);
    return;
}

//指令包
void Can2ProtRecvProcCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //指令数据包短包处理
        Can2ProtRecvProcCmdFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //指令数据包长包起始包处理
        Can2ProtRecvProcCmdFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //指令数据包长包数据包处理
        Can2ProtRecvProcCmdFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //指令数据包长包结束包处理
        Can2ProtRecvProcCmdFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}


//指令ACK数据包
void Can2ProtRecvProcAckCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //指令ACK数据包必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckCmd Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //发送给自己的ACK,从系统协议栈指令队列中找出相应的指令并处理
    //检查包长度,1字节有效数据标志,3字节cmd,一字节ACK结果
    if(canRecvPackPtr->recvDataLength != 5)
    {
        CAN_PROT_LOG("E>: %s,AckCmd Frame Length Error,Need 5 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为4
    if(canRecvPackPtr->recvDataBufferPtr[0] != 4)
    {
        CAN_PROT_LOG("E>: %s,AckCmd Frame Byte0 Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK数据包
    CAN_PROT_ACK_CMD_PACK* canAckCmdPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_CMD_PACK));
    if(canAckCmdPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_ACK_CMD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckCmdPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckCmdPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckCmdPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckCmdPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckCmdPackPtr->singalChannel = protCanID->singalChannel;
    canAckCmdPackPtr->retryCount = protCanID->retrySendCount;
    //获取指令码
    canAckCmdPackPtr->ackCmdVal = 0;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[3];
    canAckCmdPackPtr->ackCmdVal <<= 8;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[2];
    canAckCmdPackPtr->ackCmdVal <<= 8;
    canAckCmdPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[1];
    //获取ACK代码
    canAckCmdPackPtr->ackCode = (PROT_ACK)(canRecvPackPtr->recvDataBufferPtr[4]);
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckCmdPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canAckCmdPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //转发
        Can2ProtAckCmdForward(canAckCmdPackPtr);
    }
    else
    {
        //自身处理
        Can2ProtAckCmdMsgPackProc(canAckCmdPackPtr);
    }
    //释放内存
    Can2ProtPortMemFree(canAckCmdPackPtr);
}

//结果数据包短包处理
static void Can2ProtRecvProcResultFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查结果最小长度,最小11
    if(validDataByte < 11)
    {
        //报错,有效数据长度小于3
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 11,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个结果包
    CAN_PROT_RESULT_PACK* canResultPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_PACK));
    if(canResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_RESULT_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canResultPackPtr->resultParamBufferLength = validDataByte - 11;
    //根据参数长度进行缓存数据拷贝
    if(canResultPackPtr->resultParamBufferLength == 0)
    {
        canResultPackPtr->resultParamBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canResultPackPtr->resultParamBufferPtr = Can2ProtPortMemMalloc(canResultPackPtr->resultParamBufferLength);
        if(canResultPackPtr->resultParamBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can2ProtPortMemFree(canResultPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc canResultPackPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canResultPackPtr->resultParamBufferPtr,canRecvPackPtr->recvDataBufferPtr+12,canResultPackPtr->resultParamBufferLength);
    }
    //其余元素拷贝
    canResultPackPtr->srcBoardID = protCanID->srcBoardID;
    canResultPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canResultPackPtr->targetBoardID = protCanID->targetBoardID;
    canResultPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canResultPackPtr->singalChannel = protCanID->singalChannel;
    canResultPackPtr->retryCount = protCanID->retrySendCount;
    //解析cmd
    canResultPackPtr->cmdValue = 0;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[3];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[2];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += canRecvPackPtr->recvDataBufferPtr[1];
    //解析结果码
    canResultPackPtr->resultCode = (PROT_RESULT)(canRecvPackPtr->recvDataBufferPtr[4]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[6];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[5];
    canResultPackPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    canResultPackPtr->errorLevelCode = (ERROR_LEVEL)(canRecvPackPtr->recvDataBufferPtr[7]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[11];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[10];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[9];
    utilDataTemp <<= 8;
    utilDataTemp += canRecvPackPtr->recvDataBufferPtr[8];
    canResultPackPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    if((canResultPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canResultPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can2ProtResultMsgForward(canResultPackPtr);
    }
    else
    {
        //是自己的结果包
        Can2ProtResultMsgPackProc(canResultPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(canResultPackPtr->resultParamBufferPtr);
    Can2ProtPortMemFree(canResultPackPtr);
}

//结果数据包长包起始包处理
static void Can2ProtRecvProcResultFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can2ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can2ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_RESULT,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvResultMsgCachePtr = Can2ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvResultMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can2ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_RESULT,recvResultMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can2ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//结果数据包长包数据包处理
static void Can2ProtRecvProcResultFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T resultMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_RESULT,(void*)&recvPackCacheHeader);
    if(resultMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvResultMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(resultMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvResultMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can2ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvResultMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can2ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvResultMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can2ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvResultMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can2ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvResultMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvResultMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_RESULT);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_RESULT);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    resultMsgRecvCacheNodePtr = NULL;
    return;
}

//结果数据包长包结束包处理
static void Can2ProtRecvProcResultFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvResultMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T resultMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_RESULT,(void*)&recvPackCacheHeader);
    if(resultMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvResultMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(resultMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvResultMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_RESULT);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_RESULT);
        return;
    }
    if(ERROR == Can2ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvResultMsgCachePtr))
    {
        //CRC匹配失败
        recvResultMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        resultMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    //生成一个结果包
    CAN_PROT_RESULT_PACK* canResultPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_PACK));
    if(canResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_RESULT_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvResultMsgCachePtr);
        recvResultMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        resultMsgRecvCacheNodePtr = NULL;
        return;
    }
    canResultPackPtr->resultParamBufferLength = recvResultMsgCachePtr->currentPackAllByteLength - 11;
    //根据参数长度进行缓存数据拷贝
    if(canResultPackPtr->resultParamBufferLength == 0)
    {
        canResultPackPtr->resultParamBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canResultPackPtr->resultParamBufferPtr = Can2ProtPortMemMalloc(canResultPackPtr->resultParamBufferLength);
        if(canResultPackPtr->resultParamBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can2ProtPortMemFree(canResultPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc canResultPackPtr resultParamBufferPtr Failed\r\n",__func__,__LINE__);
            //释放掉这一包
            if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can2ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
            }
            Can2ProtPortMemFree(recvResultMsgCachePtr);
            recvResultMsgCachePtr = NULL;
            //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
                return;
            }
            resultMsgRecvCacheNodePtr = NULL;
            return;
        }
        //数据拷贝
        UserMemCopy(canResultPackPtr->resultParamBufferPtr,recvResultMsgCachePtr->recvDataBufferPtr+11,canResultPackPtr->resultParamBufferLength);
    }
    //其余元素拷贝
    canResultPackPtr->srcBoardID = recvResultMsgCachePtr->srcBoardID;
    canResultPackPtr->srcModuleNo = recvResultMsgCachePtr->srcModuleNo;
    canResultPackPtr->targetBoardID = recvResultMsgCachePtr->targetBoardID;
    canResultPackPtr->targetModuleNo = recvResultMsgCachePtr->targetModuleNo;
    canResultPackPtr->singalChannel = recvResultMsgCachePtr->singalChannel;
    canResultPackPtr->retryCount = recvResultMsgCachePtr->retryCount;
    //解析cmd
    canResultPackPtr->cmdValue = 0;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[2];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[1];
    canResultPackPtr->cmdValue <<= 8;
    canResultPackPtr->cmdValue += recvResultMsgCachePtr->recvDataBufferPtr[0];
    //解析结果码
    canResultPackPtr->resultCode = (PROT_RESULT)(recvResultMsgCachePtr->recvDataBufferPtr[3]);
    //解析错误主码
    uint32_t utilDataTemp = 0;
    utilDataTemp = 0;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[5];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[4];
    canResultPackPtr->errorMainCode = (ERROR_MAIN)utilDataTemp;
    //解析错误级别
    canResultPackPtr->errorLevelCode = (ERROR_LEVEL)(recvResultMsgCachePtr->recvDataBufferPtr[6]);
    //解析错误子码
    utilDataTemp = 0;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[10];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[9];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[8];
    utilDataTemp <<= 8;
    utilDataTemp += recvResultMsgCachePtr->recvDataBufferPtr[7];
    canResultPackPtr->errorSubCode = (ERROR_SUB)utilDataTemp;
    //拷贝处理数据完成,清空数据
    if(recvResultMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can2ProtPortMemFree(recvResultMsgCachePtr->recvDataBufferPtr);
    }
    Can2ProtPortMemFree(recvResultMsgCachePtr);
    recvResultMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,resultMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
    }
    resultMsgRecvCacheNodePtr = NULL;
    //数据处理
    if((canResultPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canResultPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can2ProtResultMsgForward(canResultPackPtr);
    }
    else
    {
        //是自己的结果包
        Can2ProtResultMsgPackProc(canResultPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(canResultPackPtr->resultParamBufferPtr);
    Can2ProtPortMemFree(canResultPackPtr);
    return;
}

//结果数据包
void Can2ProtRecvProcResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //结果数据包短包处理
        Can2ProtRecvProcResultFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //结果数据包长包起始包处理
        Can2ProtRecvProcResultFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //结果数据包长包数据包处理
        Can2ProtRecvProcResultFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //结果数据包长包结束包处理
        Can2ProtRecvProcResultFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//结果ACK数据包
void Can2ProtRecvProcAckResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //结果ACK数据包必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckResult Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //发送给自己的ACK,从系统协议栈指令队列中找出相应的指令并处理
    //检查包长度,1字节有效数据标志,3字节cmd
    if(canRecvPackPtr->recvDataLength != 4)
    {
        CAN_PROT_LOG("E>: %s,AckResult Frame Length Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为3
    if(canRecvPackPtr->recvDataBufferPtr[0] != 3)
    {
        CAN_PROT_LOG("E>: %s,AckResult Frame Byte0 Error,Need 3 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK结果包
    CAN_PROT_ACK_RESULT_PACK* canAckResultPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_RESULT_PACK));
    if(canAckResultPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_ACK_RESULT_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckResultPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckResultPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckResultPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckResultPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckResultPackPtr->singalChannel = protCanID->singalChannel;
    canAckResultPackPtr->retryCount = protCanID->retrySendCount;
    //获取指令码
    canAckResultPackPtr->ackCmdVal = 0;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[3];
    canAckResultPackPtr->ackCmdVal <<= 8;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[2];
    canAckResultPackPtr->ackCmdVal <<= 8;
    canAckResultPackPtr->ackCmdVal += canRecvPackPtr->recvDataBufferPtr[1];
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckResultPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canAckResultPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //转发
        Can2ProtAckResultForward(canAckResultPackPtr);
    }
    else
    {
        //自身处理
        Can2ProtAckResultMsgPackProc(canAckResultPackPtr);
    }
    //释放内存
    Can2ProtPortMemFree(canAckResultPackPtr);
}

//主动上传数据包短包处理
static void Can2ProtRecvProcDataUploadFrameShort(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr)
{
    //短帧,先获取有效数据长度,再去取数据
    uint8_t validDataByte = canRecvPackPtr->recvDataBufferPtr[0];
    //检查数据总长度对不对
    if((validDataByte + 1) > canRecvPackPtr->recvDataLength)
    {
        //报错,包长度和预期长度不匹配
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte: %d,PackAllByte: %d\r\n",__func__,validDataByte,
                                    canRecvPackPtr->recvDataLength);
        return;
    }
    //检查结果最小长度,最小4
    if(validDataByte < 4)
    {
        //报错,有效数据长度小于4
        CAN_PROT_LOG("E>: %s, RecvSubCmdFrame ValidDataByte NeedMoreThen 4,Real: %d\r\n",__func__,validDataByte);
        return;
    }
    //生成一个上报
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_PACK));
    if(canUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //参数长度
    canUploadPackPtr->uploadDataBufferLength = validDataByte - 4;
    //根据参数长度进行缓存数据拷贝
    if(canUploadPackPtr->uploadDataBufferLength == 0)
    {
        canUploadPackPtr->uploadDataBufferLength = NULL;
    }
    else
    {
        //生成日志缓存数据
        canUploadPackPtr->uploadDataBufferPtr = Can2ProtPortMemMalloc(canUploadPackPtr->uploadDataBufferLength);
        if(canUploadPackPtr->uploadDataBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can2ProtPortMemFree(canUploadPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc canUploadPackPtr Failed\r\n",__func__,__LINE__);
            return;
        }
        //数据拷贝
        UserMemCopy(canUploadPackPtr->uploadDataBufferPtr,canRecvPackPtr->recvDataBufferPtr+5,canUploadPackPtr->uploadDataBufferLength);
    }
    //其余元素拷贝
    canUploadPackPtr->srcBoardID = protCanID->srcBoardID;
    canUploadPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canUploadPackPtr->targetBoardID = protCanID->targetBoardID;
    canUploadPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canUploadPackPtr->singalChannel = protCanID->singalChannel;
    canUploadPackPtr->retryCount = protCanID->retrySendCount;
    //解析ID
    canUploadPackPtr->uploadID = 0;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[4];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[3];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[2];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[1];
    if((canUploadPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canUploadPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的指令包
        Can2ProtUploadMsgForward(canUploadPackPtr);
    }
    else
    {
        //是自己的指令包
        Can2ProtUploadMsgSelfProc(canUploadPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(canUploadPackPtr->uploadDataBufferPtr);
    Can2ProtPortMemFree(canUploadPackPtr);
}

//主动上传数据包长包起始包处理
static void Can2ProtRecvProcDataUploadFrameLongStart(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameStartCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //解析长包起始帧消息头
    CAN_PROT_MSG_FRAME_START startFrameMsg;
    Can2ProtRecvProcSetFrameStartMsg(&startFrameMsg,canRecvPackPtr);
    //从日志链表中 清除可能存在的重复冗余信息
    if(ERROR == Can2ProtRecvProcClearNodeByCacheHeader((uint8_t*)__func__,CAN_PROT_RECV_CACHE_UPLOAD,&recvPackCacheHeader))
    {
        return;
    }
    //生成新的节点
    recvUploadMsgCachePtr = Can2ProtRecvProcCreateNewCacheNode(protCanID,canRecvPackPtr,currentTickMs,&startFrameMsg);
    if(recvUploadMsgCachePtr == NULL)
    {
        return;
    }
    //新节点插入链表
    if(ERROR == Can2ProtRecvProcPushNewCacheNode(CAN_PROT_RECV_CACHE_UPLOAD,recvUploadMsgCachePtr))
    {
        CAN_PROT_LOG("E>: %s, Can2ProtRecvProcPushNewCacheNode Failed\r\n",__func__);
        return;
    }
    return;
}

//主动上传数据包长包数据包处理
static void Can2ProtRecvProcDataUploadFrameLongData(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameDataCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T uploadMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_UPLOAD,(void*)&recvPackCacheHeader);
    if(uploadMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvUploadMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(uploadMsgRecvCacheNodePtr);
    //检查当前是不是在等待数据包状态
    if(recvUploadMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_DATA)
    {
        //当前状态不是在等待数据包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitData\r\n",__func__);
        return;
    }
    uint16_t currentPackIndex = Can2ProtRecvProcDataFrameGetCurrentIndex(canRecvPackPtr);
    //最后一包的序号
    uint16_t lastPackIndex = recvUploadMsgCachePtr->exceptPackCountAll - 1;
    //检查包序号和包长度是否正确
    if(ERROR == Can2ProtRecvProcDataFrameCheckIndexAndLength((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvUploadMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //数据拷贝
    if(ERROR == Can2ProtRecvProcDataFrameCopyData((uint8_t*)__func__,currentPackIndex,lastPackIndex,recvUploadMsgCachePtr,canRecvPackPtr))
    {
        return;
    }
    //标签更新
    if(SUCCESS == Can2ProtRecvProcDataFrameLabelReflush((uint8_t*)__func__,recvUploadMsgCachePtr,currentTickMs))
    {
        return;
    }
    //发生异常,清除节点
    recvUploadMsgCachePtr = NULL;
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_UPLOAD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_UPLOAD);
        return;
    }
    //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
        return;
    }
    uploadMsgRecvCacheNodePtr = NULL;
    return;
}

//主动上传数据包长包结束包处理
static void Can2ProtRecvProcDataUploadFrameLongEnd(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先看格式对不对
    if(ERROR == Can2ProtRecvProcFrameEndCheck(canRecvPackPtr))
    {
        return;
    }
    //缓存信息
    CAN_PROT_RECV_PACK_CACHE* recvUploadMsgCachePtr = NULL;
    //生成一个缓存包头
    CAN_PROT_RECV_PACK_CACHE_HEADER recvPackCacheHeader;
    //填充数据
    Can2ProtRecvProcSetCacheHeader(&recvPackCacheHeader,protCanID);
    //从链表中查找指定元素节点
    ADT_CIRCULAR_PTR_LIST_NODE_T uploadMsgRecvCacheNodePtr = Can2ProtRecvProcGetNodeByHeader(CAN_PROT_RECV_CACHE_UPLOAD,(void*)&recvPackCacheHeader);
    if(uploadMsgRecvCacheNodePtr == NULL)
    {
        //链表中不存在这个数据的缓存,这一包丢掉
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr StartMsg Not Exist\r\n",__func__);
        return;
    }
    //获取存储的节点指针
    recvUploadMsgCachePtr = (CAN_PROT_RECV_PACK_CACHE*)ADT_CircularPtrListGetNodeVal(uploadMsgRecvCacheNodePtr);
    //检查当前是不是在等待结果包状态
    if(recvUploadMsgCachePtr->longPackStage != CAN_PROT_LONG_PACK_STA_WAIT_END)
    {
        //当前状态不是在等待结果包
        CAN_PROT_LOG("E>: %s, listRecvMsgCachePtr Stage Not WaitEnd\r\n",__func__);
        return;
    }
    ADT_CIRCULAR_PTR_LIST_T listPtr = Can2ProtRecvProcGetCacheList(CAN_PROT_RECV_CACHE_UPLOAD);
    if(listPtr == NULL)
    {
        CAN_PROT_LOG("E>: Can2ProtRecvProcGetCacheList Failed: %d\r\n",CAN_PROT_RECV_CACHE_UPLOAD);
        return;
    }
    if(ERROR == Can2ProtRecvProcFrameEndCrc16Check((uint8_t*)__func__,canRecvPackPtr,recvUploadMsgCachePtr))
    {
        //CRC匹配失败
        recvUploadMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        uploadMsgRecvCacheNodePtr = NULL;
        return;
    }
    //校验通过,将数据提取出来并转发
    //生成一个结果包
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_PACK));
    if(canUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        //释放掉这一包
        if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(recvUploadMsgCachePtr);
        recvUploadMsgCachePtr = NULL;
        //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
            return;
        }
        uploadMsgRecvCacheNodePtr = NULL;
        return;
    }
    canUploadPackPtr->uploadDataBufferLength = recvUploadMsgCachePtr->currentPackAllByteLength - 4;
    //根据参数长度进行缓存数据拷贝
    if(canUploadPackPtr->uploadDataBufferLength == 0)
    {
        canUploadPackPtr->uploadDataBufferPtr = NULL;
    }
    else
    {
        //生成日志缓存数据
        canUploadPackPtr->uploadDataBufferPtr = Can2ProtPortMemMalloc(canUploadPackPtr->uploadDataBufferLength);
        if(canUploadPackPtr->uploadDataBufferPtr == NULL)
        {
            //释放之前申请的内存
            Can2ProtPortMemFree(canUploadPackPtr);
            //内存申请失败
            CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc canUploadPackPtr resultParamBufferPtr Failed\r\n",__func__,__LINE__);
            //释放掉这一包
            if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
            {
                Can2ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
            }
            Can2ProtPortMemFree(recvUploadMsgCachePtr);
            recvUploadMsgCachePtr = NULL;
            //内存中已经存在这个节点了,把内存中的节点清除掉,保存新的节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
                return;
            }
            uploadMsgRecvCacheNodePtr = NULL;
            return;
        }
        //数据拷贝
        UserMemCopy(canUploadPackPtr->uploadDataBufferPtr,recvUploadMsgCachePtr->recvDataBufferPtr+4,canUploadPackPtr->uploadDataBufferLength);
    }
    //其余元素拷贝
    canUploadPackPtr->srcBoardID = recvUploadMsgCachePtr->srcBoardID;
    canUploadPackPtr->srcModuleNo = recvUploadMsgCachePtr->srcModuleNo;
    canUploadPackPtr->targetBoardID = recvUploadMsgCachePtr->targetBoardID;
    canUploadPackPtr->targetModuleNo = recvUploadMsgCachePtr->targetModuleNo;
    canUploadPackPtr->singalChannel = recvUploadMsgCachePtr->singalChannel;
    canUploadPackPtr->retryCount = recvUploadMsgCachePtr->retryCount;
    //解析upload ID
    canUploadPackPtr->uploadID = 0;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[3];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[2];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[1];
    canUploadPackPtr->uploadID <<= 8;
    canUploadPackPtr->uploadID += recvUploadMsgCachePtr->recvDataBufferPtr[0];
    //拷贝处理数据完成,清空数据
    if(recvUploadMsgCachePtr->recvDataBufferPtr != NULL)
    {
        Can2ProtPortMemFree(recvUploadMsgCachePtr->recvDataBufferPtr);
    }
    Can2ProtPortMemFree(recvUploadMsgCachePtr);
    recvUploadMsgCachePtr = NULL;
    //把内存中的节点清除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(listPtr,uploadMsgRecvCacheNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s, ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__);
    }
    uploadMsgRecvCacheNodePtr = NULL;
    //数据处理
    if((canUploadPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canUploadPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //不是自己的结果包
        Can2ProtUploadMsgForward(canUploadPackPtr);
    }
    else
    {
        //是自己的结果包
        Can2ProtUploadMsgSelfProc(canUploadPackPtr);
    }
    //处理完成,释放内存
    Can2ProtPortMemFree(canUploadPackPtr->uploadDataBufferPtr);
    Can2ProtPortMemFree(canUploadPackPtr);
    return;
}

//主动上传数据包
void Can2ProtRecvProcDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //检查数据帧类型
    if(protCanID->frameType == CAN_PROT_FRAME_TYPE_SHORT)
    {
        //主动上传数据包短包处理
        Can2ProtRecvProcDataUploadFrameShort(protCanID,canRecvPackPtr);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_START)
    {
        //主动上传数据包长包起始包处理
        Can2ProtRecvProcDataUploadFrameLongStart(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_DATA)
    {
        //主动上传数据包长包数据包处理
        Can2ProtRecvProcDataUploadFrameLongData(protCanID,canRecvPackPtr,currentTickMs);
    }
    else if(protCanID->frameType == CAN_PROT_FRAME_TYPE_LONG_END)
    {
        //主动上传数据包长包结束包处理
        Can2ProtRecvProcDataUploadFrameLongEnd(protCanID,canRecvPackPtr,currentTickMs);
    }
    else
    {
        //不支持的帧类型
        CAN_PROT_LOG("E>: %s, %d,UnSupport  FrameType: %d\r\n",__func__,__LINE__,(uint8_t)protCanID->frameType);
    }
}

//上传数据包的ACK
void Can2ProtRecvProcAckDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //上传数据包的ACK必须是短包,其他包不处理
    if(protCanID->frameType != CAN_PROT_FRAME_TYPE_SHORT)
    {
        //错误打印
        CAN_PROT_LOG("E>: %s, AckDataUpload Frame Not Short : %d\r\n",__func__,protCanID->frameType);
    }
    //检查包长度,1字节有效数据标志,4字节upload id
    if(canRecvPackPtr->recvDataLength != 5)
    {
        CAN_PROT_LOG("E>: %s,AckDataUpload Frame Length Error,Need 5 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataLength);
        return;
    }
    //检查第一个字节长度,必须为4
    if(canRecvPackPtr->recvDataBufferPtr[0] != 4)
    {
        CAN_PROT_LOG("E>: %s,AckDataUpload Frame Byte0 Error,Need 4 ,Real %d\r\n",__func__,canRecvPackPtr->recvDataBufferPtr[0]);
        return;
    }
    //生成ACK数据包
    CAN_PROT_ACK_UPLOAD_PACK* canAckUploadPackPtr = Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_UPLOAD_PACK));
    if(canAckUploadPackPtr == NULL)
    {
        //内存申请失败
        CAN_PROT_LOG("E>: %s, %d,Can2ProtPortMemMalloc CAN_PROT_ACK_UPLOAD_PACK Failed\r\n",__func__,__LINE__);
        return;
    }
    //数据赋值
    canAckUploadPackPtr->srcBoardID = protCanID->srcBoardID;
    canAckUploadPackPtr->srcModuleNo = protCanID->srcModuleNo;
    canAckUploadPackPtr->targetBoardID = protCanID->targetBoardID;
    canAckUploadPackPtr->targetModuleNo = protCanID->targetModuleNo;
    canAckUploadPackPtr->singalChannel = protCanID->singalChannel;
    canAckUploadPackPtr->retryCount = protCanID->retrySendCount;
    //获取上报ID
    canAckUploadPackPtr->uploadID = 0;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[4];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[3];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[2];
    canAckUploadPackPtr->uploadID <<= 8;
    canAckUploadPackPtr->uploadID += canRecvPackPtr->recvDataBufferPtr[1];
    //指令ACK,如果不是发给自己的,说明是转发数据包
    if((canAckUploadPackPtr->targetModuleNo != can2ProtInitSet.selfModuleNo)||
        (canAckUploadPackPtr->targetBoardID != can2ProtInitSet.selfBoardID))
    {
        //转发
        Can2ProtAckUploadForward(canAckUploadPackPtr);
    }
    else
    {
        //自身处理
        Can2ProtAckUploadMsgPackProc(canAckUploadPackPtr);
    }
    //释放内存
    Can2ProtPortMemFree(canAckUploadPackPtr);
}




