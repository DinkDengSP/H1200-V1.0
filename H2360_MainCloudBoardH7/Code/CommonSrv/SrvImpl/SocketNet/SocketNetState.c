/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 14:21:06 +0800
************************************************************************************************/ 
#include "SocketNetState.h"
#include "DriverConfigSystem.h"

//讯息集合
static SOCKET_STATE_MSG* socketStateMsgPtr = NULL;

//状态集合初始化
void SocketNetStateInit(void)
{
    //重复初始化,释放内存
    if(socketStateMsgPtr != NULL)
    {
        UserMemFree(NETX_LAN8700_MEM_REGION,socketStateMsgPtr);
    }
    //申请内存
    socketStateMsgPtr = UserMemMallocWhileSuccess(NETX_LAN8700_MEM_REGION,LENGTH_SOCKET_STATE_MSG);
    //设置状态为默认值
    socketStateMsgPtr->socketInitFlag = 0;
    //创建互斥信号量
    tx_mutex_create(&(socketStateMsgPtr->mutexMainSocketSendData),"mutexMainSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateMsgPtr->mutexLogSocketSendData),"mutexLogSocketSendData",TX_INHERIT);
    tx_mutex_create(&(socketStateMsgPtr->mutexProbeSocketSendData),"mutexProbeSocketSendData",TX_INHERIT);
}

//获取主网口数据发送互斥信号量
void NetStateMainPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateMsgPtr->mutexMainSocketSendData),TX_WAIT_FOREVER);
}
//释放主网口数据发送互斥信号量
void NetStateMainPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateMsgPtr->mutexMainSocketSendData));
}

//获取日志网口数据发送互斥信号量
void NetStateLogPortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateMsgPtr->mutexLogSocketSendData),TX_WAIT_FOREVER);
}
//释放日志网口数据发送互斥信号量
void NetStateLogPortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateMsgPtr->mutexLogSocketSendData));
}

//获取探针网口数据发送互斥信号量
void NetStateProbePortDataSendGetLock(void)
{
    tx_mutex_get(&(socketStateMsgPtr->mutexProbeSocketSendData),TX_WAIT_FOREVER);
}
//释放探针网口数据发送互斥信号量
void NetStateProbePortDataSendReleaseLock(void)
{
    tx_mutex_put(&(socketStateMsgPtr->mutexProbeSocketSendData));
}


//获取Socket内存池指针
NX_PACKET_POOL* SocketNetStateGetMemPoolPtr(void)
{
    return socketStateMsgPtr->netMemPoolPtr;
}

//获取网络底层信息指针
SOCKET_STATE_MSG* SocketNetStateGetBaseStatePtr(void)
{
    return socketStateMsgPtr;
}

//设置Socket初始化完成
void SocketNetStateSetBaseInitOver(void)
{
    socketStateMsgPtr->socketInitFlag = SOCKET_NET_INIT_FLAG;
}

//检查基础Socket是否初始化完成
ErrorStatus SocketNetStateCheckBaseInitOver(void)
{
    if(socketStateMsgPtr->socketInitFlag == SOCKET_NET_INIT_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//检查网线是否插入
uint32_t SocketNetStateCheckSocketBaseCheckPhyState(void)
{
    //获取当前网线连接状态
    return socketStateMsgPtr->lanPhyConnectStateCurrent;
}

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetMainSocketPtr(void)
{
    return &(socketStateMsgPtr->tcpSocketMain);
}

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetMainSocketInfoPtr(void)
{
    return &(socketStateMsgPtr->socketInfoMain);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetMainSocketStateByInfo(void)
{
    return socketStateMsgPtr->socketInfoMain.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetStateGetMainSocketStateCurrentByRecord(void)
{
    return socketStateMsgPtr->socketMainStateCurrent;
}

void SocketNetStateSetMainSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketMainStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetStateGetMainSocketStateLastByRecord(void)
{
    return socketStateMsgPtr->socketMainStateLast;
}
void SocketNetStateSetMainSocketStateLastByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketMainStateLast = setVal;
}

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetLogSocketPtr(void)
{
    return &(socketStateMsgPtr->tcpSocketLog);
}

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetLogSocketInfoPtr(void)
{
    return &(socketStateMsgPtr->socketInfoLog);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetLogSocketStateByInfo(void)
{
    return socketStateMsgPtr->socketInfoLog.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetStateGetLogSocketStateCurrentByRecord(void)
{
    return socketStateMsgPtr->socketLogStateCurrent;
}

void SocketNetStateSetLogSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketLogStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetStateGetLogSocketStateLastByRecord(void)
{
    return socketStateMsgPtr->socketLogStateLast;
}
void SocketNetStateSetLogSocketStateLastByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketLogStateLast = setVal;
}


//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetProbeSocketPtr(void)
{
    return &(socketStateMsgPtr->tcpSocketProbe);
}

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetProbeSocketInfoPtr(void)
{
    return &(socketStateMsgPtr->socketInfoProbe);
}

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetProbeSocketStateByInfo(void)
{
    return socketStateMsgPtr->socketInfoProbe.tcp_socket_state;
}

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetStateGetProbeSocketStateCurrentByRecord(void)
{
    return socketStateMsgPtr->socketProbeStateCurrent;
}

void SocketNetStateSetProbeSocketStateCurrentByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketProbeStateCurrent = setVal;
}

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetStateGetProbeSocketStateLastByRecord(void)
{
    return socketStateMsgPtr->socketProbeStateLast;
}
void SocketNetStateSetProbeSocketStateLastByRecord(ULONG setVal)
{
    socketStateMsgPtr->socketProbeStateLast = setVal;
}









