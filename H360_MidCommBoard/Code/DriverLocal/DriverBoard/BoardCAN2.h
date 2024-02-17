/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __BOARD_CAN2_H_
#define __BOARD_CAN2_H_
#include "DriverHeaderMCU.h"

//获取CAN2互斥锁
void BoardCAN2_GetLock(void);

//释放CAN2互斥锁
void BoardCAN2_ReleaseLock(void);

//初始化
void BoardCAN2_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr);

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//检查CAN2 PHY状态
ErrorStatus BoardCAN2_CheckPhyState(void);

//CAN2总线发送数据
ERROR_SUB BoardCAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN2的各项信息
MCU_CAN_INFO* BoardCAN2_GetInfo(void);

#endif


