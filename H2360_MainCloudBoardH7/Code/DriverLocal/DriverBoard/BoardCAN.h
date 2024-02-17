/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-06 10:49:13 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 11:33:20 +0800
************************************************************************************************/ 
#ifndef __BOARD_CAN_H_
#define __BOARD_CAN_H_
#include "DriverHeaderMCU.h"

/*-------------------------------------------------------------------*/
//获取CAN1互斥锁
void BoardCAN1_GetLock(void);

//释放CAN1互斥锁
void BoardCAN1_ReleaseLock(void);

//初始化
void BoardCAN1_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr);

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN1_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//检查CAN1 PHY状态
ErrorStatus BoardCAN1_CheckPhyState(void);

//CAN1总线发送数据
ERROR_SUB BoardCAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN1的各项信息
MCU_CAN_INFO* BoardCAN1_GetInfo(void);


#endif













