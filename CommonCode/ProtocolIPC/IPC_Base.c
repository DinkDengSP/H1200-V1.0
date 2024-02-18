/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 16:47:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 19:33:08 +0800
************************************************************************************************/ 
#include "IPC_Base.h"
#include "IPC_Port.h"
#include "tx_mutex.h"

//设置指令包IPC默认值
void IPC_BaseSetCmdPackDefault(IPC_CMD_BASE_DATA* cmdBaseDataPtr)
{
    cmdBaseDataPtr->commandCode = 0X00000000;
    cmdBaseDataPtr->cmdParam1 = 0X00000000;
    cmdBaseDataPtr->cmdParam2 = 0X00000000;
    cmdBaseDataPtr->cmdParam3 = 0X00000000;
    cmdBaseDataPtr->cmdParam4 = 0X00000000;
    cmdBaseDataPtr->cmdParam5 = 0X00000000;
    cmdBaseDataPtr->cmdParamBufferPtr = NULL;
    cmdBaseDataPtr->cmdParamBufferLength = 0X0000;
}

//发送上传包
ERROR_SUB IPC_BaseSendUploadPack(TX_QUEUE* queuePtr,uint32_t uploadID,uint8_t* bufferPtr,uint16_t bufferLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IPC_UPLOAD_DATA* ipcUploadPackPtr = NULL;
    //检查对应信道是否为空,为空则返回不支持该指令
    if(queuePtr == NULL)
    {
        //不支持的上传ID
        IPC_LOG("E:> %s,%d,TargetQueue NULL UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadID);          
        return ERROR_SUB_BOARD_IPC_UPLOAD_TARGET_QUEUE_NULL;                                    
    }
    //获取转发信道支持,申请内存
    ipcUploadPackPtr = IPC_PortMemMalloc(DIM_STRUCT_LENGTH_BYTE(IPC_UPLOAD_DATA));
    //检测
    if(ipcUploadPackPtr == NULL)
    {
        //内存申请失败
        IPC_LOG("E:> %s,%d,MemMallocFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,uploadID);         
        return ERROR_SUB_BOARD_IPC_UPLOAD_MEM_MALLOC_FAILED;
    }
    if(bufferLength != 0)
    {
        //申请参数缓存
        ipcUploadPackPtr->dataBufferPtr = IPC_PortMemMalloc(bufferLength);
        if(ipcUploadPackPtr->dataBufferPtr == NULL)
        {
            IPC_PortMemFree(ipcUploadPackPtr);
            //内存申请失败
            IPC_LOG("E:> %s,%d,MemMallocBufferFailed UploadID:: 0X%08X\r\n",__func__,__LINE__,ipcUploadPackPtr->uploadID);         
            return ERROR_SUB_BOARD_IPC_UPLOAD_MEM_MALLOC_BUFF_FAILED;
        }
    }
    else
    {
        ipcUploadPackPtr->dataBufferPtr = NULL;
    }
    //数据拷贝哦
    ipcUploadPackPtr->uploadID = uploadID;
    ipcUploadPackPtr->dataLength = bufferLength;
    //拷贝
    if(bufferLength != 0)
    {
        UserMemCopy(ipcUploadPackPtr->dataBufferPtr,bufferPtr,bufferLength);
    }
    //数据包发送
    errorSub = IPC_PortSendUploadPack(queuePtr,ipcUploadPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //上传包发送失败
        IPC_LOG("E:> %s,%d,IPC_PortSendUploadPack UploadID:: 0X%08X,ErrorSub: 0X%08X\r\n",
                            __func__,__LINE__,ipcUploadPackPtr->uploadID,errorSub);
        if(ipcUploadPackPtr->dataBufferPtr != NULL)
        {
            //释放内存
            IPC_PortMemFree(ipcUploadPackPtr->dataBufferPtr);
            IPC_PortMemFree(ipcUploadPackPtr);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//初始化通道数据
void IPC_BaseInitChannelData(IPC_CHANNEL_DATA* ipcChannelDataPtr,uint8_t* channelNameStr,TX_QUEUE* targetQueuePtr,
                                uint16_t dataBufferLength)
{
    //初始化互斥信号量
    if(ipcChannelDataPtr->mutexDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&(ipcChannelDataPtr->mutexDataAccess),(CHAR*)channelNameStr,TX_INHERIT);
    //初始化等待结果链表
    ipcChannelDataPtr->waitResultList = ADT_CircularPtrListCreate(IPC_PortMemMalloc,IPC_PortMemFree,
                                                                    CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                    MCU_RTT_SendStringInt);
    //设置目标队列
    ipcChannelDataPtr->targetQueuePtr = targetQueuePtr;
    //申请共享内存
    if(dataBufferLength == 0)
    {
        ipcChannelDataPtr->channelCache.dataBufferPtr = NULL;
    }
    else
    {
        ipcChannelDataPtr->channelCache.dataBufferPtr = IPC_PortMemMallocWhileSuccess(dataBufferLength);
    }
    //携带缓存长度为指定长度
    ipcChannelDataPtr->channelCache.carryDataBufferLength = dataBufferLength;
    //返回缓存数据长度为0
    ipcChannelDataPtr->channelCache.returnDataBufferLength = 0;
    //设置上一个发出的指令为无效指令
    ipcChannelDataPtr->lastSendCommand = IPC_BASE_CMD_RESERVE;
}

//获取通道互斥信号量
static void IPC_BaseGetLock(IPC_CHANNEL_DATA* channelDataPtr)
{
    tx_mutex_get(&(channelDataPtr->mutexDataAccess),TX_WAIT_FOREVER);
}
//释放锁
static void IPC_BaseReleaseLock(IPC_CHANNEL_DATA* channelDataPtr)
{
    tx_mutex_put(&(channelDataPtr->mutexDataAccess));
}

//从链表中找出指定的节点,元素内容匹配.使用指令头查找
static ADT_CMP_RESULT ADT_IpcCmdWaitElementFindWithHeader(void* cmpSrc,void* cmpDst)
{
    IPC_CMD_WAIT_RESULT* cmdWaitPackPtrSrc = (IPC_CMD_WAIT_RESULT*)cmpSrc;
    IPC_CMD_NODE_HEADER* cmdWaitPackHeaderPtrDst = (IPC_CMD_NODE_HEADER*)cmpDst;
    if(cmdWaitPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackHeaderPtrDst == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->commandCode != cmdWaitPackHeaderPtrDst->commandCode)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->sendCommandThreadID != cmdWaitPackHeaderPtrDst->sendCommandThreadID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_IpcCmdWaitPackElementCmp(void* cmpSrc,void* cmpDst)
{
    IPC_CMD_WAIT_RESULT* cmdWaitPackPtrSrc = (IPC_CMD_WAIT_RESULT*)cmpSrc;
    IPC_CMD_WAIT_RESULT* cmdWaitPackPtrDst = (IPC_CMD_WAIT_RESULT*)cmpDst;
    //元素比较,直接比较指针
    if(cmdWaitPackPtrSrc == cmdWaitPackPtrDst)
    {
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//IPC指令发送并等待ACK返回
ERROR_SUB IPC_BaseSendCmdWhileAck(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_BASE_DATA* cmdBaseDataPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    if(channelDataPtr == NULL)
    {
        IPC_LOG("E>: %s,%d,channelDataPtr NULL\r\n",__func__,__LINE__);
        return ERROR_SUB_BOARD_IPC_CMD_CHANNEL_PTR_NULL;
    }
    if(cmdBaseDataPtr == NULL)
    {
        IPC_LOG("E>: %s,%d,cmdBaseDataPtr NULL\r\n",__func__,__LINE__);
        return ERROR_SUB_BOARD_IPC_CMD_BASE_SET_PTR_NULL;
    }
    IPC_CMD_NODE_HEADER cmdHeader;
    //获取线程ID
    cmdHeader.sendCommandThreadID = (uint32_t)tx_thread_identify();
    cmdHeader.commandCode = cmdBaseDataPtr->commandCode;
    //搜索上一个指令是否已经存在
    listNodePtr = ADT_CircularPtrListFindNodeWithData(channelDataPtr->waitResultList,(void*)&cmdHeader,ADT_IpcCmdWaitElementFindWithHeader);
    if(listNodePtr != NULL)
    {
        //链表中存放了一个指令,需要对这个指令做处理
        IPC_CMD_WAIT_RESULT* cmdWaitPackExistPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
        //查看指令状态,在等待ACK和等待结果的情况下,报错,其他状态下,清除节点
        if(cmdWaitPackExistPtr->currentCmdStage == IPC_CMD_STAGE_WAIT)
        {
            IPC_LOG("E>: %s,%d,Last Command Not Complete\r\n",__func__,__LINE__);
            //上一次指令未结束
            return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LAST_NOT_COMPLETE;
        }
        //上一次指令已经结束了,但是调用者没有读取
        else
        {
#if(IPC_WAIT_CMD_COMPLETE_READ == CONFIG_FUNC_ENABLE)
            IPC_LOG("E>: %s,%d,Last Command Complete,NotRead\r\n",__func__,__LINE__);
            //配置了必须等待指令完成,上一次指令结束未读取
            return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LAST_NOT_READ;
#else
            //上一次指令完成但是没读取,配置不需要读取,那就冲掉
            IPC_PortMemFree(cmdWaitPackExistPtr);
            //删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
                return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LIST_NODE_DELETE;
            }
            //完成继续
            listNodePtr = NULL;
#endif
        }
    }
    //创建指令包
    IPC_CMD_DATA* cmdDataPtr = IPC_PortMemMalloc(DIM_STRUCT_LENGTH_BYTE(IPC_CMD_DATA));
    if(cmdDataPtr == NULL)
    {
        IPC_LOG("E>: %s,%d,IPC_PortMemMalloc Failed\r\n",__func__,__LINE__);
        return ERROR_SUB_BOARD_IPC_CMD_MEM_MALLOC_CMD_DATA;
    }
    //创建指令包里面的结果包
    cmdDataPtr->waitResultPackPtr = IPC_PortMemMalloc(DIM_STRUCT_LENGTH_BYTE(IPC_CMD_WAIT_RESULT));
    if(cmdDataPtr->waitResultPackPtr == NULL)
    {
        IPC_LOG("E>: %s,%d,IPC_PortMemMalloc Failed\r\n",__func__,__LINE__);
        IPC_PortMemFree(cmdDataPtr);
        return ERROR_SUB_BOARD_IPC_CMD_MEM_MALLOC_CMD_WAIT_RESULT_PACK;
    }
    //检查数据中是否带有BUFFER,带有BUFFER需要拷贝
    if(cmdBaseDataPtr->cmdParamBufferLength != 0)
    {
        cmdDataPtr->cmdBaseData.cmdParamBufferPtr = IPC_PortMemMalloc(cmdBaseDataPtr->cmdParamBufferLength);
        if(cmdDataPtr->cmdBaseData.cmdParamBufferPtr == NULL)
        {
            IPC_LOG("E>: %s,%d,IPC_PortMemMalloc Failed\r\n",__func__,__LINE__);
            IPC_PortMemFree(cmdDataPtr->waitResultPackPtr);
            IPC_PortMemFree(cmdDataPtr);
            return ERROR_SUB_BOARD_IPC_CMD_MEM_MALLOC_CMD_DATA_BUFFER;
        }
        //数据拷贝
        UserMemCopy(cmdDataPtr->cmdBaseData.cmdParamBufferPtr,cmdBaseDataPtr->cmdParamBufferPtr,cmdBaseDataPtr->cmdParamBufferLength);
        cmdDataPtr->cmdBaseData.cmdParamBufferLength = cmdBaseDataPtr->cmdParamBufferLength;
    }
    else
    {
        cmdDataPtr->cmdBaseData.cmdParamBufferPtr = NULL;
        cmdDataPtr->cmdBaseData.cmdParamBufferLength = 0;
    }
    //元素赋值
    cmdDataPtr->cmdBaseData.commandCode = cmdBaseDataPtr->commandCode;
    cmdDataPtr->cmdBaseData.cmdParam1 = cmdBaseDataPtr->cmdParam1;
    cmdDataPtr->cmdBaseData.cmdParam2 = cmdBaseDataPtr->cmdParam2;
    cmdDataPtr->cmdBaseData.cmdParam3 = cmdBaseDataPtr->cmdParam3;
    cmdDataPtr->cmdBaseData.cmdParam4 = cmdBaseDataPtr->cmdParam4;
    cmdDataPtr->cmdBaseData.cmdParam5 = cmdBaseDataPtr->cmdParam5;
    //结果返回的状态赋值
    cmdDataPtr->waitResultPackPtr->commandCode = cmdBaseDataPtr->commandCode;
    cmdDataPtr->waitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_WAIT;
    cmdDataPtr->waitResultPackPtr->sendCommandThreadID = cmdHeader.sendCommandThreadID;
    cmdDataPtr->waitResultPackPtr->resultPack.errorMain = ERROR_MAIN_OK;
    cmdDataPtr->waitResultPackPtr->resultPack.errorLevel = ERROR_LEVEL_NONE;
    cmdDataPtr->waitResultPackPtr->resultPack.errorSub = ERROR_SUB_OK;
    cmdDataPtr->waitResultPackPtr->resultPack.resultCode = PROT_RESULT_SUCCESS;
    //通道缓存赋值
    cmdDataPtr->channelCachePtr = &(channelDataPtr->channelCache);
    //发送之前将缓存中返回数据长度设置为0
    channelDataPtr->channelCache.returnDataBufferLength = 0;
    //获取互斥信号量
    IPC_BaseGetLock(channelDataPtr);
    //将等待节点放入等待列表
    adtResult = ADT_CircularPtrListInsertNodeFront(channelDataPtr->waitResultList,cmdDataPtr->waitResultPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //释放内存
        if(cmdDataPtr->cmdBaseData.cmdParamBufferPtr != NULL)
        {
            IPC_PortMemFree(cmdDataPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(cmdDataPtr->waitResultPackPtr);
        IPC_PortMemFree(cmdDataPtr);
        IPC_LOG("E>: %s,%d,ADT_CircularPtrListInsertNodeFront Failed\r\n",__func__,__LINE__);
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LIST_INSERT;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(channelDataPtr->waitResultList,(void*)cmdDataPtr->waitResultPackPtr,ADT_IpcCmdWaitPackElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //释放内存
        if(cmdDataPtr->cmdBaseData.cmdParamBufferPtr != NULL)
        {
            IPC_PortMemFree(cmdDataPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(cmdDataPtr->waitResultPackPtr);
        IPC_PortMemFree(cmdDataPtr);
        IPC_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LIST_FIND;
    }
    //数据发送
    errorSubLocal = IPC_PortSendCmdPack(channelDataPtr,cmdDataPtr);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        IPC_LOG("E>: %s,%d,IPC_PortSendCmdPack Failed,ErrorCode : 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //释放内存
        if(cmdDataPtr->cmdBaseData.cmdParamBufferPtr != NULL)
        {
            IPC_PortMemFree(cmdDataPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(cmdDataPtr->waitResultPackPtr);
        IPC_PortMemFree(cmdDataPtr);
        return errorSubLocal;
    }
    //发送成功,修改上一次发出的指令
    channelDataPtr->lastSendCommand = cmdBaseDataPtr->commandCode;
    //释放访问权
    IPC_BaseReleaseLock(channelDataPtr);
    return ERROR_SUB_OK;
}

//检查通道状态
PROT_CH_STA IPC_BaseCheckCmdChannelState(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    IPC_CMD_NODE_HEADER cmdHeader;
    //获取线程ID
    cmdHeader.sendCommandThreadID = (uint32_t)tx_thread_identify();
    cmdHeader.commandCode = commandCode;
    //获取互斥信号量
    IPC_BaseGetLock(channelDataPtr);
    //搜索上一个指令是否已经存在
    listNodePtr = ADT_CircularPtrListFindNodeWithData(channelDataPtr->waitResultList,(void*)&cmdHeader,ADT_IpcCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //通道空闲
        return PROT_CH_STA_IDLE;
    }
    else
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //通道忙
        return PROT_CH_STA_BUSY;
    }
}

//通道清空,保证下一个指令可以正确传输
ERROR_SUB IPC_BaseChannelClearChannel(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    IPC_CMD_NODE_HEADER cmdHeader;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //获取线程ID
    cmdHeader.sendCommandThreadID = (uint32_t)tx_thread_identify();
    cmdHeader.commandCode = commandCode;
    //获取互斥信号量
    IPC_BaseGetLock(channelDataPtr);
    //搜索上一个指令是否已经存在
    listNodePtr = ADT_CircularPtrListFindNodeWithData(channelDataPtr->waitResultList,(void*)&cmdHeader,ADT_IpcCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //通道空闲
        return ERROR_SUB_OK;
    }
    else
    {
        //通道数据存在,检查状态,状态已完成的情况下才能清除
        IPC_CMD_WAIT_RESULT* cmdWaitPackExistPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
        //查看指令状态,在等待ACK和等待结果的情况下,报错,其他状态下,清除节点
        if(cmdWaitPackExistPtr->currentCmdStage == IPC_CMD_STAGE_WAIT)
        {
            //释放访问权
            IPC_BaseReleaseLock(channelDataPtr);
            IPC_LOG("E>: %s,%d,Last Command Not Complete\r\n",__func__,__LINE__);
            //上一次指令未结束
            return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LAST_NOT_COMPLETE;
        }
        else
        {
            //上一次指令完成但是没读取,配置不需要读取,那就冲掉
            IPC_PortMemFree(cmdWaitPackExistPtr);
            //删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                //释放访问权
                IPC_BaseReleaseLock(channelDataPtr);
                IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
                return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_LIST_NODE_DELETE;
            }
            //释放访问权
            IPC_BaseReleaseLock(channelDataPtr);
            return ERROR_SUB_OK;
        }
    }
}

//IPC查询指令状态
ERROR_SUB IPC_BaseQueryCmdState(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    if(channelDataPtr == NULL)
    {
        return ERROR_SUB_BOARD_IPC_CMD_CHANNEL_PTR_NULL;
    }
    IPC_CMD_NODE_HEADER cmdHeader;
    //获取线程ID
    cmdHeader.sendCommandThreadID = (uint32_t)tx_thread_identify();
    cmdHeader.commandCode = commandCode;
    //申请访问权
    IPC_BaseGetLock(channelDataPtr);
    //搜索上一个指令是否已经存在
    listNodePtr = ADT_CircularPtrListFindNodeWithData(channelDataPtr->waitResultList,(void*)&cmdHeader,ADT_IpcCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //没找到这个元素报错
        IPC_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,commandCode);
        //查询节点信息失败
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_QUERY_STATE_NODE_FIND;
    }
    IPC_CMD_WAIT_RESULT* cmdWaitResultPtr = (IPC_CMD_WAIT_RESULT*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //找到元素了,获取当前状态
    *cmdStagePtr = cmdWaitResultPtr->currentCmdStage;
    if(cmdWaitResultPtr->currentCmdStage == IPC_CMD_STAGE_OVER)
    {
        //指令完成
        resultPackPtr->resultCode = cmdWaitResultPtr->resultPack.resultCode;
        resultPackPtr->errorMain = cmdWaitResultPtr->resultPack.errorMain;
        resultPackPtr->errorLevel = cmdWaitResultPtr->resultPack.errorLevel;
        resultPackPtr->errorSub = cmdWaitResultPtr->resultPack.errorSub;
        //释放内存
        IPC_PortMemFree(cmdWaitResultPtr);
        //删除节点
        ADT_RESULT adtResult = ADT_RESULT_OK;
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
    }
    else if(cmdWaitResultPtr->currentCmdStage == IPC_CMD_STAGE_PROC_FUNC_NULL)
    {
        //释放内存
        IPC_PortMemFree(cmdWaitResultPtr);
        //删除节点
        ADT_RESULT adtResult = ADT_RESULT_OK;
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //指令的处理函数不存在
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_PROC_FUNC_NULL;
    }
    else if(cmdWaitResultPtr->currentCmdStage == IPC_CMD_STAGE_ERROR_SUPPORT)
    {
        //释放内存
        IPC_PortMemFree(cmdWaitResultPtr);
        //删除节点
        ADT_RESULT adtResult = ADT_RESULT_OK;
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //指令不支持
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_CMD_NOT_SUPPORT;
    }
    else if(cmdWaitResultPtr->currentCmdStage == IPC_CMD_STAGE_CMD_PAR_LEN)
    {
        //释放内存
        IPC_PortMemFree(cmdWaitResultPtr);
        //删除节点
        ADT_RESULT adtResult = ADT_RESULT_OK;
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(channelDataPtr->waitResultList,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            IPC_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        IPC_BaseReleaseLock(channelDataPtr);
        //指令携带参数不对
        return ERROR_SUB_BOARD_IPC_CMD_WAIT_PACK_CMD_PARAM_LEN;
    }
    //释放访问权
    IPC_BaseReleaseLock(channelDataPtr);
    return ERROR_SUB_OK;
}


//IPC等待结果返回
ERROR_SUB IPC_BaseWaitCmdReturn(IPC_CHANNEL_DATA* channelDataPtr,uint32_t commandCode,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IPC_CMD_STAGE cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_IPC);
        //查询
        errorSub = IPC_BaseQueryCmdState(channelDataPtr,commandCode,&cmdStageCurrent,resultPackPtr);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == IPC_CMD_STAGE_WAIT)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //不是等待状态,跳出循环
        break;
    }
    return ERROR_SUB_OK;
}


//IPC指令发送并等待结果返回
ERROR_SUB IPC_BaseSendCmdWhileReturn(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_BASE_DATA* cmdBaseDataPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先发送指令
    errorSub = IPC_BaseSendCmdWhileAck(channelDataPtr,cmdBaseDataPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitCmdReturn(channelDataPtr,cmdBaseDataPtr->commandCode,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    return errorSub;
}

//IPC查询上一次发出的指令状态
ERROR_SUB IPC_BaseQueryLastCmdState(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultBasePackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查上一次发出的指令是否存在
    if(channelDataPtr->lastSendCommand == IPC_BASE_CMD_RESERVE)
    {
        //不存在默认已完成
        *cmdStagePtr = IPC_CMD_STAGE_OVER;
        return errorSub;
    }
    SYS_RESULT_PACK sysResultPack;
    //查询
    errorSub = IPC_BaseQueryCmdState(channelDataPtr,channelDataPtr->lastSendCommand,cmdStagePtr,&sysResultPack);
    if(errorSub != ERROR_SUB_OK)
    {
        //检测异常,直接返回,内部故障了
        return errorSub;
    }
    if(*cmdStagePtr == IPC_CMD_STAGE_OVER)
    {
        resultBasePackPtr->resultCode = sysResultPack.resultCode;
        resultBasePackPtr->errorMain = sysResultPack.errorMain;
        resultBasePackPtr->errorLevel = sysResultPack.errorLevel;
        resultBasePackPtr->errorSub = sysResultPack.errorSub;
        //清除上一次指令
        channelDataPtr->lastSendCommand = IPC_BASE_CMD_RESERVE;
    }
    return errorSub;
}

//IPC等待上一次发出的指令结果返回
ERROR_SUB IPC_BaseWaitLastCmdReturn(IPC_CHANNEL_DATA* channelDataPtr,SYS_RESULT_PACK* resultBasePackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    IPC_CMD_STAGE cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_IPC);
        //查询
        errorSub = IPC_BaseQueryLastCmdState(channelDataPtr,&cmdStageCurrent,resultBasePackPtr);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == IPC_CMD_STAGE_WAIT)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //不是等待状态,跳出循环
        break;
    }
    return ERROR_SUB_OK;
}

