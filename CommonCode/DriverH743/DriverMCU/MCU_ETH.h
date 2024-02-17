/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:00:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 11:01:00 +0800
************************************************************************************************/ 
#ifndef __MCU_ETH_H_
#define __MCU_ETH_H_
#include "DriverHeaderCore.h"

//多包发送结构体
typedef struct ETH_MULTI_PACK
{
    uint8_t* dataBufferPtrArray[ETH_TX_DESC_CNT];
    uint16_t dataLengthArray[ETH_TX_DESC_CNT];
    uint16_t packCount;
}ETH_MULTI_PACK;

//释放接收到的内存
void MCU_ETH_ReleaseRecvBuf(uint8_t* recvBufPtr);

//获取MAC层MAC地址指针
uint8_t* MCU_ETH_GetMacAddrPtr(void);

//打开ETH中断
void MCU_ETH_EnableInt(void);

//端口初始化
void MCU_ETH_PortInit(void);

//使能多播
void MCU_ETH_EnableMultiCast(void);

//禁止多播
void MCU_ETH_DisableMultiCast(void);

//根据网卡连接状态修改MAC状态
void MCU_ETH_ReflushModeSpeed(NET_PORT_STATE netPortState);

//读取PHY 寄存器数据
ERROR_SUB MCU_ETH_ReadPhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t *readValuePtr);

//写入PHY 寄存器数据
ERROR_SUB MCU_ETH_WritePhyReg(uint32_t phyAddr, uint32_t regAddr, uint32_t writeValue);

//写入网卡发送数据
ERROR_SUB MCU_ETH_WriteMultiPack(uint32_t checksumCtrl,ETH_MULTI_PACK* multiPackPtr);

//读取网卡接收到的数据
void MCU_ETH_ReadRecvData(uint8_t** recvBuf,uint32_t* length);

//端口开始
void MCU_ETH_Start(void);

//端口停止
void MCU_ETH_Stop(void);


#endif














