/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 17:00:10
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SocketNetUtilLAN8700.h"

//讯息集合
static SOCKET_STATE_MSG_LAN8700* socketStateLAN8700MsgPtr = NULL;

//状态集合初始化
void SocketNetUtilLAN8700Init(void)
{
    //重复初始化,释放内存
    if(socketStateLAN8700MsgPtr != NULL)
    {
        UserMemFree(NETX_LAN8700_MEM_REGION,socketStateLAN8700MsgPtr);
    }
    //申请内存
    socketStateLAN8700MsgPtr = UserMemMallocWhileSuccess(NETX_LAN8700_MEM_REGION,LENGTH_SOCKET_STATE_MSG_LAN8700);
    //设置状态为默认值
    socketStateLAN8700MsgPtr->socketInitFlag = 0;
    //创建互斥信号量
    tx_mutex_create(&(socketStateLAN8700MsgPtr->mutexMainSocketSendData),"mutexMainSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateLAN8700MsgPtr->mutexLogSocketSendData),"mutexLogSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateLAN8700MsgPtr->mutexProbeSocketSendData),"mutexProbeSocketSendData",TX_INHERIT);
}

//获取主网口数据发送互斥信号量
void NetStateLan8700MainPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateLAN8700MsgPtr->mutexMainSocketSendData),TX_WAIT_FOREVER);
}
//释放主网口数据发送互斥信号量
void NetStateLan8700MainPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateLAN8700MsgPtr->mutexMainSocketSendData));
}

//获取日志网口数据发送互斥信号量
void NetStateLan8700LogPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateLAN8700MsgPtr->mutexLogSocketSendData),TX_WAIT_FOREVER);
}
//释放日志网口数据发送互斥信号量
void NetStateLan8700LogPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateLAN8700MsgPtr->mutexLogSocketSendData));
}

//获取探针网口数据发送互斥信号量
void NetStateLan8700ProbePortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateLAN8700MsgPtr->mutexProbeSocketSendData),TX_WAIT_FOREVER);
}
//释放探针网口数据发送互斥信号量
void NetStateLan8700ProbePortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateLAN8700MsgPtr->mutexProbeSocketSendData));
}


//获取Socket内存池指针
NX_PACKET_POOL* SocketNetUtilLAN8700GetMemPoolPtr(void)
{
    return socketStateLAN8700MsgPtr->netMemPoolPtr;
}

//获取网络底层信息指针
SOCKET_STATE_MSG_LAN8700* SocketNetUtilLAN8700GetBaseStatePtr(void)
{
    return socketStateLAN8700MsgPtr;
}

//设置Socket初始化完成
void SocketNetUtilLAN8700SetBaseInitOver(void)
{
    socketStateLAN8700MsgPtr->socketInitFlag = SOCKET_NET_INIT_FLAG;
}

//检查基础Socket是否初始化完成
ErrorStatus SocketNetUtilLAN8700CheckBaseInitOver(void)
{
    if(socketStateLAN8700MsgPtr->socketInitFlag == SOCKET_NET_INIT_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//检查网线是否插入
uint32_t SocketNetUtilLAN8700CheckSocketBaseCheckPhyState(void)
{
    //获取当前网线连接状态
    return socketStateLAN8700MsgPtr->lanPhyConnectStateCurrent;
}

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetMainSocketPtr(void)
{
    return &(socketStateLAN8700MsgPtr->tcpSocketMain);
}

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetMainSocketInfoPtr(void)
{
    return &(socketStateLAN8700MsgPtr->socketInfoMain);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetMainSocketStateByInfo(void)
{
    return socketStateLAN8700MsgPtr->socketInfoMain.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetMainSocketStateCurrentByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketMainStateCurrent;
}

void SocketNetUtilLAN8700SetMainSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketMainStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetMainSocketStateLastByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketMainStateLast;
}
void SocketNetUtilLAN8700SetMainSocketStateLastByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketMainStateLast = setVal;
}

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetLogSocketPtr(void)
{
    return &(socketStateLAN8700MsgPtr->tcpSocketLog);
}

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetLogSocketInfoPtr(void)
{
    return &(socketStateLAN8700MsgPtr->socketInfoLog);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetLogSocketStateByInfo(void)
{
    return socketStateLAN8700MsgPtr->socketInfoLog.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetLogSocketStateCurrentByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketLogStateCurrent;
}

void SocketNetUtilLAN8700SetLogSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketLogStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetLogSocketStateLastByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketLogStateLast;
}
void SocketNetUtilLAN8700SetLogSocketStateLastByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketLogStateLast = setVal;
}


//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetProbeSocketPtr(void)
{
    return &(socketStateLAN8700MsgPtr->tcpSocketProbe);
}

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetProbeSocketInfoPtr(void)
{
    return &(socketStateLAN8700MsgPtr->socketInfoProbe);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetProbeSocketStateByInfo(void)
{
    return socketStateLAN8700MsgPtr->socketInfoProbe.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetProbeSocketStateCurrentByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketProbeStateCurrent;
}

void SocketNetUtilLAN8700SetProbeSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketProbeStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetProbeSocketStateLastByRecord(void)
{
    return socketStateLAN8700MsgPtr->socketProbeStateLast;
}
void SocketNetUtilLAN8700SetProbeSocketStateLastByRecord(ULONG setVal)
{
    socketStateLAN8700MsgPtr->socketProbeStateLast = setVal;
}













