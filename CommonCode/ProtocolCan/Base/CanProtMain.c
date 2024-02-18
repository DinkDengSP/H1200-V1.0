/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 11:37:01 +0800
************************************************************************************************/ 
#include "CanProtMain.h"
#include "tx_mutex.h"

//CAN1数据缓冲区
uint8_t can1ProtFrameBuf[CAN1_PROT_DATA_BUF_LEN] = {0};
//CAN2数据缓冲区
uint8_t can2ProtFrameBuf[CAN2_PROT_DATA_BUF_LEN] = {0};
//CAN1初始化标记
CAN_PROT_INIT_SET can1ProtInitSet;
//CAN2初始化标记
CAN_PROT_INIT_SET can2ProtInitSet;

//获取数据访问权
extern void Can1ProtGetDataAccessLock(void);
//释放数据访问权
extern void Can1ProtGetDataAccessRelease(void);
//获取数据访问权
extern void Can2ProtGetDataAccessLock(void);
//释放数据访问权
extern void Can2ProtGetDataAccessRelease(void);

//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtCmdWaitPackElementCmp(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtCmdWaitElementFind(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,元素内容匹配.使用指令头查找
static ADT_CMP_RESULT ADT_CanProtCmdWaitElementFindWithHeader(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtResultWaitAckElementFind(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtResultWaitAckElementCmp(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtUploadWaitAckElementCmp(void* cmpSrc,void* cmpDst);
//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtUploadWaitAckElementFind(void* cmpSrc,void* cmpDst);
/*******************************************CAN1_PORT*************************************************************/
//初始化CAN1_PORT
void Can1ProtInit(uint8_t selfModuleNo,uint8_t selfBoardID)
{
    //创建互斥信号量
    if(can1ProtInitSet.mutexSocketCanDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return ;
    }
    //创建互斥信号量
    tx_mutex_create(&(can1ProtInitSet.mutexSocketCanDataAccess),"Can1ProtDataAccessMutex",TX_INHERIT);
    //模块号板号
    can1ProtInitSet.selfModuleNo = selfModuleNo;
    can1ProtInitSet.selfBoardID = selfBoardID;
    //创建日志接收缓存链表
    can1ProtInitSet.listRecvLogMsgCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建指令接收缓存链表
    can1ProtInitSet.listRecvCmdMsgCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建结果接收缓存链表
    can1ProtInitSet.listRecvResultMsgCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建上传数据接收缓存链表
    can1ProtInitSet.listRecvUploadMsgCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建结果包发送等待ACK缓存链表
    can1ProtInitSet.listSendResultWaitAckCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建上传包发送等待ACK缓存链表
    can1ProtInitSet.listSendUploadWaitAckCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建指令等待缓存链表
    can1ProtInitSet.listSendCmdWaitCachePtr = ADT_CircularPtrListCreate(Can1ProtPortMemMalloc,Can1ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
}

//检查SOCKET是否已经初始化
FlagStatus Can1ProtInitQuery(void)
{
    if(can1ProtInitSet.mutexSocketCanDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return SET;
    }
    return RESET;
}



//日志数据包
extern void Can1ProtRecvProcLogFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//指令包
extern void Can1ProtRecvProcCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//指令ACK数据包
extern void Can1ProtRecvProcAckCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//结果数据包
extern void Can1ProtRecvProcResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//结果ACK数据包
extern void Can1ProtRecvProcAckResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//主动上传数据包
extern void Can1ProtRecvProcDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//上传数据包的ACK
extern void Can1ProtRecvProcAckDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);

//CAN_PORT1数据包接收
void Can1ProtPackRecv(CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //将接收数据包
    CAN_PROT_ID protCanID;
    //只接受扩展帧
    if(canRecvPackPtr->idType == CAN_ID_STD)
    {
        return;
    }
    //计算CAN ID
    CanProtBasePackIdAnaly(canRecvPackPtr->recvCmdID,&protCanID);
    //数据包分类型处理
    switch(protCanID.packType)
    {
        case PROT_PACK_TYPE_LOG: 
            //日志数据包
            Can1ProtRecvProcLogFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_CMD: 
            //指令数据包
            Can1ProtRecvProcCmdFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_CMD:
            //指令ACK数据包
            Can1ProtRecvProcAckCmdFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_RESULT: 
            //结果数据包
            Can1ProtRecvProcResultFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_RESULT: 
            //结果ACK数据包
            Can1ProtRecvProcAckResultFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_DATA_UPLOAD: 
            //主动上传数据包
            Can1ProtRecvProcDataUploadFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_DATA_UPLOAD: 
            //上传数据包的ACK
            Can1ProtRecvProcAckDataUploadFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        default:
            //没有任何处理
            break;
    }
}


//CAN1_PROT ACK发送
void Can1ProtAckSend(CAN_PROT_ACK_PACK* ackMsgPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(ackMsgPtr == NULL)
    {
        return;
    }
    if(ackMsgPtr->ackType == CAN_PROT_ACK_CMD)
    {
        //发送ACK CMD
        errorSub = Can1ProtSendAckCmdSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->singalChannel,ackMsgPtr->retryCount,
                                            ackMsgPtr->cmdVal,ackMsgPtr->ackCmd);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can1ProtSendAckCmdSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == CAN_PROT_ACK_RESULT)
    {
        //发送ACK RESULT
        errorSub = Can1ProtSendAckResultSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                                ackMsgPtr->singalChannel,ackMsgPtr->retryCount,
                                                ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can1ProtSendAckResultSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == CAN_PROT_ACK_UPLOAD)
    {
        //发送ACK UPLOAD
        errorSub = Can1ProtSendAckUploadSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->retryCount,ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can1ProtSendAckUploadSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    //释放内存
    Can1ProtPortMemFree(ackMsgPtr);
}

//检查接收缓存区数据是否超时
ADT_CMP_RESULT Can1ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon(void* nodePtr,uint32_t utilData)
{
    CAN_PROT_RECV_PACK_CACHE* can1RecvCachePtr = (CAN_PROT_RECV_PACK_CACHE*)nodePtr;
    uint32_t currentTickMs = utilData;
    if(can1RecvCachePtr == NULL)
    {
        return ADT_CMP_EQUAL;
    }
    if(currentTickMs - can1RecvCachePtr->lastFrameRecvTimeStamp >= TIME_OUT_MS_INTERVAL_LONG_PACK_CAN_PROT)
    {
        //当前时间比上一帧的时间差了指定时间,说明帧间隔异常,清除这一帧,首先释放内存
        if(can1RecvCachePtr->recvDataBufferPtr != NULL)
        {
            Can1ProtPortMemFree(can1RecvCachePtr->recvDataBufferPtr);
        }
        Can1ProtPortMemFree(can1RecvCachePtr);
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}


//从上传列表中找出超时的元素,决定重发或者删除
static ADT_CMP_RESULT ADT_ElementCmpCan1ProtUpdataWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //检查ACK时间是否超时
    if(currentTick >= uploadWaitAckPackPtr->lastSendTimeTickMs)
    {
        if((uploadWaitAckPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(uploadWaitAckPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - uploadWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - uploadWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
        
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//上传数据包缓存处理
static void Can1ProtRunPeriodSubUploadWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //从上传列表中找出ACK超时的节点决定重发或者删除报警
    while(1)
    {
        listNodePtr = NULL;
        uploadWaitAckPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendUploadWaitAckCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan1ProtUpdataWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(uploadWaitAckPackPtr->retryCount >= 2)
        {
            //重发超过限制,删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendUploadFailed.RetryMax.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X\r\n",
                                        __func__,__LINE__,uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                        uploadWaitAckPackPtr->uploadID);
            //释放内存
            if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
            {
                Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can1ProtPortMemFree(uploadWaitAckPackPtr);
            //结束本次循环
            continue;
        }
        else
        {
            uploadWaitAckPackPtr->retryCount += 1;
            //重发
            errorSub = Can1ProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                                    uploadWaitAckPackPtr->retryCount,uploadWaitAckPackPtr->uploadID,
                                                    uploadWaitAckPackPtr->uploadDataBufferPtr,uploadWaitAckPackPtr->uploadDataBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,删除元素
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //错误打印
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendUploadOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can1ProtSendUploadFailed.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                                        uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                        uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->retryCount);
            //释放内存
            if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
            {
                Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can1ProtPortMemFree(uploadWaitAckPackPtr);
            //结束本次循环
            continue;
        }
    }
}

//从结果列表中找出超时的元素,决定重发或者删除
static ADT_CMP_RESULT ADT_ElementCmpCan1ProtResultWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //检查ACK时间是否超时
    if(currentTick >= resultWaitAckPackPtr->lastSendTimeTickMs)
    {
        if((resultWaitAckPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(resultWaitAckPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - resultWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - resultWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//结果数据包处理
static void Can1ProtRunPeriodSubResultWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //从上传列表中找出ACK超时的节点决定重发或者删除报警
    while(1)
    {
        listNodePtr = NULL;
        resultWaitAckPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendResultWaitAckCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan1ProtResultWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(resultWaitAckPackPtr->retryCount >= 2)
        {
            //重发超过限制,删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendResultFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,
                                        resultWaitAckPackPtr->cmdValue);
            //释放内存
            if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
            {
                Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can1ProtPortMemFree(resultWaitAckPackPtr);
            //结束本次循环
            continue;
        }
        else
        {
            //重发
            errorSub = Can1ProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->singalChannel,
                                                    resultWaitAckPackPtr->retryCount,resultWaitAckPackPtr->cmdValue,resultWaitAckPackPtr->resultCode,
                                                    resultWaitAckPackPtr->errorMainCode,resultWaitAckPackPtr->errorLevelCode,resultWaitAckPackPtr->errorSubCode,
                                                    resultWaitAckPackPtr->resultParamBufferPtr,resultWaitAckPackPtr->resultParamBufferLength);
            resultWaitAckPackPtr->retryCount += 1;
            //时间更新
            resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            if(errorSub == ERROR_SUB_OK)
            {
                //结束当前循环
                continue;
            }
            //发送失败了,删除元素
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //错误打印
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendResultOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can1ProtSendResultFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,SingalChannel: %d,RetryCount: %d\r\n",
                                        resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->cmdValue,
                                        resultWaitAckPackPtr->singalChannel,resultWaitAckPackPtr->retryCount);
            //释放内存
            if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
            {
                Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can1ProtPortMemFree(resultWaitAckPackPtr);
            //结束本次循环
            continue;
        }
    }
}

//查找链表中等待ACK的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCan1ProtCmdWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //首先检查状态是不是等待ACK
    if(cmdWaitPackPtr->cmdStage != PROT_CMD_STA_WAIT_ACK)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(currentTick >= cmdWaitPackPtr->lastSendTimeTickMs)
    {
        if((cmdWaitPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(cmdWaitPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN1_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//周期性的检查指令重发
static void Can1ProtRunPeriodSubCmdWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan1ProtCmdWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(cmdWaitPackPtr->retryCount >= 2)
        {
            //状态设置为ACK超时
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_TIME_OUT;
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendCmdFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                        cmdWaitPackPtr->cmdValue);
            //结束本次循环
            continue;
        }
        else
        {
            cmdWaitPackPtr->retryCount += 1;
            //重发
            errorSub = Can1ProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->singalChannel,
                                                    cmdWaitPackPtr->retryCount,cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,
                                                    cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,错误打印
            CAN_PROT_LOG("E>: %s,%d,Can1ProtSendCmdOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can1ProtSendCmdFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,SingalChannel: %d,RetryCount: %d\r\n",
                                        cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->cmdValue,
                                        cmdWaitPackPtr->singalChannel,cmdWaitPackPtr->retryCount);
            //状态设置为重发失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
            //结束本次循环
            continue;
        }
    }
}

//查找链表中等待结果的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCan1CmdWaitResultListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
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
static void Can1ProtRunPeriodSubCmdWaitResultPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan1CmdWaitResultListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //超时的元素状态设置为超时
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_TIME_OUT;
        //日志超时
        CAN_PROT_LOG("E>: %s,%d,Can1ProtSendCmdFailed.ResultTimeOut.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X,TimeOutSet\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                    cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->resultTimeOutMs);
        //结束本次循环
        continue;
    }
}

//CAN_PORT1内部周期性数据处理
void Can1ProtRunPeriod(uint32_t currentTickMs)
{
    //检查CMD接收链表内部数据
    ADT_CircularPtrListTraverseDelete(can1ProtInitSet.listRecvCmdMsgCachePtr,Can1ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查Result链表内部数据
    ADT_CircularPtrListTraverseDelete(can1ProtInitSet.listRecvResultMsgCachePtr,Can1ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查upload链表内部数据
    ADT_CircularPtrListTraverseDelete(can1ProtInitSet.listRecvUploadMsgCachePtr,Can1ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查log链表内部数据
    ADT_CircularPtrListTraverseDelete(can1ProtInitSet.listRecvLogMsgCachePtr,Can1ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //申请访问权
    Can1ProtGetDataAccessLock();
    //上传数据包处理
    Can1ProtRunPeriodSubUploadWaitAckPackProcess();
    //结果数据包处理
    Can1ProtRunPeriodSubResultWaitAckPackProcess();
    //指令包等待ACK的数据处理
    Can1ProtRunPeriodSubCmdWaitAckPackProcess();
    //指令包等待结果的数据处理
    Can1ProtRunPeriodSubCmdWaitResultPackProcess();
    //释放访问权
    Can1ProtGetDataAccessRelease();
}

//CAN1在接收数据时候发送ACK
void Can1ProtSendAckInRecv(CAN_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ACK_PACK* can1ProtAckPtr = Can1ProtPortMemMallocWhileSuccess(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_PACK));
    can1ProtAckPtr->ackType = ackType;
    can1ProtAckPtr->targetModule = targetModule;
    can1ProtAckPtr->targetBoardID = targetBoardID;
    can1ProtAckPtr->singalChannel = singalChannel;
    can1ProtAckPtr->retryCount = retryCount;
    can1ProtAckPtr->cmdVal = cmdVal;
    can1ProtAckPtr->ackCmd = ackCmd;
    errorSub = Can1ProtAckMsgDispatch(can1ProtAckPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        CAN_PROT_LOG("%s,Can1ProtAckMsgDispatch ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //释放内存
        Can1ProtPortMemFree(can1ProtAckPtr);
    }
}

//CAN1_PROT发送日志
void Can1ProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint8_t* logBuffer,uint16_t logLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK packLog;
    packLog.retryCount = retryCount;
    packLog.singalChannel = singalChannel;
    packLog.targetBoard = (targetBoardID&0X1F);
    packLog.targetModule = (targetModule&0X07);
    packLog.srcBoardID = (srcBoardID&0X1F);
    packLog.srcModuleNo = (srcModule&0X07);
    packLog.packDataLen = logLength;
    packLog.packDataBufPtr = logBuffer;
    packLog.packType = PROT_PACK_TYPE_LOG;
    errorSub = CanProtBaseSendPack(PORT_CAN1,&packLog);
    if(errorSub != ERROR_SUB_OK)
    {
        CAN_PROT_LOG("E>: %s,%d, 0X%08X\r\n",__func__,__LINE__,errorSub);
    }
    return;
}

//CAN1_PROT发送日志,简易模式
void Can1ProtSendLogSample(uint8_t* logBuffer,uint16_t logLength)
{
    Can1ProtSendLog(can1ProtInitSet.selfModuleNo,can1ProtInitSet.selfBoardID,SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,
                                        0,0,logBuffer,logLength);
}

//CAN1_PROT发送结果,带重发功能
void Can1ProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    uint32_t waitStartTimeTick = 0;
    uint32_t currentTimeTick = 0;
    //首先创建等待结果包
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_WAIT_ACK_PACK));
    if(resultWaitAckPackPtr == NULL)
    {
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_RESULT_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    resultWaitAckPackPtr->resultParamBufferLength = resultBufferLen;
    if(resultWaitAckPackPtr->resultParamBufferLength != 0)
    {
        resultWaitAckPackPtr->resultParamBufferPtr = Can1ProtPortMemMalloc(resultWaitAckPackPtr->resultParamBufferLength);
        if(resultWaitAckPackPtr->resultParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can1ProtPortMemFree(resultWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_RESULT_WAIT_ACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        resultWaitAckPackPtr->resultParamBufferPtr = NULL;
    }
    //数据拷贝
    resultWaitAckPackPtr->targetModuleNo = targetModule;
    resultWaitAckPackPtr->targetBoardID = targetBoardID;
    resultWaitAckPackPtr->singalChannel = singalChannel;
    resultWaitAckPackPtr->retryCount = 0;
    resultWaitAckPackPtr->cmdValue = cmdVal;
    resultWaitAckPackPtr->resultCode = resultCode;
    resultWaitAckPackPtr->errorMainCode = errorMain;
    resultWaitAckPackPtr->errorLevelCode = errorLevel;
    resultWaitAckPackPtr->errorSubCode = errorSub;
    //内存拷贝
    if(resultBufferLen != 0)
    {
        UserMemCopy(resultWaitAckPackPtr->resultParamBufferPtr,resultParamBuffer,resultBufferLen);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    waitStartTimeTick = OS_TICK_TO_MS(tx_time_get());
    do
    {
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendResultWaitAckCachePtr,(void*)resultWaitAckPackPtr,
                                                            ADT_CanProtResultWaitAckElementFind);
        if(listNodePtr == NULL)
        {
            //链表中不存在指定元素,可以直接发送
            break;
        }
        //链表中存在指定元素了,不能直接发送
        currentTimeTick = OS_TICK_TO_MS(tx_time_get());
        if(currentTimeTick - waitStartTimeTick > TIME_OUT_MS_MAX_CAN_PROT_WAIT_RESULT_CH)
        {
            //超时了,这一帧数据发不出去,释放内存并报错
            if(resultBufferLen != 0)
            {
                Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can1ProtPortMemFree(resultWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_RESULT_WAIT_ACK_CHANNEL_BUSY);
            return;
        }
        else
        {
            //还没超时,延时下次检测
            CoreDelayMinTick();
        }
    }while(listNodePtr != NULL);
    //申请访问权
    Can1ProtGetDataAccessLock();
    //设置发送时间
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往结果包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can1ProtInitSet.listSendResultWaitAckCachePtr,resultWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can1ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_RESULT_WAIT_ACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendResultWaitAckCachePtr,(void*)resultWaitAckPackPtr,
                                                        ADT_CanProtResultWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can1ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_RESULT_WAIT_ACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can1ProtSendResultOnlySample(targetModule,targetBoardID,singalChannel,0,cmdVal,resultCode,errorMain,errorLevel,errorSub,
                                                    resultWaitAckPackPtr->resultParamBufferPtr,resultWaitAckPackPtr->resultParamBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can1ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can1ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}



//CAN1_PROT发送上传,带重发功能
void Can1ProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    uint32_t waitStartTimeTick = 0;
    uint32_t currentTimeTick = 0;
    //首先创建等待结果包
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_WAIT_ACK_PACK));
    if(uploadWaitAckPackPtr == NULL)
    {
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_UPLOAD_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    uploadWaitAckPackPtr->uploadDataBufferLength = uploadDataLen;
    if(uploadWaitAckPackPtr->uploadDataBufferLength != 0)
    {
        uploadWaitAckPackPtr->uploadDataBufferPtr = Can1ProtPortMemMalloc(uploadWaitAckPackPtr->uploadDataBufferLength);
        if(uploadWaitAckPackPtr->uploadDataBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can1ProtPortMemFree(uploadWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_UPLOAD_WAIT_ACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        uploadWaitAckPackPtr->uploadDataBufferPtr = NULL;
    }
    //数据拷贝
    uploadWaitAckPackPtr->targetModuleNo = targetModule;
    uploadWaitAckPackPtr->targetBoardID = targetBoardID;
    uploadWaitAckPackPtr->retryCount = 0;
    uploadWaitAckPackPtr->uploadID = uploadDataID;
    //内存拷贝
    if(uploadDataLen != 0)
    {
        UserMemCopy(uploadWaitAckPackPtr->uploadDataBufferPtr,uploadDataBuffer,uploadDataLen);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    waitStartTimeTick = OS_TICK_TO_MS(tx_time_get());
    do
    {
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadWaitAckPackPtr,
                                                            ADT_CanProtUploadWaitAckElementFind);
        if(listNodePtr == NULL)
        {
            //链表中不存在指定元素,可以直接发送
            break;
        }
        //链表中存在指定元素了,不能直接发送
        currentTimeTick = OS_TICK_TO_MS(tx_time_get());
        if(currentTimeTick - waitStartTimeTick > TIME_OUT_MS_MAX_CAN_PROT_WAIT_UPLOAD_CH)
        {
            //超时了,这一帧数据发不出去,释放内存并报错
            if(uploadDataLen != 0)
            {
                Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can1ProtPortMemFree(uploadWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_UPLOAD_WAIT_ACK_CHANNEL_BUSY);
            return;
        }
        else
        {
            //还没超时,延时下次检测
            CoreDelayMinTick();
        }
    }while(listNodePtr != NULL);
    //申请访问权
    Can1ProtGetDataAccessLock();
    //设置发送时间
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往结果包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can1ProtInitSet.listSendUploadWaitAckCachePtr,uploadWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can1ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_UPLOAD_WAIT_ACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadWaitAckPackPtr,ADT_CanProtUploadWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can1ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_UPLOAD_WAIT_ACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can1ProtSendUploadOnlySample(targetModule,targetBoardID,uploadWaitAckPackPtr->retryCount,
                                                        uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->uploadDataBufferPtr,
                                                        uploadWaitAckPackPtr->uploadDataBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can1ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can1ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//查询指令的当前状态
ERROR_SUB Can1ProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                                    PROT_CMD_STA* cmdStage)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can1ProtGetDataAccessRelease();
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_QUERY_STATE_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //找到元素了,获取当前状态
    *cmdStage = cmdWaitPackPtr->cmdStage;
    //释放访问权
    Can1ProtGetDataAccessRelease();
    return ERROR_SUB_OK;
}

//检查通道状态
PROT_CH_STA Can1ProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    //释放访问权
    Can1ProtGetDataAccessRelease();
    if(listNodePtr == NULL)
    {
        //通道空闲
        return PROT_CH_STA_IDLE;
    }
    else
    {
        //通道忙
        return PROT_CH_STA_BUSY;
    }
}

//删除节点
static void Can1ProtDeleteCmdNode(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{   
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //日志打印
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //释放节点数据
    if(cmdWaitPackPtr->resultParamBufferPtr != NULL)
    {
        Can1ProtPortMemFree(cmdWaitPackPtr->resultParamBufferPtr);
    }
    if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
    {
        Can1ProtPortMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    Can1ProtPortMemFree(cmdWaitPackPtr);
    return;
}

//拷贝节点数据
static ERROR_SUB Can1ProtCopyCmdNodeResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                                    SYS_RESULT_PACK* socketCanResultPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //日志打印
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_RESULT_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //拷贝数据
    socketCanResultPtr->resultCode = cmdWaitPackPtr->resultCode;
    socketCanResultPtr->errorMain = cmdWaitPackPtr->errorMainCode;
    socketCanResultPtr->errorLevel = cmdWaitPackPtr->errorLevelCode;
    socketCanResultPtr->errorSub = cmdWaitPackPtr->errorSubCode;
    *resultBufLen = cmdWaitPackPtr->resultParamBufferLength;
    if(*resultBufLen != 0)
    {
        *resultDataBufPtrPtr = cmdWaitPackPtr->resultParamBufferPtr;
    }
    else
    {
        *resultDataBufPtrPtr = NULL;
    }
    //释放节点数据
    if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
    {
        Can1ProtPortMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    Can1ProtPortMemFree(cmdWaitPackPtr);
    return ERROR_SUB_OK;
}

//等待ACK完成
static ERROR_SUB Can1ProtCmdWaitAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_CAN_PROT);
        //查询
        errorSub = Can1ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_ACK)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //ACK错误
        if(cmdStageCurrent == PROT_CMD_STA_ACK_ERR)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //正常结束,而且不需要结果包会进入这个状态,释放节点,返回完成
            break;
        }
        //其余状态,都是完成
        if(cmdStageCurrent >= PROT_CMD_STA_WAIT_RESULT)
        {
            break;
        }
    }
    return ERROR_SUB_OK;
}

//SOCKET1发送指令,基础,只要发出去
static void Can1ProtSendCmdBase(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,uint8_t* cmdBufferPtr,
                                        uint16_t cmdBufferLength,FunctionalState cmdNeedResult,uint32_t resultTimeOutMs,
                                        uint16_t exceptResultLengthMin,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //首先创建等待指令包
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPackPtr = (CAN_PROT_CMD_WAIT_PACK*)Can1ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_WAIT_PACK));
    if(cmdWaitPackPackPtr == NULL)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    cmdWaitPackPackPtr->cmdParamBufferLength = cmdBufferLength;
    if(cmdWaitPackPackPtr->cmdParamBufferLength != 0)
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = Can1ProtPortMemMalloc(cmdWaitPackPackPtr->cmdParamBufferLength);
        if(cmdWaitPackPackPtr->cmdParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can1ProtPortMemFree(cmdWaitPackPackPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = NULL;
    }
    //数据拷贝
    cmdWaitPackPackPtr->targetModuleNo = targetModule;
    cmdWaitPackPackPtr->targetBoardID = targetBoardID;
    cmdWaitPackPackPtr->singalChannel = singalChannel;
    cmdWaitPackPackPtr->retryCount = 0;
    cmdWaitPackPackPtr->cmdValue = cmdVal;
    cmdWaitPackPackPtr->cmdStage = PROT_CMD_STA_WAIT_ACK;
    cmdWaitPackPackPtr->cmdNeedResult = cmdNeedResult;
    cmdWaitPackPackPtr->resultTimeOutMs = resultTimeOutMs;
    cmdWaitPackPackPtr->exceptResultLengthMin = exceptResultLengthMin;
    //结果初始值
    cmdWaitPackPackPtr->resultCode = PROT_RESULT_SUCCESS;
    cmdWaitPackPackPtr->errorMainCode = ERROR_MAIN_OK;
    cmdWaitPackPackPtr->errorLevelCode = ERROR_LEVEL_NONE;
    cmdWaitPackPackPtr->errorSubCode = ERROR_SUB_OK;
    cmdWaitPackPackPtr->resultParamBufferPtr = NULL;
    cmdWaitPackPackPtr->resultParamBufferLength = 0;
    //内存拷贝
    if(cmdBufferLength != 0)
    {
        UserMemCopy(cmdWaitPackPackPtr->cmdParamBufferPtr,cmdBufferPtr,cmdBufferLength);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,
                                                        ADT_CanProtCmdWaitElementFind);
    if(listNodePtr != NULL)
    {
        //链表中存放了一个指令,需要对这个指令做处理
        CAN_PROT_CMD_WAIT_PACK* cmdWaitPackExistPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
        //查看指令状态,在等待ACK和等待结果的情况下,报错,其他状态下,清除节点
        if((cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //上一次指令未结束
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_LAST_NOT_COMPLETE);
            return;
        }
        //上一次指令已经结束了,但是调用者没有读取
        else
        {
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //上一次指令结束未读取
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_LAST_NOT_READ);
            return;
        }
    }
    //申请访问权
    Can1ProtGetDataAccessLock();
    //设置发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往指令等待包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can1ProtInitSet.listSendCmdWaitCachePtr,cmdWaitPackPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can1ProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,ADT_CanProtCmdWaitPackElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can1ProtSendCmdOnlySample(targetModule,targetBoardID,singalChannel,0,cmdVal,cmdWaitPackPackPtr->cmdParamBufferPtr,
                                                            cmdWaitPackPackPtr->cmdParamBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can1ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can1ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can1ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//CAN1_PROT发送指令,基础,只要发出去
void Can1ProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    Can1ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,
                        cmdSendPtr->resultTimeOutMs,cmdSendPtr->exceptResultLengthMin,resultPackPtr);
}

//CAN1_PROT发送指令,并等待ACK
void Can1ProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先发出去
    Can1ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待ACK
    errorSub = Can1ProtCmdWaitAck(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//CAN1_PROT等待指令结果
static ERROR_SUB Can1ProtCmdWaitResultBase(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                            SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_CAN_PROT);
        //查询
        errorSub = Can1ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_ACK)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //ACK错误
        if(cmdStageCurrent == PROT_CMD_STA_ACK_ERR)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //正常结束,而且不需要结果包会进入这个状态,释放节点,返回完成
            break;
        }
        //等待结果,继续等待
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_RESULT)
        {
            continue;
        }
        //结果超时
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_TIME_OUT)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_RESULT_TIME_OUT;;
        }
        //结果长度不对
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_LEN_SHORT)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_RESULT_LEN_SHORT;
        }
        //结果内存申请失败
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_BUF_MALLOC)
        {
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_RESULT_BUF_MALLOC;
        }
        //正常完成
        if(cmdStageCurrent == PROT_CMD_STA_OVER)
        {
            //正常完成,拷贝节点数据
            errorSub = Can1ProtCopyCmdNodeResult(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,
                                                    resultDataBufPtrPtr,resultBufLen);
            if(errorSub != ERROR_SUB_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,Can1ProtCopyCmdNodeResult Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                            __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
                return errorSub;
            }
            //返回结果超时
            break;
        }
        if(cmdStageCurrent > PROT_CMD_STA_OVER)
        {
            //不支持的状态
            //释放节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN1_PROT_CMD_WAIT_PACK_RESULT_UNSUPPORT_STATE;
        }
    }
    return ERROR_SUB_OK;
}

//CAN1_PROT等待指令结果
void Can1ProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = Can1ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        *resultDataBufPtrPtr = NULL;
        *resultBufLen = 0;
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//CAN1_PROT发送指令并等待结果返回
void Can1ProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //设置结果数据包为正常值
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    *resultBufLen = 0;
    *resultDataBufPtrPtr = NULL;
    //先发出去
    Can1ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待返回
    errorSub = Can1ProtCmdWaitResultBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                                            resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//清理指定通道的上一次指令
void Can1ProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStage = PROT_CMD_STA_OVER;
    SYS_RESULT_PACK sysResultPack;
    uint8_t* resultBufferPtr = NULL;
    uint16_t resultBufferLen = 0;
    //申请访问权
    Can1ProtGetDataAccessLock();
    //首先查询这个指令是否存在
    PROT_CH_STA channelState = Can1ProtCheckCmdChannelState(targetModule,targetBoardID,singalChannel,cmdCode);
    //如果存在上一次的指令,就要处理
    if(channelState == PROT_CH_STA_BUSY)
    {
        //查询指令的状态
        errorSub = Can1ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdCode,&cmdStage);
        if(ERROR_SUB_OK != errorSub)
        {
            //释放访问权
            Can1ProtGetDataAccessRelease();
            //日志信息输出
            CAN_PROT_LOG("Can1ProtQueryCmdState Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                            targetModule,targetBoardID,singalChannel,cmdCode,errorSub);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                            errorSub);
            return;
        }
        //正常查询到了,进行下一步
        if(cmdStage == PROT_CMD_STA_ACK_OVER)
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastComplete OnlyNeedAck,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode);
            //指令只需要ACK,不需要结果,已经完成了,这个时候调用等待,将对应的节点清除掉
            errorSub = Can1ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                Can1ProtGetDataAccessRelease();
                //日志信息输出
                CAN_PROT_LOG("Can1ProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,
                                errorSub);
                //设置错误码
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //不需要返回,通道可以使用了
        }
        else if(cmdStage == PROT_CMD_STA_OVER)
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastComplete NeedResult,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode);
            //指令完成,这个时候调用等待,将对应的节点清除掉
            errorSub = Can1ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                Can1ProtGetDataAccessRelease();
                //日志信息输出
                CAN_PROT_LOG("Can1ProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,errorSub);
                //设置错误码
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //清除完成
            CAN_PROT_LOG("CanSocket LastComplete,But AppNotRead,ResultCode: 0X%08X,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub : 0X%08X\r\n",
                            (uint8_t)sysResultPack.resultCode,(uint16_t)sysResultPack.errorMain,(uint8_t)sysResultPack.errorLevel,
                            (uint32_t)(sysResultPack.errorSub));
            //结果数据包如果有动态内存就需要释放掉
            if((resultBufferLen != 0)&&(resultBufferPtr != NULL))
            {
                //释放内存
                Can1ProtPortMemFree(resultBufferPtr);
            }
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_ACK_ERR)||(cmdStage == PROT_CMD_STA_ACK_TIME_OUT)||(cmdStage == PROT_CMD_STA_ACK_RETRY_FAIL)||
                    (cmdStage == PROT_CMD_STA_RESULT_TIME_OUT)||(cmdStage == PROT_CMD_STA_RESULT_LEN_SHORT)||(cmdStage == PROT_CMD_STA_RESULT_BUF_MALLOC))
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastError,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,cmdState: %d,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,cmdStage);
            //上一次指令报错了 ACK错误,ACK超时,ACK重发超过最大次数,回结果超时,结果长度不对,内存申请异常,执行一次等待将节点清除掉
            Can1ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //现在就剩下waitack和waitresult,报错,上一次的指令未完成
            CAN_PROT_LOG("CanSocket LastCmd NotComplete,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Stage: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,cmdStage);
#if(CAN_PROT_FORCE_CLEAR_NOT_COMPLETE == CONFIG_FUNC_DISABLE)
            //未设置强制清除,上一次的指令未完成
            //必须返回,通道还没办法用
            if(cmdStage == PROT_CMD_STA_WAIT_ACK)
            {
                //释放访问权
                Can1ProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_CAN1_PROT_LAST_WAIT_ACK_CMD_NOT_COMPLETE);
                return;
            }
            else 
            {
                //释放访问权
                Can1ProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN1_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_CAN1_PROT_LAST_WAIT_RESULT_CMD_NOT_COMPLETE);
                return;
            }
#else
            //设置了强制清除,清除掉上次没发出去的指令,删除节点
            Can1ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdCode);
#endif
        }
        else
        {
            //位置的状态
            CAN_PROT_LOG("CanSocket Unknow Stage,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,cmdStage: 0X%02X\r\n",
                            targetModule,targetBoardID,singalChannel,cmdCode,
                            (uint8_t)cmdStage);
            //也清除掉
            Can1ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不用返回,通道可以使用了
        }
    }
    //释放访问权
    Can1ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//回传指令结果
void Can1ProtSendResultByRecv(CAN_PROT_CMD_PACK* canCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    Can1ProtSendResult(canCmdRecvPtr->srcModuleNo,canCmdRecvPtr->srcBoardID,canCmdRecvPtr->singalChannel,canCmdRecvPtr->cmdValue,
                        resultPackPtr->resultCode,resultPackPtr->errorMain,resultPackPtr->errorLevel,
                        resultPackPtr->errorSub,resultParamBuffer,resultBufferLen,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        CAN_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,resultPackPtr->resultCode,resultPackPtr->errorMain,
                        resultPackPtr->errorLevel,resultPackPtr->errorSub);
    }
}

/*******************************************CAN2_PORT*************************************************************/
//初始化CAN2_PORT
void Can2ProtInit(uint8_t selfModuleNo,uint8_t selfBoardID)
{
    //创建互斥信号量
    if(can2ProtInitSet.mutexSocketCanDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return ;
    }
    //创建互斥信号量
    tx_mutex_create(&(can2ProtInitSet.mutexSocketCanDataAccess),"Can2ProtDataAccessMutex",TX_INHERIT);
    //模块号板号
    can2ProtInitSet.selfModuleNo = selfModuleNo;
    can2ProtInitSet.selfBoardID = selfBoardID;
    //创建日志接收缓存链表
    can2ProtInitSet.listRecvLogMsgCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建指令接收缓存链表
    can2ProtInitSet.listRecvCmdMsgCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建结果接收缓存链表
    can2ProtInitSet.listRecvResultMsgCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建上传数据接收缓存链表
    can2ProtInitSet.listRecvUploadMsgCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建结果包发送等待ACK缓存链表
    can2ProtInitSet.listSendResultWaitAckCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建上传包发送等待ACK缓存链表
    can2ProtInitSet.listSendUploadWaitAckCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建指令等待缓存链表
    can2ProtInitSet.listSendCmdWaitCachePtr = ADT_CircularPtrListCreate(Can2ProtPortMemMalloc,Can2ProtPortMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
}

//检查SOCKET是否已经初始化
FlagStatus Can2ProtInitQuery(void)
{
    if(can2ProtInitSet.mutexSocketCanDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return SET;
    }
    return RESET;
}

//日志数据包
extern void Can2ProtRecvProcLogFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//指令包
extern void Can2ProtRecvProcCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//指令ACK数据包
extern void Can2ProtRecvProcAckCmdFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//结果数据包
extern void Can2ProtRecvProcResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//结果ACK数据包
extern void Can2ProtRecvProcAckResultFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//主动上传数据包
extern void Can2ProtRecvProcDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//上传数据包的ACK
extern void Can2ProtRecvProcAckDataUploadFrame(CAN_PROT_ID* protCanID,CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs);
//CAN2_PROT数据包接收
void Can2ProtPackRecv(CAN_RECV_PACK* canRecvPackPtr,uint32_t currentTickMs)
{
    //将接收数据包
    CAN_PROT_ID protCanID;
    //只接受扩展帧
    if(canRecvPackPtr->idType == CAN_ID_STD)
    {
        return;
    }
    //计算CAN ID
    CanProtBasePackIdAnaly(canRecvPackPtr->recvCmdID,&protCanID);
    //数据包分类型处理
    switch(protCanID.packType)
    {
        case PROT_PACK_TYPE_LOG: 
            //日志数据包
            Can2ProtRecvProcLogFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_CMD: 
            //指令数据包
            Can2ProtRecvProcCmdFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_CMD:
            //指令ACK数据包
            Can2ProtRecvProcAckCmdFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_RESULT: 
            //结果数据包
            Can2ProtRecvProcResultFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_RESULT: 
            //结果ACK数据包
            Can2ProtRecvProcAckResultFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_DATA_UPLOAD: 
            //主动上传数据包
            Can2ProtRecvProcDataUploadFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        case PROT_PACK_TYPE_ACK_DATA_UPLOAD: 
            //上传数据包的ACK
            Can2ProtRecvProcAckDataUploadFrame(&protCanID,canRecvPackPtr,currentTickMs);
            break;
        default:
            //没有任何处理
            break;
    }
}

//CAN2在接收数据时候发送ACK
void Can2ProtSendAckInRecv(CAN_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_ACK_PACK* can2ProtAckPtr = Can2ProtPortMemMallocWhileSuccess(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_ACK_PACK));
    can2ProtAckPtr->ackType = ackType;
    can2ProtAckPtr->targetModule = targetModule;
    can2ProtAckPtr->targetBoardID = targetBoardID;
    can2ProtAckPtr->singalChannel = singalChannel;
    can2ProtAckPtr->retryCount = retryCount;
    can2ProtAckPtr->cmdVal = cmdVal;
    can2ProtAckPtr->ackCmd = ackCmd;
    errorSub = Can2ProtAckMsgDispatch(can2ProtAckPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        CAN_PROT_LOG("%s,Can2ProtAckMsgDispatch ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //释放内存
        Can2ProtPortMemFree(can2ProtAckPtr);
    }
}

//CAN2_PROT ACK发送
void Can2ProtAckSend(CAN_PROT_ACK_PACK* ackMsgPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(ackMsgPtr == NULL)
    {
        return;
    }
    if(ackMsgPtr->ackType == CAN_PROT_ACK_CMD)
    {
        //发送ACK CMD
        errorSub = Can2ProtSendAckCmdSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->singalChannel,ackMsgPtr->retryCount,
                                            ackMsgPtr->cmdVal,ackMsgPtr->ackCmd);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can2ProtSendAckCmdSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == CAN_PROT_ACK_RESULT)
    {
        //发送ACK RESULT
        errorSub = Can2ProtSendAckResultSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                                ackMsgPtr->singalChannel,ackMsgPtr->retryCount,
                                                ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can2ProtSendAckResultSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == CAN_PROT_ACK_UPLOAD)
    {
        //发送ACK UPLOAD
        errorSub = Can2ProtSendAckUploadSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->retryCount,ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            CAN_PROT_LOG("%s,Can2ProtSendAckUploadSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    //释放内存
    Can2ProtPortMemFree(ackMsgPtr);
}


//检查接收缓存区数据是否超时
ADT_CMP_RESULT Can2ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon(void* nodePtr,uint32_t utilData)
{
    CAN_PROT_RECV_PACK_CACHE* can2RecvCachePtr = (CAN_PROT_RECV_PACK_CACHE*)nodePtr;
    uint32_t currentTickMs = utilData;
    if(can2RecvCachePtr == NULL)
    {
        return ADT_CMP_EQUAL;
    }
    if(currentTickMs - can2RecvCachePtr->lastFrameRecvTimeStamp >= TIME_OUT_MS_INTERVAL_LONG_PACK_CAN_PROT)
    {
        //当前时间比上一帧的时间差了指定时间,说明帧间隔异常,清除这一帧,首先释放内存
        if(can2RecvCachePtr->recvDataBufferPtr != NULL)
        {
            Can2ProtPortMemFree(can2RecvCachePtr->recvDataBufferPtr);
        }
        Can2ProtPortMemFree(can2RecvCachePtr);
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//从日志列表中找出超时的元素,决定重发或者删除
static ADT_CMP_RESULT ADT_ElementCmpCan2ProtUpdataWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //检查ACK时间是否超时
    if(currentTick >= uploadWaitAckPackPtr->lastSendTimeTickMs)
    {
        if((uploadWaitAckPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(uploadWaitAckPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - uploadWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - uploadWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//上传数据包缓存处理
static void Can2ProtRunPeriodSubUploadWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //从上传列表中找出ACK超时的节点决定重发或者删除报警
    while(1)
    {
        listNodePtr = NULL;
        uploadWaitAckPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendUploadWaitAckCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan2ProtUpdataWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(uploadWaitAckPackPtr->retryCount >= 2)
        {
            //重发超过限制,删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendUploadFailed.RetryMax.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X\r\n",
                                        __func__,__LINE__,uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                        uploadWaitAckPackPtr->uploadID);
            //释放内存
            if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
            {
                Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can2ProtPortMemFree(uploadWaitAckPackPtr);
            //结束本次循环
            continue;
        }
        else
        {
            uploadWaitAckPackPtr->retryCount += 1;
            //重发
            errorSub = Can2ProtSendUploadOnlySample(uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                                    uploadWaitAckPackPtr->retryCount,uploadWaitAckPackPtr->uploadID,
                                                    uploadWaitAckPackPtr->uploadDataBufferPtr,uploadWaitAckPackPtr->uploadDataBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,删除元素
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //错误打印
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendUploadOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can2ProtSendUploadFailed.TargetModule: %d,TaretBoard: %d,UploadID: 0X%08X,RetryCount: %d\r\n",
                                        uploadWaitAckPackPtr->targetModuleNo,uploadWaitAckPackPtr->targetBoardID,
                                        uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->retryCount);
            //释放内存
            if(uploadWaitAckPackPtr->uploadDataBufferPtr != NULL)
            {
                Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can2ProtPortMemFree(uploadWaitAckPackPtr);
            //结束本次循环
            continue;
        }
    }
}

//从结果列表中找出超时的元素,决定重发或者删除
static ADT_CMP_RESULT ADT_ElementCmpCan2ProtResultWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //检查ACK时间是否超时
    if(currentTick >= resultWaitAckPackPtr->lastSendTimeTickMs)
    {
        if((resultWaitAckPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(resultWaitAckPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - resultWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - resultWaitAckPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//结果数据包处理
static void Can2ProtRunPeriodSubResultWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    //从上传列表中找出ACK超时的节点决定重发或者删除报警
    while(1)
    {
        listNodePtr = NULL;
        resultWaitAckPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendResultWaitAckCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan2ProtResultWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(resultWaitAckPackPtr->retryCount >= 2)
        {
            //重发超过限制,删除节点
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendResultFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,
                                        resultWaitAckPackPtr->cmdValue);
            //释放内存
            if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
            {
                Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can2ProtPortMemFree(resultWaitAckPackPtr);
            //结束本次循环
            continue;
        }
        else
        {
            //重发
            errorSub = Can2ProtSendResultOnlySample(resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->singalChannel,
                                                    resultWaitAckPackPtr->retryCount,resultWaitAckPackPtr->cmdValue,resultWaitAckPackPtr->resultCode,
                                                    resultWaitAckPackPtr->errorMainCode,resultWaitAckPackPtr->errorLevelCode,resultWaitAckPackPtr->errorSubCode,
                                                    resultWaitAckPackPtr->resultParamBufferPtr,resultWaitAckPackPtr->resultParamBufferLength);
            resultWaitAckPackPtr->retryCount += 1;
            //时间更新
            resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
            if(errorSub == ERROR_SUB_OK)
            {
                //结束当前循环
                continue;
            }
            //发送失败了,删除元素
            adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
            if(adtResult != ADT_RESULT_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
            }
            //错误打印
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendResultOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can2ProtSendResultFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,SingalChannel: %d,RetryCount: %d\r\n",
                                        resultWaitAckPackPtr->targetModuleNo,resultWaitAckPackPtr->targetBoardID,resultWaitAckPackPtr->cmdValue,
                                        resultWaitAckPackPtr->singalChannel,resultWaitAckPackPtr->retryCount);
            //释放内存
            if(resultWaitAckPackPtr->resultParamBufferPtr != NULL)
            {
                Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can2ProtPortMemFree(resultWaitAckPackPtr);
            //结束本次循环
            continue;
        }
    }
}


//查找链表中等待ACK的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCan2ProtCmdWaitAckListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    uint32_t currentTick = *((uint32_t*)cmpDst);
    //首先检查状态是不是等待ACK
    if(cmdWaitPackPtr->cmdStage != PROT_CMD_STA_WAIT_ACK)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(currentTick >= cmdWaitPackPtr->lastSendTimeTickMs)
    {
        if((cmdWaitPackPtr->targetModuleNo == SYSTEM_MODULE_NO_PC)&&(cmdWaitPackPtr->targetBoardID == SYSTEM_BOARD_ID_PC))
        {
            //目标是PC,透传
            if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_FORWARD)
            {
                return ADT_CMP_EQUAL;
            }
            return ADT_CMP_NOT_EQUAL;
        }
        else if(currentTick - cmdWaitPackPtr->lastSendTimeTickMs > TIME_OUT_MS_ACK_CAN2_PROT_NORMAL)
        {
            return ADT_CMP_EQUAL;
        }
        else
        {
            return ADT_CMP_NOT_EQUAL;
        }
    }
    //这是一种异常情况,系统时间反转,也拿出来重发一下
    return ADT_CMP_EQUAL;
}

//周期性的检查指令重发
static void Can2ProtRunPeriodSubCmdWaitAckPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan2ProtCmdWaitAckListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //检测重发次数
        if(cmdWaitPackPtr->retryCount >= 2)
        {
            //状态设置为ACK超时
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_TIME_OUT;
            //日志发送失败
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendCmdFailed.RetryMax.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X\r\n",
                                        __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                        cmdWaitPackPtr->cmdValue);
            //结束本次循环
            continue;
        }
        else
        {
            cmdWaitPackPtr->retryCount += 1;
            //重发
            errorSub = Can2ProtSendCmdOnlySample(cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->singalChannel,
                                                                cmdWaitPackPtr->retryCount,cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->cmdParamBufferPtr,
                                                                cmdWaitPackPtr->cmdParamBufferLength);
            if(errorSub == ERROR_SUB_OK)
            {
                //发送成功,时间更新
                cmdWaitPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
                //结束当前循环
                continue;
            }
            //发送失败了,错误打印
            CAN_PROT_LOG("E>: %s,%d,Can2ProtSendCmdOnlySample Failed.ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //日志发送失败
            CAN_PROT_LOG("E>: Can2ProtSendCmdFailed.TargetModule: %d,TaretBoard: %d,cmdValue: 0X%08X,SingalChannel: %d,RetryCount: %d\r\n",
                                        cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,cmdWaitPackPtr->cmdValue,
                                        cmdWaitPackPtr->singalChannel,cmdWaitPackPtr->retryCount);
            //状态设置为重发失败
            cmdWaitPackPtr->cmdStage = PROT_CMD_STA_ACK_RETRY_FAIL;
            //结束本次循环
            continue;
        }
    }
}

//查找链表中等待结果的元素且已经超时的元素
static ADT_CMP_RESULT ADT_ElementCmpCan2CmdWaitResultListFindTimeOut(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
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
static void Can2ProtRunPeriodSubCmdWaitResultPackProcess(void)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    uint32_t currentTickMs = OS_TICK_TO_MS(tx_time_get());
    while(1)
    {
        listNodePtr = NULL;
        cmdWaitPackPtr = NULL;
        currentTickMs = OS_TICK_TO_MS(tx_time_get());
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&currentTickMs,
                                                            ADT_ElementCmpCan2CmdWaitResultListFindTimeOut);
        if(listNodePtr == NULL)
        {
            //结束大循环
            break;
        }
        //找到元素了,获取数据指针
        cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
        //超时的元素状态设置为超时
        cmdWaitPackPtr->cmdStage = PROT_CMD_STA_RESULT_TIME_OUT;
        //日志超时
        CAN_PROT_LOG("E>: %s,%d,Can2ProtSendCmdFailed.ResultTimeOut.TargetModule: %d,TaretBoard: %d,CmdVal: 0X%06X,TimeOutSet\r\n",
                                    __func__,__LINE__,cmdWaitPackPtr->targetModuleNo,cmdWaitPackPtr->targetBoardID,
                                    cmdWaitPackPtr->cmdValue,cmdWaitPackPtr->resultTimeOutMs);
        //结束本次循环
        continue;
    }
}

//CAN2_PROT内部周期性数据处理
void Can2ProtRunPeriod(uint32_t currentTickMs)
{
    //检查CMD接收链表内部数据
    ADT_CircularPtrListTraverseDelete(can2ProtInitSet.listRecvCmdMsgCachePtr,Can2ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查Result链表内部数据
    ADT_CircularPtrListTraverseDelete(can2ProtInitSet.listRecvResultMsgCachePtr,Can2ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查upload链表内部数据
    ADT_CircularPtrListTraverseDelete(can2ProtInitSet.listRecvUploadMsgCachePtr,Can2ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //检查log链表内部数据
    ADT_CircularPtrListTraverseDelete(can2ProtInitSet.listRecvLogMsgCachePtr,Can2ProtRunPeriodSubRecvCacheDeleteNodeMeetConditon,
                                        currentTickMs);
    //申请访问权
    Can2ProtGetDataAccessLock();
    //上传数据包处理
    Can2ProtRunPeriodSubUploadWaitAckPackProcess();
    //结果数据包处理
    Can2ProtRunPeriodSubResultWaitAckPackProcess();
    //指令包等待ACK的数据处理
    Can2ProtRunPeriodSubCmdWaitAckPackProcess();
    //指令包等待结果的数据处理
    Can2ProtRunPeriodSubCmdWaitResultPackProcess();
    //释放访问权
    Can2ProtGetDataAccessRelease();
}

//CAN2_PROT发送日志
void Can2ProtSendLog(uint8_t srcModule,uint8_t srcBoardID,uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,
                            uint8_t retryCount,uint8_t* logBuffer,uint16_t logLength)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    CAN_PROT_PACK packLog;
    packLog.retryCount = retryCount;
    packLog.singalChannel = singalChannel;
    packLog.targetBoard = (targetBoardID&0X1F);
    packLog.targetModule = (targetModule&0X07);
    packLog.srcBoardID = (srcBoardID&0X1F);
    packLog.srcModuleNo = (srcModule&0X07);
    packLog.packDataLen = logLength;
    packLog.packDataBufPtr = logBuffer;
    packLog.packType = PROT_PACK_TYPE_LOG;
    errorSub = CanProtBaseSendPack(PORT_CAN2,&packLog);
    if(errorSub != ERROR_SUB_OK)
    {
        CAN_PROT_LOG("E>: %s,%d, 0X%08X\r\n",__func__,__LINE__,errorSub);
    }
    return;
}

//CAN2_PROT发送日志,简易模式
void Can2ProtSendLogSample(uint8_t* logBuffer,uint16_t logLength)
{
    Can2ProtSendLog(can2ProtInitSet.selfModuleNo,can2ProtInitSet.selfBoardID,SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,
                                        0,0,logBuffer,logLength);
}

//CAN2_PROT发送结果,带重发功能
void Can2ProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    uint32_t waitStartTimeTick = 0;
    uint32_t currentTimeTick = 0;
    //首先创建等待结果包
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = (CAN_PROT_RESULT_WAIT_ACK_PACK*)Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_RESULT_WAIT_ACK_PACK));
    if(resultWaitAckPackPtr == NULL)
    {
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_RESULT_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    resultWaitAckPackPtr->resultParamBufferLength = resultBufferLen;
    if(resultWaitAckPackPtr->resultParamBufferLength != 0)
    {
        resultWaitAckPackPtr->resultParamBufferPtr = Can2ProtPortMemMalloc(resultWaitAckPackPtr->resultParamBufferLength);
        if(resultWaitAckPackPtr->resultParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can2ProtPortMemFree(resultWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_RESULT_WAIT_ACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        resultWaitAckPackPtr->resultParamBufferPtr = NULL;
    }
    //数据拷贝
    resultWaitAckPackPtr->targetModuleNo = targetModule;
    resultWaitAckPackPtr->targetBoardID = targetBoardID;
    resultWaitAckPackPtr->singalChannel = singalChannel;
    resultWaitAckPackPtr->retryCount = 0;
    resultWaitAckPackPtr->cmdValue = cmdVal;
    resultWaitAckPackPtr->resultCode = resultCode;
    resultWaitAckPackPtr->errorMainCode = errorMain;
    resultWaitAckPackPtr->errorLevelCode = errorLevel;
    resultWaitAckPackPtr->errorSubCode = errorSub;
    //内存拷贝
    if(resultBufferLen != 0)
    {
        UserMemCopy(resultWaitAckPackPtr->resultParamBufferPtr,resultParamBuffer,resultBufferLen);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    waitStartTimeTick = OS_TICK_TO_MS(tx_time_get());
    do
    {
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendResultWaitAckCachePtr,(void*)resultWaitAckPackPtr,
                                                            ADT_CanProtResultWaitAckElementFind);
        if(listNodePtr == NULL)
        {
            //链表中不存在指定元素,可以直接发送
            break;
        }
        //链表中存在指定元素了,不能直接发送
        currentTimeTick = OS_TICK_TO_MS(tx_time_get());
        if(currentTimeTick - waitStartTimeTick > TIME_OUT_MS_MAX_CAN_PROT_WAIT_RESULT_CH)
        {
            //超时了,这一帧数据发不出去,释放内存并报错
            if(resultBufferLen != 0)
            {
                Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            Can2ProtPortMemFree(resultWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_RESULT_WAIT_ACK_CHANNEL_BUSY);
            return;
        }
        else
        {
            //还没超时,延时下次检测
            CoreDelayMinTick();
        }
    }while(listNodePtr != NULL);
    //申请访问权
    Can2ProtGetDataAccessLock();
    //设置发送时间
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往结果包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can2ProtInitSet.listSendResultWaitAckCachePtr,resultWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can2ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_RESULT_WAIT_ACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendResultWaitAckCachePtr,(void*)resultWaitAckPackPtr,
                                                        ADT_CanProtResultWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can2ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_RESULT_WAIT_ACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can2ProtSendResultOnlySample(targetModule,targetBoardID,singalChannel,0,cmdVal,resultCode,errorMain,errorLevel,errorSub,
                                                    resultWaitAckPackPtr->resultParamBufferPtr,resultWaitAckPackPtr->resultParamBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendResultWaitAckCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            Can2ProtPortMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        Can2ProtPortMemFree(resultWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}



//CAN2_PROT发送上传,带重发功能
void Can2ProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    uint32_t waitStartTimeTick = 0;
    uint32_t currentTimeTick = 0;
    //首先创建等待结果包
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_UPLOAD_WAIT_ACK_PACK));
    if(uploadWaitAckPackPtr == NULL)
    {
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_UPLOAD_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    uploadWaitAckPackPtr->uploadDataBufferLength = uploadDataLen;
    if(uploadWaitAckPackPtr->uploadDataBufferLength != 0)
    {
        uploadWaitAckPackPtr->uploadDataBufferPtr = Can2ProtPortMemMalloc(uploadWaitAckPackPtr->uploadDataBufferLength);
        if(uploadWaitAckPackPtr->uploadDataBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can2ProtPortMemFree(uploadWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_UPLOAD_WAIT_ACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        uploadWaitAckPackPtr->uploadDataBufferPtr = NULL;
    }
    //数据拷贝
    uploadWaitAckPackPtr->targetModuleNo = targetModule;
    uploadWaitAckPackPtr->targetBoardID = targetBoardID;
    uploadWaitAckPackPtr->retryCount = 0;
    uploadWaitAckPackPtr->uploadID = uploadDataID;
    //内存拷贝
    if(uploadDataLen != 0)
    {
        UserMemCopy(uploadWaitAckPackPtr->uploadDataBufferPtr,uploadDataBuffer,uploadDataLen);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    waitStartTimeTick = OS_TICK_TO_MS(tx_time_get());
    do
    {
        listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadWaitAckPackPtr,
                                                            ADT_CanProtUploadWaitAckElementFind);
        if(listNodePtr == NULL)
        {
            //链表中不存在指定元素,可以直接发送
            break;
        }
        //链表中存在指定元素了,不能直接发送
        currentTimeTick = OS_TICK_TO_MS(tx_time_get());
        if(currentTimeTick - waitStartTimeTick > TIME_OUT_MS_MAX_CAN_PROT_WAIT_UPLOAD_CH)
        {
            //超时了,这一帧数据发不出去,释放内存并报错
            if(uploadDataLen != 0)
            {
                Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            Can2ProtPortMemFree(uploadWaitAckPackPtr);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_UPLOAD_WAIT_ACK_CHANNEL_BUSY);
            return;
        }
        else
        {
            //还没超时,延时下次检测
            CoreDelayMinTick();
        }
    }while(listNodePtr != NULL);
    //申请访问权
    Can2ProtGetDataAccessLock();
    //设置发送时间
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往结果包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can2ProtInitSet.listSendUploadWaitAckCachePtr,uploadWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can2ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_UPLOAD_WAIT_ACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendUploadWaitAckCachePtr,(void*)uploadWaitAckPackPtr,ADT_CanProtUploadWaitAckElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can2ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_UPLOAD_WAIT_ACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can2ProtSendUploadOnlySample(targetModule,targetBoardID,uploadWaitAckPackPtr->retryCount,
                                                        uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->uploadDataBufferPtr,
                                                        uploadWaitAckPackPtr->uploadDataBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendUploadWaitAckCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            Can2ProtPortMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        Can2ProtPortMemFree(uploadWaitAckPackPtr);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


//查询指令的当前状态
ERROR_SUB Can2ProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                                    PROT_CMD_STA* cmdStage)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can2ProtGetDataAccessRelease();
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_QUERY_STATE_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //找到元素了,获取当前状态
    *cmdStage = cmdWaitPackPtr->cmdStage;
    //释放访问权
    Can2ProtGetDataAccessRelease();
    return ERROR_SUB_OK;
}

//检查通道状态
PROT_CH_STA Can2ProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    //释放访问权
    Can2ProtGetDataAccessRelease();
    if(listNodePtr == NULL)
    {
        //通道空闲
        return PROT_CH_STA_IDLE;
    }
    else
    {
        //通道忙
        return PROT_CH_STA_BUSY;
    }
}

//删除节点
static void Can2ProtDeleteCmdNode(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{   
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //日志打印
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //释放节点数据
    if(cmdWaitPackPtr->resultParamBufferPtr != NULL)
    {
        Can2ProtPortMemFree(cmdWaitPackPtr->resultParamBufferPtr);
    }
    if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
    {
        Can2ProtPortMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    Can2ProtPortMemFree(cmdWaitPackPtr);
    return;
}

//拷贝节点数据
static ERROR_SUB Can2ProtCopyCmdNodeResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                                    SYS_RESULT_PACK* socketCanResultPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    CAN_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.singalChannel = singalChannel;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_CanProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //日志打印
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_RESULT_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (CAN_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //拷贝数据
    socketCanResultPtr->resultCode = cmdWaitPackPtr->resultCode;
    socketCanResultPtr->errorMain = cmdWaitPackPtr->errorMainCode;
    socketCanResultPtr->errorLevel = cmdWaitPackPtr->errorLevelCode;
    socketCanResultPtr->errorSub = cmdWaitPackPtr->errorSubCode;
    *resultBufLen = cmdWaitPackPtr->resultParamBufferLength;
    if(*resultBufLen != 0)
    {
        *resultDataBufPtrPtr = cmdWaitPackPtr->resultParamBufferPtr;
    }
    else
    {
        *resultDataBufPtrPtr = NULL;
    }
    //释放节点数据
    if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
    {
        Can2ProtPortMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    Can2ProtPortMemFree(cmdWaitPackPtr);
    return ERROR_SUB_OK;
}

//等待ACK完成
static ERROR_SUB Can2ProtCmdWaitAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_CAN_PROT);
        //查询
        errorSub = Can2ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_ACK)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //ACK错误
        if(cmdStageCurrent == PROT_CMD_STA_ACK_ERR)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //正常结束,而且不需要结果包会进入这个状态,释放节点,返回完成
            break;
        }
        //其余状态,都是完成
        if(cmdStageCurrent >= PROT_CMD_STA_WAIT_RESULT)
        {
            break;
        }
    }
    return ERROR_SUB_OK;
}

//SOCKET1发送指令,基础,只要发出去
static void Can2ProtSendCmdBase(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,uint8_t* cmdBufferPtr,
                                        uint16_t cmdBufferLength,FunctionalState cmdNeedResult,uint32_t resultTimeOutMs,
                                        uint16_t exceptResultLengthMin,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //首先创建等待指令包
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPackPtr = (CAN_PROT_CMD_WAIT_PACK*)Can2ProtPortMemMalloc(DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_WAIT_PACK));
    if(cmdWaitPackPackPtr == NULL)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    cmdWaitPackPackPtr->cmdParamBufferLength = cmdBufferLength;
    if(cmdWaitPackPackPtr->cmdParamBufferLength != 0)
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = Can2ProtPortMemMalloc(cmdWaitPackPackPtr->cmdParamBufferLength);
        if(cmdWaitPackPackPtr->cmdParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            Can2ProtPortMemFree(cmdWaitPackPackPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_BUF_MEM_MALLOC);
            return;
        }
    }
    else
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = NULL;
    }
    //数据拷贝
    cmdWaitPackPackPtr->targetModuleNo = targetModule;
    cmdWaitPackPackPtr->targetBoardID = targetBoardID;
    cmdWaitPackPackPtr->singalChannel = singalChannel;
    cmdWaitPackPackPtr->retryCount = 0;
    cmdWaitPackPackPtr->cmdValue = cmdVal;
    cmdWaitPackPackPtr->cmdStage = PROT_CMD_STA_WAIT_ACK;
    cmdWaitPackPackPtr->cmdNeedResult = cmdNeedResult;
    cmdWaitPackPackPtr->resultTimeOutMs = resultTimeOutMs;
    cmdWaitPackPackPtr->exceptResultLengthMin = exceptResultLengthMin;
    //结果初始值
    cmdWaitPackPackPtr->resultCode = PROT_RESULT_SUCCESS;
    cmdWaitPackPackPtr->errorMainCode = ERROR_MAIN_OK;
    cmdWaitPackPackPtr->errorLevelCode = ERROR_LEVEL_NONE;
    cmdWaitPackPackPtr->errorSubCode = ERROR_SUB_OK;
    cmdWaitPackPackPtr->resultParamBufferPtr = NULL;
    cmdWaitPackPackPtr->resultParamBufferLength = 0;
    //内存拷贝
    if(cmdBufferLength != 0)
    {
        UserMemCopy(cmdWaitPackPackPtr->cmdParamBufferPtr,cmdBufferPtr,cmdBufferLength);
    }
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,
                                                        ADT_CanProtCmdWaitElementFind);
    if(listNodePtr != NULL)
    {
        //链表中存放了一个指令,需要对这个指令做处理
        CAN_PROT_CMD_WAIT_PACK* cmdWaitPackExistPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
        //查看指令状态,在等待ACK和等待结果的情况下,报错,其他状态下,清除节点
        if((cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //上一次指令未结束
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_LAST_NOT_COMPLETE);
            return;
        }
        //上一次指令已经结束了,但是调用者没有读取
        else
        {
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //上一次指令结束未读取
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_LAST_NOT_READ);
            return;
        }
    }
    //申请访问权
    Can2ProtGetDataAccessLock();
    //设置发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //往指令等待包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeFront(can2ProtInitSet.listSendCmdWaitCachePtr,cmdWaitPackPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(can2ProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,ADT_CanProtCmdWaitPackElementCmp);
    if(listNodePtr == NULL)
    {
        CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_LIST_FIND);
        return;
    }
    //数据发送
    errorSubLocal = Can2ProtSendCmdOnlySample(targetModule,targetBoardID,singalChannel,0,cmdVal,cmdWaitPackPackPtr->cmdParamBufferPtr,
                                                            cmdWaitPackPackPtr->cmdParamBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //删除插入的节点
        adtResult = ADT_CircularPtrListDeleteNodeCurrent(can2ProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
        if(adtResult != ADT_RESULT_OK)
        {
            CAN_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
        }
        //释放访问权
        Can2ProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        Can2ProtPortMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        errorSubLocal);
        return;
    }
    //发送成功,更新发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//CAN2_PROT发送指令,基础,只要发出去
void Can2ProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    Can2ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,
                        cmdSendPtr->resultTimeOutMs,cmdSendPtr->exceptResultLengthMin,resultPackPtr);
}

//CAN2_PROT发送指令,并等待ACK
void Can2ProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先发出去
    Can2ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待ACK
    errorSub = Can2ProtCmdWaitAck(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//CAN2_PROT等待指令结果
static ERROR_SUB Can2ProtCmdWaitResultBase(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                            SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_CAN_PROT);
        //查询
        errorSub = Can2ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //检测异常,直接返回,内部故障了
            return errorSub;
        }
        //检查状态
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_ACK)
        {
            //还在等待ACK,下次运转
            continue; 
        }
        //ACK错误
        if(cmdStageCurrent == PROT_CMD_STA_ACK_ERR)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //正常结束,而且不需要结果包会进入这个状态,释放节点,返回完成
            break;
        }
        //等待结果,继续等待
        if(cmdStageCurrent == PROT_CMD_STA_WAIT_RESULT)
        {
            continue;
        }
        //结果超时
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_TIME_OUT)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_RESULT_TIME_OUT;;
        }
        //结果长度不对
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_LEN_SHORT)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_RESULT_LEN_SHORT;
        }
        //结果内存申请失败
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_BUF_MALLOC)
        {
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_RESULT_BUF_MALLOC;
        }
        //正常完成
        if(cmdStageCurrent == PROT_CMD_STA_OVER)
        {
            //正常完成,拷贝节点数据
            errorSub = Can2ProtCopyCmdNodeResult(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,
                                                    resultDataBufPtrPtr,resultBufLen);
            if(errorSub != ERROR_SUB_OK)
            {
                CAN_PROT_LOG("E>: %s,%d,Can2ProtCopyCmdNodeResult Failed,targetModule: %d,targetBoardID: %d,singalChannel: %d,cmdVal: 0X%08X\r\n",
                                            __func__,__LINE__,targetModule,targetBoardID,singalChannel,cmdVal);
                return errorSub;
            }
            //返回结果超时
            break;
        }
        if(cmdStageCurrent > PROT_CMD_STA_OVER)
        {
            //不支持的状态
            //释放节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_CAN2_PROT_CMD_WAIT_PACK_RESULT_UNSUPPORT_STATE;
        }
    }
    return ERROR_SUB_OK;
}

//CAN2_PROT等待指令结果
void Can2ProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = Can2ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        *resultDataBufPtrPtr = NULL;
        *resultBufLen = 0;
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//CAN2_PROT发送指令并等待结果返回
void Can2ProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //设置结果数据包为正常值
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    *resultBufLen = 0;
    *resultDataBufPtrPtr = NULL;
    //先发出去
    Can2ProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                        cmdSendPtr->cmdBufferPtr,cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待返回
    errorSub = Can2ProtCmdWaitResultBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->singalChannel,cmdSendPtr->cmdVal,
                                            resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//清理指定通道的上一次指令
void Can2ProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStage = PROT_CMD_STA_OVER;
    SYS_RESULT_PACK sysResultPack;
    uint8_t* resultBufferPtr = NULL;
    uint16_t resultBufferLen = 0;
    //申请访问权
    Can2ProtGetDataAccessLock();
    //首先查询这个指令是否存在
    PROT_CH_STA channelState = Can2ProtCheckCmdChannelState(targetModule,targetBoardID,singalChannel,cmdCode);
    //如果存在上一次的指令,就要处理
    if(channelState == PROT_CH_STA_BUSY)
    {
        //查询指令的状态
        errorSub = Can2ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdCode,&cmdStage);
        if(ERROR_SUB_OK != errorSub)
        {
            //释放访问权
            Can2ProtGetDataAccessRelease();
            //日志信息输出
            CAN_PROT_LOG("Can2ProtQueryCmdState Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                            targetModule,targetBoardID,singalChannel,cmdCode,errorSub);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                            errorSub);
            return;
        }
        //正常查询到了,进行下一步
        if(cmdStage == PROT_CMD_STA_ACK_OVER)
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastComplete OnlyNeedAck,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode);
            //指令只需要ACK,不需要结果,已经完成了,这个时候调用等待,将对应的节点清除掉
            errorSub = Can2ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                Can2ProtGetDataAccessRelease();
                //日志信息输出
                CAN_PROT_LOG("Can2ProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,
                                errorSub);
                //设置错误码
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //不需要返回,通道可以使用了
        }
        else if(cmdStage == PROT_CMD_STA_OVER)
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastComplete NeedResult,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode);
            //指令完成,这个时候调用等待,将对应的节点清除掉
            errorSub = Can2ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                Can2ProtGetDataAccessRelease();
                //日志信息输出
                CAN_PROT_LOG("Can2ProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,errorSub);
                //设置错误码
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //清除完成
            CAN_PROT_LOG("CanSocket LastComplete,But AppNotRead,ResultCode: 0X%08X,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub : 0X%08X\r\n",
                            (uint8_t)sysResultPack.resultCode,(uint16_t)sysResultPack.errorMain,(uint8_t)sysResultPack.errorLevel,
                            (uint32_t)(sysResultPack.errorSub));
            //结果数据包如果有动态内存就需要释放掉
            if((resultBufferLen != 0)&&(resultBufferPtr != NULL))
            {
                //释放内存
                Can2ProtPortMemFree(resultBufferPtr);
            }
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_ACK_ERR)||(cmdStage == PROT_CMD_STA_ACK_TIME_OUT)||(cmdStage == PROT_CMD_STA_ACK_RETRY_FAIL)||
                    (cmdStage == PROT_CMD_STA_RESULT_TIME_OUT)||(cmdStage == PROT_CMD_STA_RESULT_LEN_SHORT)||(cmdStage == PROT_CMD_STA_RESULT_BUF_MALLOC))
        {
            //日志打印
            CAN_PROT_LOG("CanSocket LastError,But AppNotRead,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,cmdState: %d,Will Clear\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,cmdStage);
            //上一次指令报错了 ACK错误,ACK超时,ACK重发超过最大次数,回结果超时,结果长度不对,内存申请异常,执行一次等待将节点清除掉
            Can2ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //现在就剩下waitack和waitresult,报错,上一次的指令未完成
            CAN_PROT_LOG("CanSocket LastCmd NotComplete,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,Stage: 0X%08X\r\n",
                                targetModule,targetBoardID,singalChannel,cmdCode,cmdStage);
#if(CAN_PROT_FORCE_CLEAR_NOT_COMPLETE == CONFIG_FUNC_DISABLE)
            //未设置强制清除,上一次的指令未完成
            //必须返回,通道还没办法用
            if(cmdStage == PROT_CMD_STA_WAIT_ACK)
            {
                //释放访问权
                Can2ProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_CAN2_PROT_LAST_WAIT_ACK_CMD_NOT_COMPLETE);
                return;
            }
            else 
            {
                //释放访问权
                Can2ProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN2_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_CAN2_PROT_LAST_WAIT_RESULT_CMD_NOT_COMPLETE);
                return;
            }
#else
            //设置了强制清除,清除掉上次没发出去的指令,删除节点
            Can2ProtDeleteCmdNode(targetModule,targetBoardID,singalChannel,cmdCode);
#endif
        }
        else
        {
            //位置的状态
            CAN_PROT_LOG("CanSocket Unknow Stage,TargetModule: %d,TargetBoard: %d,Channel: %d,Cmd: 0X%08X,cmdStage: 0X%02X\r\n",
                            targetModule,targetBoardID,singalChannel,cmdCode,
                            (uint8_t)cmdStage);
            //也清除掉
            Can2ProtCmdWaitResultBase(targetModule,targetBoardID,singalChannel,cmdCode,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不用返回,通道可以使用了
        }
    }
    //释放访问权
    Can2ProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//回传指令结果
void Can2ProtSendResultByRecv(CAN_PROT_CMD_PACK* canCmdRecvPtr,SYS_RESULT_PACK* resultPackPtr,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen)
{
    Can2ProtSendResult(canCmdRecvPtr->srcModuleNo,canCmdRecvPtr->srcBoardID,canCmdRecvPtr->singalChannel,canCmdRecvPtr->cmdValue,
                        resultPackPtr->resultCode,resultPackPtr->errorMain,resultPackPtr->errorLevel,
                        resultPackPtr->errorSub,resultParamBuffer,resultBufferLen,resultPackPtr);
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //错误日志打印
        CAN_PROT_LOG("%s,%d,Result: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,__LINE__,resultPackPtr->resultCode,resultPackPtr->errorMain,
                        resultPackPtr->errorLevel,resultPackPtr->errorSub);
    }
}



/***********************************************************CAN1_CAN2共用************************************************************/
//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtCmdWaitPackElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrDst = (CAN_PROT_CMD_WAIT_PACK*)cmpDst;
    //元素比较,直接比较指针
    if(cmdWaitPackPtrSrc == cmdWaitPackPtrDst)
    {
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtCmdWaitElementFind(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrDst = (CAN_PROT_CMD_WAIT_PACK*)cmpDst;
    if(cmdWaitPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrDst == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->targetModuleNo != cmdWaitPackPtrDst->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->targetBoardID != cmdWaitPackPtrDst->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->singalChannel != cmdWaitPackPtrDst->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->cmdValue != cmdWaitPackPtrDst->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//从链表中找出指定的节点,元素内容匹配.使用指令头查找
static ADT_CMP_RESULT ADT_CanProtCmdWaitElementFindWithHeader(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (CAN_PROT_CMD_WAIT_PACK*)cmpSrc;
    CAN_PROT_CMD_WAIT_PACK_HEADER* cmdWaitPackHeaderPtrDst = (CAN_PROT_CMD_WAIT_PACK_HEADER*)cmpDst;
    if(cmdWaitPackPtrSrc == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackHeaderPtrDst == NULL)
    {
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->targetModuleNo != cmdWaitPackHeaderPtrDst->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->targetBoardID != cmdWaitPackHeaderPtrDst->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->singalChannel != cmdWaitPackHeaderPtrDst->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(cmdWaitPackPtrSrc->cmdValue != cmdWaitPackHeaderPtrDst->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtUploadWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrSrc = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrDst = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpDst;
    //元素比较,直接比较指针
    if(uploadWaitAckPackPtrDst == uploadWaitAckPackPtrSrc)
    {
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtUploadWaitAckElementFind(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrSrc = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtrDst = (CAN_PROT_UPLOAD_WAIT_ACK_PACK*)cmpDst;
    if(uploadWaitAckPackPtrSrc->targetModuleNo != uploadWaitAckPackPtrDst->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->targetBoardID != uploadWaitAckPackPtrDst->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(uploadWaitAckPackPtrSrc->uploadID != uploadWaitAckPackPtrDst->uploadID)
    {
        //上报ID号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}
//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_CanProtResultWaitAckElementFind(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrSrc = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrDst = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpDst;
    if(resultWaitAckPackPtrSrc->targetModuleNo != resultWaitAckPackPtrDst->targetModuleNo)
    {
        //模块号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultWaitAckPackPtrSrc->targetBoardID != resultWaitAckPackPtrDst->targetBoardID)
    {
        //板号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultWaitAckPackPtrSrc->singalChannel != resultWaitAckPackPtrDst->singalChannel)
    {
        //通道号匹配
        return ADT_CMP_NOT_EQUAL;
    }
    if(resultWaitAckPackPtrSrc->cmdValue != resultWaitAckPackPtrDst->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_CanProtResultWaitAckElementCmp(void* cmpSrc,void* cmpDst)
{
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrSrc = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpSrc;
    CAN_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtrDst = (CAN_PROT_RESULT_WAIT_ACK_PACK*)cmpDst;
    //元素比较,直接比较指针
    if(resultWaitAckPackPtrDst == resultWaitAckPackPtrSrc)
    {
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//申请内存
uint8_t* CanProtMemMalloc(uint8_t targetModule,uint8_t targetBoardID,uint32_t byteCount)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        return Can1ProtPortMemMallocWhileSuccess(byteCount);
    }
    else
    {
        return Can2ProtPortMemMallocWhileSuccess(byteCount);
    }
}

//释放内存
void CanProtMemFree(uint8_t targetModule,uint8_t targetBoardID,uint8_t* memPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtPortMemFree(memPtr);
    }
    else
    {
        Can2ProtPortMemFree(memPtr);
    }
}

//创建结果包
SYS_RESULT_PACK* CanProtCreateSysResultPack(uint8_t targetModule,uint8_t targetBoardID)
{
    SYS_RESULT_PACK* sysResultPackPtr = (SYS_RESULT_PACK*)CanProtMemMalloc(targetModule,targetBoardID,DIM_STRUCT_LENGTH_BYTE(SYS_RESULT_PACK));
    sysResultPackPtr->errorMain = ERROR_MAIN_OK;
    sysResultPackPtr->errorLevel = ERROR_LEVEL_NONE;
    sysResultPackPtr->errorSub = ERROR_SUB_OK;
    sysResultPackPtr->resultCode = PROT_RESULT_FAIL;
    return sysResultPackPtr;
}

//释放结果包
void CanProtFreeSysResultPack(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* sysResultPackPtr,uint8_t* resultDataBufPtr,uint16_t resultBufLen)
{
    if((resultBufLen != 0)&&(resultDataBufPtr != NULL))
    {
        CanProtMemFree(targetModule,targetBoardID,resultDataBufPtr);
    }
    CanProtMemFree(targetModule,targetBoardID,(uint8_t*)sysResultPackPtr);
}

//释放接收缓存
void CanProtFreeRecvDataBuf(uint8_t targetModule,uint8_t targetBoardID,uint8_t* resultDataBufPtr,uint16_t resultBufLen)
{
    if((resultBufLen != 0)&&(resultDataBufPtr != NULL))
    {
        CanProtMemFree(targetModule,targetBoardID,resultDataBufPtr);
    }
}

//获取指定指令的配置讯息
CAN_PROT_CMD_CONFIG* CanProtGetCmdConfig(uint32_t cmd,CAN_PROT_CMD_CONFIG* configArray,uint16_t arrayLen)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < arrayLen; indexUtil++)
    {
        if(cmd == configArray[indexUtil].cmdVal)
        {
            return (CAN_PROT_CMD_CONFIG*)(&(configArray[indexUtil]));
        }
    }
    return NULL;
}

//初始化CAN CMD
static void CanProtSetDefaultCanCmdSend(CAN_PROT_CMD_SEND* cmdSendPtr)
{
    cmdSendPtr->targetModule = SYSTEM_MODULE_NO_PC;
    cmdSendPtr->targetBoardID = SYSTEM_BOARD_ID_PC;
    cmdSendPtr->singalChannel = 0XFF;
    cmdSendPtr->cmdVal = 0X00FFFFFF;
    cmdSendPtr->cmdBufferPtr = NULL;
    cmdSendPtr->cmdBufferLength = 0;
    cmdSendPtr->cmdNeedResult = DISABLE;
    cmdSendPtr->resultTimeOutMs = 60000;
    cmdSendPtr->exceptResultLengthMin = 65535;
}

//创建发送指令
CAN_PROT_CMD_SEND* CanProtCreateCmdSend(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                                CAN_PROT_CMD_CONFIG* canCmdConfigPtr)
{
    CAN_PROT_CMD_SEND* canCmdSendPtr = (CAN_PROT_CMD_SEND*)CanProtMemMalloc(targetModule,targetBoardID,
                                                                                DIM_STRUCT_LENGTH_BYTE(CAN_PROT_CMD_SEND));
    //设置默认值
    CanProtSetDefaultCanCmdSend(canCmdSendPtr);
    //指令赋值
    canCmdSendPtr->targetModule = targetModule;
    canCmdSendPtr->targetBoardID = targetBoardID;
    canCmdSendPtr->singalChannel = singalChannel;
    canCmdSendPtr->cmdVal = cmdCode;
    canCmdSendPtr->cmdNeedResult = canCmdConfigPtr->cmdNeedResult;
    canCmdSendPtr->exceptResultLengthMin = canCmdConfigPtr->exceptResultLengthMin;
    canCmdSendPtr->resultTimeOutMs = canCmdConfigPtr->resultTimeOutMs;
    //内存申请
    if(canCmdConfigPtr->paramBufferLen != 0)
    {
        canCmdSendPtr->cmdBufferPtr = CanProtMemMalloc(canCmdSendPtr->targetModule,canCmdSendPtr->targetBoardID,canCmdConfigPtr->paramBufferLen);
        //参数参数
        canCmdSendPtr->cmdBufferLength = canCmdConfigPtr->paramBufferLen;
        //参数清除
        UserMemClear(canCmdSendPtr->cmdBufferPtr,canCmdSendPtr->cmdBufferLength);
    }
    return canCmdSendPtr;
}

//销毁发送指令
void CanProtDestroyCmdSend(CAN_PROT_CMD_SEND** canCmdSendPtrPtr)
{
    CAN_PROT_CMD_SEND* canCmdSendPtr = *canCmdSendPtrPtr;
    if(canCmdSendPtr == NULL)
    {
        return;
    }
    //释放缓存
    if((canCmdSendPtr->cmdBufferLength != 0)&&(canCmdSendPtr->cmdBufferPtr != NULL))
    {
        CanProtMemFree(canCmdSendPtr->targetModule,canCmdSendPtr->targetBoardID,canCmdSendPtr->cmdBufferPtr);
    }
    //释放本体
    CanProtMemFree(canCmdSendPtr->targetModule,canCmdSendPtr->targetBoardID,(uint8_t*)canCmdSendPtr);
}

//CAN_PROT发送结果,带重发功能
void CanProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                PROT_RESULT resultCode,ERROR_MAIN errorMain,ERROR_LEVEL errorLevel,ERROR_SUB errorSub,
                                uint8_t* resultParamBuffer,uint16_t resultBufferLen,SYS_RESULT_PACK* resultPackPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtSendResult(targetModule,targetBoardID,singalChannel,cmdVal,resultCode,errorMain,errorLevel,errorSub,
                            resultParamBuffer,resultBufferLen,resultPackPtr);
    }
    else
    {
        Can2ProtSendResult(targetModule,targetBoardID,singalChannel,cmdVal,resultCode,errorMain,errorLevel,errorSub,
                            resultParamBuffer,resultBufferLen,resultPackPtr);
    }
}

//CAN_PROT发送上传,带重发功能
void CanProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtSendUpload(targetModule,targetBoardID,uploadDataID,uploadDataBuffer,uploadDataLen,resultPackPtr);
    }
    else
    {
        Can2ProtSendUpload(targetModule,targetBoardID,uploadDataID,uploadDataBuffer,uploadDataLen,resultPackPtr);
    }
}

//查询指令的当前状态
ERROR_SUB CanProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,PROT_CMD_STA* cmdStage)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        return Can1ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,cmdStage);
    }
    else
    {
        return Can2ProtQueryCmdState(targetModule,targetBoardID,singalChannel,cmdVal,cmdStage);
    }
}

//检查通道状态
PROT_CH_STA CanProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        return Can1ProtCheckCmdChannelState(targetModule,targetBoardID,singalChannel,cmdVal);
    }
    else
    {
        return Can2ProtCheckCmdChannelState(targetModule,targetBoardID,singalChannel,cmdVal);
    }
}

//CAN_PROT发送指令,基础,只要发出去
void CanProtSendCmdNoWait(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtSendCmdNoWait(cmdSendPtr,resultPackPtr);
    }
    else
    {
        Can2ProtSendCmdNoWait(cmdSendPtr,resultPackPtr);
    }
}

//CAN_PROT发送指令,并等待ACK
void CanProtSendCmdWaitAck(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtSendCmdWaitAck(cmdSendPtr,resultPackPtr);
    }
    else
    {
        Can2ProtSendCmdWaitAck(cmdSendPtr,resultPackPtr);
    }
}

//CAN_PROT等待指令结果
void CanProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdVal,
                                SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtCmdWaitResult(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    }
    else
    {
        Can2ProtCmdWaitResult(targetModule,targetBoardID,singalChannel,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    }
}

//CAN_PROT发送指令并等待结果返回
void CanProtSendCmdWaitResult(CAN_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                    uint16_t* resultBufLen)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtSendCmdWaitResult(cmdSendPtr,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    }
    else
    {
        Can2ProtSendCmdWaitResult(cmdSendPtr,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    }
}

//清理指定通道的上一次指令
void CanProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint8_t singalChannel,uint32_t cmdCode,
                                SYS_RESULT_PACK* resultPackPtr)
{
    //首先查看使用的SOCKET
    PORT_CAN socketCan = CanProtPortCalcPort(targetModule,targetBoardID);
    //不同的SOCKET选用不同的函数
    if(socketCan == PORT_CAN1)
    {
        Can1ProtClearLastCmd(targetModule,targetBoardID,singalChannel,cmdCode,resultPackPtr);
    }
    else
    {
        Can2ProtClearLastCmd(targetModule,targetBoardID,singalChannel,cmdCode,resultPackPtr);
    }
}
