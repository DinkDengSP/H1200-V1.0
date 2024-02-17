/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 10:52:24 +0800
************************************************************************************************/ 
#ifndef __BOARD_CAN_H_
#define __BOARD_CAN_H_
#include "DriverHeaderMCU.h"
#include "DriverConfigBoard.h"

/*-------------------------------------------------------------------*/
#ifdef BOARD_CAN1_VDD_SENSE_FUNC
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


/*-------------------------------------------------------------------*/
#ifdef BOARD_CAN2_VDD_SENSE_FUNC
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

#endif











