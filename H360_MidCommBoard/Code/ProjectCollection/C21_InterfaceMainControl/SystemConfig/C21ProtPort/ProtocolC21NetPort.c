/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-06 09:24:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-06 12:06:12 +0800
************************************************************************************************/ 
#include "ProtocolC21NetPort.h"
#include "UserMemManage.h"
#include "DriverHeaderBoard.h"
#include "DriverConfigSystem.h"
#include "SrvImplHeader.h"

//获取主网口数据发送互斥信号量
extern void NetUtilW5500MainPortDataSendGetLock(void);
//释放主网口数据发送互斥信号量
extern void NetUtilW5500MainPortDataSendReleaseLock(void);
//获取主通讯网口状态
extern ULONG SrvImplSocketNetW5500MainGetState(void);


//获取SOCKET互斥锁
void ProtocolC21NetSocketGetMutexLock(void)
{
    NetUtilW5500MainPortDataSendGetLock();
}

//释放SOCKET互斥锁
void ProtocolC21NetSocketReleaseMutexLock(void)
{
    NetUtilW5500MainPortDataSendReleaseLock();
}

//获取SOCKET是否建立稳定链接
FlagStatus ProtocolC21NetSocketGetConnectFlag(void)
{
    ULONG socketState = SrvImplSocketNetW5500MainGetState();
    if(socketState == NX_TCP_ESTABLISHED)
    {
        return SET;
    }
    return RESET;
}

//发送SOCKET一帧
ERROR_SUB ProtocolC21NetSocketSendFrame(uint16_t msgBodyLen,uint8_t* bodyBuf)
{
    uint32_t nxResult = SrvImplSocketNetW5500MainSendBuf(bodyBuf,msgBodyLen);
    if(nxResult == NX_SUCCESS)
    {
        return ERROR_SUB_OK;
    }
    return (ERROR_SUB)(ERROR_SUB_BOARD_NETX_DUO_BASE + nxResult);
}

//内存申请
void* ProtocolC21NetSocketMemMalloc(uint32_t sizeBytes)
{
    return UserMemMalloc(MEM_REGION_SYS_MSG_NET,sizeBytes);
}

//内存申请,必须成功
void* ProtocolC21NetSocketMemMallocWhileSuccess(uint32_t sizeBytes)
{
    return UserMemMallocWhileSuccess(MEM_REGION_SYS_MSG_NET,sizeBytes);
}

//内存释放
void ProtocolC21NetSocketMemFree(void* memPtr)
{
    UserMemFree(MEM_REGION_SYS_MSG_NET,memPtr);
}

//ACK信息转发
void ProtocolC21AckMsgDispatch(C21_ACK_RESULT ackResult)
{
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_queue_send(&queueSocketNet5500AckSend,(void*)(&ackResult),TX_NO_WAIT);
    if(txResult == TX_SUCCESS)
    {
        //转发成功          
        return;
    }
    NET_PROT_LOG("ProtocolC21AckMsgDispatch Failed,txResult: %d\r\n",txResult);
}













