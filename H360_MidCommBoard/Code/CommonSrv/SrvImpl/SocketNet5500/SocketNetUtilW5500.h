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
#ifndef __SOCKET_NET_UTIL_W5500_H_
#define __SOCKET_NET_UTIL_W5500_H_
#include "CommonSrvImplBase.h"


//网络底层信息集合
typedef struct SOCKET_STATE_MSG_W5500
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
}SOCKET_STATE_MSG_W5500;
#define LENGTH_SOCKET_STATE_MSG_W5500       (sizeof(SOCKET_STATE_MSG_W5500)/sizeof(uint8_t))


//状态集合初始化
void SocketNetUtilW5500Init(void);

/*--------------------------------网络底层讯息相关接口------------------------------------------*/
//获取网络底层信息指针
SOCKET_STATE_MSG_W5500* SocketNetUtilW5500GetBaseStatePtr(void);

//设置Socket初始化完成
void SocketNetUtilW5500SetBaseInitOver(void);

//检查基础Socket是否初始化完成
ErrorStatus SocketNetUtilW5500CheckBaseInitOver(void);

//检查网线是否插入
uint32_t SocketNetUtilW5500CheckBaseCheckPhyState(void);

//获取Socket内存池指针
NX_PACKET_POOL* SocketNetUtilW5500GetMemPoolPtr(void);

//获取网络底层讯息,主通讯端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetMainSocketPtr(void);

//获取网络底层讯息,主通讯端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetMainSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetMainSocketStateByInfo(void);

//读写当前主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetMainSocketStateCurrentByRecord(void);
void SocketNetUtilW5500SetMainSocketStateCurrentByRecord(ULONG setVal);

//读写上一次主通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetMainSocketStateLastByRecord(void);
void SocketNetUtilW5500SetMainSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,日志端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetLogSocketPtr(void);

//获取网络底层讯息,日志端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetLogSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetLogSocketStateByInfo(void);

//读写当前日志通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetLogSocketStateCurrentByRecord(void);
void SocketNetUtilW5500SetLogSocketStateCurrentByRecord(ULONG setVal);

//读写上一次日志通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetLogSocketStateLastByRecord(void);
void SocketNetUtilW5500SetLogSocketStateLastByRecord(ULONG setVal);

//获取网络底层讯息,探针端口Socket指针
NX_TCP_SOCKET* SocketNetUtilW5500GetProbeSocketPtr(void);

//获取网络底层讯息,探针端口Socket状态集合指针
NX_SOCKET_INFO* SocketNetUtilW5500GetProbeSocketInfoPtr(void);

//获取Socket状态,依靠Socket Info
ULONG SocketNetUtilW5500GetProbeSocketStateByInfo(void);

//读写当前探针通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetProbeSocketStateCurrentByRecord(void);
void SocketNetUtilW5500SetProbeSocketStateCurrentByRecord(ULONG setVal);

//读写上一次探针通讯Socket状态,内存记录的
ULONG SocketNetUtilW5500GetProbeSocketStateLastByRecord(void);
void SocketNetUtilW5500SetProbeSocketStateLastByRecord(ULONG setVal);
/*---------------------------------------------------------------------------------------------*/
//获取主网口数据发送互斥信号量
void NetUtilW5500MainPortDataSendGetLock(void);
//释放主网口数据发送互斥信号量
void NetUtilW5500MainPortDataSendReleaseLock(void);

//获取日志网口数据发送互斥信号量
void NetUtilW5500LogPortDataSendGetLock(void);
//释放日志网口数据发送互斥信号量
void NetUtilW5500LogPortDataSendReleaseLock(void);

//获取探针网口数据发送互斥信号量
void NetUtilW5500ProbePortDataSendGetLock(void);
//释放探针网口数据发送互斥信号量
void NetUtilW5500ProbePortDataSendReleaseLock(void);


#endif









