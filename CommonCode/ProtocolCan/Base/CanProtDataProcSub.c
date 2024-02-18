/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 20:39:10 +0800
************************************************************************************************/ 
#include "CanProtBase.h"
#include "CanProtMain.h"

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

/*******************************************基础共用*************************************************************/

/*******************************************CAN1_PROT*************************************************************/
//指令ACK的链表查找函数
static ADT_CMP_RESULT ADT_Can1ProtCmdWaitPackAckCmdElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr = (CAN_PROT_ACK_CMD_PACK*)cmpDst;
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
    if(ackCmdPackPtr->singalChannel != cmdWaitPackPtrSrc->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr->ackCmdVal != cmdWaitPackPtrSrc->cmdValue)
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

//指令包的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can1ProtAckCmdMsgPackProc(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr)
{
    //收到了指令的ACK,从队列中找出对应的指令等待序列
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)ackCmdPackPtr,
                                                                                        ADT_Can1ProtCmdWaitPackAckCmdElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,根据ACK的状态切换数据状态
    if(ackCmdPackPtr->ackCode == PROT_ACK_REJECT)
    {
        //ACK错误,指令被拒绝
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_ERR;
        //释放访问权
        Can1ProtGetDataAccessRelease();
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
    //释放访问权
    Can1ProtGetDataAccessRelease();
    return;
}

//指令结果的链表查找函数
static ADT_CMP_RESULT ADT_Can1ProtCmdWaitPackResultElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_RESULT_PACK* resultPackPtr = (CAN_PROT_RESULT_PACK*)cmpDst;
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
    if(resultPackPtr->singalChannel != cmdWaitPackPtrSrc->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr->cmdValue != cmdWaitPackPtrSrc->cmdValue)
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
void Can1ProtResultMsgPackProc(CAN_PROT_RESULT_PACK* resultPackPtr)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //收到结果数据包的第一件事是立即回传ACK
    Can1ProtSendAckInRecv(CAN_PROT_ACK_RESULT,resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->singalChannel,
                            resultPackPtr->retryCount,resultPackPtr->cmdValue,PROT_ACK_OK);
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)resultPackPtr,
                                                                                        ADT_Can1ProtCmdWaitPackResultElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //查看结果长度和预期数据长度
    if(resultPackPtr->resultParamBufferLength < cmdWaitPackPtr->exceptResultLengthMin)
    {
        //数据长度小于最小预期长度,数据不要
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_LEN_SHORT;
        //释放访问权
        Can1ProtGetDataAccessRelease();
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
        cmdWaitPackPtr->resultParamBufferPtr = Can1ProtPortMemMalloc(cmdWaitPackPtr->resultParamBufferLength);
        if(cmdWaitPackPtr->resultParamBufferPtr == NULL)
        {
            //内存申请失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_BUF_MALLOC;
            //释放访问权
            Can1ProtGetDataAccessRelease();
            CAN_PROT_LOG("E>: %s,%d,resultParamBufferPtr MemMalloc Failed\r\n",__func__,__LINE__);
            return;
        }
        //内存申请成功,拷贝内存
        UserMemCopy(cmdWaitPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
        //释放访问权
        Can1ProtGetDataAccessRelease();
        return;
    }
    else
    {
        cmdWaitPackPtr->resultParamBufferPtr = NULL;
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
        //释放访问权
        Can1ProtGetDataAccessRelease();
    }
}

//结果上传的链表查找函数
static ADT_CMP_RESULT ADT_Can1ProtResultWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrSrc = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr = (CAN_PROT_ACK_RESULT_PACK*)cmpDst;
    if(resultWaitAckPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->srcModuleNo != resultWaitAckPackPtrSrc->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->srcBoardID != resultWaitAckPackPtrSrc->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->singalChannel != ackResultPackPtr->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->ackCmdVal != ackResultPackPtr->ackCmdVal)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//结果上传的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can1ProtAckResultMsgPackProc(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendResultWaitAckCachePtr,(void*)ackResultPackPtr,
                                                                                        ADT_Can1ProtResultWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    resultWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,将这个节点删除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
    {
        Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
    }
    Can1ProtPortMemFree(resultWaitAckPackPtr);
    //释放访问权
    Can1ProtGetDataAccessRelease();
    return;
}

//数据上报的链表查找函数
static ADT_CMP_RESULT ADT_Can1ProtUploadWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrSrc = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr = (CAN_PROT_ACK_UPLOAD_PACK*)cmpDst;
    if(uploadWaitAckPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackUploadPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->targetModuleNo != ackUploadPackPtr->srcModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->targetBoardID != ackUploadPackPtr->srcBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->uploadID != ackUploadPackPtr->uploadID)
    {
        //上传代码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//主动上报的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can1ProtAckUploadMsgPackProc(CAN_PROT_ACK_UPLOAD_PACK* uploadPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadPackPtr,
                                                                                    ADT_Can1ProtUploadWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    uploadWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,将这个节点删除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
    {
        Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
    }
    Can1ProtPortMemFree(uploadWaitAckPackPtr);
    //释放访问权
    Can1ProtGetDataAccessRelease();
    return;
}





























/*******************************************CAN2_PROT*************************************************************/

//指令ACK的链表查找函数
static ADT_CMP_RESULT ADT_Can2ProtCmdWaitPackAckCmdElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr = (CAN_PROT_ACK_CMD_PACK*)cmpDst;
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
    if(ackCmdPackPtr->singalChannel != cmdWaitPackPtrSrc->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackCmdPackPtr->ackCmdVal != cmdWaitPackPtrSrc->cmdValue)
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

//指令包的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can2ProtAckCmdMsgPackProc(CAN_PROT_ACK_CMD_PACK* ackCmdPackPtr)
{
    //收到了指令的ACK,从队列中找出对应的指令等待序列
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)ackCmdPackPtr,
                                                                                        ADT_Can2ProtCmdWaitPackAckCmdElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,根据ACK的状态切换数据状态
    if(ackCmdPackPtr->ackCode == PROT_ACK_REJECT)
    {
        //ACK错误,指令被拒绝
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_ERR;
        //释放访问权
        Can2ProtGetDataAccessRelease();
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
    //释放访问权
    Can2ProtGetDataAccessRelease();
    return;
}


//指令结果的链表查找函数
static ADT_CMP_RESULT ADT_Can2ProtCmdWaitPackResultElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_RESULT_PACK* resultPackPtr = (CAN_PROT_RESULT_PACK*)cmpDst;
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
    if(resultPackPtr->singalChannel != cmdWaitPackPtrSrc->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultPackPtr->cmdValue != cmdWaitPackPtrSrc->cmdValue)
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
void Can2ProtResultMsgPackProc(CAN_PROT_RESULT_PACK* resultPackPtr)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //收到结果数据包的第一件事是立即回传ACK
    Can2ProtSendAckInRecv(CAN_PROT_ACK_RESULT,resultPackPtr->srcModuleNo,resultPackPtr->srcBoardID,resultPackPtr->singalChannel,
                            resultPackPtr->retryCount,resultPackPtr->cmdValue,PROT_ACK_OK);
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从列表中找到对应的等待节点并处理
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)resultPackPtr,
                                                                                        ADT_Can2ProtCmdWaitPackResultElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,RetryCount: %d\r\n",__func__,__LINE__,resultPackPtr->retryCount);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    cmdWaitPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //查看结果长度和预期数据长度
    if(resultPackPtr->resultParamBufferLength < cmdWaitPackPtr->exceptResultLengthMin)
    {
        //数据长度小于最小预期长度,数据不要
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_LEN_SHORT;
        //释放访问权
        Can2ProtGetDataAccessRelease();
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
        cmdWaitPackPtr->resultParamBufferPtr = Can2ProtPortMemMalloc(cmdWaitPackPtr->resultParamBufferLength);
        if(cmdWaitPackPtr->resultParamBufferPtr == NULL)
        {
            //内存申请失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_BUF_MALLOC;
            //释放访问权
            Can2ProtGetDataAccessRelease();
            CAN_PROT_LOG("E>: %s,%d,resultParamBufferPtr MemMalloc Failed\r\n",__func__,__LINE__);
            return;
        }
        //内存申请成功,拷贝内存
        UserMemCopy(cmdWaitPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferPtr,resultPackPtr->resultParamBufferLength);
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
        //释放访问权
        Can2ProtGetDataAccessRelease();
        return;
    }
    else
    {
        cmdWaitPackPtr->resultParamBufferPtr = NULL;
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_OVER;
        //释放访问权
        Can2ProtGetDataAccessRelease();
        return;
    }
}

//结果上传的链表查找函数
static ADT_CMP_RESULT ADT_Can2ProtResultWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrSrc = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr = (CAN_PROT_ACK_RESULT_PACK*)cmpDst;
    if(resultWaitAckPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->srcModuleNo != resultWaitAckPackPtrSrc->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->srcBoardID != resultWaitAckPackPtrSrc->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->singalChannel != ackResultPackPtr->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackResultPackPtr->ackCmdVal != ackResultPackPtr->ackCmdVal)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//结果上传的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can2ProtAckResultMsgPackProc(CAN_PROT_ACK_RESULT_PACK* ackResultPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendResultWaitAckCachePtr,(void*)ackResultPackPtr,
                                                                                        ADT_Can2ProtResultWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    resultWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,将这个节点删除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
    {
        Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
    }
    Can2ProtPortMemFree(resultWaitAckPackPtr);
    //释放访问权
    Can2ProtGetDataAccessRelease();
    return;
}

//数据上报的链表查找函数
static ADT_CMP_RESULT ADT_Can2ProtUploadWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrSrc = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_ACK_UPLOAD_PACK* ackUploadPackPtr = (CAN_PROT_ACK_UPLOAD_PACK*)cmpDst;
    if(uploadWaitAckPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(ackUploadPackPtr == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->targetModuleNo != ackUploadPackPtr->srcModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->targetBoardID != ackUploadPackPtr->srcBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->uploadID != ackUploadPackPtr->uploadID)
    {
        //上传代码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//主动上报的ACK处理,收到的指针的内存不能二次转发,调用者会清理内存
void Can2ProtAckUploadMsgPackProc(CAN_PROT_ACK_UPLOAD_PACK* uploadPackPtr)
{
    //收到了结果的ACK,首先从链表中找出等待元素
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从链表中找出对应的节点
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadPackPtr,
                                                                                    ADT_Can2ProtUploadWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        return;
    }
    //找到了节点,将这个节点的数据搞出来
    uploadWaitAckPackPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
    //节点数据不可能为空,将这个节点删除掉
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //找不到对应的节点,报个错
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放内存
    if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
    {
        Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
    }
    Can2ProtPortMemFree(uploadWaitAckPackPtr);
    //释放访问权
    Can2ProtGetDataAccessRelease();
    return;
}

