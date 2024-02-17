/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 14:42:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 16:21:00 +0800
************************************************************************************************/ 
#include "SocketNetState.h"
#include "DriverConfigSystem.h"

//NETX状态映射表
static const NETX_STATE_MAP_STR NetX_StateStrMapArray[] = {
    {NX_TCP_CLOSED          ,"NX_TCP_CLOSED"      },
    {NX_TCP_LISTEN_STATE    ,"NX_TCP_LISTEN_STATE"},
    {NX_TCP_SYN_SENT        ,"NX_TCP_SYN_SENT"    },
    {NX_TCP_SYN_RECEIVED    ,"NX_TCP_SYN_RECEIVED"},
    {NX_TCP_ESTABLISHED     ,"NX_TCP_ESTABLISHED" },
    {NX_TCP_CLOSE_WAIT      ,"NX_TCP_CLOSE_WAIT"  },
    {NX_TCP_FIN_WAIT_1      ,"NX_TCP_FIN_WAIT_1"  },
    {NX_TCP_FIN_WAIT_2      ,"NX_TCP_FIN_WAIT_2"  },
    {NX_TCP_CLOSING         ,"NX_TCP_CLOSING"     },
    {NX_TCP_TIMED_WAIT      ,"NX_TCP_TIMED_WAIT"  },
    {NX_TCP_LAST_ACK        ,"NX_TCP_LAST_ACK"    },
    {0                      ,"NX_TCP_UN_INIT"     },
};
#define LENGTH_NETX_STATE_MAP_STR_MAP       (sizeof(NetX_StateStrMapArray)/sizeof(NetX_StateStrMapArray[0]))
//未知状态描述表
static const uint8_t NetX_DespStrUnknow[] = "NX_TCP_UNKNOW!!";

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SocketNetStateGetSocketState(ULONG state)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < LENGTH_NETX_STATE_MAP_STR_MAP;indexUtil++)
    {
        if(state == NetX_StateStrMapArray[indexUtil].state)
        {
            return (uint8_t*)(NetX_StateStrMapArray[indexUtil].despStr);
        }
    }
    return (uint8_t*)(NetX_DespStrUnknow);
}

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













