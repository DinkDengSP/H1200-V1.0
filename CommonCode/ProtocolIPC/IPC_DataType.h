/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-06-08 11:39:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 18:49:54 +0800
************************************************************************************************/ 
#ifndef __IPC_DATA_TYPE_H_
#define __IPC_DATA_TYPE_H_
#include "IPC_Macro.h"
#include "ADT_CircularPtrList.h"

//IPC数据上报发送结构体
typedef struct IPC_UPLOAD_DATA
{
    //指令码
    uint32_t uploadID;
    //扩展指令参数缓存
    uint8_t* dataBufferPtr;
    uint16_t dataLength;
}IPC_UPLOAD_DATA;

//IPC返回数据
typedef struct IPC_RETURN_DATA
{
    uint8_t* returnBufferPtr;
    uint16_t returnDataLength;
}IPC_RETURN_DATA;

//IPC缓存
typedef struct IPC_CHANNEL_CACHE
{
    //IPC附带缓存
    uint8_t* dataBufferPtr;
    //标记携带的缓存长度
    uint16_t carryDataBufferLength;
    //返回的数据长度
    uint16_t returnDataBufferLength;
}IPC_CHANNEL_CACHE;

//IPC通道数据
typedef struct IPC_CHANNEL_DATA
{
    //数据访问互斥信号量
    TX_MUTEX mutexDataAccess;
    //结果等待列表
    ADT_CIRCULAR_PTR_LIST_T waitResultList; 
    //目标队列
    TX_QUEUE* targetQueuePtr;
    //上一个发出的指令
    uint32_t lastSendCommand;
    //携带缓存
    IPC_CHANNEL_CACHE channelCache;
}IPC_CHANNEL_DATA;

//IPC基础指令包
typedef struct IPC_CMD_BASE_DATA
{
    //指令码
    uint32_t commandCode;
    //携带参数
    uint32_t cmdParam1;
    uint32_t cmdParam2;
    uint32_t cmdParam3;
    uint32_t cmdParam4;
    uint32_t cmdParam5;
    //扩展缓存
    uint8_t* cmdParamBufferPtr;
    uint16_t cmdParamBufferLength;
}IPC_CMD_BASE_DATA;

//IPC等待结果返回数据包
typedef struct IPC_CMD_WAIT_RESULT
{
    uint32_t sendCommandThreadID;//发送指令的线程ID
    uint32_t commandCode;//指令码
    IPC_CMD_STAGE currentCmdStage;//当前指令状态
    SYS_RESULT_PACK resultPack;//结果包状态
}IPC_CMD_WAIT_RESULT;

//IPC指令包
typedef struct IPC_CMD_DATA
{
    IPC_CMD_BASE_DATA cmdBaseData;//发送的指令结构体
    IPC_CMD_WAIT_RESULT* waitResultPackPtr;//等待返回的指令结构体
    //通道缓存,用于数据返回状态共享
    IPC_CHANNEL_CACHE* channelCachePtr;
}IPC_CMD_DATA;

//IPC指令缓存头,用于指令查找
typedef struct IPC_CMD_NODE_HEADER
{
    uint32_t sendCommandThreadID;
    uint32_t commandCode;
}IPC_CMD_NODE_HEADER;




#endif




