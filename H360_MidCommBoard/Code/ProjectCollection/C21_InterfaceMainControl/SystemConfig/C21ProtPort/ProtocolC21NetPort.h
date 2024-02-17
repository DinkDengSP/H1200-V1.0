/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-06 09:24:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-06 11:59:07 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_C21_NET_PORT_H_
#define __PROTOCOL_C21_NET_PORT_H_
#include "DriverHeaderBoard.h"
#include "ProtocolC21NetMacro.h"
#include "ProtocolC21NetDataType.h"

/****************************************************SOCKET ALL***********************************************************/
//错误信息打印函数
#define PROTOCOL_C21_NET_SOCKET_LOG(format,...)             MCU_RTT_Printf(format, ##__VA_ARGS__)    
//等待链表的容量上限
#define CAPACITY_MAX_C21_NET_SOCKET_WAIT_LIST               1000
//SOCKET查询指令是否完成的周期
#define TIME_MS_INTERVAL_CMD_QUERY_C21_NET_SOCKET           20
//两帧数据之间的延迟时间
#define TIME_INVERVAL_MS_C21_NET_SOCKET_FRAME               50
//ACK超时时间
#define TIME_OUT_MS_ACK_C21_NET_SOCKET                      3000
//重发失败之后的指令最大滞留时间,超过这个时间发不出去指令就不要了
#define TIME_OUT_MS_RETRY_C21_NET_SOCKET                    30000

/****************************************************协议栈信息处理数据结构**************************************************/
//网口指令转发单元配置
typedef struct C21_NET_CMD_DISPATCH_UNIT
{
    uint16_t cmdValue;
    TX_QUEUE* dispatchQueuePtr;
}C21_NET_CMD_DISPATCH_UNIT;

//获取SOCKET互斥锁
void ProtocolC21NetSocketGetMutexLock(void);

//释放SOCKET互斥锁
void ProtocolC21NetSocketReleaseMutexLock(void);

//获取SOCKET是否建立稳定链接
FlagStatus ProtocolC21NetSocketGetConnectFlag(void);

//发送SOCKET一帧
ERROR_SUB ProtocolC21NetSocketSendFrame(uint16_t msgBodyLen,uint8_t* bodyBuf);

//内存申请
void* ProtocolC21NetSocketMemMalloc(uint32_t sizeBytes);

//内存申请,必须成功
void* ProtocolC21NetSocketMemMallocWhileSuccess(uint32_t sizeBytes);

//内存释放
void ProtocolC21NetSocketMemFree(void* memPtr);

//ACK信息转发
void ProtocolC21AckMsgDispatch(C21_ACK_RESULT ackResult);

#endif
















