/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 15:04:23 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-18 11:52:13 +0800
************************************************************************************************/ 
#ifndef __UART_PROT_DATA_TYPE_H_
#define __UART_PROT_DATA_TYPE_H_
#include "UartProtMacro.h"
#include "ADT_CircularPtrList.h"


/********************************************原生数据包***********************************************/
//数据包接收缓存
typedef struct UART_PROT_RECV_CACHE
{
    uint32_t lastRecvTimeStamp;//本指令数据包的接收时间
    uint16_t recvDataLength;//接收到的指令长度,包含指令的头
    uint16_t recvBodyLength;//指令包体的长度,也就是有效的数据长度
    uint8_t recvDataBuf[UART_PROT_SINGLE_PACK_DATA_MAX];//缓存的数据,包含指令头部数据
}UART_PROT_RECV_CACHE;

//数据包发送结构体
typedef struct UART_PROT_PACKAGE
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;    //目标板号
    PROT_PACK_TYPE packType;//包类型
    uint32_t idCode;//接收数据ID码
    uint8_t retryCount;//重发次数记录
    uint16_t dataBodyLength;//数据长度
    uint8_t* dataBodyBufferPtr;//数据缓存
}UART_PROT_PACKAGE;

/********************************************接收到的不同类型数据包***********************************************/
//接收到的指令包
typedef struct UART_PROT_CMD_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;  //目标板号
    uint32_t cmdCode;       //指令码
    uint8_t retryCount;     //重发次数记录
    uint16_t dataBodyLength;//接收有效数据长度
    uint8_t* dataBodyBufferPtr;//接收到的有效数据缓存
}UART_PROT_CMD_PACK_RECV;

//接收到的指令ACK包结构体
typedef struct UART_PROT_ACK_CMD_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;  //目标板号
    uint32_t ackCmdCode;    //ACK对应的指令
    uint8_t retryCount;     //重发次数记录
    PROT_ACK ackCode;//ACK代码
}UART_PROT_ACK_CMD_PACK_RECV;

//接收到的数据上传包
typedef struct UART_PROT_UPLOAD_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;    //目标板号
    uint32_t uploadID;
    uint8_t retryCount;//重发次数记录
    uint8_t* paramBufferPtr;//结果参数缓存区
    uint16_t paramBufferLength;//结果参数长度
}UART_PROT_UPLOAD_PACK_RECV;

//接收到的数据上传ACK包结构体
typedef struct UART_PROT_ACK_UPLOAD_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;    //目标板号
    uint32_t uploadID;
    uint8_t retryCount;//重发次数记录
}UART_PROT_ACK_UPLOAD_PACK_RECV;

//接收日志包结构体
typedef struct UART_PROT_LOG_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;  //目标板号
    uint16_t logBodyLength; //接收有效数据长度
    uint8_t* logBodyBufferPtr;//接收到的有效数据缓存
}UART_PROT_LOG_PACK_RECV;

//接收到的结果包结构体
typedef struct UART_PROT_RESULT_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;    //目标板号
    uint32_t cmdCode;
    uint8_t retryCount;//重发次数记录
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
}UART_PROT_RESULT_PACK_RECV;

//接收到的结果包ACK结构体
typedef struct UART_PROT_ACK_RESULT_PACK_RECV
{
    uint8_t srcModuleNo;    //源模块号
    uint8_t srcBoardID;     //源板号
    uint8_t targetModule;   //目标模块号
    uint8_t targetBoardID;    //目标板号
    uint32_t ackCmdCode;//ACK对应的指令
    uint8_t retryCount;//重发次数记录
}UART_PROT_ACK_RESULT_PACK_RECV;

/********************************************程序内部缓存数据类型***********************************************/
//指令包发送后等待数据缓冲
typedef struct UART_PROT_CMD_WAIT_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;//指令码
    uint16_t cmdParamBufferLength;//指令参数长度
    uint8_t* cmdParamBufferPtr;//指令参数缓存
    uint32_t lastSendTimeTickMs;//上一次发送的时间
    PROT_CMD_STA cmdStage;//指令状态
    FunctionalState cmdNeedResult;//指令是否需要结果返回
    uint32_t resultTimeOutMs;//指令结果返回的超时时间
    uint16_t exceptResultLengthMin;//预期返回结果参数的最小长度 
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
}UART_PROT_CMD_WAIT_PACK;

//数据上传包发送并等待ACK的缓存数据
typedef struct UART_PROT_UPLOAD_WAIT_ACK_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t retryCount;//重发次数
    uint32_t uploadID;
    uint8_t* uploadDataBufferPtr;//结果参数缓存区
    uint16_t uploadDataBufferLength;//结果参数长度
    uint32_t lastSendTimeTickMs;//上一次发送的时间
}UART_PROT_UPLOAD_WAIT_ACK_PACK;

//结果包发送并等待返回ACK的缓存数据
typedef struct UART_PROT_RESULT_WAIT_ACK_PACK
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint8_t retryCount;//重发次数
    uint32_t cmdValue;//对应的指令
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN errorMainCode;//错误主码
    ERROR_LEVEL errorLevelCode;//错误级别
    ERROR_SUB errorSubCode;//错误子码
    uint8_t* resultParamBufferPtr;//结果参数缓存区
    uint16_t resultParamBufferLength;//结果参数长度
    uint32_t lastSendTimeTickMs;//上一次发送的时间
}UART_PROT_RESULT_WAIT_ACK_PACK;

/********************************************内部缓存查找辅助数据***********************************************/
//指令等待数据头
typedef struct UART_PROT_CMD_WAIT_PACK_HEADER
{
    uint8_t targetBoardID;//目标板号
    uint8_t targetModuleNo;//目标模块号
    uint32_t cmdValue;//指令码
}UART_PROT_CMD_WAIT_PACK_HEADER;

/********************************************指令发送集成数据***********************************************/
typedef struct UART_PROT_CMD_SEND
{
    uint8_t targetModule;
    uint8_t targetBoardID;
    uint32_t cmdVal;
    uint8_t* cmdBufferPtr;
    uint16_t cmdBufferLength;
    FunctionalState cmdNeedResult;
    uint32_t resultTimeOutMs;
    uint16_t exceptResultLengthMin;
}UART_PROT_CMD_SEND;

//转发数据类型
typedef struct UART_PROT_FWD_DAT
{
    UART_PROT_FWD_TYPE forwardType;
    void* forwardPackPtr;
}UART_PROT_FWD_DAT;

//ACK发送数据类型
typedef struct UART_PROT_ACK_PACK
{
    UART_PROT_ACK_TYPE ackType;//ACK类型
    uint8_t targetModule;
    uint8_t targetBoardID;
    uint8_t retryCount;
    uint32_t cmdVal;
    PROT_ACK ackCmd;
}UART_PROT_ACK_PACK;

/********************************************指令配置数据***********************************************/
//CAN指令的基础配置
typedef struct UART_CMD_CONFIG
{
    uint32_t cmdVal;                //指令
    FunctionalState cmdNeedResult;  //指令是否需要返回
    uint32_t resultTimeOutMs;       //指令超时时间
    uint16_t exceptResultLengthMin; //指令返回数据的最小长度
    uint16_t paramBufferLen;        //参数长度,如果参数是不定长的话,写0,由高层取申请
}UART_CMD_CONFIG;

/********************************************协议栈集成数据维护***********************************************/
//UART初始化信息集合
typedef struct UART_PROT_INIT_SET
{
    //当前模块号
    uint8_t selfModuleNo;
    //当前自身板号
    uint8_t selfBoardID;
    //SOCKET数据访问互斥信号量
    TX_MUTEX mutexUartProtDataAccess;
    //发送缓存
    ADT_CIRCULAR_PTR_LIST_T listSendResultWaitAckCachePtr;//发送结果包等待ACK的缓存指针
    ADT_CIRCULAR_PTR_LIST_T listSendUploadWaitAckCachePtr;//发送上传包等待ACK的缓存指针
    ADT_CIRCULAR_PTR_LIST_T listSendCmdWaitCachePtr;//发送指令包等待ACK或者return的缓存指针
}UART_PROT_INIT_SET;



#endif











