/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 10:30:01
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplSocketNet5500.h"
#include "CommonSrvTaskHeader.h"

//任务全局对象初始化
void SrvImplSocketNet5500VarInit(void)
{

}

//主端口消息处理
static void SrvImplSocketNet5500MainMsgProcess(NET_RECV_PACK* netPackPtr);
//日志端口消息处理
static void SrvImplSocketNet5500LogMsgProcess(NET_RECV_PACK* netPackPtr);
//探针端口消息处理
static void SrvImplSocketNet5500ProbeMsgProcess(NET_RECV_PACK* netPackPtr);


//任务接收消息处理
void SrvImplSocketNet5500MsgProcess(void* taskMsgPtr)
{
    //处理不同端口过来的讯息
    NET_RECV_PACK* netPackPtr = (NET_RECV_PACK*)taskMsgPtr;
    //解析不同的端口
    if(netPackPtr->netPackSrc == NET_PACK_SRC_MAIN)
    {
        //主端口消息处理
        SrvImplSocketNet5500MainMsgProcess(netPackPtr);
    }
    else if(netPackPtr->netPackSrc == NET_PACK_SRC_LOG)
    {
        //日志端口消息处理
        SrvImplSocketNet5500LogMsgProcess(netPackPtr);
    }
    else if(netPackPtr->netPackSrc == NET_PACK_SRC_PROBE)
    {
        //探针端口消息处理
        SrvImplSocketNet5500ProbeMsgProcess(netPackPtr);
    }
    else
    {
        //不支持的端口信息
        SystemPrintf("E : %s, RecvNetPack Unsupport,Src: %d\r\n",__func__,netPackPtr->netPackSrc);
    }
    //释放缓存
    if(netPackPtr->recvDataLength > 0)
    {
        UserMemFree(NETX_W5500_MEM_REGION,netPackPtr->recvDatBufPtr);
    }
    //释放内存
    UserMemFree(NETX_W5500_MEM_REGION,netPackPtr);
}


//ACK消息发送处理
void SrvImplSocketNet5500AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    SrvImplExternSocket5500AckSendMsgProcess(socketAckMsgSendPtr);
}

//外扩消息处理程序
__weak void SrvImplExternSocket5500AckSendMsgProcess(void* socketAckMsgSendPtr)
{
    asm("nop");
}


//主端口消息处理
static void SrvImplSocketNet5500MainMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_W5500_MAIN_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
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
    //扩展消息处理程序
    SrvImplExternSocket5500MainMsgProcess(netPackPtr);
}

//外扩主端口消息处理程序
__weak void SrvImplExternSocket5500MainMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//日志端口消息处理
static void SrvImplSocketNet5500LogMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_W5500_LOG_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
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
    //扩展消息处理程序
    SrvImplExternSocket5500LogMsgProcess(netPackPtr);
}

//外扩日志端口消息处理程序
__weak void SrvImplExternSocket5500LogMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//探针端口消息处理
static void SrvImplSocketNet5500ProbeMsgProcess(NET_RECV_PACK* netPackPtr)
{
#if(TCP_W5500_PROBE_RECV_MSG_SHOW_BUF == CONFIG_FUNC_ENABLE)
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
    //扩展消息处理程序
    SrvImplExternSocket5500ProbeMsgProcess(netPackPtr);
}

//外扩探针端口消息处理程序
__weak void SrvImplExternSocket5500ProbeMsgProcess(NET_RECV_PACK* netPackPtr)
{
    asm("nop");
}

//W5500函数导入
//网卡协议栈初始化
extern void SrvImplSocketNetW5500StackInit(void);
//W5500函数导入
//PHY网线链接检测状态机
extern void SrvImplSocketNetW5500PhyLinkFSM(void);
//主通讯端口状态机
extern void SrvImplSocketNetW5500MainPortFSM(void);
//日志通讯端口状态机
extern void SrvImplSocketNetW5500LogPortFSM(void);
//探针通讯端口状态机
extern void SrvImplSocketNetW5500ProbePortFSM(void);

//任务状态机初始化
void SrvImplSocketNet5500FSM_Init(void)
{
    //初始化W5500网卡协议栈
    //SrvImplSocketNetW5500StackInit();
}


//任务状态机周期运行
void SrvImplSocketNet5500FSM_RunPeriod(void)
{
    //W5500PHY网线链接检测状态机
    SrvImplSocketNetW5500PhyLinkFSM();
    //W5500主通讯端口状态机
    SrvImplSocketNetW5500MainPortFSM();
    //W5500日志通讯端口状态机
    SrvImplSocketNetW5500LogPortFSM();
    //W5500探针通讯端口状态机
    SrvImplSocketNetW5500ProbePortFSM();
    //扩展定时处理程序
    uint32_t currentTimeTick = OS_TICK_TO_MS(tx_time_get());
    SrvImplSocketNetW5500TimerPeriod(currentTimeTick);
}


//扩展定时处理程序
__weak void SrvImplSocketNetW5500TimerPeriod(uint32_t currentTimeTick)
{
    asm("nop");
}





