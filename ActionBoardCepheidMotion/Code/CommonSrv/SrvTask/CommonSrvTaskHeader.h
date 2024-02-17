/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 10:52:03 +0800
************************************************************************************************/ 
#ifndef __COMMON_SRV_TASK_HEADER_H_
#define __COMMON_SRV_TASK_HEADER_H_
#include "TaskConfigCommonSrv.h"
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

/********************************************步进电机1任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor1[STK_SIZE_SRV_TASK_STEP_MOTOR1/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor1;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor1;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor1[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR1];
//任务函数
void SrvTaskStepMotor1FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机2任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor2[STK_SIZE_SRV_TASK_STEP_MOTOR2/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor2;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor2;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor2[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR2];
//任务函数
void SrvTaskStepMotor2FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机3任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor3[STK_SIZE_SRV_TASK_STEP_MOTOR3/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor3;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor3;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor3[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR3];
//任务函数
void SrvTaskStepMotor3FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机4任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor4[STK_SIZE_SRV_TASK_STEP_MOTOR4/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor4;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor4;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor4[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR4];
//任务函数
void SrvTaskStepMotor4FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机5任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor5[STK_SIZE_SRV_TASK_STEP_MOTOR5/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor5;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor5;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor5[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR5];
//任务函数
void SrvTaskStepMotor5FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机6任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor6[STK_SIZE_SRV_TASK_STEP_MOTOR6/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor6;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor6;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor6[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR6];
//任务函数
void SrvTaskStepMotor6FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机7任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor7[STK_SIZE_SRV_TASK_STEP_MOTOR7/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor7;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor7;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor7[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR7];
//任务函数
void SrvTaskStepMotor7FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机8任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor8[STK_SIZE_SRV_TASK_STEP_MOTOR8/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor8;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor8;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor8[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR8];
//任务函数
void SrvTaskStepMotor8FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机9任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor9[STK_SIZE_SRV_TASK_STEP_MOTOR9/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor9;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor9;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor9[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR9];
//任务函数
void SrvTaskStepMotor9FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机10任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor10[STK_SIZE_SRV_TASK_STEP_MOTOR10/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor10;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor10;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor10[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR10];
//任务函数
void SrvTaskStepMotor10FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机11任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor11[STK_SIZE_SRV_TASK_STEP_MOTOR11/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor11;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor11;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor11[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR11];
//任务函数
void SrvTaskStepMotor11FuncEntry(ULONG threadInput);
/***************************************************************************************************************/

/********************************************步进电机12任务************************************************************/
//任务堆栈
extern uint64_t stackBufferSrvTaskStepMotor12[STK_SIZE_SRV_TASK_STEP_MOTOR12/8];
//任务控制块
extern TX_THREAD tcbSrvTaskStepMotor12;
//任务专用的消息队列
extern TX_QUEUE queueSrvTaskStepMotor12;
//任务的消息队列缓冲区
extern uint32_t queueBufferSrvTaskStepMotor12[LENGTH_QUEUE_SRV_TASK_STEP_MOTOR12];
//任务函数
void SrvTaskStepMotor12FuncEntry(ULONG threadInput);
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
//系统日志发送缓冲区
void SystemSendBufferOther(uint8_t moduleNo,uint8_t boardID,uint8_t* bufferPtr,uint16_t bufferLength);
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







