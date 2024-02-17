/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 18:05:47 +0800
************************************************************************************************/ 
#ifndef __MCU_CAN1_H_
#define __MCU_CAN1_H_
#include "DriverHeaderCore.h"

//获取CAN互斥锁
void MCU_CAN1_GetLock(void);

//释放CAN1互斥锁
void MCU_CAN1_ReleaseLock(void);

//初始化
void MCU_CAN1_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr);

//将接收到并处理完成的数据放回驱动空闲队列
void MCU_CAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//CAN总线发送数据
ERROR_SUB MCU_CAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN1的各项信息
MCU_CAN_INFO* MCU_CAN1_GetInfo(void);

//清除状态
void MCU_Can1InfoClear(void);


#endif












