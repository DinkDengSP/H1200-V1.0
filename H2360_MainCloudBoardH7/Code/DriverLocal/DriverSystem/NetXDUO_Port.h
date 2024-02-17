/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __NETXDUO_PORT_H_
#define __NETXDUO_PORT_H_
#include "DriverHeaderBoard.h"
#include "DriverConfigSystem.h"

//驱动程序接口的保存讯息
typedef struct NETX_PORT_INFO
{
    //IP实例信息
    NX_IP* nxPortInfoIP_Ptr;          
    //接口信息  
    NX_INTERFACE* nxPortInfoInterfacePtr;
    //多播计数
    uint32_t  nxPortInfoMultiCastCount;
    //接口状态
    NETX_PORT_STATE nxPortInfoState;
    //网线接口状态
    NET_PORT_STATE netPortState;
    //连接状态
    uint8_t linkState;
    //连接速度
    uint32_t linkSpeed;
    //连接模式
    uint32_t deplexMode;
    //状态标记
    uint32_t dataEvent;
    //接收计数
    uint32_t recvPackCount;
    //发送计数
    uint32_t sendPackCount;
    //内存分配失败计数
    uint32_t allocFailCount;
    //错误计数
    uint32_t errorCount;
}NETX_PORT_INFO;

/*----------------------------------------NETX LAN8700驱动接口----------------------------------------*/


//IP接口保存的讯息
extern NX_IP lan8700NetX;
//执行LAN8700 IP协议栈的线程
extern TX_THREAD *netx_lan8700_thread_ptr;  
//ARP缓存长度
extern ULONG lan8700ArpTableBufNetX[NX_LAN8700_ARP_LENGTH/sizeof(ULONG)];

//LAN8700创建驱动缓存
NX_PACKET_POOL* NetX_DriverLAN8700_CreateMemoryPool(void);

//LAN8700 NetX驱动接口
void NetX_DriverLAN8700(NX_IP_DRIVER *nxReqPtr);


#endif















