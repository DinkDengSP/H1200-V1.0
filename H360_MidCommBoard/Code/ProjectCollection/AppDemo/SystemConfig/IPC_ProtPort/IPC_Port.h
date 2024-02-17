/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-07-26 12:07:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-07-26 12:09:56 +0800
************************************************************************************************/ 
#ifndef __IPC_PORT_H_
#define __IPC_PORT_H_
#include "DriverHeaderBoard.h"
#include "IPC_DataType.h"

//错误信息打印函数
#define IPC_LOG(format,...)                     MCU_RTT_Printf(format, ##__VA_ARGS__) 
//IPC指令执行情况检测周期
#define TIME_MS_INTERVAL_CMD_QUERY_IPC          5
//是否设置每个发送带结果的指令都必须等待完成并且查询,否则新的相同的指令发不出去
#define IPC_WAIT_CMD_COMPLETE_READ              CONFIG_FUNC_DISABLE

//内存申请
void* IPC_PortMemMalloc(uint32_t sizeBytes);
//内存申请,必须成功
void* IPC_PortMemMallocWhileSuccess(uint32_t sizeBytes);
//内存释放
void IPC_PortMemFree(void* memPtr);

//发送上报包
ERROR_SUB IPC_PortSendUploadPack(TX_QUEUE* queuePtr,IPC_UPLOAD_DATA* uploadDataPtr);
//发送指令包
ERROR_SUB IPC_PortSendCmdPack(IPC_CHANNEL_DATA* channelDataPtr,IPC_CMD_DATA* cmdDataPtr);

#endif



