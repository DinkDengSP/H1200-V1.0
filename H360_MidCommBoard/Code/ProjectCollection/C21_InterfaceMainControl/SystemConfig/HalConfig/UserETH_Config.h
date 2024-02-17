/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-24 17:26:53 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-24 17:27:28 +0800
************************************************************************************************/ 
#ifndef __USER_ETH_CONFIG_H_
#define __USER_ETH_CONFIG_H_

//发送DMA描述符场电影
#define ETH_TX_DESC_CNT                                 4  
//接收DMA描述符长度
#define ETH_RX_DESC_CNT                                 4  

//以太网MAC地址设定
//1.单播 MAC 地址是指第一个字节的最低位是 0 的 MAC 地址。
//2.组播 MAC 地址是指第一个字节的最低位是 1 的 MAC 地址。
//3.广播 MAC 地址是指每个比特都是 1 的 MAC 地址。广播 MAC 地址是组播 MAC 地址的一个特例
#define ETH_MAC_ADDR0                                   ((uint8_t)0X90)
#define ETH_MAC_ADDR1                                   ((uint8_t)0X05)
#define ETH_MAC_ADDR2                                   ((uint8_t)0X25)
#define ETH_MAC_ADDR3                                   ((uint8_t)0X20)
#define ETH_MAC_ADDR4                                   ((uint8_t)0X07)
#define ETH_MAC_ADDR5                                   ((uint8_t)0X11)

#endif




