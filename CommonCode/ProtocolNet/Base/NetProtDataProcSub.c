/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-18 16:28:31 +0800
************************************************************************************************/ 
#include "NetProtBase.h"
#include "NetProtMain.h"

//NET SOCKET初始化标记
extern NET_PROT_INIT_SET netProtInitSet;
//网口接收缓存
extern NET_PROT_RECV_CACHE netProtRecvCache;

//获取数据访问权
extern void NetProtGetDataAccessLock(void);
//释放数据访问权
extern void NetProtGetDataAccessRelease(void);

//指令ACK的链表查找函数
static ADT_CMP_RESULT ADT_NetProtCmdWaitPackAckCmdElementCmp(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr = (NET_PROT_ACK_CMD_PACK_RECV*)cmpDst;
    if(cmdWaitPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr->srcModuleNo != cmdWaitPackPtrSrc->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr->srcBoardID != cmdWaitPackPtrSrc->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr->ackCmdCode != cmdWaitPackPtrSrc->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    //当前状态是等待ACK状态
    if(cmdWaitPackPtrSrc->cmdStage != PROT_CMD_STA_WAIT_ACK)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//指令结果的链表查找函数
static ADT_CMP_RESULT ADT_NetProtCmdWaitPackResultElementCmp(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    NET_PROT_RESULT_PACK_RECV* resultPackPtr = (NET_PROT_RESULT_PACK_RECV*)cmpDst;
    if(cmdWaitPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr->srcModuleNo != cmdWaitPackPtrSrc->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr->srcBoardID != cmdWaitPackPtrSrc->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr->cmdCode != cmdWaitPackPtrSrc->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    //当前状态是等待结果状态
    if(cmdWaitPackPtrSrc->cmdStage != PROT_CMD_STA_WAIT_RESULT)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//收到的结果包的处理,收到的指针的内存不能二次转发,调用者会清理内存
void NetProtPackRecvProcessResult(NET_PROT_RESULT_PACK_RECV* resultPackPtr)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //收到结果数据包的第一件事是立即回传ACK
    NetProtSendAckInRecv(NET_PROT_ACK_RESULT,resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,
                            resultPackPtr->retryCount,resultPackPtr->cmdCode,PROT_ACK_OK);
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)resultPackPtr,
                                                                                        ADT_NetProtCmdWaitPackResultElementCmp);
    if(listNodePtr == NULL)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //查看结果长度和预期数据长度
    if(resultPackPtr->resultParamBufferLength < cmdWaitPackPtr->exceptResultLengthMin)
    {
        //数据长度小于最小预期长度,数据不要
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_LEN_SHORT;
        return;
    }
    //长度匹配,数据拷贝
    cmdWaitPackPtr->resultCode = resultPackPtr->resultCode;
    cmdWaitPackPtr->errorMainCode = resultPackPtr->errorMainCode;
    cmdWaitPackPtr->errorLevelCode = resultPackPtr->errorLevelCode;
    cmdWaitPackPtr->errorSubCode = resultPackPtr->errorSubCode;
    //结果参数长度
    cmdWaitPackPtr->resultParamBufferLength = resultPackPtr->resultParamBufferLength;
    if(cmdWaitPackPtr->resultParamBufferLength != 0)
    {
        //内存申请
        cmdWaitPackPtr->resultParamBufferPtr = NetProtMemMalloc(cmdWaitPackPtr->resultParamBufferLength);
        if(cmdWaitPackPtr->resultParamBufferPtr == NULL)
        {
            //内存申请失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_BUF_MALLOC;
            NET_PROT_LOG("E>: %s,%d,resultParamBufferPtr MemMalloc Failed\r\n",__func__,__LINE__);
            return;
        }
        //内存申请成功,拷贝内存
        UserMemCopy(cmdWaitPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
        return;
    }
    else
    {
        cmdWaitPackPtr->resultParamBufferPtr = NULL;
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
    }
}

//指令ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void NetProtPackRecvProcessAckCmd(NET_PROT_ACK_CMD_PACK_RECV* ackCmdPackPtr)
{
    //收到了指令的ACK,从队列中找出对应的指令等待序列
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)ackCmdPackPtr,
                                                                                        ADT_NetProtCmdWaitPackAckCmdElementCmp);
    if(listNodePtr == NULL)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,根据ACK的状态切换数据状态
    if(ackCmdPackPtr->ackCode == PROT_ACK_REJECT)
    {
        //ACK错误,指令被拒绝
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_ERR;
        return;
    }
    //ACK状态是正常,看看是否需要结果,不需要结果,指令就完成了,需要结果,切换等待结果
    if(cmdWaitPackPtr->cmdNeedResult == ENABLE)
    {
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_WAIT_RESULT;
    }
    else
    {
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_OVER;
    }
    return;
}

//结果包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void NetProtPackRecvProcessAckResult(NET_PROT_ACK_RESULT_PACK_RECV* ackResultPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    NET_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //从链表中取出头节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListPeekTop(netProtInitSet.listSendResultWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListPeekTop Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    resultWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //匹配数据
    if(resultWaitAckPackPtr->targetModuleNo != ackResultPackPtr->srcModuleNo)
    {
        //模块号不匹配
        return;
    }
    if(resultWaitAckPackPtr->targetBoardID != ackResultPackPtr->srcBoardID)
    {
        //板号不匹配
        return;
    }
    if(resultWaitAckPackPtr->cmdValue != ackResultPackPtr->ackCmdCode)
    {
        //命令号不匹配
        return;
    }
    //节点数据不可能为空,将这个节点删除掉,匹配上了,工作完成
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
    {
        NetProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
    }
    NetProtMemFree(resultWaitAckPackPtr);
}

//数据上传包ACK处理,发送过来的数据的内存不能直接用指针,必须深拷贝,因为调用者会清除内存
void NetProtPackRecvProcessAckDataUpload(NET_PROT_ACK_UPLOAD_PACK_RECV* ackUploadPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    NET_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListPeekTop(netProtInitSet.listSendUploadWaitAckCachePtr);
    if(listNodePtr == NULL)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListPeekTop Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    uploadWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //匹配数据
    if(uploadWaitAckPackPtr->targetModuleNo != ackUploadPackPtr->srcModuleNo)
    {
        //模块号不匹配
        return;
    }
    if(uploadWaitAckPackPtr->targetBoardID != ackUploadPackPtr->srcBoardID)
    {
        //板号不匹配
        return;
    }
    if(uploadWaitAckPackPtr->uploadID != ackUploadPackPtr->uploadID)
    {
        //命令号不匹配
        return;
    }
    //节点数据不可能为空,将这个节点删除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
    {
        NetProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
    }
    NetProtMemFree(uploadWaitAckPackPtr);
}






