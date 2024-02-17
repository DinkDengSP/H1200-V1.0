/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-19 16:52:46 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-20 11:47:26 +0800
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

//设置AUX输出
void BoardCAN1_SetAuxOutState(PIN_STATE setState);

//CAN1总线发送数据
ERROR_SUB BoardCAN1_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN1的各项信息
MCU_CAN_INFO* BoardCAN1_GetInfo(void);

/*-------------------------------------------------------------------*/
//获取CAN2互斥锁
void BoardCAN2_GetLock(void);

//释放CAN2互斥锁
void BoardCAN2_ReleaseLock(void);

//初始化
void BoardCAN2_Init(uint32_t extFilterVal,uint32_t extFilterMask,MCU_CAN_RecvPackProcFunc recvCallBackPtr);

//将接收到并处理完成的数据放回驱动空闲队列
void BoardCAN2_RecvDatRelease(CAN_RECV_PACK* recvDataPtr);

//设置AUX输出
void BoardCAN2_SetAuxOutState(PIN_STATE setState);

//CAN2总线发送数据
ERROR_SUB BoardCAN2_SendMsg(uint32_t id,CAN_ID_TYPE idType,uint8_t dataLen,uint8_t* dataBuf);

//获取CAN2的各项信息
MCU_CAN_INFO* BoardCAN2_GetInfo(void);


#endif

