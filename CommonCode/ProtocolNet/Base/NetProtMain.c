/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-03 14:56:17 +0800
************************************************************************************************/ 
#include "NetProtMain.h"
#include "tx_mutex.h"

//NET 初始化标记
NET_PROT_INIT_SET netProtInitSet;
//网口接收缓存
NET_PROT_RECV_CACHE netProtRecvCache;

//初始化接收缓存
extern void NetProtRecvCacheInit(void);
//获取数据访问权
extern void NetProtGetDataAccessLock(void);
//释放数据访问权
extern void NetProtGetDataAccessRelease(void);

//初始化
void NetProtInit(uint8_t selfModuleNo,uint8_t selfBoardID)
{
    //创建互斥信号量
    if(netProtInitSet.mutexNetProtDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return ;
    }
    //创建互斥信号量
    tx_mutex_create(&(netProtInitSet.mutexNetProtDataAccess),"mutexNetProtDataAccess",TX_INHERIT);
    //模块号板号
    netProtInitSet.selfModuleNo = selfModuleNo;
    netProtInitSet.selfBoardID = selfBoardID;
    //创建结果包发送等待ACK缓存链表
    netProtInitSet.listSendResultWaitAckCachePtr = ADT_CircularPtrListCreate(NetProtMemMalloc,NetProtMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建上传包发送等待ACK缓存链表
    netProtInitSet.listSendUploadWaitAckCachePtr = ADT_CircularPtrListCreate(NetProtMemMalloc,NetProtMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //创建指令等待缓存链表
    netProtInitSet.listSendCmdWaitCachePtr = ADT_CircularPtrListCreate(NetProtMemMalloc,NetProtMemFree,
                                                                            CoreLockIntBasePri,CoreUnlockIntBasePri,
                                                                            MCU_RTT_SendStringInt);
    //初始化接收缓存
    NetProtRecvCacheInit();
    //日志打印
    NET_PROT_LOG("%s,ModuleNo: %d,BoardID: %d\r\n",__func__,selfModuleNo,selfBoardID);
}

//检查是否已经初始化
FlagStatus NetProtInitQuery(void)
{
    if(netProtInitSet.mutexNetProtDataAccess.tx_mutex_id == TX_MUTEX_ID)
    {
        return SET;
    }
    return RESET;
}


//网口接收一个字节数据的处理
extern NET_PROT_RECV_STA NetProtPackRecvByteProc(uint8_t recvByte,uint32_t currentTickMs);
//通过接收缓存创建接收信息
extern NET_PROT_PACKAGE* NetProtPackRecvCreatePackMsg(void);
//复位接收缓存
extern void NetProtRecvCacheReset(void);
//处理接受的NET消息
void NetProtPackRecvProcess(NET_PROT_PACKAGE* netProtPackagePtr);

//数据包接收
void NetProtPackRecv(NET_RECV_PACK* netRecvPackPtr,uint32_t currentTickMs)
{
    uint16_t indexUtil = 0;
    NET_PROT_RECV_STA netRecvSta = NET_PROT_RECV_STA_NONE;
    NET_PROT_PACKAGE* netProtPackagePtr = NULL;
    for(indexUtil = 0; indexUtil < netRecvPackPtr->recvDataLength; indexUtil++)
    {
        //单字节循环解析
        netRecvSta = NetProtPackRecvByteProc(netRecvPackPtr->recvDatBufPtr[indexUtil],currentTickMs);
        //检查是否接收到完整的一包数据
        if(netRecvSta == NET_PROT_RECV_STA_COMPLETE)
        {
            //从接收缓存创建消息报
            netProtPackagePtr = NetProtPackRecvCreatePackMsg();
            //调用下一级处理
            if(netProtPackagePtr != NULL)
            {
                //处理掉
                NetProtPackRecvProcess(netProtPackagePtr);
            }
            //处理完成之后清空缓存
            NetProtRecvCacheReset();
            //处理状态设置为初始状态
            netRecvSta = NET_PROT_RECV_STA_NONE;
        }
    }
}

//NET_PROT ACK发送
void NetProtAckSend(NET_PROT_ACK_PACK* ackMsgPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(ackMsgPtr == NULL)
    {
        return;
    }
    if(ackMsgPtr->ackType == NET_PROT_ACK_CMD)
    {
        //发送ACK CMD
        errorSub = NetProtSendAckCmdSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->retryCount,ackMsgPtr->cmdVal,
                                            ackMsgPtr->ackCmd);
        if(errorSub != ERROR_SUB_OK)
        {
            NET_PROT_LOG("%s,NetProtSendAckCmdSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == NET_PROT_ACK_RESULT)
    {
        //发送ACK RESULT
        errorSub = NetProtSendAckResultSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                                ackMsgPtr->retryCount,ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            NET_PROT_LOG("%s,NetProtSendAckResultSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    else if(ackMsgPtr->ackType == NET_PROT_ACK_UPLOAD)
    {
        //发送ACK UPLOAD
        errorSub = NetProtSendAckUploadSample(ackMsgPtr->targetModule,ackMsgPtr->targetBoardID,
                                            ackMsgPtr->retryCount,ackMsgPtr->cmdVal);
        if(errorSub != ERROR_SUB_OK)
        {
            NET_PROT_LOG("%s,NetProtSendAckUploadSample : 0X%08X\r\n",__func__,errorSub);
        }
    }
    //释放内存
    NetProtMemFree(ackMsgPtr);
}

//上传数据包缓存处理
extern void NetProtRunPeriodSubUploadWaitAckPackProcess(void);
//结果数据包处理
extern void NetProtRunPeriodSubResultWaitAckPackProcess(void);
//周期性的检查指令重发
extern void NetProtRunPeriodSubCmdWaitAckPackProcess(void);
//周期性的检查指令超时
extern void NetProtRunPeriodSubCmdWaitResultPackProcess(void);

//内部周期性数据处理
void NetProtRunPeriod(uint32_t currentTickMs)
{
    //获取数据锁
    NetProtGetDataAccessLock();
    //上传数据包处理
    NetProtRunPeriodSubUploadWaitAckPackProcess();
    //结果数据包处理
    NetProtRunPeriodSubResultWaitAckPackProcess();
    //指令包等待ACK的数据处理
    NetProtRunPeriodSubCmdWaitAckPackProcess();
    //指令包等待结果的数据处理
    NetProtRunPeriodSubCmdWaitResultPackProcess();
    //释放数据锁
    NetProtGetDataAccessRelease();
}

//NET在接收数据时候发送ACK
void NetProtSendAckInRecv(NET_PROT_ACK_TYPE ackType,uint8_t targetModule,uint8_t targetBoardID,
                            uint8_t retryCount,uint32_t cmdVal,PROT_ACK ackCmd)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    NET_PROT_ACK_PACK* netProtAckPtr = NetProtMemMallocWhileSuccess(DIM_STRUCT_LENGTH_BYTE(NET_PROT_ACK_PACK));
    netProtAckPtr->ackType = ackType;
    netProtAckPtr->targetModule = targetModule;
    netProtAckPtr->targetBoardID = targetBoardID;
    netProtAckPtr->retryCount = retryCount;
    netProtAckPtr->cmdVal = cmdVal;
    netProtAckPtr->ackCmd = ackCmd;
    errorSub = NetProtAckMsgDispatch(netProtAckPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        NET_PROT_LOG("%s,NetProtAckMsgDispatch ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //释放内存
        NetProtMemFree(netProtAckPtr);
    }
}

//初始化发送指令
void NetProtSetDefaultCmdSend(NET_PROT_CMD_SEND* netSendPtr)
{
    netSendPtr->targetModule = SYSTEM_MODULE_NO_PC;
    netSendPtr->targetBoardID = SYSTEM_BOARD_ID_PC;
    netSendPtr->cmdVal = 0XFFFFFFFF;
    netSendPtr->cmdBufferPtr = NULL;
    netSendPtr->cmdBufferLength = 0;
    netSendPtr->cmdNeedResult = ENABLE;
    netSendPtr->resultTimeOutMs = 60000;
    netSendPtr->exceptResultLengthMin = 0;
}

//获取指定指令的配置讯息
NET_CMD_CONFIG* NetProtGetCmdConfig(uint32_t cmdCode,NET_CMD_CONFIG* configArray,uint16_t arrayLen)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < arrayLen; indexUtil++)
    {
        if(cmdCode == configArray[indexUtil].cmdVal)
        {
            return (NET_CMD_CONFIG*)(&(configArray[indexUtil]));
        }
    }
    return NULL;
}

//创建发送指令
NET_PROT_CMD_SEND* NetProtCreateCmdSend(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdCode,NET_CMD_CONFIG* netCmdConfigPtr)
{
    NET_PROT_CMD_SEND* netCmdSendPtr = (NET_PROT_CMD_SEND*)NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_CMD_SEND));
    //设置默认值
    NetProtSetDefaultCmdSend(netCmdSendPtr);
    //指令赋值
    netCmdSendPtr->targetModule = targetModule;
    netCmdSendPtr->targetBoardID = targetBoardID;
    netCmdSendPtr->cmdVal = cmdCode;
    netCmdSendPtr->cmdNeedResult = netCmdConfigPtr->cmdNeedResult;
    netCmdSendPtr->exceptResultLengthMin = netCmdConfigPtr->exceptResultLengthMin;
    netCmdSendPtr->resultTimeOutMs = netCmdConfigPtr->resultTimeOutMs;
    netCmdSendPtr->cmdBufferLength = netCmdConfigPtr->paramBufferLen;
    //内存申请
    if(netCmdSendPtr->cmdBufferLength != 0)
    {
        netCmdSendPtr->cmdBufferPtr = NetProtMemMallocWhileSuccess(netCmdSendPtr->cmdBufferLength);
        //参数清除
        UserMemClear(netCmdSendPtr->cmdBufferPtr,netCmdSendPtr->cmdBufferLength);
    }
    else
    {
        netCmdSendPtr->cmdBufferPtr = NULL;
    }
    return netCmdSendPtr;
}

//销毁发送指令
void NetProtDestroyCmdSend(NET_PROT_CMD_SEND** netCmdSendPtrPtr)
{
    NET_PROT_CMD_SEND* netCmdSendPtr = *netCmdSendPtrPtr;
    if(netCmdSendPtr == NULL)
    {
        return;
    }
    //释放缓存
    if((netCmdSendPtr->cmdBufferLength != 0)&&(netCmdSendPtr->cmdBufferPtr != NULL))
    {
        NetProtMemFree(netCmdSendPtr->cmdBufferPtr);
    }
    //释放本体
    NetProtMemFree((uint8_t*)netCmdSendPtr);
}

//从链表中找出指定的节点,元素内容匹配.使用指令头查找
static ADT_CMP_RESULT ADT_NetProtCmdWaitElementFindWithHeader(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    NET_PROT_CMD_WAIT_PACK_HEADER* cmdWaitPackHeaderPtrDst = (NET_PROT_CMD_WAIT_PACK_HEADER*)cmpDst;
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
    if(cmdWaitPackPtrSrc->cmdValue != cmdWaitPackHeaderPtrDst->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//检查通道状态
PROT_CH_STA NetProtCheckCmdChannelState(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //构建指令头
    NET_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    NetProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                            ADT_NetProtCmdWaitElementFindWithHeader);
    //释放访问权
    NetProtGetDataAccessRelease();
    if(listNodePtr == NULL)
    {
        //不存在节点,通道空闲
        return PROT_CH_STA_IDLE;
    }
    else
    {
        //存在节点,通道忙
        return PROT_CH_STA_BUSY;
    }
}

//发送结果,带重发功能
void NetProtSendResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,PROT_RESULT resultCode,ERROR_MAIN errorMain,
                        ERROR_LEVEL errorLevel,ERROR_SUB errorSub,uint8_t* resultParamBuffer,uint16_t resultBufferLen,
                        SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先创建等待结果包
    NET_PROT_RESULT_WAIT_ACK_PACK* resultWaitAckPackPtr = (NET_PROT_RESULT_WAIT_ACK_PACK*)NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_RESULT_WAIT_ACK_PACK));
    if(resultWaitAckPackPtr == NULL)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_RESULT_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    resultWaitAckPackPtr->resultParamBufferLength = resultBufferLen;
    if(resultWaitAckPackPtr->resultParamBufferLength != 0)
    {
        resultWaitAckPackPtr->resultParamBufferPtr = NetProtMemMalloc(resultWaitAckPackPtr->resultParamBufferLength);
        if(resultWaitAckPackPtr->resultParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            NetProtMemFree(resultWaitAckPackPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_RESULT_WAIT_ACK_BUF_MEM_MALLOC);
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
    //申请访问权
    NetProtGetDataAccessLock();
    //查看结果上传链表长度,超过指定长度发送失败
    uint32_t listNodeCount = ADT_CircularPtrListGetNodeCount(netProtInitSet.listSendResultWaitAckCachePtr);
    if(listNodeCount >= CAPACITY_MAX_NET_PROT_WAIT_LIST)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //等待队列满
        if(resultBufferLen != 0)
        {
            NetProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        NetProtMemFree(resultWaitAckPackPtr);
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListGetNodeCount ListNodeFull: %d\r\n",__func__,__LINE__,listNodeCount);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                    ERROR_SUB_BOARD_NET_PROT_RESULT_WAIT_ACK_LIST_FULL);
        return;
    }
    if(listNodeCount == 0)
    {
        //没有元素,第一个元素那就需要执行一次发送
        //往结果包中插入节点
        adtResult = ADT_CircularPtrListInsertNodeEnd(netProtInitSet.listSendResultWaitAckCachePtr,resultWaitAckPackPtr);
        if(adtResult != ADT_RESULT_OK)
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //释放内存
            if(resultBufferLen != 0)
            {
                NetProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
            }
            NetProtMemFree(resultWaitAckPackPtr);
            NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListInsertNodeEnd Failed\r\n",__func__,__LINE__);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_RESULT_WAIT_ACK_LIST_INSERT);
            return;
        }
        //未发送,设置默认发送时间和发送次数
        resultWaitAckPackPtr->lastSendTimeTickMs = 0;
        resultWaitAckPackPtr->retryCount = 0;
        //检查网络是否连接,未连接的情况下不发送
        if(RESET == NetProtGetConnectFlag())
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //设置无错误
            CoreUtilSetSystemResultPackNoError(resultPackPtr);
            return;
        }
        //数据发送
        errorSubLocal = NetProtSendResultOnlySample(targetModule,targetBoardID,0,cmdVal,resultCode,errorMain,errorLevel,errorSub,
                                                                resultWaitAckPackPtr->resultParamBufferPtr,resultWaitAckPackPtr->resultParamBufferLength);
        //发送失败
        if(errorSubLocal != ERROR_SUB_OK)
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            NET_PROT_LOG("E>: %s,%d,NetProtSendResultOnlySample Failed: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
            //设置无错误
            CoreUtilSetSystemResultPackNoError(resultPackPtr);
            return;
        }
        //发送成功,更新发送时间
        resultWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        resultWaitAckPackPtr->retryCount = 1;
        //释放访问权
        NetProtGetDataAccessRelease();
        //设置无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //已经有了元素了,排队发送,放到队列尾巴上
    adtResult = ADT_CircularPtrListInsertNodeEnd(netProtInitSet.listSendResultWaitAckCachePtr,resultWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //释放内存
        if(resultBufferLen != 0)
        {
            NetProtMemFree(resultWaitAckPackPtr->resultParamBufferPtr);
        }
        NetProtMemFree(resultWaitAckPackPtr);
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListInsertNodeEnd Failed\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_RESULT,ERROR_LEVEL_ERROR,
                                    ERROR_SUB_BOARD_NET_PROT_RESULT_WAIT_ACK_LIST_INSERT);
        return;
    }
    //未发送,设置默认发送时间和发送次数
    resultWaitAckPackPtr->lastSendTimeTickMs = 0;
    resultWaitAckPackPtr->retryCount = 0;
    //释放访问权
    NetProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//发送上传,带重发功能
void NetProtSendUpload(uint8_t targetModule,uint8_t targetBoardID,uint32_t uploadDataID,uint8_t* uploadDataBuffer,
                                        uint16_t uploadDataLen,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    //首先创建等待结果包
    NET_PROT_UPLOAD_WAIT_ACK_PACK* uploadWaitAckPackPtr = (NET_PROT_UPLOAD_WAIT_ACK_PACK*)NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_UPLOAD_WAIT_ACK_PACK));
    if(uploadWaitAckPackPtr == NULL)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_UPLOAD_WAIT_ACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    uploadWaitAckPackPtr->uploadDataBufferLength = uploadDataLen;
    if(uploadWaitAckPackPtr->uploadDataBufferLength != 0)
    {
        uploadWaitAckPackPtr->uploadDataBufferPtr = NetProtMemMalloc(uploadWaitAckPackPtr->uploadDataBufferLength);
        if(uploadWaitAckPackPtr->uploadDataBufferPtr == NULL)
        {
            //申请失败,释放缓存
            NetProtMemFree(uploadWaitAckPackPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_UPLOAD_WAIT_ACK_BUF_MEM_MALLOC);
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
    //申请访问权
    NetProtGetDataAccessLock();
    //查看日志上传链表长度,超过指定长度发送失败
    uint32_t listNodeCount = ADT_CircularPtrListGetNodeCount(netProtInitSet.listSendUploadWaitAckCachePtr);
    if(listNodeCount >= CAPACITY_MAX_NET_PROT_WAIT_LIST)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //等待队列满
        if(uploadDataLen != 0)
        {
            NetProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        NetProtMemFree(uploadWaitAckPackPtr);
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListGetNodeCount ListNodeFull: %d\r\n",__func__,__LINE__,listNodeCount);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                    ERROR_SUB_BOARD_NET_PROT_UPLOAD_WAIT_ACK_LIST_FULL);
        return;
    }
    if(listNodeCount == 0)
    {
        //没有元素,第一个元素那就需要执行一次发送
        //往结果包中插入节点
        adtResult = ADT_CircularPtrListInsertNodeEnd(netProtInitSet.listSendUploadWaitAckCachePtr,uploadWaitAckPackPtr);
        if(adtResult != ADT_RESULT_OK)
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //释放内存
            if(uploadDataLen != 0)
            {
                NetProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
            }
            NetProtMemFree(uploadWaitAckPackPtr);
            NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListInsertNodeEnd Failed\r\n",__func__,__LINE__);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_UPLOAD_WAIT_ACK_LIST_INSERT);
            return;
        }
        //未发送,设置默认发送时间和发送次数
        uploadWaitAckPackPtr->lastSendTimeTickMs = 0;
        uploadWaitAckPackPtr->retryCount = 0;
        //检查网络是否连接,未连接的情况下不发送
        if(RESET == NetProtGetConnectFlag())
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //设置无错误
            CoreUtilSetSystemResultPackNoError(resultPackPtr);
            return;
        }
        //数据发送
        errorSubLocal = NetProtSendUploadOnlySample(targetModule,targetBoardID,uploadWaitAckPackPtr->retryCount,
                                                                uploadWaitAckPackPtr->uploadID,uploadWaitAckPackPtr->uploadDataBufferPtr,
                                                                uploadWaitAckPackPtr->uploadDataBufferLength);
        //发送失败
        if(errorSubLocal != ERROR_SUB_OK)
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            NET_PROT_LOG("E>: %s,%d,NetProtSendUploadOnlySample Failed: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
            //设置无错误
            CoreUtilSetSystemResultPackNoError(resultPackPtr);
            return;
        }
        //发送成功,更新发送时间
        uploadWaitAckPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        uploadWaitAckPackPtr->retryCount = 1;
        //释放访问权
        NetProtGetDataAccessRelease();
        //设置无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //已经有了元素了,排队发送,放到队列尾巴上
    adtResult = ADT_CircularPtrListInsertNodeEnd(netProtInitSet.listSendUploadWaitAckCachePtr,uploadWaitAckPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //释放内存
        if(uploadDataLen != 0)
        {
            NetProtMemFree(uploadWaitAckPackPtr->uploadDataBufferPtr);
        }
        NetProtMemFree(uploadWaitAckPackPtr);
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListInsertNodeEnd Failed\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_UPLOAD,ERROR_LEVEL_ERROR,
                                    ERROR_SUB_BOARD_NET_PROT_UPLOAD_WAIT_ACK_LIST_INSERT);
        return;
    }
    //未发送,设置默认发送时间和发送次数
    uploadWaitAckPackPtr->lastSendTimeTickMs = 0;
    uploadWaitAckPackPtr->retryCount = 0;
    //释放访问权
    NetProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//从链表中找出指定的节点,元素内容匹配
static ADT_CMP_RESULT ADT_NetProtCmdWaitElementFind(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrDst = (NET_PROT_CMD_WAIT_PACK*)cmpDst;
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
    if(cmdWaitPackPtrSrc->cmdValue != cmdWaitPackPtrDst->cmdValue)
    {
        //指令码匹配
        return ADT_CMP_NOT_EQUAL;
    }
    return ADT_CMP_EQUAL;
}

//从链表中找出指定的节点,内存地址匹配
static ADT_CMP_RESULT ADT_NetProtCmdWaitPackElementCmp(void* cmpSrc,void* cmpDst)
{
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrSrc = (NET_PROT_CMD_WAIT_PACK*)cmpSrc;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtrDst = (NET_PROT_CMD_WAIT_PACK*)cmpDst;
    //元素比较,直接比较指针
    if(cmdWaitPackPtrSrc == cmdWaitPackPtrDst)
    {
        return ADT_CMP_EQUAL;
    }
    return ADT_CMP_NOT_EQUAL;
}

//发送指令,基础,只要发出去
void NetProtSendCmdBase(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,uint8_t* cmdBufferPtr,uint16_t cmdBufferLength,
                                FunctionalState cmdNeedResult,uint32_t resultTimeOutMs,uint16_t exceptResultLengthMin,
                                SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    //首先创建等待指令包
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPackPtr = (NET_PROT_CMD_WAIT_PACK*)NetProtMemMalloc(DIM_STRUCT_LENGTH_BYTE(NET_PROT_CMD_WAIT_PACK));
    if(cmdWaitPackPackPtr == NULL)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_MEM_MALLOC);
        return;
    }
    //申请数据缓存
    cmdWaitPackPackPtr->cmdParamBufferLength = cmdBufferLength;
    if(cmdWaitPackPackPtr->cmdParamBufferLength != 0)
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = NetProtMemMalloc(cmdWaitPackPackPtr->cmdParamBufferLength);
        if(cmdWaitPackPackPtr->cmdParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            NetProtMemFree(cmdWaitPackPackPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_BUF_MEM_MALLOC);
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
    //申请访问权
    NetProtGetDataAccessLock();
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,
                                                        ADT_NetProtCmdWaitElementFind);
    if(listNodePtr != NULL)
    {
        //链表中存放了一个指令,需要对这个指令做处理
        NET_PROT_CMD_WAIT_PACK* cmdWaitPackExistPtr = ADT_CircularPtrListGetNodeVal(listNodePtr);
        //查看指令状态,在等待ACK和等待结果的情况下,报错,其他状态下,清除节点
        if((cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdWaitPackExistPtr->cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_LAST_NOT_COMPLETE);
            return;
        }
        //上一次指令已经结束了,但是调用者没有读取
        else
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //释放内存并报错
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            //上一次指令结束未读取
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                            ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_LAST_NOT_READ);
            return;
        }
    }
    //未发送,设置默认发送时间和发送次数
    cmdWaitPackPackPtr->lastSendTimeTickMs = 0;
    cmdWaitPackPackPtr->retryCount = 0;
    //往指令等待包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeEnd(netProtInitSet.listSendCmdWaitCachePtr,cmdWaitPackPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_LIST_INSERT);
        return;
    }
    //把插入的那个元素揪出来
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)cmdWaitPackPackPtr,
                                                        ADT_NetProtCmdWaitPackElementCmp);
    if(listNodePtr == NULL)
    {
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed\r\n",__func__,__LINE__);
        //释放访问权
        NetProtGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        NetProtMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_SEND_CMD,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_LIST_FIND);
        return;
    }
    //检查网络是否连接,未连接直接完成
    if(RESET == NetProtGetConnectFlag())
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        //设置无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //数据发送
    errorSubLocal = NetProtSendCmdOnlySample(targetModule,targetBoardID,0,cmdVal,cmdWaitPackPackPtr->cmdParamBufferPtr,
                                                            cmdWaitPackPackPtr->cmdParamBufferLength);
    //发送失败
    if(errorSubLocal != ERROR_SUB_OK)
    {
        //释放访问权
        NetProtGetDataAccessRelease();
        NET_PROT_LOG("E>: %s,%d,NetProtSendCmdOnlySample Failed: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
        //设置无错误
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //发送成功,更新发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
    cmdWaitPackPackPtr->retryCount = 1;
    //释放访问权
    NetProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//发送指令,基础,只要发出去
void NetProtSendCmdNoWait(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    NetProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->cmdVal,cmdSendPtr->cmdBufferPtr,
                        cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
}

//查询指令的当前状态
static ERROR_SUB NetProtQueryCmdState(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,PROT_CMD_STA* cmdStage)
{
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    NET_PROT_CMD_WAIT_PACK cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    NetProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_NetProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        NetProtGetDataAccessRelease();
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_QUERY_STATE_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //找到元素了,获取当前状态
    *cmdStage = cmdWaitPackPtr->cmdStage;
    //释放访问权
    NetProtGetDataAccessRelease();
    return ERROR_SUB_OK;
}


//删除节点
static void NetProtDeleteCmdNode(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal)
{   
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    NET_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    NetProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,如果已经存在,这里就需要等待上一个指令超时
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_NetProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        NetProtGetDataAccessRelease();
        //日志打印
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,cmdVal);
        //查询节点信息失败
        return;
    }
    //把元素搞出来
    cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    NetProtGetDataAccessRelease();
    //释放节点数据
    if(cmdWaitPackPtr->resultParamBufferPtr != NULL)
    {
        NetProtMemFree(cmdWaitPackPtr->resultParamBufferPtr);
    }
    if(cmdWaitPackPtr->cmdParamBufferPtr != NULL)
    {
        NetProtMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    NetProtMemFree(cmdWaitPackPtr);
    return;
}

//等待ACK完成
static ERROR_SUB NetProtCmdWaitAck(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_NET_PROT);
        //查询
        errorSub = NetProtQueryCmdState(targetModule,targetBoardID,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //设置错误码
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
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //正常结束,而且不需要结果包会进入这个状态,释放节点,返回完成
            break;
        }
        //其余状态,都是完成
        if(cmdStageCurrent >= PROT_CMD_STA_WAIT_RESULT)
        {
            break;
        }
    }
    //设置无错误
    return ERROR_SUB_OK;
}

//发送指令,并等待ACK
void NetProtSendCmdWaitAck(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先发出去
    NetProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->cmdVal,cmdSendPtr->cmdBufferPtr,
                        cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待ACK
    errorSub = NetProtCmdWaitAck(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->cmdVal);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//拷贝节点数据
static ERROR_SUB NetProtCopyCmdNodeResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,
                                            uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    ADT_CIRCULAR_PTR_LIST_NODE_T listNodePtr = NULL;
    NET_PROT_CMD_WAIT_PACK* cmdWaitPackPtr = NULL;
    //构建指令头
    NET_PROT_CMD_WAIT_PACK_HEADER cmdWaitPackHeader;
    cmdWaitPackHeader.targetModuleNo = targetModule;
    cmdWaitPackHeader.targetBoardID = targetBoardID;
    cmdWaitPackHeader.cmdValue = cmdVal;
    //申请访问权
    NetProtGetDataAccessLock();
    //从列表中找出来节点
    //从链表中查找指定元素是否已经存在,
    listNodePtr = ADT_CircularPtrListFindNodeWithData(netProtInitSet.listSendCmdWaitCachePtr,(void*)&cmdWaitPackHeader,
                                                        ADT_NetProtCmdWaitElementFindWithHeader);
    if(listNodePtr == NULL)
    {
        //没找到这个元素,报错
        //释放访问权
        NetProtGetDataAccessRelease();
        //日志打印
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListFindNodeWithData Failed,targetModule: %d,targetBoardID: %d,cmdVal: 0X%08X\r\n",
                                    __func__,__LINE__,targetModule,targetBoardID,cmdVal);
        //查询节点信息失败
        return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_RESULT_NODE_FIND;
    }
    //把元素搞出来
    cmdWaitPackPtr = (NET_PROT_CMD_WAIT_PACK*)ADT_CircularPtrListGetNodeVal(listNodePtr);
    //释放节点
    adtResult = ADT_CircularPtrListDeleteNodeCurrent(netProtInitSet.listSendCmdWaitCachePtr,listNodePtr);
    if(adtResult != ADT_RESULT_OK)
    {
        NET_PROT_LOG("E>: %s,%d,ADT_CircularPtrListDeleteNodeCurrent Failed\r\n",__func__,__LINE__);
    }
    //释放访问权
    NetProtGetDataAccessRelease();
    //拷贝数据
    resultPackPtr->resultCode = cmdWaitPackPtr->resultCode;
    resultPackPtr->errorMain = cmdWaitPackPtr->errorMainCode;
    resultPackPtr->errorLevel = cmdWaitPackPtr->errorLevelCode;
    resultPackPtr->errorSub = cmdWaitPackPtr->errorSubCode;
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
        NetProtMemFree(cmdWaitPackPtr->cmdParamBufferPtr);
    }
    NetProtMemFree(cmdWaitPackPtr);
    return ERROR_SUB_OK;
}

//等待结果
ERROR_SUB NetProtCmdWaitResultBase(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStageCurrent;
    while(1)
    {
        //延时查询
        CoreDelayMs(TIME_MS_INTERVAL_CMD_QUERY_NET_PROT);
        //查询
        errorSub = NetProtQueryCmdState(targetModule,targetBoardID,cmdVal,&cmdStageCurrent);
        if(errorSub != ERROR_SUB_OK)
        {
            //设置错误码
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
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_ERR;
        }
        //ACK超时
        if(cmdStageCurrent == PROT_CMD_STA_ACK_TIME_OUT)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_TIME_OUT;
        }
        //指令重发失败
        if(cmdStageCurrent == PROT_CMD_STA_ACK_RETRY_FAIL)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //ACK返回错误
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_ACK_TIME_RETRY_FAIL;
        }
        //指令完成
        if(cmdStageCurrent == PROT_CMD_STA_ACK_OVER)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
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
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_RESULT_TIME_OUT;
        }
        //结果长度不对
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_LEN_SHORT)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_RESULT_LEN_SHORT;
        }
        //结果内存申请失败
        if(cmdStageCurrent == PROT_CMD_STA_RESULT_BUF_MALLOC)
        {
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            //返回结果超时
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_RESULT_BUF_MALLOC;
        }
        //正常完成
        if(cmdStageCurrent == PROT_CMD_STA_OVER)
        {
            //正常完成,拷贝节点数据
            errorSub = NetProtCopyCmdNodeResult(targetModule,targetBoardID,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
            if(errorSub != ERROR_SUB_OK)
            {
                NET_PROT_LOG("E>: %s,%d,NetProtCopyCmdNodeResult Failed,targetModule: %d,targetBoardID: %d,cmdVal: 0X%08X\r\n",
                                            __func__,__LINE__,targetModule,targetBoardID,cmdVal);
                return errorSub;
            }
            //返回结果超时
            break;
        }
        if(cmdStageCurrent > PROT_CMD_STA_OVER)
        {
            //不支持的状态
            //释放节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
            return ERROR_SUB_BOARD_NET_PROT_CMD_WAIT_PACK_RESULT_UNSUPPORT_STATE;
        }
    }
    //设置无错误
    return ERROR_SUB_OK;
}

//等待结果
void NetProtCmdWaitResult(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** resultDataBufPtrPtr,uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = NetProtCmdWaitResultBase(targetModule,targetBoardID,cmdVal,resultPackPtr,resultDataBufPtrPtr,resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        *resultDataBufPtrPtr = NULL;
        *resultBufLen = 0;
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//发送指令并等待结果返回
void NetProtSendCmdWaitResult(NET_PROT_CMD_SEND* cmdSendPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** resultDataBufPtrPtr,
                                uint16_t* resultBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //设置结果数据包为正常值
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    *resultBufLen = 0;
    *resultDataBufPtrPtr = NULL;
    //先发出去
    NetProtSendCmdBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->cmdVal,cmdSendPtr->cmdBufferPtr,
                        cmdSendPtr->cmdBufferLength,cmdSendPtr->cmdNeedResult,cmdSendPtr->resultTimeOutMs,
                        cmdSendPtr->exceptResultLengthMin,resultPackPtr);
    if(resultPackPtr->errorSub != ERROR_SUB_OK)
    {
        return;
    }
    //等待返回
    errorSub = NetProtCmdWaitResultBase(cmdSendPtr->targetModule,cmdSendPtr->targetBoardID,cmdSendPtr->cmdVal,resultPackPtr,resultDataBufPtrPtr,
                                        resultBufLen);
    if(errorSub != ERROR_SUB_OK)
    {
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//清理指定通道的上一次指令
void NetProtClearLastCmd(uint8_t targetModule,uint8_t targetBoardID,uint32_t cmdVal,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CMD_STA cmdStage = PROT_CMD_STA_OVER;
    SYS_RESULT_PACK sysResultPack;
    uint8_t* resultBufferPtr = NULL;
    uint16_t resultBufferLen = 0;
    //申请访问权
    NetProtGetDataAccessLock();
    //首先查询这个指令是否存在
    PROT_CH_STA channelState = NetProtCheckCmdChannelState(targetModule,targetBoardID,cmdVal);
    //如果存在上一次的指令,就要处理
    if(channelState == PROT_CH_STA_BUSY)
    {
        //查询指令的状态
        errorSub = NetProtQueryCmdState(targetModule,targetBoardID,cmdVal,&cmdStage);
        if(ERROR_SUB_OK != errorSub)
        {
            //释放访问权
            NetProtGetDataAccessRelease();
            //日志信息输出
            NET_PROT_LOG("NetProtQueryCmdState Failed,TargetModule: %d,TargetBoard: %d,cmdVal: 0X%08X,ErrorSub: 0X%08X\r\n",
                            targetModule,targetBoardID,cmdVal,errorSub);
            //设置错误码
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                            errorSub);
            return;
        }
        //正常查询到了,进行下一步
        if(cmdStage == PROT_CMD_STA_ACK_OVER)
        {
            //日志打印
            NET_PROT_LOG("NetProt LastComplete OnlyNeedAck,But AppNotRead,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,cmdVal);
            //指令只需要ACK,不需要结果,已经完成了,这个时候调用等待,将对应的节点清除掉
            errorSub = NetProtCmdWaitResultBase(targetModule,targetBoardID,cmdVal,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                NetProtGetDataAccessRelease();
                //日志信息输出
                NET_PROT_LOG("NetProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,cmdVal,errorSub);
                //查询错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //不需要返回,通道可以使用了.只需要ACK的指令是不会有结果数据的
        }
        else if(cmdStage == PROT_CMD_STA_OVER)
        {
            //日志打印
            NET_PROT_LOG("NetProt LastComplete NeedResult,But AppNotRead,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,Will Clear\r\n",
                                targetModule,targetBoardID,cmdVal);
            //指令完成,这个时候调用等待,将对应的节点清除掉
            errorSub = NetProtCmdWaitResultBase(targetModule,targetBoardID,cmdVal,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            if(ERROR_SUB_OK != errorSub)
            {
                //释放访问权
                NetProtGetDataAccessRelease();
                //日志信息输出
                NET_PROT_LOG("NetProtCmdWaitResultBase Failed,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",
                                targetModule,targetBoardID,cmdVal,errorSub);
                //查询错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                errorSub);
                return;
            }
            //清除完成
            NET_PROT_LOG("NetProt LastComplete,But AppNotRead,ResultCode: 0X%08X,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub : 0X%08X\r\n",
                            (uint8_t)sysResultPack.resultCode,(uint16_t)sysResultPack.errorMain,(uint8_t)sysResultPack.errorLevel,
                            (uint32_t)(sysResultPack.errorSub));
            //结果数据包如果有动态内存就需要释放掉
            if((resultBufferLen != 0)&&(resultBufferPtr != NULL))
            {
                //释放内存
                NetProtMemFree(resultBufferPtr);
            }
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_ACK_ERR)||(cmdStage == PROT_CMD_STA_ACK_TIME_OUT)||(cmdStage == PROT_CMD_STA_ACK_RETRY_FAIL)||
                    (cmdStage == PROT_CMD_STA_RESULT_TIME_OUT)||(cmdStage == PROT_CMD_STA_RESULT_LEN_SHORT)||(cmdStage == PROT_CMD_STA_RESULT_BUF_MALLOC))
        {
            //日志打印
            NET_PROT_LOG("NetProt LastError,But AppNotRead,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,cmdState: %d,Will Clear\r\n",
                                targetModule,targetBoardID,cmdVal,cmdStage);
            //上一次指令报错了 ACK错误,ACK超时,ACK重发超过最大次数,回结果超时,结果长度不对,内存申请异常,执行一次等待将节点清除掉
            NetProtCmdWaitResultBase(targetModule,targetBoardID,cmdVal,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不需要返回,通道可以使用了
        }
        else if((cmdStage == PROT_CMD_STA_WAIT_ACK)||(cmdStage == PROT_CMD_STA_WAIT_RESULT))
        {
            //现在就剩下waitack和waitresult
            NET_PROT_LOG("NetProt LastCmd NotComplete,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,Stage: 0X%08X\r\n",
                                targetModule,targetBoardID,cmdVal,cmdStage);
#if(NET_PROT_FORCE_CLEAR_NOT_COMPLETE == CONFIG_FUNC_DISABLE)
            //未设置强制清除,上一次的指令未完成
            //必须返回,通道还没办法用
            if(cmdStage == PROT_CMD_STA_WAIT_ACK)
            {
                //释放访问权
                NetProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_NET_PROT_LAST_WAIT_ACK_CMD_NOT_COMPLETE);
                return;
            }
            else 
            {
                //释放访问权
                NetProtGetDataAccessRelease();
                //错误,返回
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_NET_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                                ERROR_SUB_BOARD_NET_PROT_LAST_WAIT_RESULT_CMD_NOT_COMPLETE);
                return;
            }
#else
            //设置了强制清除,清除掉上次没发出去的指令,删除节点
            NetProtDeleteCmdNode(targetModule,targetBoardID,cmdVal);
#endif
        }
        else
        {
            //位置的状态
            NET_PROT_LOG("NetProt Unknow Stage,TargetModule: %d,TargetBoard: %d,Cmd: 0X%08X,cmdStage: 0X%02X\r\n",
                            targetModule,targetBoardID,cmdVal,(uint8_t)cmdStage);
            //也清除掉
            NetProtCmdWaitResultBase(targetModule,targetBoardID,cmdVal,&sysResultPack,&resultBufferPtr,&resultBufferLen);
            //不用返回,通道可以使用了
        }
    }
    //释放访问权
    NetProtGetDataAccessRelease();
    //设置无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}








