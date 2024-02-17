/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 16:50:52
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __SRV_TASK_HEADER_H_
#define __SRV_TASK_HEADER_H_
#include "TaskConfigSrv.h"
#include "DriverHeaderSystem.h"

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


/********************************************网口接收任务************************************************************/
//tcpip协议栈
extern uint64_t stackBufferSrvTaskPhyNetX8700[STK_SIZE_SRV_TASK_NETX_IP_8700_PHY/8];
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketNet8700[STK_SIZE_SRV_TASK_SOCKET_NET_8700/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketNet8700;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketNet8700;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketNet8700[LENGTH_QUEUE_SRV_TASK_SOCKET_NET_8700];
//SOCKET NET ACK SEND消息队列
extern TX_QUEUE queueSocketNet8700AckSend;
//消息缓冲区
extern uint32_t queueBufferSocketNet8700AckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET_8700];
//任务函数
void SrvTaskSocketNet8700FuncEntry(ULONG threadInput);
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

/********************************************参数读写任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskParam[STK_SIZE_SRV_TASK_PARAM/8];
//任务控制块
extern TX_THREAD tcbSrvTaskParam;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskParam;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskParam[LENGTH_QUEUE_SRV_TASK_PARAM];
//任务函数
void SrvTaskParamFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

#endif






