/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 15:52:06 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_SYSTEM_H_
#define __DRIVER_CONFIG_SYSTEM_H_
#include "DriverConfigBoard.h"

/*----------------------------------------系统消息使用的内存区域---------------------------------------*/
//主消息使用的内存空间
#define MEM_REGION_SYS_MSG_MAIN                                         MEM_AXI_SRAM
//网口消息使用的内存空间            
#define MEM_REGION_SYS_MSG_NET                                          MEM_EXT_SRAM
//CAN消息使用的内存空间            
#define MEM_REGION_SYS_MSG_CAN                                          MEM_EXT_SRAM
//日志消息使用的内存空间            
#define MEM_REGION_SYS_MSG_LOG                                          MEM_EXT_SRAM
//服务层任务使用的内存空间          
#define MEM_REGION_SRV_TASK                                             MEM_AXI_SRAM
//模块层任务使用的内存空间          
#define MEM_REGION_MOD_TASK                                             MEM_EXT_SRAM
//应用层任务使用的内存空间          
#define MEM_REGION_APP_TASK                                             MEM_EXT_SRAM
//系统参数内存
#define MEM_REGION_SYS_PARAM                                            MEM_EXT_SRAM
//日志文件包含讯息          
#define SYS_LOG_INCLUDE_FILE                                            "SrvTaskHeader.h"
//printf输出            
#define SYS_LOG_Printf(format,...)                                      SystemPrintf(format, ##__VA_ARGS__)
//单独日志打印              
#define SYS_LOG_SHOW_STR(str)                                           SystemSendString(str)

/*----------------------------------------NETX 驱动层基础定义----------------------------------------*/
//接收事件
#define NX_DRIVER_DEFERRED_PACKET_RECEIVED                              0X02
//发送事件
#define NX_DRIVER_DEFERRED_PACKET_TRANSMITTED                           0X04
//数据包类型识别 IP协议                     
#define NX_DRIVER_ETHERNET_IP                                           0x0800
#define NX_DRIVER_ETHERNET_IPV6                                         0x86dd
#define NX_DRIVER_ETHERNET_ARP                                          0x0806
#define NX_DRIVER_ETHERNET_RARP                                         0x8035
//MTU一般最长1500,加上14字节MAC头正好是1514                     
#define NX_DRIVER_ETHERNET_MTU                                          1514
//MAC地址头,始终为14字节                        
#define NX_DRIVER_ETHERNET_FRAME_SIZE                                   14

//移除接收数据的网络帧头,这样可以直接调用相应数据包格式的数据解析
#define NX_DRIVER_ETHERNET_HEADER_REMOVE(p)   \
{   \
    p -> nx_packet_prepend_ptr =  p -> nx_packet_prepend_ptr + NX_DRIVER_ETHERNET_FRAME_SIZE;  \
    p -> nx_packet_length =  p -> nx_packet_length - NX_DRIVER_ETHERNET_FRAME_SIZE;    \
} 

//网卡硬件驱动扩展
#define NX_DRIVER_CAPABILITY                ( NX_INTERFACE_CAPABILITY_IPV4_TX_CHECKSUM   | \
                                                NX_INTERFACE_CAPABILITY_IPV4_RX_CHECKSUM   | \
                                                NX_INTERFACE_CAPABILITY_TCP_TX_CHECKSUM    | \
                                                NX_INTERFACE_CAPABILITY_TCP_RX_CHECKSUM    | \
                                                NX_INTERFACE_CAPABILITY_UDP_TX_CHECKSUM    | \
                                                NX_INTERFACE_CAPABILITY_UDP_RX_CHECKSUM    | \
                                                NX_INTERFACE_CAPABILITY_ICMPV4_TX_CHECKSUM   | \
                                                NX_INTERFACE_CAPABILITY_ICMPV4_RX_CHECKSUM   | \
                                                NX_INTERFACE_CAPABILITY_ICMPV6_TX_CHECKSUM | \
                                                NX_INTERFACE_CAPABILITY_ICMPV6_RX_CHECKSUM )

//NETX端口状态
typedef enum NETX_PORT_STATE
{
    NETX_PORT_STATE_IDLE   = 0X00,//接口空闲
    NETX_PORT_STATE_ATTACH = 0X01,//接口附着
    NETX_PORT_STATE_INIT   = 0X02,//接口初始化
    NETX_PORT_STATE_ENABLE = 0X03,//接口使能
    NETX_PORT_STATE_ERR    = 0XFF,//接口错误
}NETX_PORT_STATE;

/*----------------------------------------NETX 驱动层基础定义----------------------------------------*/
//NETX使用的动态内存空间
#define NETX_LAN8700_MEM_REGION                                         MEM_EXT_SRAM
//NETX驱动调试任务使能              
#define NETX_LAN8700_DRIVER_DEBUG_SHOW                                  CONFIG_FUNC_DISABLE
//是否显示收发缓存              
#define NETX_LAN8700_DRIVER_DEBUG_SHOW_BUF                              CONFIG_FUNC_DISABLE
//LAN8700协议栈缓存             
#define NETX_LAN8700_PACKET_POOL_SIZE                                   (256*1024)
//1522的ETH帧加上12个字节的IP包头               
#define NETX_LAN8700_PACKET_SIZE                                        1536
//LAN8700 IP协议的ARP缓存列表长度               
#define NX_LAN8700_ARP_LENGTH                                           1024
//LAN8700是否开启文件传输功能                           
#define NETX_FILE_FTP_SERVICE_MODE                                      CONFIG_FUNC_DISABLE


/********************************************IP SOCKET实例状态机********************************************************/
//主通讯端口服务器缓存
#define TCP_SERVICE_MAIN_SOCKET_BUF_LENGTH                              (32*1024)
//是否使能主通讯端口日志信息打印
#define TCP_SERVICE_MAIN_RECV_MSG_SHOW_BUF                              CONFIG_FUNC_DISABLE
//主通讯端口号                  
#define TCP_MAIN_SOCKET_PORT                                            12460
//主通讯端口可以连接的设备总数                  
#define TCP_MAIN_SOCKET_PORT_MAX_CLIENT                                 1

//日志通讯端口服务器缓存                    
#define TCP_SERVICE_LOG_SOCKET_BUF_LENGTH                               (32*1024)
//是否使能日志通讯端口日志信息打印
#define TCP_SERVICE_LOG_RECV_MSG_SHOW_BUF                               CONFIG_FUNC_DISABLE
//日志通讯端口号                    
#define TCP_LOG_SOCKET_PORT                                             20711
//日志端口可以连接的设备总数                    
#define TCP_LOG_SOCKET_PORT_MAX_CLIENT                                  1

//探针通讯端口服务器缓存                    
#define TCP_SERVICE_PROBE_SOCKET_BUF_LENGTH                             (32*1024)
//是否使能探针通讯端口日志信息打印
#define TCP_SERVICE_PROBE_RECV_MSG_SHOW_BUF                             CONFIG_FUNC_DISABLE
//探针通讯端口号                    
#define TCP_PROBE_SOCKET_PORT                                           19900
//探针端口可以连接的设备总数                    
#define TCP_PROBE_SOCKET_PORT_MAX_CLIENT                                1

//发送最大超时时间                  
#define TRANS_TIME_OUT_MS                                               TIME_OUT_MS_ETH_SEND
//PHY网络连接延时消抖时间                   
#define TIME_MS_PHY_DISCONNECT_WAIT_STABLE                              5000
//PHY连接检测周期                   
#define TIME_MS_PHY_DISCONNECT_CHECK_PERIOD                             1000
//检测网线连接稳定时间                  
#define COUNT_MAX_PHY_DISCONNECT_WAIT_STABLE                            (TIME_MS_PHY_DISCONNECT_WAIT_STABLE/TIME_MS_PHY_DISCONNECT_CHECK_PERIOD)


#endif


















