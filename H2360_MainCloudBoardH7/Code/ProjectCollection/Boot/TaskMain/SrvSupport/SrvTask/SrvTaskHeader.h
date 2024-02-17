/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#ifndef __SRV_TASK_HEADER_H_
#define __SRV_TASK_HEADER_H_
#include "TaskConfigSrv.h"


/********************************************启动心跳任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStartHeart[STK_SIZE_SRV_TASK_START_HEART/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStartHeart;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStartHeart;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStartHeart[LENGTH_QUEUE_SRV_TASK_START_HEART];
//任务函数
void SrvTaskStartHeartFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************CAN1接收任务************************************************************/
//将CAN1接收到的数据转发到处理线程
ErrorStatus Can1RecvDataDispatchToSrvTask(CAN_RECV_PACK* canRecvPackPtr);
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketCan1[STK_SIZE_SRV_TASK_SOCKET_CAN1/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketCan1;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketCan1;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketCan1[LENGTH_QUEUE_SRV_TASK_SOCKET_CAN1];
//SOCKET CAN1 ACK SEND消息队列
extern TX_QUEUE queueSocketCan1AckSend;
//消息缓冲区
extern uint32_t queueBufferSocketCan1AckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN1];
//任务函数
void SrvTaskSocketCan1FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************网口接收任务************************************************************/
//tcpip协议栈
extern uint64_t stackBufferSrvTaskPhyNetX[STK_SIZE_SRV_TASK_NETX_IP_PHY/8];
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketNet[STK_SIZE_SRV_TASK_SOCKET_NET/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketNet;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketNet;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketNet[LENGTH_QUEUE_SRV_TASK_SOCKET_NET];
//SOCKET NET ACK SEND消息队列
extern TX_QUEUE queueSocketNetAckSend;
//消息缓冲区
extern uint32_t queueBufferSocketNetAckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET];
//任务函数
void SrvTaskSocketNetFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************日志任务************************************************************/
//系统日志发送函数
void SystemPrintf(const char *format, ...);
//系统日志发送字符串
void SystemSendString(uint8_t* strPtr);
//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);
//系统发送数组显示
void SysLogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);
//任务堆栈
extern uint64_t stackBufferSrvTaskSysLog[STK_SIZE_SRV_TASK_SYS_LOG/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSysLog;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSysLog;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSysLog[LENGTH_QUEUE_SRV_TASK_SYS_LOG];
//任务函数
void SrvTaskSysLogFuncEntry(ULONG threadInput);
/***************************************************************************************************************/


/********************************************定时查询任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskTimer[STK_SIZE_SRV_TASK_TIMER/8];
//任务控制块
extern TX_THREAD tcbSrvTaskTimer;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskTimer;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskTimer[LENGTH_QUEUE_SRV_TASK_TIMER];
//任务函数
void SrvTaskTimerFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************系统辅助任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskUtil[STK_SIZE_SRV_TASK_UTIL/8];
//任务控制块
extern TX_THREAD tcbSrvTaskUtil;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskUtil;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskUtil[LENGTH_QUEUE_SRV_TASK_UTIL];
//任务函数
void SrvTaskUtilFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************在线升级任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskFlashIap[STK_SIZE_SRV_TASK_FLASH_IAP/8];
//任务控制块
extern TX_THREAD tcbSrvTaskFlashIap;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskFlashIap;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskFlashIap[LENGTH_QUEUE_SRV_TASK_FLASH_IAP];
//任务函数
void SrvTaskFlashIapFuncEntry(ULONG threadInput);
/***************************************************************************************************************/



#endif

















