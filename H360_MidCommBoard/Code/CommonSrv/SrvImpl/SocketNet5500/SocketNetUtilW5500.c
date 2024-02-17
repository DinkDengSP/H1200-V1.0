/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SocketNetUtilW5500.h"

//讯息集合
static SOCKET_STATE_MSG_W5500* socketStateW5500MsgPtr = NULL;

//状态集合初始化
void SocketNetUtilW5500Init(void)
{
    //重复初始化,释放内存
    if(socketStateW5500MsgPtr != NULL)
    {
        UserMemFree(NETX_W5500_MEM_REGION,socketStateW5500MsgPtr);
    }
    //申请内存
    socketStateW5500MsgPtr = UserMemMallocWhileSuccess(NETX_W5500_MEM_REGION,LENGTH_SOCKET_STATE_MSG_W5500);
    //设置状态为默认值
    socketStateW5500MsgPtr->socketInitFlag = 0;
    //创建互斥信号量
    tx_mutex_create(&(socketStateW5500MsgPtr->mutexMainSocketSendData),"mutexMainSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateW5500MsgPtr->mutexLogSocketSendData),"mutexLogSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateW5500MsgPtr->mutexProbeSocketSendData),"mutexProbeSocketSendData",TX_INHERIT);
}

//获取主网口数据发送互斥信号量
void NetUtilW5500MainPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateW5500MsgPtr->mutexMainSocketSendData),TX_WAIT_FOREVER);
}
//释放主网口数据发送互斥信号量
void NetUtilW5500MainPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateW5500MsgPtr->mutexMainSocketSendData));
}

//获取日志网口数据发送互斥信号量
void NetUtilW5500LogPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateW5500MsgPtr->mutexLogSocketSendData),TX_WAIT_FOREVER);
}
//释放日志网口数据发送互斥信号量
void NetUtilW5500LogPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateW5500MsgPtr->mutexLogSocketSendData));
}

//获取探针网口数据发送互斥信号量
void NetUtilW5500ProbePortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateW5500MsgPtr->mutexProbeSocketSendData),TX_WAIT_FOREVER);
}
//释放探针网口数据发送互斥信号量
void NetUtilW5500ProbePortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateW5500MsgPtr->mutexProbeSocketSendData));
}


//获取Socket内存池指针
NX_PACKET_POOL* SocketNetUtilW5500GetMemPoolPtr(void)
{
    return socketStateW5500MsgPtr->netMemPoolPtr;
}

//获取网络底层信息指针
SOCKET_STATE_MSG_W5500* SocketNetUtilW5500GetBaseStatePtr(void)
{
    return socketStateW5500MsgPtr;
}

//设置Socket初始化完成
void SocketNetUtilW5500SetBaseInitOver(void)
{
    socketStateW5500MsgPtr->socketInitFlag = SOCKET_NET_INIT_FLAG;
}

//检查基础Socket是否初始化完成
ErrorStatus SocketNetUtilW5500CheckBaseInitOver(void)
{
    if(socketStateW5500MsgPtr->socketInitFlag == SOCKET_NET_INIT_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//检查网线是否插入
uint32_t SocketNetUtilW5500CheckSocketBaseCheckPhyState(void)
{
    //获取当前网线连接状态
    return socketStateW5500MsgPtr->lanPhyConnectStateCurrent;
}

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetMainSocketPtr(void)
{
    return &(socketStateW5500MsgPtr->tcpSocketMain);
}

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetMainSocketInfoPtr(void)
{
    return &(socketStateW5500MsgPtr->socketInfoMain);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetMainSocketStateByInfo(void)
{
    return socketStateW5500MsgPtr->socketInfoMain.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetMainSocketStateCurrentByRecord(void)
{
    return socketStateW5500MsgPtr->socketMainStateCurrent;
}

void SocketNetUtilW5500SetMainSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketMainStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetMainSocketStateLastByRecord(void)
{
    return socketStateW5500MsgPtr->socketMainStateLast;
}
void SocketNetUtilW5500SetMainSocketStateLastByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketMainStateLast = setVal;
}

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetLogSocketPtr(void)
{
    return &(socketStateW5500MsgPtr->tcpSocketLog);
}

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetLogSocketInfoPtr(void)
{
    return &(socketStateW5500MsgPtr->socketInfoLog);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetLogSocketStateByInfo(void)
{
    return socketStateW5500MsgPtr->socketInfoLog.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetLogSocketStateCurrentByRecord(void)
{
    return socketStateW5500MsgPtr->socketLogStateCurrent;
}

void SocketNetUtilW5500SetLogSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketLogStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetLogSocketStateLastByRecord(void)
{
    return socketStateW5500MsgPtr->socketLogStateLast;
}
void SocketNetUtilW5500SetLogSocketStateLastByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketLogStateLast = setVal;
}


//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetProbeSocketPtr(void)
{
    return &(socketStateW5500MsgPtr->tcpSocketProbe);
}

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetProbeSocketInfoPtr(void)
{
    return &(socketStateW5500MsgPtr->socketInfoProbe);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetProbeSocketStateByInfo(void)
{
    return socketStateW5500MsgPtr->socketInfoProbe.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetProbeSocketStateCurrentByRecord(void)
{
    return socketStateW5500MsgPtr->socketProbeStateCurrent;
}

void SocketNetUtilW5500SetProbeSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketProbeStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetProbeSocketStateLastByRecord(void)
{
    return socketStateW5500MsgPtr->socketProbeStateLast;
}
void SocketNetUtilW5500SetProbeSocketStateLastByRecord(ULONG setVal)
{
    socketStateW5500MsgPtr->socketProbeStateLast = setVal;
}








