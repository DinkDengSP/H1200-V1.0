/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 17:01:03
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplSocketNet8700.h"
#include "CommonSrvTaskHeader.h"


//任务全局对象初始化
void SrvImplSocketNet8700VarInit(void)
{

}

//主端口消息处理
static void SrvImplSocketNet8700MainMsgProcess(NET_RECV_PACK* netPackPtr);
//日志端口消息处理
static void SrvImplSocketNet8700LogMsgProcess(NET_RECV_PACK* netPackPtr);
//探针端口消息处理
static void SrvImplSocketNet8700ProbeMsgProcess(NET_RECV_PACK* netPackPtr);

//任务接收消息处理
void SrvImplSocketNet8700MsgProcess(void* taskMsgPtr)
{
    //处理不同端口过来的讯息
    NET_RECV_PACK* netPackPtr = (NET_RECV_PACK*)taskMsgPtr;
    //解析不同的端口
    if(netPackPtr->netPackSrc == NET_PACK_SRC_MAIN)
    {
        //主端口消息处理
        SrvImplSocketNet8700MainMsgProcess(netPackPtr);
    }
    else if(netPackPtr->netPackSrc == NET_PACK_SRC_LOG)
    {
        //日志端口消息处理
        SrvImplSocketNet8700LogMsgProcess(netPackPtr);
    }
    else if(netPackPtr->netPackSrc == NET_PACK_SRC_PROBE)
    {
        //探针端口消息处理
        SrvImplSocketNet8700ProbeMsgProcess(netPackPtr);
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
static void SrvImplSocketNet8700MainMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_LAN8700_MAIN_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
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
    //SOCKET1数据处理,内存必须深拷贝,调用者会释放内存
    NetProtPackRecv(netPackPtr,currentTimeTick);
    //更新接收数据的时间
    SYSTEM_STATE_HEART* sysCommHeartStatePtr = SystemStateGetCommHeart();
    sysCommHeartStatePtr->lastRecvDataTimeStamp = currentTimeTick;
}

//日志端口消息处理
static void SrvImplSocketNet8700LogMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_LAN8700_LOG_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
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

//探针端口消息处理
static void SrvImplSocketNet8700ProbeMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_LAN8700_PROBE_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
    //日志打印
    SystemPrintf("\r\nProbeSocket RecvMsg,PeerIP: %d.%d.%d.%d,PeerPort: %d,Len: %d, Context: \r\n",(uint8_t)(netPackPtr->peerIpAddress>>24),
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
void SrvImplSocketNet8700AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    NET_PROT_ACK_PACK* netProtAckPackPtr = (NET_PROT_ACK_PACK*)socketAckMsgSendPtr;
    NetProtAckSend(netProtAckPackPtr);
    netProtAckPackPtr = NULL;
}

//LAN8700函数导入
//网卡协议栈初始化
extern void SrvImplSocketNetLAN8700StackInit(void);
//LAN8700函数导入
//PHY网线链接检测状态机
extern void SrvImplSocketNetLAN8700PhyLinkFSM(void);
//主通讯端口状态机
extern void SrvImplSocketNetLAN8700MainPortFSM(void);
//日志通讯端口状态机
extern void SrvImplSocketNetLAN8700LogPortFSM(void);
//探针通讯端口状态机
extern void SrvImplSocketNetLAN8700ProbePortFSM(void);
//创建FTP服务
#if(NETX_FILE_FTP_SERVICE_MODE != CONFIG_FUNC_DISABLE)
extern void SrvImplNetSocketLAN8700CreateFtpServer(void);
#endif

//任务状态机初始化
void SrvImplSocketNet8700FSM_Init(void)
{
    //初始化LAN8700网卡协议栈
    //SrvImplSocketNetLAN8700StackInit();
}


//任务状态机周期运行
void SrvImplSocketNet8700FSM_RunPeriod(void)
{
    //LAN8700PHY网线链接检测状态机
    SrvImplSocketNetLAN8700PhyLinkFSM();
    //LAN8700主通讯端口状态机
    SrvImplSocketNetLAN8700MainPortFSM();
    //LAN8700日志通讯端口状态机
    SrvImplSocketNetLAN8700LogPortFSM();
    //LAN8700探针通讯端口状态机
    SrvImplSocketNetLAN8700ProbePortFSM();
#if(NETX_FILE_FTP_SERVICE_MODE != CONFIG_FUNC_DISABLE)
    //创建FTP服务器
    SrvImplNetSocketLAN8700CreateFtpServer();
#endif
    //协议SOCKET定时检测
    uint32_t currentTimeTick = OS_TICK_TO_MS(tx_time_get());
    NetProtRunPeriod(currentTimeTick);
}








