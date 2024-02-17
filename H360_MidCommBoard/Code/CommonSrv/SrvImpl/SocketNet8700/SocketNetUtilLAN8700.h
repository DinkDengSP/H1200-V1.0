/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 17:27:56
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SOCKET_NET_UTIL_LAN8700_H_
#define __SOCKET_NET_UTIL_LAN8700_H_
#include "CommonSrvImplBase.h"

//网络底层信息集合
typedef struct SOCKET_STATE_MSG_LAN8700
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
}SOCKET_STATE_MSG_LAN8700;
#define LENGTH_SOCKET_STATE_MSG_LAN8700       (sizeof(SOCKET_STATE_MSG_LAN8700)/sizeof(uint8_t))

//通过Socket状态值获取Socket状态描述字符串
uint8_t* SocketNetUtilLAN8700GetSocketState(ULONG state);

//状态集合初始化
void SocketNetUtilLAN8700Init(void);

/*--------------------------------网络底层讯息相关接口------------------------------------------*/
//获取网络底层信息指针
SOCKET_STATE_MSG_LAN8700* SocketNetUtilLAN8700GetBaseStatePtr(void);

//设置Socket初始化完成
void SocketNetUtilLAN8700SetBaseInitOver(void);

//检查基础Socket是否初始化完成
ErrorStatus SocketNetUtilLAN8700CheckBaseInitOver(void);

//检查网线是否插入
uint32_t SocketNetUtilLAN8700CheckBaseCheckPhyState(void);

//获取Socket内存池指针
NX_PACKET_POOL* SocketNetUtilLAN8700GetMemPoolPtr(void);

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetMainSocketPtr(void);

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetMainSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetMainSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetMainSocketStateCurrentByRecord(void);
void SocketNetUtilLAN8700SetMainSocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetMainSocketStateLastByRecord(void);
void SocketNetUtilLAN8700SetMainSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetLogSocketPtr(void);

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetLogSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetLogSocketStateByInfo(void);

//读写当前日志通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetLogSocketStateCurrentByRecord(void);
void SocketNetUtilLAN8700SetLogSocketStateCurrentByRecord(ULONG setVal);

//读写上一次日志通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetLogSocketStateLastByRecord(void);
void SocketNetUtilLAN8700SetLogSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetUtilLAN8700GetProbeSocketPtr(void);

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilLAN8700GetProbeSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilLAN8700GetProbeSocketStateByInfo(void);

//读写当前探针通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetProbeSocketStateCurrentByRecord(void);
void SocketNetUtilLAN8700SetProbeSocketStateCurrentByRecord(ULONG setVal);

//读写上一次探针通讯Socket状态,内存记录的
ULONG SocketNetUtilLAN8700GetProbeSocketStateLastByRecord(void);
void SocketNetUtilLAN8700SetProbeSocketStateLastByRecord(ULONG setVal);
/*---------------------------------------------------------------------------------------------*/
//获取主网口数据发送互斥信号量
void NetStateLan8700MainPortDataSendGetLock(void);
//释放主网口数据发送互斥信号量
void NetStateLan8700MainPortDataSendReleaseLock(void);

//获取日志网口数据发送互斥信号量
void NetStateLan8700LogPortDataSendGetLock(void);
//释放日志网口数据发送互斥信号量
void NetStateLan8700LogPortDataSendReleaseLock(void);

//获取探针网口数据发送互斥信号量
void NetStateLan8700ProbePortDataSendGetLock(void);
//释放探针网口数据发送互斥信号量
void NetStateLan8700ProbePortDataSendReleaseLock(void);



#endif









