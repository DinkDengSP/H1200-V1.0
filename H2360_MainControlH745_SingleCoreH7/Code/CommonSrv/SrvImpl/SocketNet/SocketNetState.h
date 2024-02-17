/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 14:42:28 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 20:27:45 +0800
************************************************************************************************/ 
#ifndef __SOCKET_NET_STATE_H_
#define __SOCKET_NET_STATE_H_
#include "SrvImplBase.h"

//Socket初始化标记  
#define SOCKET_NET_INIT_FLAG        0XAA55A55A

//NETX网卡状态和描述映射
typedef struct NETX_STATE_MAP_STR
{
    ULONG state;
    uint8_t* despStr;
}NETX_STATE_MAP_STR;

//Socket的状态
typedef struct NX_SOCKET_INFO
{
    ULONG tcp_packets_sent;
    ULONG tcp_bytes_sent;
    ULONG tcp_packets_received;
    ULONG tcp_bytes_received;
    ULONG tcp_retransmit_packets;
    ULONG tcp_packets_queued;
    ULONG tcp_checksum_errors;
    ULONG tcp_socket_state;
    ULONG tcp_transmit_queue_depth;
    ULONG tcp_transmit_window;
    ULONG tcp_receive_window;
}NX_SOCKET_INFO;
#define LENGTH_NX_SOCKET_INFO       (sizeof(NX_SOCKET_INFO)/sizeof(uint8_t))

//网络底层信息集合
typedef struct SOCKET_STATE_MSG
{
    //Socket初始化标记
    uint32_t socketInitFlag;
    //Socket内存池
    NX_PACKET_POOL* netMemPoolPtr;
    //互斥信号量,用于MAIN BUF发送
    TX_MUTEX mutexMainSocketSendData;
    TX_MUTEX mutexLogSocketSendData;
    TX_MUTEX mutexProbeSocketSendData;
    /*--------------------------网口PHY链接参数---------------------------*/
    //当前网线连接状态
    ULONG lanPhyConnectStateCurrent;
    //上一次的网线连接状态
    ULONG lanPhyConnectStateLast;
    /*--------------------------主网口相关参数---------------------------*/
    //主通讯Socket
    NX_TCP_SOCKET tcpSocketMain;
    //主通讯网口Socket状态集合
    NX_SOCKET_INFO socketInfoMain;
    //主通讯网口Socket的状态,上一次
    ULONG socketMainStateLast;
    //主通讯网口Socket的状态,当前
    ULONG socketMainStateCurrent;
    /*--------------------------日志网口相关参数---------------------------*/
    //日志通讯Socket
    NX_TCP_SOCKET tcpSocketLog;
    //日志通讯Socket状态集合
    NX_SOCKET_INFO socketInfoLog;
    //日志通讯Socket的状态,上一次
    ULONG socketLogStateLast;
    //日志通讯Socket的状态,当前
    ULONG socketLogStateCurrent;
    /*--------------------------探针网口相关参数---------------------------*/
    //探针通讯Socket
    NX_TCP_SOCKET tcpSocketProbe;
    //探针通讯Socket状态集合
    NX_SOCKET_INFO socketInfoProbe;
    //探针通讯Socket的状态,上一次
    ULONG socketProbeStateLast;
    //探针通讯Socket的状态,当前
    ULONG socketProbeStateCurrent;
}SOCKET_STATE_MSG;
#define LENGTH_SOCKET_STATE_MSG       (sizeof(SOCKET_STATE_MSG)/sizeof(uint8_t))

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SocketNetStateGetSocketState(ULONG state);

//状态集合初始化
void SocketNetStateInit(void);

/*--------------------------------网络底层讯息相关接口------------------------------------------*/
//获取网络底层信息指针
SOCKET_STATE_MSG* SocketNetStateGetBaseStatePtr(void);

//设置Socket初始化完成
void SocketNetStateSetBaseInitOver(void);

//检查基础Socket是否初始化完成
ErrorStatus SocketNetStateCheckBaseInitOver(void);

//检查网线是否插入
uint32_t SocketNetStateCheckBaseCheckPhyState(void);

//获取Socket内存池指针
NX_PACKET_POOL* SocketNetStateGetMemPoolPtr(void);

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetMainSocketPtr(void);

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetMainSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetMainSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetStateGetMainSocketStateCurrentByRecord(void);
void SocketNetStateSetMainSocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetStateGetMainSocketStateLastByRecord(void);
void SocketNetStateSetMainSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetLogSocketPtr(void);

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetLogSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetLogSocketStateByInfo(void);

//读写当前日志通讯Socket状态,内存记录的
ULONG SocketNetStateGetLogSocketStateCurrentByRecord(void);
void SocketNetStateSetLogSocketStateCurrentByRecord(ULONG setVal);

//读写上一次日志通讯Socket状态,内存记录的
ULONG SocketNetStateGetLogSocketStateLastByRecord(void);
void SocketNetStateSetLogSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetStateGetProbeSocketPtr(void);

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetStateGetProbeSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetStateGetProbeSocketStateByInfo(void);

//读写当前探针通讯Socket状态,内存记录的
ULONG SocketNetStateGetProbeSocketStateCurrentByRecord(void);
void SocketNetStateSetProbeSocketStateCurrentByRecord(ULONG setVal);

//读写上一次探针通讯Socket状态,内存记录的
ULONG SocketNetStateGetProbeSocketStateLastByRecord(void);
void SocketNetStateSetProbeSocketStateLastByRecord(ULONG setVal);
/*---------------------------------------------------------------------------------------------*/
//获取主网口数据发送互斥信号量
void NetStateMainPortDataSendGetLock(void);
//释放主网口数据发送互斥信号量
void NetStateMainPortDataSendReleaseLock(void);

//获取日志网口数据发送互斥信号量
void NetStateLogPortDataSendGetLock(void);
//释放日志网口数据发送互斥信号量
void NetStateLogPortDataSendReleaseLock(void);

//获取探针网口数据发送互斥信号量
void NetStateProbePortDataSendGetLock(void);
//释放探针网口数据发送互斥信号量
void NetStateProbePortDataSendReleaseLock(void);



#endif













