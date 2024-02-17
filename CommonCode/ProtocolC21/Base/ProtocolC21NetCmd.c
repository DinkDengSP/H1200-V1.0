/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-05 17:26:05 +0800
************************************************************************************************/ 
#include "ProtocolC21NetMain.h"
#include "ProtocolC21NetBase.h"

//NET SOCKET初始化标记
extern PROTOCOL_C21_NET_INIT_SET c21NetInitSet;
//获取数据访问权
extern void ProtocolC21NetSocketGetDataAccessLock(void);
//释放数据访问权
extern void ProtocolC21NetSocketGetDataAccessRelease(void);

//发送指令,使用结构体指针作为参数
ERROR_SUB ProtocolC21NetSocketSendCmdSample(C21_NET_PACKAGE* cmdSendPtr)
{
    return ProtocolC21NetSocketSendCmdDetail(cmdSendPtr->cmdCode,cmdSendPtr->dataBodyBufferPtr,cmdSendPtr->dataBodyLength);
}

//发送指令,使用详细信息作为参数
ERROR_SUB ProtocolC21NetSocketSendCmdDetail(uint16_t cmdCode,uint8_t* dataBody,uint16_t dataBodyLength)
{
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    ADT_RESULT adtResult = ADT_RESULT_OK;
    uint8_t elementCount = 0;
    if(RESET == ProtocolC21NetSocketGetConnectFlag())
    {
        //SOCKET未连接
        return ERROR_SUB_BOARD_C21_NET_SOCKET_DISCONNECT;
    }
    //首先创建等待指令包
    C21_NET_CMD_WAIT_PACK* cmdWaitPackPackPtr = (C21_NET_CMD_WAIT_PACK*)ProtocolC21NetSocketMemMalloc(DIM_STRUCT_LENGTH_BYTE(C21_NET_CMD_WAIT_PACK));
    if(cmdWaitPackPackPtr == NULL)
    {
        return ERROR_SUB_BOARD_C21_NET_SOCKET_CMD_WAIT_PACK_MEM_MALLOC;
    }
    //申请数据缓存
    cmdWaitPackPackPtr->cmdParamBufferLength = dataBodyLength;
    if(cmdWaitPackPackPtr->cmdParamBufferLength != 0)
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = ProtocolC21NetSocketMemMalloc(cmdWaitPackPackPtr->cmdParamBufferLength);
        if(cmdWaitPackPackPtr->cmdParamBufferPtr == NULL)
        {
            //申请失败,释放缓存
            ProtocolC21NetSocketMemFree(cmdWaitPackPackPtr);
            return ERROR_SUB_BOARD_C21_NET_SOCKET_CMD_WAIT_PACK_BUF_MEM_MALLOC;
        }
    }
    else
    {
        cmdWaitPackPackPtr->cmdParamBufferPtr = NULL;
    }
    //数据拷贝
    cmdWaitPackPackPtr->cmdValue = cmdCode;
    //内存拷贝
    if(dataBodyLength != 0)
    {
        UserMemCopy(cmdWaitPackPackPtr->cmdParamBufferPtr,dataBody,dataBodyLength);
    }
    //申请访问权
    ProtocolC21NetSocketGetDataAccessLock();
    //查询链表中是否存在元素,已经存在元素就直接放进去,等待定时任务去发送
    elementCount = ADT_CircularPtrListGetNodeCount(c21NetInitSet.listSendCmdWaitAckPtr);
    if(elementCount == 0)
    {
        //没有元素,第一个元素那就需要执行一次发送
        //往指令等待包中插入节点
        adtResult = ADT_CircularPtrListInsertNodeEnd(c21NetInitSet.listSendCmdWaitAckPtr,cmdWaitPackPackPtr);
        if(adtResult != ADT_RESULT_OK)
        {
            //释放访问权
            ProtocolC21NetSocketGetDataAccessRelease();
            //释放内存
            if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
            {
                ProtocolC21NetSocketMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            }
            ProtocolC21NetSocketMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
            return ERROR_SUB_BOARD_C21_NET_SOCKET_CMD_WAIT_PACK_LIST_INSERT;
        }
        //数据发送
        errorSubLocal = ProtocolC21NetSendCmdOnly(cmdCode,cmdWaitPackPackPtr->cmdParamBufferPtr,cmdWaitPackPackPtr->cmdParamBufferLength);
        //发送失败
        if(errorSubLocal != ERROR_SUB_OK)
        {
            //释放访问权
            ProtocolC21NetSocketGetDataAccessRelease();
            PROTOCOL_C21_NET_SOCKET_LOG("E>: %s,%d,ProtocolC21NetSendCmdOnly Failed: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
            return ERROR_SUB_OK;
        }
        //发送成功,更新发送时间
        cmdWaitPackPackPtr->lastSendTimeTickMs = OS_TICK_TO_MS(tx_time_get());
        //更新发送次数
        cmdWaitPackPackPtr->retryCount = 1;
        //释放访问权
        ProtocolC21NetSocketGetDataAccessRelease();
        return errorSubLocal;
    }
    //已经有了元素了,排队发送,放到队列尾巴上
    //往指令等待包中插入节点
    adtResult = ADT_CircularPtrListInsertNodeEnd(c21NetInitSet.listSendCmdWaitAckPtr,cmdWaitPackPackPtr);
    if(adtResult != ADT_RESULT_OK)
    {
        //释放访问权
        ProtocolC21NetSocketGetDataAccessRelease();
        //释放内存
        if(cmdWaitPackPackPtr->cmdParamBufferPtr != NULL)
        {
            ProtocolC21NetSocketMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        }
        ProtocolC21NetSocketMemFree(cmdWaitPackPackPtr->cmdParamBufferPtr);
        return ERROR_SUB_BOARD_C21_NET_SOCKET_CMD_WAIT_PACK_LIST_INSERT;
    }
    //发送成功,更新发送时间
    cmdWaitPackPackPtr->lastSendTimeTickMs = 0;
    //更新发送次数
    cmdWaitPackPackPtr->retryCount = 0;
    //释放访问权
    ProtocolC21NetSocketGetDataAccessRelease();
    return errorSubLocal;
}



