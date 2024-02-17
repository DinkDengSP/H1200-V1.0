/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 14:30:16 +0800
************************************************************************************************/ 
#include "SrvImplSocketNet.h"
#include "SrvTaskHeader.h"

//任务全局对象初始化
void SrvImplSocketNetVarInit(void)
{

}

//主端口消息处理
static void SrvImplSocketNetMainMsgProcess(NET_RECV_PACK* netPackPtr);
//日志端口消息处理
static void SrvImplSocketNetLogMsgProcess(NET_RECV_PACK* netPackPtr);

//任务接收消息处理
void SrvImplSocketNetMsgProcess(void* taskMsgPtr)
{
    //处理不同端口过来的讯息
    NET_RECV_PACK* netPackPtr = (NET_RECV_PACK*)taskMsgPtr;
    //解析不同的端口
    if(netPackPtr->netPackSrc == NET_PACK_SRC_MAIN)
    {
        //主端口消息处理
        SrvImplSocketNetMainMsgProcess(netPackPtr);
    }
    else if(netPackPtr->netPackSrc == NET_PACK_SRC_LOG)
    {
        //日志端口消息处理
        SrvImplSocketNetLogMsgProcess(netPackPtr);
    }
    else
    {
        //不支持的端口信息
        SystemPrintf("E : %s, RecvNetPack Unsupport,Src: %d\r\n",__func__,netPackPtr->netPackSrc);
    }
    //释放缓存
    if(netPackPtr->recvDataLength > 0)
    {
        UserMemFree(NETX_LAN8700_MEM_REGION,netPackPtr->recvDatBufPtr);
    }
    //释放内存
    UserMemFree(NETX_LAN8700_MEM_REGION,netPackPtr);
}

//主端口消息处理
static void SrvImplSocketNetMainMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_SERVICE_MAIN_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
    //日志打印
    SystemPrintf("\r\nMainSocket RecvMsg,PeerIP: %d.%d.%d.%d,PeerPort: %d,Len: %d, Context: \r\n",(uint8_t)(netPackPtr->peerIpAddress>>24),
                        (uint8_t)(netPackPtr->peerIpAddress>>16),(uint8_t)(netPackPtr->peerIpAddress>>8),(uint8_t)(netPackPtr->peerIpAddress),
                        netPackPtr->peerPort,netPackPtr->recvDataLength);
    //数据打印
    if(netPackPtr->recvDataLength > 0)
    {
        SystemSendBuffer(netPackPtr->recvDatBufPtr,netPackPtr->recvDataLength);
    }
#endif
    uint32_t currentTimeTick = OS_TICK_TO_MS(tx_time_get());
    //SOCKET数据处理,内存必须深拷贝,调用者会释放内存
    NetProtPackRecv(netPackPtr,currentTimeTick);
}

//日志端口消息处理
static void SrvImplSocketNetLogMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_SERVICE_LOG_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
    //日志打印
    SystemPrintf("\r\nLogSocket RecvMsg,PeerIP: %d.%d.%d.%d,PeerPort: %d,Len: %d, Context: \r\n",(uint8_t)(netPackPtr->peerIpAddress>>24),
                        (uint8_t)(netPackPtr->peerIpAddress>>16),(uint8_t)(netPackPtr->peerIpAddress>>8),(uint8_t)(netPackPtr->peerIpAddress),
                        netPackPtr->peerPort,netPackPtr->recvDataLength);
    //数据打印
    if(netPackPtr->recvDataLength > 0)
    {
        SystemSendBuffer(netPackPtr->recvDatBufPtr,netPackPtr->recvDataLength);
    }
#endif
}

//ACK消息发送处理
void SrvImplSocketNetAckSendMsgProcess(void* socketAckMsgSendPtr)
{
    NET_PROT_ACK_PACK* netProtAckPackPtr = (NET_PROT_ACK_PACK*)socketAckMsgSendPtr;
    NetProtAckSend(netProtAckPackPtr);
    netProtAckPackPtr = NULL;
}

//LAN8700函数导入
//网卡协议栈初始化
extern void SrvImplSocketLAN8700StackInit(void);
//LAN8700函数导入
//PHY网线链接检测状态机
extern void SrvImplSocketLAN8700PhyLinkFSM(void);
//主通讯端口状态机
extern void SrvImplSocketLAN8700MainPortFSM(void);
//日志通讯端口状态机
extern void SrvImplSocketLAN8700LogPortFSM(void);

//任务状态机初始化
void SrvImplSocketNetFSM_Init(void)
{
    //初始化LAN8700网卡协议栈
    //SrvImplSocketLAN8700StackInit();
}

//任务状态机周期运行
void SrvImplSocketNetFSM_RunPeriod(void)
{
    //LAN8700PHY网线链接检测状态机
    SrvImplSocketLAN8700PhyLinkFSM();
    //LAN8700主通讯端口状态机
    SrvImplSocketLAN8700MainPortFSM();
    //LAN8700日志通讯端口状态机
    SrvImplSocketLAN8700LogPortFSM();
    //协议SOCKET定时检测
    uint32_t currentTimeTick = OS_TICK_TO_MS(tx_time_get());
    NetProtRunPeriod(currentTimeTick);
}














