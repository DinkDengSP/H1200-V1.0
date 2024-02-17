/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 09:21:41
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __COMMON_SRV_TASK_HEADER_H_
#define __COMMON_SRV_TASK_HEADER_H_
#include "DriverHeaderSystem.h"
#include "DriverConfigSystem.h"
#include "TaskConfigCommonSrv.h"

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

/********************************************UART接收任务************************************************************/
//将串口接收到的数据转发到处理线程
ErrorStatus UartRecvDataDispatchToSrvTask(UART_RECV_PACK* uartRecvPackPtr);
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketUart[STK_SIZE_SRV_TASK_SOCKET_UART/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketUart;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketUart;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketUart[LENGTH_QUEUE_SRV_TASK_SOCKET_UART];
//SOCKET UART ACK SEND消息队列
extern TX_QUEUE queueSocketUartAckSend;
//消息缓冲区
extern uint32_t queueBufferSocketUartAckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_UART];
//任务函数
void SrvTaskSocketUartFuncEntry(ULONG threadInput);
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

/********************************************CAN2接收任务************************************************************/
//将CAN2接收到的数据转发到处理线程
ErrorStatus Can2RecvDataDispatchToSrvTask(CAN_RECV_PACK* canRecvPackPtr);
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketCan2[STK_SIZE_SRV_TASK_SOCKET_CAN2/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketCan2;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketCan2;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketCan2[LENGTH_QUEUE_SRV_TASK_SOCKET_CAN2];
//SOCKET CAN2 ACK SEND消息队列
extern TX_QUEUE queueSocketCan2AckSend;
//消息缓冲区
extern uint32_t queueBufferSocketCan2AckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_CAN2];
//任务函数
void SrvTaskSocketCan2FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************网口8700接收任务************************************************************/
//tcpip协议栈
extern uint64_t stackBufferSrvTaskPhyNetX8700[STK_SIZE_SRV_TASK_NETX_IP_8700_PHY/8];
//FTP协议栈
#if(NETX_FILE_FTP_SERVICE_MODE != CONFIG_FUNC_DISABLE)
extern uint64_t stackBufferSrvTaskPhyFtpNetX[STK_SIZE_SRV_TASK_NETX_FTP_SOCKET/8];
#endif
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

/********************************************网口5500接收任务************************************************************/
//tcpip协议栈
extern uint64_t stackBufferSrvTaskPhyNetX5500[STK_SIZE_SRV_TASK_NETX_IP_5500_PHY/8];
//任务堆栈
extern uint64_t stackBufferSrvTaskSocketNet5500[STK_SIZE_SRV_TASK_SOCKET_NET_5500/8];
//任务控制块
extern TX_THREAD tcbSrvTaskSocketNet5500;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskSocketNet5500;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskSocketNet5500[LENGTH_QUEUE_SRV_TASK_SOCKET_NET_5500];
//SOCKET NET ACK SEND消息队列
extern TX_QUEUE queueSocketNet5500AckSend;
//消息缓冲区
extern uint32_t queueBufferSocketNet5500AckSend[LENGTH_QUEUE_SRV_SOCKET_ACK_SEND_NET_5500];
//任务函数
void SrvTaskSocketNet5500FuncEntry(ULONG threadInput);
/***************************************************************************************************************/


/********************************************日志任务************************************************************/
//系统日志发送函数
void SystemPrintf(const char *format, ...);
//系统日志发送字符串
void SystemSendString(uint8_t* strPtr);
//系统日志发送缓冲区
void SystemSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);
//系统日志发送缓冲区
void SystemSendBufferOther(uint8_t moduleNo,uint8_t boardID,uint8_t* bufferPtr,uint16_t bufferLength);
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
//探针数据发送函数
void SystemProbePrintf(const char *format, ...);
//探针数据发送字符串
void SystemProbeSendString(uint8_t* strPtr);
//探针数据发送缓冲区
void SystemProbeSendBuffer(uint8_t* bufferPtr,uint16_t bufferLength);
//探针数据发送数组显示
void SysProbeShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength);
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

/********************************************输入读取任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskPortIn[STK_SIZE_SRV_TASK_PORT_IN/8];
//任务控制块
extern TX_THREAD tcbSrvTaskPortIn;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskPortIn;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskPortIn[LENGTH_QUEUE_SRV_TASK_PORT_IN];
//任务函数
void SrvTaskPortInFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************输出写入任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskPortOut[STK_SIZE_SRV_TASK_PORT_OUT/8];
//任务控制块
extern TX_THREAD tcbSrvTaskPortOut;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskPortOut;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskPortOut[LENGTH_QUEUE_SRV_TASK_PORT_OUT];
//任务函数
void SrvTaskPortOutFuncEntry(ULONG threadInput);
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


/********************************************CAN数据透传任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskForwardSocketCan[STK_SIZE_SRV_TASK_FORWARD_SOCKET_CAN/8];
//任务控制块
extern TX_THREAD tcbSrvTaskForwardSocketCan;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskForwardSocketCan;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskForwardSocketCan[LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_CAN];
//任务函数
void SrvTaskForwardSocketCanFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************网口数据透传任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskForwardSocketNet[STK_SIZE_SRV_TASK_FORWARD_SOCKET_NET/8];
//任务控制块
extern TX_THREAD tcbSrvTaskForwardSocketNet;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskForwardSocketNet;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskForwardSocketNet[LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_NET];
//任务函数
void SrvTaskForwardSocketNetFuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************串口数据透传任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskForwardSocketUart[STK_SIZE_SRV_TASK_FORWARD_SOCKET_UART/8];
//任务控制块
extern TX_THREAD tcbSrvTaskForwardSocketUart;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskForwardSocketUart;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskForwardSocketUart[LENGTH_QUEUE_SRV_TASK_FORWARD_SOCKET_UART];
//任务函数
void SrvTaskForwardSocketUartFuncEntry(ULONG threadInput);
/***************************************************************************************************************/


#endif









